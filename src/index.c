#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "index.h"
#include "map.h"
#include "common.h"
#include "list.h"
#include "set.h"
#include "parser.h"
#include "printing.h"

int compare_results_by_path(void *a, void *b)
{
    query_result_t *qa = a,
                   *qb = b;

    return compare_strings(qa->path, qb->path);
}

int compare_results_by_score(void *a, void *b)
{
    query_result_t *qa = a,
                   *qb = b;

    if(qa->score < qb->score)
    {
        return 1;
    }
    
    if (qa->score > qb->score)
    {
        return -1;
    }

    return 0;
    
}

struct index
{
    map_t *documents;
    int numdocs;
};

/*
 * Creates a new, empty index.
 */
index_t *index_create()
{
    index_t *new = malloc(sizeof(index_t));
    new->documents = map_create(compare_strings, hash_string);
    new->numdocs = 0;
    return new;
}

/*
 * Destroys the given index.  Subsequently accessing the index will
 * lead to undefined behavior.
 */
void destroy_set_of_results(void *a)
{
    set_t *set = a;
    set_iter_t *iter = set_createiter(set);
    query_result_t *res = NULL;

    while (set_hasnext(iter))
    {
        res = set_next(iter);
        free(res->path);
        free(res);
    }

    set_destroyiter(iter);
    set_destroy(set);
}

void index_destroy(index_t *index)
{
    map_destroy(index->documents, free, destroy_set_of_results);
    free(index);
}

/*
 * Adds the given path to the given index, and index the given
 * list of words under that path.
 * NOTE: It is the responsibility of index_addpath() to deallocate (free)
 *       'path' and the contents of the 'words' list.
 */
void index_addpath(index_t *index, char *path, list_t *terms)
{
    int total_term_count = list_size(terms);
    char *term = list_popfirst(terms);
    map_t *term_counter = map_create(compare_strings, hash_string);
    set_t *unique_terms = set_create(compare_strings);
   
    while (term != NULL)
    {
        int *counter;
        if (map_haskey(term_counter, term))
        {
            counter = map_get(term_counter, term);
        }
        else
        {
            counter = malloc(sizeof(int));
            *counter = 0;
            map_put(term_counter, strdup(term), counter);
        }
        *counter += 1;

        if (!set_contains(unique_terms, term))
        {
            set_add(unique_terms, strdup(term));
        }
        free(term);
        term = list_popfirst(terms);
    }

    //Counter to keep track of the number of documents mapped to each term
    index->numdocs += 1;
    
    set_iter_t *iter = set_createiter(unique_terms);
    while (set_hasnext(iter))
    {
        term = set_next(iter);
        set_t *results;
        if (map_haskey(index->documents, term))
        {
            results = map_get(index->documents, term);
        }
        else    
        {
            results = set_create(compare_results_by_path);
            map_put(index->documents, strdup(term), results);
        }

        int *occurences = map_get(term_counter, term);

        query_result_t *res = malloc(sizeof(query_result_t));
        res->path = strdup(path);
        res->score = *occurences / (double)total_term_count;
        set_add(results, res);

        free(term);
    }

    free(path);
}

set_t *get_documents_for_term(index_t *index, char *term)
{
    set_t *results = set_create(compare_results_by_path);

    if (!map_haskey(index->documents, term))
    {
        return results;
    }

    set_t *base_results = map_get(index->documents, term);

    // Calculate the idf for this word in the corpus
    double num_docs_containing_term = (double)set_size(base_results);
    double idf = 1 + log((double)index->numdocs / num_docs_containing_term);

    query_result_t *res;
    set_iter_t *resultiter = set_createiter(base_results);
    while (set_hasnext(resultiter))
    {
        res = set_next(resultiter);
        query_result_t *copy = malloc(sizeof(query_result_t));
        copy->path = res->path;
        //tf*idf
        copy->score = res->score * idf;
        set_add(results, copy);
    }
    set_destroyiter(resultiter);

    return results;
}

/* Evaluates each node to check if its an operand or term 
* and returns it accordingly.
*/
static set_t *evaluate(node_t *query, index_t * index)
{
    set_t *compare_set1;
    set_t *compare_set2;
    
    switch(query->type)
    {
        //difference
        case ANDNOT:
            compare_set1 = evaluate(query->left, index);
            compare_set2 = evaluate(query->right, index);
            return set_difference(compare_set1, compare_set2);

        //intersection 
        case AND:
            compare_set1 = evaluate(query->left, index);
            compare_set2 = evaluate(query->right, index);
            return set_intersection(compare_set1, compare_set2);

        //union
        case OR:
            compare_set1 = evaluate(query->left, index);
            compare_set2 = evaluate(query->right, index);
            return set_union(compare_set1, compare_set2);

        case TERM:
        return get_documents_for_term(index, query->term);
        
    }

    ERROR_PRINT("Unknown/no node type\n");
    return 0;

}

/*
 * Performs the given query on the given index.  If the query
 * succeeds, the return value will be a list of paths (query_result_t). 
 * If there is an error (e.g. a syntax error in the query), an error 
 * message is assigned to the given errmsg pointer and the return value
 * will be NULL.
 */
list_t *index_query(index_t *index, list_t *query, char **errmsg)
{
    node_t *ast;
    list_t *result_list = list_create(compare_results_by_score);
    set_t *index_res_set;

    ast = parse(query);
    
    index_res_set = evaluate(ast, index);
    
    set_iter_t *iterator = set_createiter(index_res_set);

    while (set_hasnext(iterator))
    {
        query_result_t * res = set_next(iterator);
        list_addfirst(result_list, res);
    }

    set_destroyiter(iterator);

    set_destroy(index_res_set);

    list_sort(result_list);

    return result_list; 
}
