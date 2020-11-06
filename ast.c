/* AST - Abstract Syntax Tree */
/* Caetano Jaeger Stradolini */

#include <stdio.h>
#include <stdlib.h>
#include "ast.h"

int level = 0;
void ident(FILE *f);

AST *astCreate(int type, HASH_NODE *symbol , 
                AST* s0, AST* s1, AST* s2, AST* s3)
{
    AST*newnode;
    newnode = (AST*) calloc(1,sizeof(AST));
    newnode->type = type;
    newnode->symbol = symbol;
    newnode->son[0] = s0;
    newnode->son[1] = s1;
    newnode->son[2] = s2;
    newnode->son[3] = s3;
    return newnode;
}
void astPrint(AST *node, int level)
{
    int i = 0;
    if (node == 0)
        return;

    for (i=0; i<level; ++i)
        fprintf(stderr,"  ");

    fprintf(stderr, "ast(");
    switch (node->type)
    {
    case AST_SYMBOL: fprintf(stderr, "AST_SYMBOL"); break;
    case AST_ADD: fprintf(stderr, "AST_ADD"); break;
    case AST_SUB: fprintf(stderr, "AST_SUB"); break;
    case AST_MULT: fprintf(stderr, "AST_MULT"); break;
    case AST_GRT: fprintf(stderr, "AST_GRT"); break;
    case AST_LESS: fprintf(stderr, "AST_LESS"); break;
    case AST_OR: fprintf(stderr, "AST_OR"); break;
    case AST_POW: fprintf(stderr, "AST_POW"); break;
    case AST_VECTOR: fprintf(stderr, "AST_VECTOR"); break;
    case AST_FUNCTION_ARGS: fprintf(stderr, "AST_FUNCTION_ARGS"); break;
    case AST_FUNCTION_ARGS_LIST: fprintf(stderr, "AST_FUNCTION_ARGS_LIST"); break;
    case AST_LE: fprintf(stderr, "AST_LE"); break;
    case AST_GE: fprintf(stderr, "AST_GE"); break;
    case AST_EQ: fprintf(stderr, "AST_EQ"); break;
    case AST_DIF: fprintf(stderr, "AST_DIF"); break;
    case AST_DIV: fprintf(stderr, "AST_DIV"); break;
    case AST_FUNCTION_CALL: fprintf(stderr, "AST_FUNCTION_CALL"); break;
    case AST_VAR_ATTRIBUTION: fprintf(stderr, "AST_VAR_ATTRIBUTION"); break;
    case AST_VECTOR_ATTRIBUTION: fprintf(stderr, "AST_VECTOR_ATTRIBUTION"); break;
    case AST_PRINT: fprintf(stderr, "AST_PRINT"); break;
    case AST_PRINT_ARGS: fprintf(stderr, "AST_PRINT_ARGS"); break;
    case AST_READ: fprintf(stderr, "AST_READ"); break;
    case AST_RETURN: fprintf(stderr, "AST_RETURN"); break;
    case AST_IF: fprintf(stderr, "AST_IF"); break;
    case AST_IF_ELSE: fprintf(stderr, "AST_IF_ELSE"); break;
    case AST_WHILE: fprintf(stderr, "AST_WHILE"); break;
    case AST_LOOP: fprintf(stderr, "AST_LOOP"); break;
    case AST_CMD_BLOCK: fprintf(stderr, "AST_CMD_BLOCK"); break;
    case AST_CMD_LIST: fprintf(stderr, "AST_CMD_LIST"); break;
    case AST_FUNCTION_DECL: fprintf(stderr, "AST_FUNCTION_DECL"); break;
    case AST_BLOCK: fprintf(stderr, "AST_BLOCK"); break;
    case AST_PARAMS_FUNC: fprintf(stderr, "AST_PARAMS_FUNC"); break;
    case AST_PARAMS_FUNC_LIST: fprintf(stderr, "AST_PARAMS_FUNC_LIST"); break;
    case AST_TYPE_CHAR: fprintf(stderr, "AST_TYPE_CHAR"); break;
    case AST_TYPE_INT: fprintf(stderr, "AST_TYPE_INT"); break;
    case AST_TYPE_FLOAT: fprintf(stderr, "AST_TYPE_FLOAT"); break;
    case AST_TYPE_BOOL: fprintf(stderr, "AST_TYPE_BOOL"); break;
    case AST_DECLARATION: fprintf(stderr, "AST_DECLARATION"); break;
    case AST_VARIABLE_DECL: fprintf(stderr, "AST_VARIABLE_DECL"); break;
    case AST_VECTOR_DECL_WITH_PARAM: fprintf(stderr, "AST_VECTOR_DECL_WITH_PARAM"); break;
    case AST_LIT_LIST: fprintf(stderr, "AST_LIT_LIST"); break;
    case AST_VECTOR_DECL: fprintf(stderr, "AST_VECTOR_DECL"); break;
    case AST_PRINT_ARGS_LIST: fprintf(stderr, "AST_PRINT_ARGS_LIST"); break;
    case AST_PARENTHESES_EXPR: fprintf(stderr, "AST_PARENTHESES_EXPR"); break;

    default: fprintf(stderr,"AST_UNKNOWN"); break;
    }
    if (node->symbol !=0)
        fprintf(stderr, ",%s\n", node->symbol->text);
    else
        fprintf(stderr, ",0\n");
    for (i=0; i<MAX_SONS;i++)
        astPrint(node->son[i], level+1);
    for (i=0; i<level; ++i)
        fprintf(stderr,"  ");

    fprintf(stderr, ")\n");
}


