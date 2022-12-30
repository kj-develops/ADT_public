#ifndef PARSER_H
#define PARSER_H

#include "list.h"

typedef enum 
{
	ANDNOT, AND, OR, TERM
} node_type;


typedef struct node node_t;
struct node
{
    char *term;
    node_type type;
    node_t *left;
    node_t *right;
};

/*

BNF grammar used:

query ::= andterm | andterm "ANDNOT" query
andterm ::= orterm | orterm "AND" andterm
orterm ::= term | term "OR" orterm
term ::= "(" query ")" | <word>

*/

node_t *parse(list_t *query);

#endif