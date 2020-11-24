/* Caetano Jaeger Stradolini */

#include <stdio.h>
#include <stdlib.h>
#include "hash.h"
#include "ast.h"

extern FILE *yyin;
int yyparse();

void astToFile(AST* node, FILE *fileTree);
void astPrint(AST *node, int level);
AST* getAST(void);


int main(int argc, char ** argv)
{
    hashInit();

    FILE *outputFile;

    if(argc<2)
    {
        fprintf(stderr, "Call: etapa1 filename");
        exit(1);
    }
    yyin = fopen(argv[1], "r");
    if (yyin==0)
    {
        fprintf(stderr, "Cannot open file %s\n",argv[1]);
        exit(2);
    }

    yyparse();

    //astPrint(getAST(), 0); // Etapa 3
    hashPrint();
    // astToFile(getAST(), outputFile); // Etapa 3
    check_and_set_declarations(getAST()); // Etapa 4
    check_undeclared(); // Etapa 4
    check_commands(getAST()); // Etapa 4
    set_func_params(getAST()); // Etapa 4
    check_func_call(getAST()); // Etapa 4
    check_syntax_errors(); // Etapa 4
    tacPrintBackwards(generateCode(getAST())); // Etapa 5
    fprintf(stderr, "Super! Compilation successful! \n");
    exit(0);
}