# Caetano Jaeger Stradolini
# 09/09/2020
# Etapa 2 do trabalho de compiladores

etapa2: y.tab.c lex.yy.c
	gcc -o etapa2 lex.yy.c hash.c
y.tab.c: parser.y
	yacc -d parser.y
lex.yy.c: scanner.l
	lex scanner.l

clean:
	rm lex.yy.c y.tab.c y.tab.h etapa2