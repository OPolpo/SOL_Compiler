CC = cc -g $(CFLAGS)
CFLAGS = -Wall -Wextra
C_PATH = s_compiler
C_PATH_LEX = s_compiler
C_PATH_PRSR = s_compiler

M_PATH = s_machine
M_PATH_LEX = s_machine
M_PATH_PRSR = s_machine
M_PATH_F_LEX = s_machine
M_PATH_F_PRSR = s_machine

C_OBJ = $(C_PATH_LEX)/lex.o $(C_PATH_PRSR)/parser.o $(C_PATH)/tree.o $(C_PATH)/hash_table.o $(C_PATH)/symbol_table.o $(C_PATH)/semantic.o $(C_PATH)/code_gen.o
M_OBJ = $(M_PATH_LEX)/lex.o $(M_PATH_PRSR)/parser.o $(M_PATH)/parser.o $(M_PATH)/s_exec.o $(M_PATH)/s_machine.o $(C_PATH)/code_gen.o $(M_PATH_F_LEX)/format_lexer.o $(M_PATH_F_PRSR)/format_parser.o

all: compiler machine

machine: $(M_OBJ)
	$(CC) -o $@ $^

compiler: $(C_OBJ)
	$(CC) -o $@ $^

%.o: %.c
	$(CC) -o $*.o -c $*.c

$(M_PATH_LEX)/lex.c: $(M_PATH_LEX)/lexer.lex
	flex -o $(M_PATH_LEX)/lex.c $(M_PATH_LEX)/lexer.lex

$(M_PATH_F_LEX)/lex.c: $(M_PATH_F_LEX)/format_lexer.lex $(M_PATH_F_PRSR)/parser.h
	flex -o $(M_PATH_F_LEX)/format_lexer.c $(M_PATH_F_LEX)/format_lexer.lex

$(M_PATH_F_PRSR)/parser.h: $(M_PATH_F_PRSR)/format_parser.y
	bison -d -o $(M_PATH_F_PRSR)/format_parser.c $(M_PATH_F_PRSR)/format_parser.y

$(C_PATH_LEX)/lex.c: $(C_PATH_LEX)/lexer.lex $(C_PATH_PRSR)/parser.h
	flex -o $(C_PATH_LEX)/lex.c $(C_PATH_LEX)/lexer.lex

$(C_PATH_PRSR)/parser.h: $(C_PATH_PRSR)/parser.y
	bison -d -o $(C_PATH_PRSR)/parser.c $(C_PATH_PRSR)/parser.y

clean: 
	rm -f $(C_PATH)/lex.c $(C_PATH)/*.o $(C_PATH)/parser.h $(M_PATH)/lex.c $(M_PATH)/*.o machine compiler