CC=cc
CFLAGS=-c -Wall

all:prova

prova: lex.o parser.o
	cc -g -o prova lex.o parser.o

lex.o: lex.c parser.h def_y.h
	cc -g -c lex.c 

parser.o: parser.c def_y.h
	cc -g -c parser.c

lex.c: lexer.lex parser.y parser.h parser.c def_y.h
	flex -o lex.c lexer.lex

parser.h: parser.y def_y.h
	bison -d -o parser.c parser.y

lexer: lex.c parser.h def_y.h
	cc -o lexer lex.c