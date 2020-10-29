# Caetano Jaeger Stradolini
# 09/09/2020
# Etapa 2 do trabalho de compiladores

etapa3: main.o lex.yy.o parser.tab.o hash.o ast.o semantic.o
	gcc main.o lex.yy.o parser.tab.o hash.o ast.o semantic.o -o etapa3

main.o: main.c
	gcc -c main.c

semantic.o: semantic.c
	gcc -c semantic.c

hash.o: hash.c
	gcc -c hash.c

ast.o: ast.c
	gcc -c ast.c

parser.tab.o: parser.tab.c
	gcc -c parser.tab.c

lex.yy.o: lex.yy.c
	gcc -c lex.yy.c

parser.tab.c: parser.y
	bison -d parser.y

lex.yy.c: scanner.l
	flex scanner.l

clean:
	rm lex.yy.c parser.tab.c *.o etapa3.exe