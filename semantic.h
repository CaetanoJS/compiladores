
#ifndef SEMANTIC_HEADER
#define SEMANTIC_HEADER


#include "ast.h"
#include "hash.h"

extern int SemanticErrors;

void check_and_set_declarations(AST *node);
void set_datatype_for_identifiers(AST *node, int sonNumber);
int check_operands(AST *node);
void check_undeclared(void);
void printNode(AST *node);


#endif