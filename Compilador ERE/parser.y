%token KW_CHAR       
%token KW_INT        
%token KW_FLOAT      
%token KW_BOOL       

%token KW_IF         
%token KW_THEN       
%token KW_ELSE       
%token KW_WHILE      
%token KW_LOOP       
%token KW_READ       
%token KW_PRINT      
%token KW_RETURN     

%token OPERATOR_LE   
%token OPERATOR_GE   
%token OPERATOR_EQ   
%token OPERATOR_DIF  
%token TK_IDENTIFIER 

%token LIT_INTEGER   
%token LIT_FLOAT     
%token LIT_TRUE      
%token LIT_FALSE     
%token LIT_CHAR      
%token LIT_STRING    
%token TOKEN_ERROR

%left '|' '~'
%left '<' '>' OPERATOR_LE OPERATOR_GE OPERATOR_EQ OPERATOR_DIF
%left '+' '-'
%left '*' '/' '^'


%%

programa: decl
    ;
    
decl: dec ';' decl
    |
    ;

dec: TK_IDENTIFIER '=' types ':' literals
    | TK_IDENTIFIER '=' types '[' LIT_INTEGER ']' ':' litList
    | TK_IDENTIFIER '=' types '[' LIT_INTEGER ']'
    | TK_IDENTIFIER '(' paramsFunc ')' '=' types block
    ;

block: '{' commandList '}'
    ;

commandList: cmd commandList
    |
    ;

cmd: TK_IDENTIFIER '=' expr
    | TK_IDENTIFIER '[' expr ']' '=' expr
    | KW_PRINT printList
    | KW_READ TK_IDENTIFIER
    | KW_RETURN expr
    | KW_IF '(' expr ')' KW_THEN cmd
    | KW_IF '(' expr ')' KW_THEN cmd KW_ELSE cmd
    | KW_WHILE '(' expr ')' cmd
    | KW_LOOP '(' TK_IDENTIFIER ':' expr ',' expr ',' expr ')' cmd
    | block
    |
    ;

printList: printCmd restPrintList
    ;

restPrintList: ',' printCmd restPrintList
    |
    ;

printCmd: LIT_STRING
    | expr
    ;

expr: TK_IDENTIFIER
    | TK_IDENTIFIER '[' expr ']'
    | TK_IDENTIFIER '(' funcArgs ')'
    | literals
    | expr '+' expr
    | expr '-' expr
    | expr '*' expr
    | expr '/' expr
    | expr '>' expr
    | expr '<' expr
    | expr '|' expr
    | expr '^' expr
    | expr '~' expr
    | expr OPERATOR_LE expr
    | expr OPERATOR_GE expr
    | expr OPERATOR_EQ expr
    | expr OPERATOR_DIF expr
    | '(' expr ')'
    ;

funcArgs: expr restFuncArgs
    |
    ;

restFuncArgs: ',' expr restFuncArgs
    |
    ;

paramsFunc: params paramsResto
    |
    ;

paramsResto: ',' params paramsResto
    |
    ;

params: TK_IDENTIFIER '=' types
    ;

types:
    | KW_CHAR
    | KW_INT
    | KW_FLOAT
    | KW_BOOL
    ;

litList: literals litList
    |
    ;

literals: LIT_CHAR
    | LIT_FALSE
    | LIT_TRUE
    | LIT_FLOAT
    | LIT_INTEGER
    ;

%%

int yyerror()
{
    fprintf(stderr, "Syntax error at line %d.\n", getLineNumber());
    exit(3);
}

