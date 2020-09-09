etapa2: parser.tab.c lex.yy.c
	gcc -o etapa2 lex.yy.c hash.c
parser.tab.c: parser.y
	bison -v parser.y
lex.yy.c: scanner.l
	flex scanner.l

clean:
	rm lex.yy.c parser.tab.c etapa2.exe