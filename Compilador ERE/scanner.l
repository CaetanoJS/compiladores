/* Caetano Jaeger Stradolini */

%{
    #include "parser.tab.h"
    #include "hash.h"
    int Running = 1;
    int LineNumber = 1;
%}

%x COMMENT

%%

"char"                           return KW_CHAR;
"int"                            return KW_INT;
"float"                          return KW_FLOAT;
"bool"                           return KW_BOOL;
"if"                             return KW_IF;
"then"                           return KW_THEN;
"else"                           return KW_ELSE;
"while"                          return KW_WHILE;
"loop"                           return KW_LOOP;
"read"                           return KW_READ;
"print"                          return KW_PRINT;
"return"                         return KW_RETURN;
[,;:()\[\]{}\+-/<>=|^~&$#]       return yytext[0];
"<="                             return OPERATOR_LE;
">="                             return OPERATOR_GE;
"=="                             return OPERATOR_EQ;
"!="                             return OPERATOR_DIF;
'.'                              hashInsert(yytext, LIT_CHAR); return LIT_CHAR;
"TRUE"                           hashInsert(yytext, LIT_TRUE); return LIT_TRUE;
"FALSE"                          hashInsert(yytext, LIT_FALSE); return LIT_FALSE;
\"(\\.|[^"])*\"	                 hashInsert(yytext, LIT_STRING); return LIT_STRING;
[0-9A-F]+                        hashInsert(yytext, LIT_INTEGER); return LIT_INTEGER;
([0-9]([0-9A-F])*)*\.[0-9A-F]+   hashInsert(yytext, LIT_FLOAT); return LIT_FLOAT;
[a-zA-Z][0-9a-zA-Z@_]*           hashInsert(yytext, TK_IDENTIFIER); return TK_IDENTIFIER;
[ \t]
"\n"                             ++LineNumber;
"//".*
"/*"                             BEGIN(COMMENT);
.                                return TOKEN_ERROR;

<COMMENT>.
<COMMENT>"\n"   ++LineNumber;
<COMMENT>"*/"   BEGIN(INITIAL);

%%
#include "parser.tab.c"
#include "main.c"

int yywrap()
{
    Running = 0;
    return 1;
}

int isRunning()
{
    return Running;
}

void initMe()
{
    hashInit();
}

int getLineNumber()
{
    return LineNumber;
}
