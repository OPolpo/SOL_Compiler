CC = cc -g
CFLAGS = -c -Wall
COMP_PATH = s_compiler
COMP_PATH_LEXER = s_compiler
COMP_PATH_PARSER = s_compiler

OBJ = $(COMP_PATH_LEXER)/lex.o $(COMP_PATH_PARSER)/parser.o $(COMP_PATH)/tree.o $(COMP_PATH)/hash_table.o $(COMP_PATH)/symbol_table.o $(COMP_PATH)/semantic.o $(COMP_PATH)/code_gen.o

all: compiler clean

$(COMP_PATH_LEXER)/lexer: $(COMP_PATH_LEXER)/lex.c $(COMP_PATH_PARSER)/parser.h
	cc -o lexer $(COMP_PATH_LEXER)/lex.c -DLEXER

compiler: $(OBJ)
	$(CC) -o $@ $^

%.o: %.c
	$(CC) -o $*.o -c $*.c

$(COMP_PATH_LEXER)/lex.c: $(COMP_PATH_LEXER)/lexer.lex $(COMP_PATH_PARSER)/parser.h
	flex -o $(COMP_PATH_LEXER)/lex.c $(COMP_PATH_LEXER)/lexer.lex

$(COMP_PATH_PARSER)/parser.h: $(COMP_PATH_PARSER)/parser.y
	bison -d -o $(COMP_PATH_PARSER)/parser.c $(COMP_PATH_PARSER)/parser.y

clean: 
	rm -f $(COMP_PATH)/lex.c $(COMP_PATH)/*.o $(COMP_PATH)/parser.c $(COMP_PATH)/parser.h