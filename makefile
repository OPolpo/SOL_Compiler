CC = cc -g $(CFLAGS)
CFLAGS = -Wall -Wextra -D_GNU_SOURCE
C_PATH = s_compiler/
C_PATH_PRSR = s_compiler/parser/

M_PATH = s_machine/
M_PATH_LEX = s_machine/
M_PATH_PRSRS = s_machine/parsers/

SHARED_PATH = s_shared/

C_OBJ = $(C_PATH_PRSR)lex.o $(C_PATH)tree.o $(C_PATH)hash_table.o $(C_PATH)symbol_table.o $(C_PATH)semantic.o $(C_PATH)code_gen.o $(SHARED_PATH)schema.o $(SHARED_PATH)mem_utils.o $(C_PATH_PRSR)parser.o

M_OBJ = $(M_PATH_PRSRS)lex.o $(M_PATH_PRSRS)parser.o  $(M_PATH_PRSRS)format_lexer.o $(M_PATH_PRSRS)format_parser.o $(M_PATH_PRSRS)formatted_lexer.o $(M_PATH_PRSRS)formatted_parser.o $(M_PATH)s_exec.o $(SHARED_PATH)schema.o $(SHARED_PATH)mem_utils.o $(M_PATH)sol_io.o $(M_PATH)s_machine.o 

all: scompiler smachine

smachine: $(M_OBJ) $(SHARED_PATH)SOL_def.h
	$(CC) -o $@ $(M_OBJ)

scompiler: $(C_OBJ) $(C_PATH)def.h
	$(CC) -o $@ $(C_OBJ) 

%.o: %.c %.h
	$(CC) -o $*.o -c $*.c

$(M_PATH_PRSRS)lex.c: $(M_PATH_PRSRS)lexer.lex
	flex -o $(M_PATH_PRSRS)lex.c $(M_PATH_PRSRS)lexer.lex

$(M_PATH_PRSRS)format_lexer.c: $(M_PATH_PRSRS)format_lexer.lex $(M_PATH_PRSRS)format_parser.h
	flex -o $(M_PATH_PRSRS)format_lexer.c -Pformat $(M_PATH_PRSRS)format_lexer.lex

$(M_PATH_PRSRS)format_parser.h: $(M_PATH_PRSRS)format_parser.y
	bison -d -o $(M_PATH_PRSRS)format_parser.c $(M_PATH_PRSRS)format_parser.y --name-prefix=format

$(M_PATH_PRSRS)formatted_lexer.c: $(M_PATH_PRSRS)formatted_lexer.lex $(M_PATH_PRSRS)formatted_parser.h
	flex -o $(M_PATH_PRSRS)formatted_lexer.c -Pformatted $(M_PATH_PRSRS)formatted_lexer.lex

$(M_PATH_PRSRS)formatted_parser.h: $(M_PATH_PRSRS)formatted_parser.y
	bison -d -o $(M_PATH_PRSRS)formatted_parser.c $(M_PATH_PRSRS)formatted_parser.y --name-prefix=formatted

$(C_PATH_PRSR)lex.c: $(C_PATH_PRSR)lexer.lex $(C_PATH_PRSR)parser.h
	flex -o $(C_PATH_PRSR)lex.c $(C_PATH_PRSR)lexer.lex

$(C_PATH_PRSR)parser.h: $(C_PATH_PRSR)parser.y
	bison -d -o $(C_PATH_PRSR)parser.c $(C_PATH_PRSR)parser.y

clean: 
	rm -f $(C_PATH_PRSR)lex.c $(C_PATH)*.o $(C_PATH_PRSR)*.o $(C_PATH_PRSR)parser.h $(C_PATH_PRSR)parser.c scompiler
	rm -f $(M_PATH_PRSRS)lex.c $(M_PATH)*.o $(M_PATH_PRSRS)*.o $(M_PATH_PRSRS)format_parser.h $(M_PATH_PRSRS)format_parser.c $(M_PATH_PRSRS)formatted_parser.h $(M_PATH_PRSRS)formatted_parser.c $(M_PATH_PRSRS)format_lexer.c $(M_PATH_PRSRS)formatted_lexer.c smachine

install: scompiler smachine
	cp scompiler /usr/local/bin
	cp smachine /usr/local/bin

uninstall: 
	rm -f /usr/local/bin/scompiler
	rm -f /usr/local/bin/smachine