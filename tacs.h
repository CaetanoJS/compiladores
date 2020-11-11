
#ifndef TACS_HEADER
#define TACS_HEADER

#include "hash.h"
#include "ast.h"

#define TAC_SYMBOL 1
#define TAC_ADD 2
#define TAC_SUB 3
#define TAC_COPY 4
#define TAC_MULT 5
#define TAC_DIV 6
#define TAC_GTR 7
#define TAC_LESS 8
#define TAC_OR 9
#define TAC_POW 10
#define TAC_LE 11
#define TAC_GE 12
#define TAC_EQ 13
#define TAC_DIFF 14
#define TAC_VECTOR 15
#define TAC_JFALSE 16
#define TAC_LABEL 17
#define TAC_JUMP 18
#define TAC_READ 19
#define TAC_RETURN 20
#define TAC_COPY_VECTOR 21


typedef struct tac_node
{
    int type;
    HASH_NODE * res;
    HASH_NODE * op1;
    HASH_NODE * op2;

    struct tac_node *prev;
    struct tac_node *next;

} TAC;

TAC* tacCreate(int type, HASH_NODE *res, HASH_NODE *op1, HASH_NODE *op2);
void tacPrint(TAC* tac);
void tacPrintBackwards(TAC *tac);
TAC* tacJoin(TAC* l1, TAC* l2);

// CODE GENERATION

TAC* generateCode(AST *node);


#endif