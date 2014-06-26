#ifndef S_PARSER
#define S_PARSER
#include "../s_compiler/code_gen.h"

void load_scode();

int next();

void print_loaded_code(Scode * prog);
void print_code_instruction();

#endif