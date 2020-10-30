
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
                set_datatype_for_identifiers(node, 0);
            break;
        case AST_VECTOR_DECL_WITH_PARAM:
            if(node->symbol)
                if(node->symbol->type != TK_IDENTIFIER)
                {
                    fprintf(stderr, "Semantic ERROR: Vector %s already declared\n", node->symbol->text);
                    ++ SemanticErrors;
                }
                node->symbol->type = TK_VECTOR;
                set_datatype_for_identifiers(node, 0);
            break;
        case AST_VARIABLE_DECL:
            if(node->symbol)
                if(node->symbol->type != TK_IDENTIFIER)
                {
                    fprintf(stderr, "Semantic ERROR: variable %s already declared\n", node->symbol->text);
                    ++ SemanticErrors;
                }
                node->symbol->type = TK_VARIABLE;
                set_datatype_for_identifiers(node, 0);
            break;
        case AST_FUNCTION_DECL:
            if(node->symbol)
                if(node->symbol->type != TK_IDENTIFIER)
                {
                    fprintf(stderr, "Semantic ERROR: function %s already declared\n", node->symbol->text);
                    ++ SemanticErrors;
                }
                node->symbol->type = TK_FUNCTION;
                set_datatype_for_identifiers(node, 1);
            break;
        case AST_PARAMS_FUNC:
            if(node->symbol)
                if(node->symbol->type != TK_IDENTIFIER)
                {
                    fprintf(stderr, "Semantic ERROR: function %s already declared\n", node->symbol->text);
                    ++ SemanticErrors;
                }
                node->symbol->type = TK_VARIABLE;
                set_datatype_for_identifiers(node, 0);
            break;
        case AST_PARAMS_FUNC_LIST:
            if(node->symbol)
                if(node->symbol->type != TK_IDENTIFIER)
                {
                    fprintf(stderr, "Semantic ERROR: function %s already declared\n", node->symbol->text);
                    ++ SemanticErrors;
                }
                node->symbol->type = TK_VARIABLE;
                set_datatype_for_identifiers(node, 0);
            break;
    }

    for( i=0 ; i < MAX_SONS ; ++i)
        check_and_set_declarations(node->son[i]);
}

void set_datatype_for_identifiers(AST *node, int sonNumber){
    if(node->son[sonNumber] && (node->son[sonNumber]->type == AST_TYPE_CHAR))
        node->symbol->datatype = KW_CHAR;
    if(node->son[sonNumber] && (node->son[sonNumber]->type == AST_TYPE_INT))
        node->symbol->datatype = KW_INT;
    if(node->son[sonNumber] && (node->son[sonNumber]->type == AST_TYPE_FLOAT))
        node->symbol->datatype = KW_FLOAT;
    if(node->son[sonNumber] && (node->son[sonNumber]->type == AST_TYPE_BOOL))
        node->symbol->datatype = KW_BOOL;

}

// int is_number(AST *son){
//     if (son->type == AST_ADD 
//         || son->type == AST_SUB
//         || son->type == AST_MULT
//         || son->type == AST_DIV
//         || (son->type == AST_SYMBOL 
//             && (son->symbol->type == LIT_INTEGER
//                 || son->symbol->type == LIT_FLOAT
//                 || son->symbol->type == LIT_CHAR)
//             || (son->symbol->type == TK_VARIABLE
//                 && (son->symbol->datatype == KW_INT 
//                 || son->symbol->datatype == KW_FLOAT
//                 || son->symbol->datatype == KW_CHAR)))
//         || (son->type == AST_FUNCTION_CALL
//             && (son->symbol->datatype == KW_INT 
//                 || son->symbol->datatype == KW_FLOAT
//                 || son->symbol->datatype == KW_CHAR)))
//         return 1;
//         else
//             return 0;
// }

int isNumber(int type)
{
    return (type == LIT_INTEGER || type == LIT_FLOAT || type == LIT_CHAR || type == KW_INT || type == KW_FLOAT || type == KW_CHAR);
}

int return_symbol(HASH_NODE *symbol)
{
    if(symbol->type == TK_VARIABLE || symbol->type == TK_FUNCTION)
    {
        return symbol->datatype;
    }
    else
        return symbol->type;
}

int check_operands(AST *node){
    int i;
    if (node == 0)
        return;

    switch (node->type)
    {
        case AST_ADD:
        {
            int leftOperand = check_operands(node->son[0]);
            int rightOperand = check_operands(node->son[1]);

            // fprintf(stderr, "left operand %i\n", leftOperand);
            // fprintf(stderr, "right operand %i\n", rightOperand);

            if (!(isNumber(leftOperand)))
            {
                fprintf(stderr, "Semantic ERROR: invalid left operand for ADD\n");
                ++ SemanticErrors;
            }
            if(!(isNumber(rightOperand)))
            {
                fprintf(stderr, "Semantic ERROR: invalid right operand for ADD\n");
                ++ SemanticErrors;
            }
            return KW_INT;
        }
        case AST_SYMBOL:
        {   
            return return_symbol(node->symbol);
            break;
        }
        case AST_FUNCTION_CALL:
        {
            // printf("type da func: %i\n", node->symbol->type);
            // printf("datatype da func: %i\n", node->symbol->datatype);
            return return_symbol(node->symbol);
            break;
        }
        default:
        {
            for( i=0 ; i < MAX_SONS ; ++i)
                check_operands(node->son[i]);
        }
            

    }
return 200;
}

void check_undeclared(){
  SemanticErrors += hash_check_undeclared();
}

void printNode(AST *node)
{
    printf("node type: %i\n", node->type);
    printf("node symbol type: %i\n", node->symbol->type);
    printf("node symbol datatype: %i\n", node->symbol->datatype);
    printf("node symbol text %c\n", node->symbol->text);
}