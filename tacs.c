#include "tacs.h"

TAC* tacCreate(int type, HASH_NODE *res, HASH_NODE *op1, HASH_NODE *op2)
{
    TAC* newtac = 0;
    newtac = (TAC*) calloc(1, sizeof(TAC));
    newtac->type = type;
    newtac->res = res;
    newtac->op1 = op1;
    newtac->op2 = op2;
    newtac->prev = 0;
    newtac->next = 0;
    return newtac;

}
void tacPrint(TAC* tac)
{
    if(!tac)
        return;
    if(tac->type == TAC_SYMBOL)
        return;

    fprintf(stderr, "TAC(");
    switch (tac->type)
    {
        case TAC_JUMP:
        {
            fprintf(stderr, "TAC_JUMP");
            break;   
        }
        case TAC_PRINT_ARGS:
        {
            fprintf(stderr, "TAC_PRINT_ARGS");
            break;          
        }
        case TAC_PRINT:
        {
            fprintf(stderr, "TAC_PRINT");
            break;        
        }
        case TAC_END_FUN:
        {
            fprintf(stderr, "TAC_END_FUN");
            break;   
        }
        case TAC_FUNC_DECL:
        {
            fprintf(stderr, "TAC_FUNC_DECL");
            break;      
        }
        case TAC_FUNC_PARAMS_DECL:
        {
            fprintf(stderr, "TAC_FUNC_PARAMS_DECL");
            break;    
        }
        case TAC_BEGIN_FUN:
        {
            fprintf(stderr, "TAC_BEGIN_FUN");
            break;   
        }
        case TAC_VAR_VECTOR_INIT_ATTR:
        {
            fprintf(stderr, "TAC_VAR_VECTOR_INIT_ATTR");
            break;     
        }
        case TAC_VECTOR_INIT_PARAMS:
        {
            fprintf(stderr, "TAC_VECTOR_INIT_PARAMS");
            break;          
        }
        case TAC_VAR_VECTOR_INIT:
        {
            fprintf(stderr, "TAC_VAR_VECTOR_INIT");
            break; 
        }
        case TAC_VAR_INIT_ATTR:
        {
            fprintf(stderr, "TAC_VAR_INIT_ATTR");
            break;   
        }
        case TAC_COPY_VECTOR:
        {
            fprintf(stderr, "TAC_COPY_VECTOR");
            break;   
        }
        case TAC_READ:
        {
            fprintf(stderr, "TAC_READ");
            break;   
        }
        case TAC_JFALSE:
        {
            fprintf(stderr, "TAC_JFALSE");
            break;   
        }
        case TAC_LABEL:
        {
            fprintf(stderr, "TAC_LABEL");
            break;   
        }
        case TAC_SYMBOL:
        {
            fprintf(stderr, "TAC_SYMBOL");
            break;
        }
        case TAC_VECTOR:
        {
            fprintf(stderr, "TAC_VECTOR");
            break;
        }
        case TAC_SUB:
        {
            fprintf(stderr, "TAC_SUB");
            break;
        }
        case TAC_ADD:
        {
            fprintf(stderr, "TAC_ADD");
            break;
        }
        case TAC_MULT:
        {
            fprintf(stderr, "TAC_MULT");
            break;
        }
        case TAC_DIV:
        {
            fprintf(stderr, "TAC_DIV");
            break;
        }
        case TAC_GTR:
        {
            fprintf(stderr, "TAC_GTR");
            break;
        }
        case TAC_LESS:
        {
            fprintf(stderr, "TAC_LESS");
            break;
        }
        case TAC_OR:
        {
            fprintf(stderr, "TAC_OR");
            break;
        }
        case TAC_POW:
        {
            fprintf(stderr, "TAC_POW");
            break;
        }
        case TAC_LE:
        {
            fprintf(stderr, "TAC_LE");
            break;
        }
        case TAC_GE:
        {
            fprintf(stderr, "TAC_GE");
            break;
        }
        case TAC_EQ:
        {
            fprintf(stderr, "TAC_EQ");
            break;
        }
        case TAC_DIFF:
        {
            fprintf(stderr, "TAC_DIFF");
            break;
        }
        case TAC_COPY:
        {
            fprintf(stderr, "TAC_COPY");
            break;
        }
        default:
        {
            fprintf(stderr, "TAC_UNKNOW");
            break;
        }
    }
    fprintf(stderr,",%s", (tac->res)?tac->res->text:"0");
    fprintf(stderr,",%s", (tac->op1)?tac->op1->text:"0");
    fprintf(stderr,",%s", (tac->op2)?tac->op2->text:"0");
    fprintf(stderr, ");\n");
}
void tacPrintBackwards(TAC *tac)
{
    if(!tac)
        return;
    else
    {
        tacPrintBackwards(tac->prev);
        tacPrint(tac);
    }
    
}

