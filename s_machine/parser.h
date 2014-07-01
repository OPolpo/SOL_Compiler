#ifndef S_PARSER
#define S_PARSER
#include "../s_shared/SOL_def.h"

void load_scode(FILE * input);

int next();

void print_loaded_code(Scode * prog);
void print_code_instruction();

#endif