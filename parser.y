/* Caetano Jaeger Stradolini */

%{
    #include "hash.h"
    #include "ast.h"
    #include "semantic.h"

    int yyerror();
    int getLineNumber();
    int yylex();            //find a better solution
    AST *root;
%}


%union
{
    HASH_NODE *symbol;
    AST *ast;
}

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
%token<symbol> TK_IDENTIFIER
%token<symbol> TK_VARIABLE
%token<symbol> TK_FUNCTION
%token<symbol> TK_VECTOR

%token<symbol> LIT_INTEGER   
%token<symbol> LIT_FLOAT     
%token<symbol> LIT_TRUE      
%token<symbol> LIT_FALSE     
%token<symbol> LIT_CHAR      
%token<symbol> LIT_STRING    
%token TOKEN_ERROR

%type<ast> expr
%type<ast> literals
%type<ast> funcArgs
%type<ast> restFuncArgs
%type<ast> cmd
%type<ast> commandList
%type<ast> printCmd
%type<ast> printList
%type<ast> restPrintList
%type<ast> block
%type<ast> dec
%type<ast> paramsFunc
%type<ast> paramsResto
%type<ast> types
%type<ast> programa
%type<ast> decl
%type<ast> litList

%left '|' '~'
%left '<' '>' OPERATOR_LE OPERATOR_GE OPERATOR_EQ OPERATOR_DIF
%left '+' '-'
%left '*' '/' '^'


%%

programa: decl { root = $$ ; check_and_set_declarations($1); check_undeclared(); check_commands($1); set_func_params($1); }
    ;
    
decl: dec ';' decl  { $$ = astCreate(AST_DECLARATION, 0, $1, $3, 0, 0); }
    |               { $$ = 0; }
    ;

dec:  TK_IDENTIFIER '(' paramsFunc ')' '=' types block          { $$ = astCreate(AST_FUNCTION_DECL, $1, $3, $6, $7, 0); }
    | TK_IDENTIFIER '=' types ':' literals                      { $$ = astCreate(AST_VARIABLE_DECL, $1, $3, $5, 0, 0); }
    | TK_IDENTIFIER '=' types '[' LIT_INTEGER ']' ':' litList   { $$ = astCreate(AST_VECTOR_DECL_WITH_PARAM, $1, $3,astCreate(AST_TYPE_INT, $5, 0, 0, 0, 0), $8, 0); }
    | TK_IDENTIFIER '=' types '[' LIT_INTEGER ']'                { $$ = astCreate(AST_VECTOR_DECL, $1, $3,astCreate(AST_TYPE_INT, $5, 0, 0, 0, 0), 0, 0); }
    ;

block: '{' commandList '}'                  { $$ = astCreate(AST_CMD_BLOCK, 0, $2, 0, 0, 0); }             
    ;

commandList: cmd commandList                { $$ = astCreate(AST_CMD_LIST, 0, $1, $2, 0, 0); }  
    |                                       { $$ = 0; }
    ;

cmd: TK_IDENTIFIER '=' expr                 { $$ = astCreate(AST_VAR_ATTRIBUTION, $1, $3, 0, 0, 0); }       
    | TK_IDENTIFIER '[' expr ']' '=' expr   { $$ = astCreate(AST_VECTOR_ATTRIBUTION, $1, $3, $6, 0, 0); }
    | KW_PRINT printList                    { $$ = astCreate(AST_PRINT, 0, $2, 0, 0, 0); }
    | KW_READ TK_IDENTIFIER                 { $$ = astCreate(AST_READ, $2, 0, 0, 0, 0); }
    | KW_RETURN expr                        { $$ = astCreate(AST_RETURN, 0, $2, 0, 0, 0); }
    | KW_IF '(' expr ')' KW_THEN cmd        { $$ = astCreate(AST_IF, 0, $3, $6, 0, 0); }
    | KW_IF '(' expr ')' KW_THEN cmd KW_ELSE cmd { $$ = astCreate(AST_IF_ELSE, 0, $3, $6, $8, 0); }
    | KW_WHILE '(' expr ')' cmd             { $$ = astCreate(AST_WHILE, 0, $3, $5, 0, 0); }
    | KW_LOOP '(' TK_IDENTIFIER ':' expr ',' expr ',' expr ')' cmd  { $$ = astCreate(AST_LOOP, $3, $5, $7, $9, $11); }
    | block                                 { $$ = $1 }
    |                                       { $$ = 0; }
    ;

printList: printCmd restPrintList           { $$ = astCreate(AST_PRINT_ARGS, 0, $1, $2, 0, 0); }
    |                                       { $$ = 0; }
    ;

