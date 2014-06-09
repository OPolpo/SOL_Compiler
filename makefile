CC=cc
CFLAGS=-c -Wall

all: prova clean

lexer: lex.c parser.h def.h
	cc -o lexer lex.c -DLEXER

prova: lex.o parser.o tree.o hash_table.o symbol_table.o semantic.o
	cc -g -o prova lex.o parser.o tree.o hash_table.o symbol_table.o semantic.o

lex.o: lex.c parser.h def.h
	cc -g -c lex.c 

parser.o: parser.c def.h 
	cc -g -c parser.c

tree.o: tree.c tree.h def.h parser.h
	cc -g -c tree.c

hash_table.o: hash_table.c hash_table.h
	cc -g -c hash_table.c

symbol_table.o: symbol_table.c symbol_table.h
	cc -g -c symbol_table.c

semantic.o: semantic.c semantic.h
	cc -g -c semantic.c

lex.c: lexer.lex parser.y parser.h parser.c def.h
	flex -o lex.c lexer.lex

parser.h: parser.y def.h
	bison -d -o parser.c parser.y

clean: 
	rm lex.c *.o parser.c parser.h