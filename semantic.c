
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
        node->symbol->datatype = DATATYPE_CHAR;
    if(node->son[sonNumber] && (node->son[sonNumber]->type == AST_TYPE_INT))
        node->symbol->datatype = DATATYPE_INT;
    if(node->son[sonNumber] && (node->son[sonNumber]->type == AST_TYPE_FLOAT))
        node->symbol->datatype = DATATYPE_FLOAT;
    if(node->son[sonNumber] && (node->son[sonNumber]->type == AST_TYPE_BOOL))
        node->symbol->datatype = DATATYPE_BOOL;

}

int isNumber(int type)
{
    return (type == LIT_INTEGER || type == LIT_FLOAT || type == LIT_CHAR || type == DATATYPE_INT || type == DATATYPE_FLOAT || type == DATATYPE_CHAR);
}

int isInt(int type)
{
    return (type == LIT_INTEGER || type == DATATYPE_INT);
}

int isBool(int type)
{
    return (type == DATATYPE_BOOL || type == LIT_TRUE || type == LIT_FALSE);
}

int return_symbol(HASH_NODE *symbol)
{
    if(symbol->type == TK_VARIABLE || symbol->type == TK_FUNCTION || symbol->type == TK_VECTOR)
    {
        return symbol->datatype;
    }
    else
        return symbol->type;
}

int check_valid_for_bool_expr(AST *node)
{
    switch (node->type)
    {
        case AST_DIV:
        case AST_MULT:
        case AST_SUB:
        case AST_ADD:
        {
            return 0;
        }
        default:
            return 1;
    }
}

int check_valid_for_int_expr(AST *node)
{
    switch (node->type)
    {
        case AST_GRT:
        case AST_LESS:
        case AST_OR:
        case AST_POW:
        case AST_LE:
        case AST_GE:
        case AST_EQ:
        case AST_DIF:
        {
            return 0;
        }
        default:
            return 1;
    }
}

int isVariable(int type)
{
    return (type == TK_VARIABLE);
}

int isVector(int type)
{
    return (type == TK_VECTOR);
}

void check_commands(AST *node)
{
    int i;
    if (node == 0)
        return;

    switch (node->type)
    {
        case AST_VAR_ATTRIBUTION:
        {
            int identifierType = return_symbol(node->symbol);
            int exprType = check_operands(node->son[0]);

            if (!(isVariable(node->symbol->type)))
            {
                fprintf(stderr, "Semantic ERROR: Identifier '%s' is not a scalar variable\n", node->symbol->text);
                ++ SemanticErrors;               
            }

            if((isNumber(exprType) && isNumber(identifierType)) || (isBool(exprType) && isBool(identifierType)))
            {
                break;
            } else {
                fprintf(stderr, "Semantic ERROR: expr does not have the same type as the idenfier\n");
                ++ SemanticErrors;
            }

            break;
        }
        case AST_VECTOR_ATTRIBUTION:
        {
            int identifierType = return_symbol(node->symbol);
            int indexType = check_operands(node->son[0]);
            int exprType = check_operands(node->son[1]);

            if(!(isVector(node->symbol->type)))
            {
                fprintf(stderr, "Semantic ERROR: Identifier '%s' is not a vector variable\n", node->symbol->text);
                ++ SemanticErrors;                 
            }

            if(!isInt(indexType))
            {
                fprintf(stderr, "Semantic ERROR: index is not an INT on '%s' variable\n", node->symbol->text);
                ++ SemanticErrors;      
            }

            if((isNumber(exprType) && isNumber(identifierType)) || (isBool(exprType) && isBool(identifierType)))
            {
                break;
            } else {
                fprintf(stderr, "Semantic ERROR: expr does not have the same type as the idenfier\n");
                ++ SemanticErrors;
            }

            break;
        }
        default:
        {
            for( i=0 ; i < MAX_SONS ; ++i)
                check_commands(node->son[i]);
        }
    }
}

int check_operands(AST *node){
    int i;
    if (node == 0)
        return;

    switch (node->type)
    {
        case AST_DIV:
        case AST_MULT:
        case AST_SUB:
        case AST_ADD:
        {
            int leftOperand = check_operands(node->son[0]);
            int rightOperand = check_operands(node->son[1]);

            fprintf(stderr, "left operand %i\n", leftOperand);
            fprintf(stderr, "right operand %i\n", rightOperand);

            if (!(isNumber(leftOperand)))
            {
                fprintf(stderr, "Semantic ERROR: invalid left operand for int expr\n");
                ++ SemanticErrors;
            }
            if (!check_valid_for_int_expr(node->son[0]))
            {
                fprintf(stderr, "Semantic ERROR: invalid expr on the left side\n");
                ++ SemanticErrors;               
            }
            if(!(isNumber(rightOperand)))
            {
                fprintf(stderr, "Semantic ERROR: invalid right operand for int expr\n");
                ++ SemanticErrors;
            }
            if (!check_valid_for_int_expr(node->son[1]))
            {
                fprintf(stderr, "Semantic ERROR: invalid expr on the right side\n");
                ++ SemanticErrors;               
            }

            if (leftOperand == DATATYPE_FLOAT || leftOperand == LIT_FLOAT)
                return DATATYPE_FLOAT;
            if (rightOperand == DATATYPE_FLOAT || rightOperand == LIT_FLOAT)
                return DATATYPE_FLOAT;

            return DATATYPE_INT;
        }        
        case AST_GRT:
        case AST_LESS:
        case AST_OR:
        case AST_POW:
        case AST_LE:
        case AST_GE:
        case AST_EQ:
        case AST_DIF:
        {
            int leftOperand = check_operands(node->son[0]);
            int rightOperand = check_operands(node->son[1]);

            if (!(isNumber(leftOperand)))
            {
                fprintf(stderr, "Semantic ERROR: invalid left operand for bool expr\n");
                ++ SemanticErrors;
            }
            if (!check_valid_for_bool_expr(node->son[0]))
            {
                fprintf(stderr, "Semantic ERROR: invalid expr on the left side\n");
                ++ SemanticErrors;               
            }
            if(!(isNumber(rightOperand)))
            {
                fprintf(stderr, "Semantic ERROR: invalid right operand for bool expr\n");
                ++ SemanticErrors;
            }
            if (!check_valid_for_bool_expr(node->son[1]))
            {
                fprintf(stderr, "Semantic ERROR: invalid expr on the right side\n");
                ++ SemanticErrors;               
            }

            return DATATYPE_BOOL;
        }
        case AST_VECTOR:
        {
            int vectorInd = check_operands(node->son[0]);
            if (!isInt(vectorInd))
            {
                fprintf(stderr, "Semantic ERROR: vector indice is not an INT\n");
                ++ SemanticErrors; 
            } else
                return return_symbol(node->symbol);
            break;
        }
        case AST_SYMBOL:
        {   
            return return_symbol(node->symbol);
            break;
        }
        case AST_FUNCTION_CALL:
        {
            // TODO: Verify function ARGS
            return return_symbol(node->symbol);
            break;
        }
        case AST_PARENTHESES_EXPR:
        {
            int parentesesExpr = check_operands(node->son[0]);
            return parentesesExpr;
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