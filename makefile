CC=cc
CFLAGS=-c -Wall

all: prova clean

lexer: lex.c parser.h def_y.h
	cc -o lexer lex.c -DLEXER

prova: lex.o parser.o tree.o
	cc -g -o prova lex.o parser.o tree.o

lex.o: lex.c parser.h def_y.h
	cc -g -c lex.c 

parser.o: parser.c def_y.h tree.o
	cc -g -c parser.c tree.o

tree.o:tree.c def_y.h
	cc -g -c tree.c

lex.c: lexer.lex parser.y parser.h parser.c def_y.h
	flex -o lex.c lexer.lex

parser.h: parser.y def_y.h
	bison -d -o parser.c parser.y

clean: 
	rm lex.c lex.o parser.c parser.h