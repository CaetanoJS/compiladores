/* Caetano Jaeger Stradolini */
#include "semantic.h"
#include "parser.tab.h"

#define FUNC_PARAMS_MAX_LENGHT 100
int SemanticErrors = 0;
PARAMS_FUNC* funcParams[FUNC_PARAMS_MAX_LENGHT];
void check_syntax_errors()
{
    if(SemanticErrors > 0)
        exit(4);
}
void check_func_call_params(AST *node, PARAMS_FUNC *paramsFunc);

void clear_func_params()
{
    int i;
    for (i = 0 ; i < FUNC_PARAMS_MAX_LENGHT ; i++)
    {
        funcParams[i] = NULL;
    }
}

void add_to_params(AST *node)
{

    int i;
    if (node == 0)
        return;

    switch (node->type)
    {
        case AST_PARAMS_FUNC:
        {

            int x;
            for (x = 0 ; x < FUNC_PARAMS_MAX_LENGHT ; x++)
            {
                if(!funcParams[x])
                {
                    funcParams[x] = (PARAMS_FUNC*) calloc(1, sizeof(PARAMS_FUNC));
                    funcParams[x]->text = node->symbol->text;
                    funcParams[x]->type = node->symbol->datatype;
                    break;
                }
            }
            if(node->son[1])
                add_to_params(node->son[1]);
            break;
        }

        case AST_PARAMS_FUNC_LIST:
        {
            int x;
            for (x = 0 ; x < FUNC_PARAMS_MAX_LENGHT ; x++)
            {
                if(!funcParams[x])
                {
                    funcParams[x] = (PARAMS_FUNC*) calloc(1, sizeof(PARAMS_FUNC));
                    funcParams[x]->text = node->symbol->text;
                    funcParams[x]->type = node->symbol->datatype;
                    break;
                }
            }
            if(node->son[1])
                add_to_params(node->son[1]);
            break;
        }
        default:
            for( i=0 ; i < MAX_SONS ; ++i)
                add_to_params(node->son[i]);
    }
}

void set_func_params(AST *node)
{
    int i;
    if (node == 0)
        return;

    switch (node->type)
    {
        case AST_FUNCTION_DECL:
        {
            if(node->son[0])
            {
                PARAMS_FUNC *params;
                clear_func_params();
                add_to_params(node->son[0]);

                int params_count = 0;
                for (params_count = 0 ; params_count < 100 ; params_count++)
                {
                    if(!funcParams[params_count])
                    {
                        break;
                    }
                }

                int ind = 0;
                node->symbol->paramsFunc = (PARAMS_FUNC*) calloc(1, sizeof(PARAMS_FUNC));
                for (params=node->symbol->paramsFunc; params; params = params->next)
                {
                    params->text = funcParams[ind]->text;
                    params->type = funcParams[ind]->type;

                    ind = ind + 1;

                    if(ind < params_count)
                    {
                        params->next = (PARAMS_FUNC*) calloc(1, sizeof(PARAMS_FUNC));
                    }
                }
                // printf("Function: %s\n", node->symbol->text);
                // for (params=node->symbol->paramsFunc; params; params = params->next)
                // {
                //     printf("paramsFunc->type: %i\n", params->type);
                //     printf("paramsFunc->text: %s\n", params->text);

                //     ind = ind + 1;
                // }
                // printf("\n");
                clear_func_params();
                break;
            }
            break;
        }
    
        default:
            for( i=0 ; i < MAX_SONS ; ++i)
                set_func_params(node->son[i]);
    }

}

void check_func_call(AST *node)
{
    int i;
    if (node == 0)
        return;

    switch (node->type)
    {
        case AST_FUNCTION_CALL:
        {
            if(node->son[0])
                check_func_call_params(node->son[0], node->symbol->paramsFunc);
            break;
        }
        default:
        {
            for( i=0 ; i < MAX_SONS ; ++i)
                check_func_call(node->son[i]);
            break;
        }
    }
}

