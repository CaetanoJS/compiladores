
#ifndef SEMANTIC_HEADER
#define SEMANTIC_HEADER


#include "ast.h"
#include "hash.h"

extern int SemanticErrors;

void check_and_set_declarations(AST *node);
void set_datatype_for_identifiers(AST *node);
void check_undeclared();


#endif