
#include "semantic.h"
#include "parser.tab.h"

int SemanticErrors = 0;

void check_and_set_declarations(AST *node){
    int i;
    if (node == 0)
        return;

    switch (node->type)
    {
        case AST_VECTOR_DECL:
            if(node->symbol)
                if(node->symbol->type != TK_IDENTIFIER)
                {
                    fprintf(stderr, "Semantic ERROR: Vector %s already declared\n", node->symbol->text);
                    ++ SemanticErrors;
                }
                node->symbol->type = TK_VECTOR;
                set_datatype_for_identifiers(node);
            break;
        case AST_VECTOR_DECL_WITH_PARAM:
            if(node->symbol)
                if(node->symbol->type != TK_IDENTIFIER)
                {
                    fprintf(stderr, "Semantic ERROR: Vector %s already declared\n", node->symbol->text);
                    ++ SemanticErrors;
                }
                node->symbol->type = TK_VECTOR;
                set_datatype_for_identifiers(node);
            break;
        case AST_VARIABLE_DECL:
            if(node->symbol)
                if(node->symbol->type != TK_IDENTIFIER)
                {
                    fprintf(stderr, "Semantic ERROR: variable %s already declared\n", node->symbol->text);
                    ++ SemanticErrors;
                }
                node->symbol->type = TK_VARIABLE;
                set_datatype_for_identifiers(node);
            break;
        case AST_FUNCTION_DECL:
            if(node->symbol)
                if(node->symbol->type != TK_IDENTIFIER)
                {
                    fprintf(stderr, "Semantic ERROR: function %s already declared\n", node->symbol->text);
                    ++ SemanticErrors;
                }
                node->symbol->type = TK_FUNCTION;
                set_datatype_for_identifiers(node);
            break;
        case AST_PARAMS_FUNC:
            if(node->symbol)
                if(node->symbol->type != TK_IDENTIFIER)
                {
                    fprintf(stderr, "Semantic ERROR: function %s already declared\n", node->symbol->text);
                    ++ SemanticErrors;
                }
                node->symbol->type = TK_VARIABLE;
                set_datatype_for_identifiers(node);
            break;
        case AST_PARAMS_FUNC_LIST:
            if(node->symbol)
                if(node->symbol->type != TK_IDENTIFIER)
                {
                    fprintf(stderr, "Semantic ERROR: function %s already declared\n", node->symbol->text);
                    ++ SemanticErrors;
                }
                node->symbol->type = TK_VARIABLE;
                set_datatype_for_identifiers(node);
            break;
    }

    for( i=0 ; i < MAX_SONS ; ++i)
        check_and_set_declarations(node->son[i]);
}

void set_datatype_for_identifiers(AST *node){
    if(node->son[0] && (node->son[0]->type == AST_TYPE_CHAR))
        node->symbol->datatype = KW_CHAR;
    if(node->son[0] && (node->son[0]->type == AST_TYPE_INT))
        node->symbol->datatype = KW_INT;
    if(node->son[0] && (node->son[0]->type == AST_TYPE_FLOAT))
        node->symbol->datatype = KW_FLOAT;
    if(node->son[0] && (node->son[0]->type == AST_TYPE_BOOL))
        node->symbol->datatype = KW_BOOL;
}

void check_undeclared(){
  SemanticErrors += hash_check_undeclared();
}