void check_func_call_params(AST *node, PARAMS_FUNC *paramsFunc)
{

    int i;
    PARAMS_FUNC *params;
    if (node == 0)
        return;

    switch (node->type)
    {
        case AST_FUNCTION_ARGS_LIST:
        case AST_FUNCTION_ARGS:
        {
            int firstArgs = check_operands(node->son[0]);
            if(paramsFunc)
            {
                if(isNumber(firstArgs) && isNumber(paramsFunc->type))
                {
                    
                } else if(isBool(firstArgs) && isBool(paramsFunc->type)){

                } else {
                    fprintf(stderr, "Semantic ERROR: arguments of function call has the type different from function declaration\n");
                    ++ SemanticErrors;  
                }
            } else {
                if(!node->son[0])
                {
                    fprintf(stderr, "Semantic ERROR: Declared function does not have parameters, but is called with paremeters\n");
                    ++ SemanticErrors;
                } 
            }
            if(!(node->son[1]) && paramsFunc && paramsFunc->next)
            {
                fprintf(stderr, "Semantic ERROR: less parameters on func call than func declaration\n");
                ++ SemanticErrors;
            }
            if(node->son[1] && paramsFunc && !paramsFunc->next)
            {
                fprintf(stderr, "Semantic ERROR: more parameters on func call than func declaration\n");
                ++ SemanticErrors;
            }
            if(paramsFunc)
                check_func_call_params(node->son[1], paramsFunc->next);
            break;
        }
        default:
        {
            break;
        }
    }  
}

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
                int litList = check_operands(node->son[2]);

                if(isInt(return_symbol(node->symbol)))
                {
                    if(!(isInt(litList)))
                    {
                        fprintf(stderr, "Semantic ERROR: variable '%s' is declared as int but is not initialized with int values\n", node->symbol->text);
                        ++ SemanticErrors;
                    }
                }
                if(isBool(return_symbol(node->symbol)))
                {
                    if(!(isBool(litList)))
                    {
                        fprintf(stderr, "Semantic ERROR: variable '%s' is declared as bool but is not initialized with bool values\n", node->symbol->text);
                        ++ SemanticErrors;
                    }
                }
                if(isFloat(return_symbol(node->symbol)))
                {
                    if(!(isFloat(litList)))
                    {
                        fprintf(stderr, "Semantic ERROR: variable '%s' is declared as float but is not initialized with float values\n", node->symbol->text);
                        ++ SemanticErrors;
                    }
                }
                if(isChar(return_symbol(node->symbol)))
                {
                    if(!(isChar(litList)))
                    {
                        fprintf(stderr, "Semantic ERROR: variable '%s' is declared as char but is not initialized with char values\n", node->symbol->text);
                        ++ SemanticErrors;
                    }
                }
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
                if(isBool(node->son[1]->symbol->type))
                {
                    if(isInt(return_symbol(node->symbol)))
                    {
                        fprintf(stderr, "scalar variable '%s' is declared as int but is initialized with bool values\n", node->symbol->text);
                        ++ SemanticErrors;   
                        }
                    if(isFloat(return_symbol(node->symbol)))
                    {
                        fprintf(stderr, "scalar variable '%s' is declared as float but is initialized with bool values\n", node->symbol->text);
                        ++ SemanticErrors;   
                    }
                }
                if(isNumber(node->son[1]->symbol->type))
                {
                    if(isBool(return_symbol(node->symbol)))
                    {
                        fprintf(stderr, "scalar variable '%s' is declared as bool but is initialized with number values\n", node->symbol->text);
                        ++ SemanticErrors;   
                    }
                }
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

int isFloat(int type)
{
    return (type == DATATYPE_FLOAT || type == LIT_FLOAT);
}

int isChar(int type)
{
    return (type == DATATYPE_CHAR || type == KW_CHAR);
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
        case AST_LOOP:
        {
            if (node->symbol->type == TK_FUNCTION)
            {
                fprintf(stderr, "Semantic ERROR: identifier in 'loop' method cannot be a function\n");
                ++ SemanticErrors;               
            }
            break;
        }
        case AST_WHILE:
        {
            int exprType = check_operands(node->son[0]);
            check_commands(node->son[1]);

            if (!(isBool(exprType)))
            {
                fprintf(stderr, "Semantic ERROR: the condition of 'while' method is not bool\n");
                ++ SemanticErrors;      
            }

            break;
        }
        case AST_IF_ELSE:
        case AST_IF:
        {
            int exprType = check_operands(node->son[0]);
            check_commands(node->son[1]);
            
            if(node->son[2])
                check_commands(node->son[2]);

            if (!(isBool(exprType)))
            {
                fprintf(stderr, "Semantic ERROR: the condition of 'if' method is not bool\n");
                ++ SemanticErrors;      
            }

            break;
        }
        case AST_READ:
        {
            int identifierType = return_symbol(node->symbol);
            if (node->symbol->type == TK_FUNCTION)
            {
                fprintf(stderr, "Semantic ERROR: identifier of READ method is not a scalar or vector variable\n");
                ++ SemanticErrors;               
            }

            break;
        }
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
        case AST_LIT_LIST:
        {

            int litList = check_operands(node->son[0]);
            int litListRest = check_operands(node->son[1]);
            
            if(!node->son[1])
            {
                return litList;
            } else
            {
                if(isInt(litList) && isInt(litListRest))
                    return LIT_INTEGER;
                if(isBool(litList) && isBool(litListRest))
                    return DATATYPE_BOOL;
                if(isFloat(litList) && isFloat(litListRest))
                    return DATATYPE_FLOAT;
                if(isChar(litList) && isChar(litListRest))
                    return DATATYPE_FLOAT;
            }
        }
        case AST_DIV:
        case AST_MULT:
        case AST_SUB:
        case AST_ADD:
        {
            int leftOperand = check_operands(node->son[0]);
            int rightOperand = check_operands(node->son[1]);

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
        case AST_OR:
        {
            int leftOperand = check_operands(node->son[0]);
            int rightOperand = check_operands(node->son[1]);

            if(!(isBool(leftOperand)))
            {
                fprintf(stderr, "Semantic ERROR: invalid left operand for bool expr\n");
                ++ SemanticErrors;
            }
            if(!(isBool(rightOperand)))
            {
                fprintf(stderr, "Semantic ERROR: invalid right operand for bool expr\n");
                ++ SemanticErrors;
            }
            if (!check_valid_for_bool_expr(node->son[0]))
            {
                fprintf(stderr, "Semantic ERROR: invalid expr on the left side\n");
                ++ SemanticErrors;               
            }
            if (!check_valid_for_bool_expr(node->son[1]))
            {
                fprintf(stderr, "Semantic ERROR: invalid expr on the right side\n");
                ++ SemanticErrors;               
            }

            return DATATYPE_BOOL;
        }   
        case AST_GRT:
        case AST_LESS:
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