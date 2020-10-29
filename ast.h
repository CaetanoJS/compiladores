// AST - Abstract Syntax Tree
#include "hash.h"

#ifndef AST_HEADER
#define AST_HEADER

#define MAX_SONS 4

#define AST_SYMBOL 1
#define AST_ADD 2
#define AST_SUB 3
#define AST_MULT 4
#define AST_GRT 5
#define AST_LESS 6
#define AST_OR 7
#define AST_POW 8
#define AST_VECTOR 9
#define AST_LE 10
#define AST_GE 11
#define AST_EQ 12
#define AST_DIF 13
#define AST_DIV 14
#define AST_PRINT_ARGS_LIST 15
#define AST_FUNCTION_ARGS_LIST 16
#define AST_FUNCTION_CALL 19
#define AST_FUNCTION_ARGS 20
#define AST_VAR_ATTRIBUTION 21
#define AST_VECTOR_ATTRIBUTION 22
#define AST_PRINT 23
#define AST_PRINT_ARGS 24
#define AST_READ 26
#define AST_RETURN 27
#define AST_IF 28
#define AST_IF_ELSE 29
#define AST_WHILE 30
#define AST_LOOP 31
#define AST_CMD_BLOCK 32
#define AST_CMD_LIST 33
#define AST_FUNCTION_DECL 34
#define AST_BLOCK 35
#define AST_PARAMS_FUNC 36
#define AST_PARAMS_FUNC_LIST 37
#define AST_TYPE_CHAR 38
#define AST_TYPE_INT 39
#define AST_TYPE_FLOAT 40
#define AST_TYPE_BOOL 41
#define AST_DECLARATION 42
#define AST_VARIABLE_DECL 43
#define AST_VECTOR_DECL_WITH_PARAM 44
#define AST_LIT_LIST 45
#define AST_VECTOR_DECL 46
#define AST_PARENTHESES_EXPR 47


typedef struct astnode
{
    int type;
    HASH_NODE *symbol;
    struct astnode *son[MAX_SONS];
} AST;

AST *astCreate(int type, HASH_NODE *symbol , 
                AST* s0, AST* s1, AST* s2, AST* s3);
void astPrint(AST *node, int level);
void astToFile(AST *node, FILE *fileTree);


#endif

// END OF FILE