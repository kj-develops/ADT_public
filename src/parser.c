#include "list.h"
#include "set.h"
#include "printing.h"
#include "parser.h"


// Forward declarations of all recursive methods needed
static node_t *parse_query(list_iter_t *iterator);
static node_t *parse_andterm(list_iter_t *iterator);
static node_t *parse_orterm(list_iter_t *iterator);
static node_t *parse_term(list_iter_t *iterator);


static node_t *newnode(node_type type, char *term, node_t *left, node_t *right) 
{
    node_t *node = (node_t *)calloc(1, sizeof(node_t));
    if (node == NULL)
    {
        ERROR_PRINT("Could not allocate space for node\n");
    }

    node->type = type;
    node->term = term;
    node->left = left;
    node->right = right;

    return node;

}

//Implementing: query ::= andterm | andterm "ANDNOT" query
node_t *parse_query(list_iter_t *iterator)
{
    node_t *q2, *q1 = parse_andterm(iterator);

    if (list_hasnext(iterator))
    {
        void *elem = list_peek(iterator);
        char *test_string = "ANDNOT";

        if(compare_strings(elem, test_string) == 0)
        {
            //kan bare kastes!
            list_next(iterator);
            q2 = parse_query(iterator);
            return newnode(ANDNOT, NULL, q1, q2);
        }
    }
 
    return q1;
}

//Implementing: andterm ::= orterm | orterm "AND" andterm
node_t *parse_andterm(list_iter_t *iterator)
{
    node_t *a2, *a1 = parse_orterm(iterator);
    
    if (list_hasnext (iterator))
    {
        void *elem = list_peek(iterator);
        char *test_string = "AND";

        if(compare_strings(elem, test_string) == 0)
        {
            //kan bare kastes!
            list_next(iterator);
            a2 = parse_andterm(iterator);
            return newnode(AND, NULL, a1, a2);
        }
    }
    
    return a1;
}

//Implementing: orterm ::= term | term "OR" orterm
node_t *parse_orterm(list_iter_t *iterator)
{
    node_t *o2, *o1 = parse_term(iterator);
    if (list_hasnext (iterator))
    {
        void *elem = list_peek(iterator);
        char *test_string = "OR";

        if(compare_strings(elem, test_string) == 0)
        {
            //kan bare kastes!
            list_next(iterator);
            o2 = parse_orterm(iterator);
            return newnode(OR, NULL, o1, o2);
        }
    }
    
    return o1;
}

//Implementing: term ::= "(" query ")" | <word>
node_t *parse_term(list_iter_t *iterator)
{
    void *elem = list_next(iterator);

    char *test_left = "(";
    char *test_right = ")";

    if(compare_strings(elem, test_left) == 0)
    {
        node_t *q;

        q = parse_query(iterator);
        elem = list_next(iterator);

        if(compare_strings(elem, test_right) != 0)
        {
            ERROR_PRINT("Missing )\n");
        }

        return q;
    }
    else
    {
        return newnode(TERM, elem, NULL, NULL);
    }
       
}

//Initiates parsing
node_t *parse(list_t *query) 
{
    node_t *result;
    list_iter_t *iterator = list_createiter(query);

    result = parse_query(iterator);

    if (list_hasnext(iterator))
    {
        ERROR_PRINT("Unparsed input:");// %s\n", list_next(iterator));
    }

    return result;
}