/* Caetano Jaeger Stradolini */

int main(int argc, char ** argv)
{
    hashInit();
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
    int tok;

    yyparse();

    hashPrint();
    fprintf(stderr, "Super! Compilation successful! \n");
    exit(0);
}