restPrintList: ',' printCmd restPrintList   { $$ = astCreate(AST_PRINT_ARGS_LIST, 0, $2, $3, 0, 0); }
    |                                       { $$ = 0; }
    ;

printCmd: LIT_STRING                        { $$ = astCreate(AST_SYMBOL, $1, 0, 0, 0, 0); }
    | expr                                  { $$ = $1 }
    ;

expr: TK_IDENTIFIER                     { $$ = astCreate(AST_SYMBOL, $1, 0, 0, 0, 0); }               
    | TK_IDENTIFIER '[' expr ']'        { $$ = astCreate(AST_VECTOR, $1, $3, 0, 0, 0); }
    | TK_IDENTIFIER '(' funcArgs ')'    { $$ = astCreate(AST_FUNCTION_CALL, $1, $3, 0, 0, 0); }
    | literals                          { $$ = $1 }
    | expr '+' expr                     { $$ = astCreate(AST_ADD, 0, $1, $3, 0, 0); }             
    | expr '-' expr                     { $$ = astCreate(AST_SUB, 0, $1, $3, 0, 0); }                   
    | expr '*' expr                     { $$ = astCreate(AST_MULT, 0, $1, $3, 0, 0); }
    | expr '/' expr                     { $$ = astCreate(AST_DIV, 0, $1, $3, 0, 0); }
    | expr '>' expr                     { $$ = astCreate(AST_GRT, 0, $1, $3, 0, 0); }
    | expr '<' expr                     { $$ = astCreate(AST_LESS, 0, $1, $3, 0, 0); }
    | expr '|' expr                     { $$ = astCreate(AST_OR, 0, $1, $3, 0, 0); }
    | expr '^' expr                     { $$ = astCreate(AST_POW, 0, $1, $3, 0, 0); }
    | expr OPERATOR_LE expr             { $$ = astCreate(AST_LE, 0, $1, $3, 0, 0); }
    | expr OPERATOR_GE expr             { $$ = astCreate(AST_GE, 0, $1, $3, 0, 0); }
    | expr OPERATOR_EQ expr             { $$ = astCreate(AST_EQ, 0, $1, $3, 0, 0); }
    | expr OPERATOR_DIF expr            { $$ = astCreate(AST_DIF, 0, $1, $3, 0, 0); }
    | '(' expr ')'                      { $$ = astCreate(AST_PARENTHESES_EXPR, 0, $2, 0, 0, 0); }        
    ;

funcArgs: expr restFuncArgs             { $$ = astCreate(AST_FUNCTION_ARGS, 0, $1, $2, 0, 0); }
    |                                   { $$ = 0; }
    ;

restFuncArgs: ',' expr restFuncArgs     { $$ = astCreate(AST_FUNCTION_ARGS_LIST, 0, $2, $3, 0, 0); }
    |                                   { $$ = 0; }
    ;

paramsFunc: TK_IDENTIFIER '=' types paramsResto          { $$ = astCreate(AST_PARAMS_FUNC, $1, $3, $4, 0, 0); }
    |                                                    { $$ = 0; }
    ;

paramsResto: ',' TK_IDENTIFIER '=' types paramsResto     { $$ = astCreate(AST_PARAMS_FUNC_LIST, $2, $4, $5, 0, 0); }
    |                                                    { $$ = 0; }
    ;

types: KW_CHAR   { $$ = astCreate(AST_TYPE_CHAR, 0, 0, 0, 0, 0); }
    | KW_INT     { $$ = astCreate(AST_TYPE_INT, 0, 0, 0, 0, 0); }
    | KW_FLOAT   { $$ = astCreate(AST_TYPE_FLOAT, 0, 0, 0, 0, 0); }
    | KW_BOOL    { $$ = astCreate(AST_TYPE_BOOL, 0, 0, 0, 0, 0); }
    ;

litList: literals litList   { $$ = astCreate(AST_LIT_LIST, 0, $1, $2, 0, 0); }
    |                       { $$ = 0; }
    ;

literals: LIT_CHAR          { $$ = astCreate(AST_SYMBOL, $1, 0, 0, 0, 0); }
    | LIT_FALSE             { $$ = astCreate(AST_SYMBOL, $1, 0, 0, 0, 0); }
    | LIT_TRUE              { $$ = astCreate(AST_SYMBOL, $1, 0, 0, 0, 0); }
    | LIT_FLOAT             { $$ = astCreate(AST_SYMBOL, $1, 0, 0, 0, 0); }
    | LIT_INTEGER           { $$ = astCreate(AST_SYMBOL, $1, 0, 0, 0, 0); }
    ;

%%

int yyerror()
{
    fprintf(stderr, "Syntax error at line %d.\n", getLineNumber());
    exit(3);
}

AST* getAST(){
	return root;
}
