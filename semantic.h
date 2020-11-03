
#ifndef SEMANTIC_HEADER
#define SEMANTIC_HEADER


#include "ast.h"
#include "hash.h"

extern int SemanticErrors;

void check_and_set_declarations(AST *node);
void set_datatype_for_identifiers(AST *node, int sonNumber);
int check_operands(AST *node);
void check_commands(AST *node);
int check_valid_for_int_expr(AST *node);
int check_valid_for_bool_expr(AST *node);
int return_symbol(HASH_NODE *symbol);
int isInt(int type);
int isNumber(int type);
void check_undeclared(void);
void printNode(AST *node);


#endif