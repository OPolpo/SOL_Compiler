#ifndef S_MACHINE
#define S_MACHINE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../s_compiler/code_gen.h"

#define MAXARGS 3
#define STR_CONST_DIM 401
#define STR_SHIFT 4
#define ASTACK_UNIT 1000
#define OSTACK_UNIT 1000
#define ISTACK_UNIT 1000000

typedef struct{
    Operator op;
    Value args[MAXARGS];
} Scode;

typedef enum {
    EMB,
    STA
} Mode;

typedef struct {
    Mode mode;
    int size;
    Value inst;
} Odescr;

typedef struct adescr {
    int numobj;
    int pos_objects;
    int raddr;
    struct adescr *alink;
} Adescr;

typedef struct str_c_node{
    char * string;
    struct str_c_node * next;
} Str_c_node;

void start_machine();
void end_machine();
void *newmem(int size);
void freemem(char *p, int size);

Adescr * top_astack();
Adescr * push_astack();
void pop_astack();

Odescr * get_p2objects(int i);
int get_next_op();

Odescr * top_ostack();
Odescr * over_top_ostack();
Odescr * under_top_ostack();
Odescr * push_ostack();
void pop_ostack();

char * push_istack(int size);
void pop_istack(int size);
void move_down_istack(int to_move, int this_much);

void load_scode();
void machine_error(char * err_str);


int pop_int();
void push_int(int i);
float pop_real();
void push_real(float r);
char pop_char();
void push_char(char c);
int pop_bool();
void push_bool(int b);
char * pop_string();
void push_string(char * s);

char * insert_str_c(char * s);
char * get_str_c(char * s);
int hash_str_c(char * s);

#endif
