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

    // if ((outputFile = fopen(argv[2], "w")) == 0)
    // {
    //     fprintf(stderr, "Cannot open file %s\n", argv[2]);

    //     exit(4);
    // }

    yyparse();

    // astPrint(getAST(), 0);
    hashPrint();
    // astToFile(getAST(), outputFile);
    fprintf(stderr, "Super! Compilation successful! \n");
    exit(0);
}