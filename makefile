CC = cc -g $(CFLAGS)
CFLAGS = -Wall -Wextra
COMP_PATH = s_compiler
COMP_PATH_LEXER = s_compiler
COMP_PATH_PARSER = s_compiler

MACH_PATH = s_machine
MACH_PATH_LEXER = s_machine
MACH_PATH_PARSER = s_machine

COMP_OBJ = $(COMP_PATH_LEXER)/lex.o $(COMP_PATH_PARSER)/parser.o $(COMP_PATH)/tree.o $(COMP_PATH)/hash_table.o $(COMP_PATH)/symbol_table.o $(COMP_PATH)/semantic.o $(COMP_PATH)/code_gen.o
MACH_OBJ = $(MACH_PATH_LEXER)/lex.o $(MACH_PATH_PARSER)/parser.o $(MACH_PATH)/parser.o $(MACH_PATH)/s_exec.o $(MACH_PATH)/s_machine.o $(COMP_PATH)/code_gen.o

all:

machine: $(MACH_OBJ)
	$(CC) -o $@ $^

compiler: $(COMP_OBJ)
	$(CC) -o $@ $^

%.o: %.c
	$(CC) -o $*.o -c $*.c

$(MACH_PATH_LEXER)/lex.c: $(MACH_PATH_LEXER)/lexer.lex
	flex -o $(MACH_PATH_LEXER)/lex.c $(MACH_PATH_LEXER)/lexer.lex

$(COMP_PATH_LEXER)/lex.c: $(COMP_PATH_LEXER)/lexer.lex $(COMP_PATH_PARSER)/parser.h
	flex -o $(COMP_PATH_LEXER)/lex.c $(COMP_PATH_LEXER)/lexer.lex

$(COMP_PATH_PARSER)/parser.h: $(COMP_PATH_PARSER)/parser.y
	bison -d -o $(COMP_PATH_PARSER)/parser.c $(COMP_PATH_PARSER)/parser.y

clean: 
	rm -f $(COMP_PATH)/lex.c $(COMP_PATH)/*.o $(COMP_PATH)/parser.h $(MACH_PATH)/lex.c $(MACH_PATH)/*.o machine compiler