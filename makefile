CC=cc
CFLAGS=-c -Wall

all: prova clean

lexer: lex.c parser.h def_y.h
	cc -o lexer lex.c -DLEXER

prova: lex.o parser.o tree.o hash_table.o
	cc -g -o prova lex.o parser.o tree.o hash_table.o

lex.o: lex.c parser.h def_y.h
	cc -g -c lex.c 

parser.o: parser.c def_y.h 
	cc -g -c parser.c

tree.o: tree.c tree.h def_y.h parser.h
	cc -g -c tree.c

hash_table.o: hash_table.c hash_table.h
	cc -g -c hash_table.c

lex.c: lexer.lex parser.y parser.h parser.c def_y.h
	flex -o lex.c lexer.lex

parser.h: parser.y def_y.h
	bison -d -o parser.c parser.y

clean: 
	rm lex.c lex.o parser.c parser.h tree.o