void astToFile(AST* node, FILE *f)
{
    int i = 0;


    if(node == 0)
        return;

    switch(node->type)
    {
        case AST_SYMBOL: {
            fprintf(f, " %s", node->symbol->text);
            break;
        }
        case AST_TYPE_INT: {
            fprintf(f, "int");
            break;
        }
        case AST_TYPE_CHAR: {
            fprintf(f, "char");
            break;
        }
        case AST_TYPE_FLOAT: {
            fprintf(f, "float");
            break;
        }
        case AST_TYPE_BOOL: {
            fprintf(f, "bool");
            break;
        }
        case AST_DECLARATION:
        case AST_LIT_LIST:
        case AST_CMD_LIST:
        case AST_PRINT_ARGS:
        case AST_FUNCTION_ARGS: {
            for(i = 0; i < MAX_SONS; i++)
                astToFile(node->son[i], f);
            break;
        }
        case AST_VARIABLE_DECL: {
            fprintf(f, "%s",node->symbol->text);
            fprintf(f, " = ");

            astToFile(node->son[0], f);
            fprintf(f, " :");

            astToFile(node->son[1], f);
            fprintf(f, ";\n");
            break;
        }
        case AST_PARAMS_FUNC: {
            fprintf(f, "%s",node->symbol->text);
            fprintf(f, " = ");

            astToFile(node->son[0], f);
            astToFile(node->son[1], f);
            break;
        }
        case AST_PARAMS_FUNC_LIST: {
            fprintf(f, " , ");
            fprintf(f, "%s",node->symbol->text);
            fprintf(f, " = ");

            astToFile(node->son[0], f);
            astToFile(node->son[1], f);
            break;
        }
        case AST_VECTOR_DECL_WITH_PARAM: {
            fprintf(f, "%s",node->symbol->text);
            fprintf(f, " = ");

            astToFile(node->son[0], f);
            fprintf(f, " [");
            astToFile(node->son[1], f);
            fprintf(f, "]");
            fprintf(f, " :");

            astToFile(node->son[2], f);
            fprintf(f, ";\n");
            break;
        }
        case AST_VECTOR_DECL: {
            fprintf(f, "%s",node->symbol->text);
            fprintf(f, " = ");

            astToFile(node->son[0], f);
            fprintf(f, " [");
            astToFile(node->son[1], f);
            fprintf(f, "]");
            fprintf(f, ";\n");
            break;
        }
        case AST_FUNCTION_DECL: {
            fprintf(f, "\n");
            fprintf(f, "%s",node->symbol->text);

            fprintf(f, " (");
            astToFile(node->son[0], f);
            fprintf(f, ")");

            fprintf(f, " = ");

            astToFile(node->son[1], f);
            astToFile(node->son[2], f);
            fprintf(f, ";\n");
            break;
        }
        case AST_CMD_BLOCK: {
            fprintf(f, " {");
            level++;
            astToFile(node->son[0], f);
            fprintf(f, "\n");
            level--;
            ident(f);
            fprintf(f, "}");
            break;
        }
        case AST_VAR_ATTRIBUTION: {
            fprintf(f, "\n");
            ident(f);
            fprintf(f, "%s",node->symbol->text);
            fprintf(f, " =");
            astToFile(node->son[0], f);
            break;
        }
        case AST_VECTOR_ATTRIBUTION: {
            fprintf(f, "\n");
            ident(f);
            fprintf(f, "%s",node->symbol->text);
            fprintf(f, " [");
            astToFile(node->son[0], f);
            fprintf(f, "]");
            fprintf(f, " =");
            astToFile(node->son[1], f);
            break;
        }
        case AST_PRINT: {
            fprintf(f, "\n");
            ident(f);
            fprintf(f, "print ");
            astToFile(node->son[0], f);
            break;
        }
        case AST_READ: {
            fprintf(f, "\n");
            ident(f);
            fprintf(f, "read ");
            fprintf(f, "%s",node->symbol->text);
            break;
        }
        case AST_RETURN: {
            fprintf(f, "\n");
            ident(f);
            fprintf(f, "return");
            astToFile(node->son[0], f);
            break;
        }
        case AST_IF: {
            fprintf(f, "\n");
            fprintf(f, "\n");
            ident(f);
            fprintf(f, "if ");
            fprintf(f, "(");
            astToFile(node->son[0], f);
            fprintf(f, " )");
            fprintf(f, " then");
            level++;
            astToFile(node->son[1], f);
            level--;
            break;
        }
        case AST_IF_ELSE: {
            fprintf(f, "\n");
            fprintf(f, "\n");
            ident(f);
            fprintf(f, "if ");
            fprintf(f, "(");
            astToFile(node->son[0], f);
            fprintf(f, " )");
            fprintf(f, " then");
            astToFile(node->son[1], f);
            fprintf(f, " else");
            astToFile(node->son[2], f);
            break;
        }
        case AST_WHILE: {
            fprintf(f, "\n");
            fprintf(f, "\n");
            ident(f);
            fprintf(f, "while ");
            fprintf(f, "(");
            astToFile(node->son[0], f);
            fprintf(f, " )");
            level++;
            astToFile(node->son[1], f);
            level--;
            break;
        }
        case AST_LOOP: {
            fprintf(f, "\n");
            fprintf(f, "\n");
            ident(f);
            fprintf(f, "loop ");
            fprintf(f, "(");
            fprintf(f, "%s",node->symbol->text);
            fprintf(f, " :");
            astToFile(node->son[0], f);
            fprintf(f, ",");
            astToFile(node->son[1], f);
            fprintf(f, ",");
            astToFile(node->son[2], f);
            fprintf(f, ")");
            level++;
            astToFile(node->son[3], f);
            level--;
            break;
        }
        case AST_VECTOR: {
            fprintf(f, "%s",node->symbol->text);
            fprintf(f, "[");
            astToFile(node->son[0], f);
            fprintf(f, "]");
            break;
        }
        case AST_FUNCTION_CALL: {
            fprintf(f, " %s",node->symbol->text);
            fprintf(f, "(");
            astToFile(node->son[0], f);
            fprintf(f, ")");
            break;
        }
        case AST_ADD: {
            astToFile(node->son[0], f);
            fprintf(f, " +");
            astToFile(node->son[1], f);
            break;
        }
        case AST_SUB: {
            astToFile(node->son[0], f);
            fprintf(f, " -");
            astToFile(node->son[1], f);
            break;
        }
        case AST_DIV: {
            astToFile(node->son[0], f);
            fprintf(f, " /");
            astToFile(node->son[1], f);
            break;
        }
        case AST_MULT: {
            astToFile(node->son[0], f);
            fprintf(f, " *");
            astToFile(node->son[1], f);
            break;
        }
        case AST_POW: {
            astToFile(node->son[0], f);
            fprintf(f, " ^");
            astToFile(node->son[1], f);
            break;
        }
        case AST_OR: {
            astToFile(node->son[0], f);
            fprintf(f, " |");
            astToFile(node->son[1], f);
            break;
        }
        case AST_GRT: {
            astToFile(node->son[0], f);
            fprintf(f, " >");
            astToFile(node->son[1], f);
            break;
        }
        case AST_LESS: {
            astToFile(node->son[0], f);
            fprintf(f, " <");
            astToFile(node->son[1], f);
            break;
        }
        case AST_LE: {
            astToFile(node->son[0], f);
            fprintf(f, " <=");
            astToFile(node->son[1], f);
            break;
        }
        case AST_GE: {
            astToFile(node->son[0], f);
            fprintf(f, " >=");
            astToFile(node->son[1], f);
            break;
        }
        case AST_EQ: {
            astToFile(node->son[0], f);
            fprintf(f, " ==");
            astToFile(node->son[1], f);
            break;
        }
        case AST_DIF: {
            astToFile(node->son[0], f);
            fprintf(f, " !=");
            astToFile(node->son[1], f);
            break;
        }
        case AST_PARENTHESES_EXPR: {
            fprintf(f, " (");
            astToFile(node->son[0], f);
            fprintf(f, " )");
            break;
        }
        case AST_FUNCTION_ARGS_LIST:
        case AST_PRINT_ARGS_LIST: {
            fprintf(f, ",");
            astToFile(node->son[0], f);
            astToFile(node->son[1], f);
            break;
        }
    }
}

void ident(FILE *f)
{
    int i = 0;
    for (i = 0; i < level; i++)
        fprintf(f, "    ");
}



// END OF FILE