// CODE GENERATION

TAC* tacJoin(TAC* l1, TAC* l2)
{
    TAC *point;
    if(!l1) return l2;
    if(!l2) return l1;

    for (point = l2; point->prev !=0; point = point->prev)
        ;
    point->prev = l1;
    return l2;
}

TAC* makeBinaryOperation(int type, TAC *code[])
{
    tacJoin(
        tacJoin(code[0],code[1]), 
            tacCreate(type, makeTemp(), 
                        code[0]?code[0]->res:0, 
                        code[1]?code[1]->res:0));
}

TAC* generateCode(AST *node)
{
    int i;
    TAC *result = 0;
    TAC *code[MAX_SONS];

    if(!node)
        return 0;

    for(i=0; i<MAX_SONS; ++i)
        code[i] = generateCode(node->son[i]);

    switch (node->type)
    {
        case AST_VARIABLE_DECL:
        {
            return tacJoin(code[1], tacCreate(TAC_VAR_INIT_ATTR, node->symbol, code[1]?code[1]->res:0, 0));
        }
        case AST_PRINT:
        {
            return tacJoin(code[0], tacCreate(TAC_PRINT, 0, 0, 0));
        }
        case AST_PRINT_ARGS:
        {
            return tacJoin(tacJoin(code[0], tacCreate(TAC_PRINT_ARGS, code[0]->res, 0, 0)), code[1]);
        }
        case AST_PRINT_ARGS_LIST:
        {
            return tacJoin(tacJoin(code[0], tacCreate(TAC_PRINT_ARGS, code[0]->res, 0, 0)), code[1]);
        }
        case AST_CMD_BLOCK:
        {
            HASH_NODE *beginLabel = 0;
            HASH_NODE *endLabel = 0;
            TAC *beginFuncTac = 0;
            TAC *endFuncTac = 0;
            beginLabel = makeLabel();
            endLabel = makeLabel();
            beginFuncTac = tacCreate(TAC_BEGIN_FUN, beginLabel, 0, 0);
            endFuncTac = tacCreate(TAC_END_FUN, endLabel, 0, 0);
            return tacJoin(beginFuncTac, tacJoin(code[0], endFuncTac));
        }
        case AST_FUNCTION_DECL:
        {
            return tacJoin(code[0], tacJoin(tacCreate(TAC_FUNC_DECL, node->symbol, 0, 0), code[2]));
        }
        case AST_PARAMS_FUNC:
        {
            return tacJoin(tacCreate(TAC_FUNC_PARAMS_DECL, node->symbol, 0, 0),code[1]);
        }
        case AST_PARAMS_FUNC_LIST:
        {
            return tacJoin(tacCreate(TAC_FUNC_PARAMS_DECL, node->symbol, 0, 0),code[1]);
        }
        case AST_VECTOR_DECL_WITH_PARAM:
        {
            return tacJoin(code[2], tacCreate(TAC_VAR_VECTOR_INIT_ATTR, node->symbol, code[1]?code[1]->res:0, 0));
            break;   
        }
        case AST_LIT_LIST:
        {
            return tacJoin(tacCreate(TAC_VECTOR_INIT_PARAMS, code[0]->res, 0, 0),code[1]);
            break;
        }
        case AST_VECTOR_DECL:
        {
            return tacJoin(code[1], tacCreate(TAC_VAR_VECTOR_INIT, node->symbol, code[1]?code[1]->res:0, 0));
        }
        case AST_IF:
        {
            TAC *jumpTac = 0;
            TAC *labelTac = 0;
            HASH_NODE *newLabel = 0;

            newLabel = makeLabel();
            jumpTac = tacCreate(TAC_JFALSE, newLabel, code[0]?code[0]->res:0, 0);
            jumpTac->prev = code[0];
            labelTac = tacCreate(TAC_LABEL, newLabel, 0, 0);
            labelTac->prev = code[1];

            return tacJoin(jumpTac, labelTac);

            
            break;
        }
        case AST_READ:
        {
            return tacJoin(code[0], tacCreate(TAC_READ, node->symbol, 0, 0));
            break;
        }
        case AST_VECTOR_ATTRIBUTION:
        {
            result = tacJoin(code[0], 
                        tacJoin(code[1],
                            tacCreate(TAC_COPY_VECTOR, 
                                  node->symbol, 
                                  code[0]?code[0]->res:0, code[1]?code[1]->res:0)));
            break;
        }
        case AST_IF_ELSE:
        {
            TAC *jumpTac = 0;
            TAC *jumpTacInconditional = 0;
            TAC *labelTac = 0;
            TAC *labelTacFinal = 0;
            HASH_NODE *newLabel = 0;
            HASH_NODE *newLabelFinal = 0;

            newLabel = makeLabel();
            newLabelFinal = makeLabel();
            jumpTac = tacCreate(TAC_JFALSE, newLabel, code[0]?code[0]->res:0, 0);
            jumpTac->prev = code[0];
            jumpTacInconditional = tacCreate(TAC_JUMP, newLabelFinal, 0, 0);
            jumpTacInconditional->prev = code[1];
            labelTac = tacCreate(TAC_LABEL, newLabel, 0, 0);
            labelTac->prev = jumpTacInconditional;
            labelTacFinal = tacCreate(TAC_LABEL, newLabelFinal, 0, 0);
            labelTacFinal->prev = code[2];

            return tacJoin(jumpTac, tacJoin(labelTac, labelTacFinal));
            break;
        }
        case AST_VECTOR:
        {
            result = tacJoin(code[0], tacCreate(TAC_VECTOR, node->symbol, code[0]?code[0]->res:0, 0));
            break;
        }
        case AST_SYMBOL:
        {
            result = tacCreate(TAC_SYMBOL, node->symbol, 0, 0);
            break;
        }
        case AST_ADD:
        {
            result = makeBinaryOperation(TAC_ADD, code);
            break;
        }
        case AST_SUB:
        {
            result = makeBinaryOperation(TAC_SUB, code);
            break;
        }
        case AST_MULT:
        {
            result = makeBinaryOperation(TAC_MULT, code);
            break;
        }
        case AST_DIV:
        {
            result = makeBinaryOperation(TAC_DIV, code);
            break;
        }
        case AST_GRT:
        {
            result = makeBinaryOperation(TAC_GTR, code);
            break;
        }
        case AST_LESS:
        {
            result = makeBinaryOperation(TAC_LESS, code);
            break;
        }
        case AST_OR:
        {
            result = makeBinaryOperation(TAC_OR, code);
            break;
        }
        case AST_POW:
        {
            result = makeBinaryOperation(TAC_POW, code);
            break;
        }
        case AST_LE:
        {
            result = makeBinaryOperation(TAC_LE, code);
            break;
        }
        case AST_GE:
        {
            result = makeBinaryOperation(TAC_GE, code);
            break;
        }
        case AST_EQ:
        {
            result = makeBinaryOperation(TAC_EQ, code);
            break;
        }
        case AST_DIF:
        {
            result = makeBinaryOperation(TAC_DIFF, code);
            break;
        }
        case AST_VAR_ATTRIBUTION:
        {
            result = tacJoin(code[0], 
                        tacCreate(TAC_COPY, 
                                  node->symbol, 
                                  code[0]?code[0]->res:0, 0));
            break;
        }
        default:
        {
            result = tacJoin(code[0], tacJoin(code[1], tacJoin(code[2], code[3])));
            break;
        }
    }

    return result;
}