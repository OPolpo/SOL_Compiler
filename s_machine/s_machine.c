/**
 * @author Andrea Bocchese
 * @author Ilaria Martinelli
 * @brief Virtual Machine
 */

#include "s_machine.h"
#include "s_exec.h"
#include "../s_shared/schema.h"
#include "sol_io.h"

extern Scode *prog;
extern int pc;

Adescr **astack;
Odescr **ostack;
char *istack;
int asize, osize, isize, code_size;
int ap, op, ip; // point to the first free elements of their stack
extern long size_allocated;
extern long size_deallocated;

Str_c_node ** str_const_table;

int main(int argc, char* argv[]){
	Scode * stat;
    if(argc != 2){
        fprintf(stderr, "%s %s %s\n", "\tUsage: ", argv[0], "filename");
        exit(0);
    }
	start_machine(argv[1]);
	while((stat = &prog[pc++])->op != S_HALT){
        exec(stat);
#if DEBUG
        printf("ap: %d, op: %d, ip: %d\n", ap, op, ip);
#endif
    }
    
#if DEBUG
    printf("\n");
    print_str_c_table();
#endif
    
    end_machine();
    return 0;
}

/**
 * @brief This functin opens the input file and do the initial memory allocation of data structures.
 * @param input the s_code filename.
 */
void start_machine(char * input) {
    str_const_table = (Str_c_node **)newmem(sizeof(Str_c_node*)*STR_CONST_DIM);
    FILE *input_file = fopen(input, "r");
    if (!input_file){
        machine_error("ERRORE nel caricamento del file");
    }
    load_scode(input_file);
    fclose (input_file);
    
    pc = ap = op = ip = 0;
    astack = (Adescr**)newmem(sizeof(Adescr*)*ASTACK_UNIT);
    asize = ASTACK_UNIT;
    ostack = (Odescr**)newmem(sizeof(Odescr*)*OSTACK_UNIT);
    osize = OSTACK_UNIT;
    istack = (char*)newmem(ISTACK_UNIT);
    isize = ISTACK_UNIT;
}

/**
 * @brief This function frees the data structures.
 */
void end_machine() {
#if DEBUG
    printf("\n%d %d %d\n", ap, op, ip);
#endif
    freemem((char*)prog, sizeof(Scode)*code_size);
    freemem((char*)astack, sizeof(Adescr*)*asize);
    freemem((char*)ostack, sizeof(Odescr*)*osize);
    freemem(istack, isize);
    free_str_c_table();
    
    printf("\nProgram executed without errors\n");
    printf("Allocation: %ld bytes\n", size_allocated);
    printf("Deallocation: %ld bytes\n", size_deallocated);
    printf("Residue: %ld bytes\n", size_allocated - size_deallocated);
}

/**
 * This function returns a pointer to the top of the astack.
 * @return The Adescr* on top of the astack.
 */
Adescr * top_astack(){
    if (ap==0) {
        machine_error("top_astack");
    }
    return astack[ap-1];
}

/**
 * @brief This function allocates memory for the adescr and puts it on top of the astack.
 *
 * If necessary it expands the astack.
 *
 * @return a pointer to the top of astack.
 */
Adescr * push_astack(){
    Adescr **old_astack; int i;
    if(ap == asize) {
        old_astack = astack;
        astack = (Adescr**) newmem(sizeof(Adescr*)*(asize + ASTACK_UNIT));
        for(i = 0; i < asize; i++)
            astack[i] = old_astack[i];
        freemem((char*)old_astack, sizeof(Adescr*)*asize);
        asize += ASTACK_UNIT;
    }
    return (astack[ap++] = (Adescr*)newmem(sizeof(Adescr)));
}

/**
 * @brief Free the adescr on the top of the astack.
 */
void pop_astack() {
    if(ap == 0) machine_error("pop_adescr()");
    freemem((char*)astack[--ap], sizeof(Adescr));
}

/**
 * @brief return the pointer of an object given the index of the ostack.
 * @param i The index of the object.
 * @return the Odescr** that points to the given index.
 */
Odescr ** get_p2objects(int i){
    return &ostack[i];
}

/**
 * return a pointer the top of the ostack.
 * @return The Odescr** on top of the astack.
 */
Odescr ** top_ostack_addr(){
    if (op==0) {
        machine_error("top_ostack");
    }
    return &ostack[op-1];
}

/**
 * return the pointer to the first free elements of ostack.
 * @return op.
 */
int get_next_op(){
    return op;
}

/**
 * return a pointer the top of the ostack.
 * @return The Odescr* on top of the astack.
 */
Odescr * top_ostack(){
    if (op==0) {
        machine_error("top_ostack");
    }
    return ostack[op-1];
}

/**
 * return a pointer the element over the element on top of the ostack.
 * @return The Odescr* to the Odescr over the Odescr on top of the astack.
 */
Odescr * over_top_ostack(){
    return top_ostack()+sizeof(Odescr *);
}

/**
 * return a pointer the element below the element on top of the ostack.
 * @return The Odescr* to the Odescr below the Odescr on top of the astack.
 */
Odescr * under_top_ostack(){
    if (op<=1) {
        machine_error("under_top_ostack");
    }
    return ostack[op-2];
}

/**
 * @brief Allocate memory for the odescr adn put it on top of the ostack.
 *
 * If necessary expand the ostack.
 *
 * @return a pointer to the top of ostack.
 */
Odescr * push_ostack(){
    Odescr **old_ostack; int i;
    if(op == osize) {
        old_ostack = ostack;
        ostack = (Odescr**) newmem(sizeof(Odescr*)*(osize + OSTACK_UNIT));
        for(i = 0; i < osize; i++)
            ostack[i] = old_ostack[i];
        freemem((char*)old_ostack, sizeof(Odescr*)*osize);
        osize += OSTACK_UNIT;
    }
    return (ostack[op++] = (Odescr*)newmem(sizeof(Odescr)));
}

/**
 * @brief Free the odescr on the top of the ostack.
 */
void pop_ostack() {
    if(op == 0) machine_error("pop_odescr()");
    freemem((char*)ostack[--op], sizeof(Odescr));
}

/**
 * @brief Increment the istack with the size and return the new top.
 *
 * If necessary expand the istack.
 *
 * @param size how much increment the istack.
 * @return a pointer to the top of istack.
 */
char * push_istack(int size){
    char *old_istack; int i, n;
    if(ip+size >= isize) {
        n = size/ISTACK_UNIT;
        old_istack = istack;
        istack = newmem(isize + ISTACK_UNIT);
        istack = newmem(isize + (n+1) * ISTACK_UNIT);
        for(i = 0; i < isize; i++)
            istack[i] = old_istack[i];
        freemem(old_istack, isize);
        isize += ISTACK_UNIT;
        isize += (n+1) * ISTACK_UNIT;
    }
    
    ip+=size;
    return &(istack[ip-size]);
}

/**
 * @brief Decrement the istack with the size and return the new top.
 *
 * @param size how much increment the istack.
 */
void pop_istack(int size) {
    if(ip < size) machine_error("pop_istack()");
    ip = ip-size;
}

/**
 * @brief This function shift down the memory chunk express by to move of the quantity express by this much.
 * @param to_move how much bytes are to move.
 * @param this_much How much byte shift down the memory block
 */
void move_down_istack(int to_move, int this_much){
    if(ip - to_move - this_much < 0) machine_error("move_down_istack()");
    if (to_move<0 || this_much<0) machine_error("move_down_istack() parameters");
    memmove(&istack[ip-to_move-this_much], &istack[ip-to_move], to_move);
    ip -= this_much;
}

int pop_int(){
    int i = top_ostack()->inst.ival;
    pop_ostack();
    return i;
}

void push_int(int i){
    Odescr * new_o = push_ostack();
    new_o->mode = EMB;
    new_o->size = sizeof(int);
    new_o->inst.ival = i;
}

float pop_real(){
    float r = top_ostack()->inst.rval;
    pop_ostack();
    return r;
}

void push_real(float r){
    Odescr * new_o = push_ostack();
    new_o->mode = EMB;
    new_o->size = sizeof(float);
    new_o->inst.rval = r;
}

char pop_char(){
    char c = top_ostack()->inst.cval;
    pop_ostack();
    return c;
}

void push_char(char c){
    Odescr * new_o = push_ostack();
    new_o->mode = EMB;
    new_o->size = sizeof(char);
    new_o->inst.cval = c;
}

int pop_bool(){
    int i = (top_ostack()->inst.cval != '0');
    pop_ostack();
    return i;
}

void push_bool(int b){
    Odescr * new_o = push_ostack();
    new_o->mode = EMB;
    new_o->size = sizeof(char);
    new_o->inst.cval = b ? '1' :'0';
}

char * pop_string(){
    char * s = top_ostack()->inst.sval;
    pop_ostack();
    return s;
}

void push_string(char * s){ //assuming is "mallocated" and is in the hash table
    Odescr * new_o = push_ostack();
    new_o->mode = EMB;
    new_o->size = sizeof(char*);
    new_o->inst.sval = insert_str_c(s);
}


/**
 * @brief Insert a string in the string table
 * @param s The string to insert.
 * @return the pointer to ther string in the string table.
 */
char * insert_str_c(char * s){
    char *without = s;
    
    char * format_position = strstr(without, "\\n");
    while (format_position) {
        *format_position = '\n';
        ++format_position;
        memmove(format_position, format_position+1, strlen(format_position+1)+1);
        format_position = strstr(without, "\\n");
        
    }
    
    char * p_on_table = get_str_c(without);
    if (!p_on_table){
        int pos = hash_str_c(without);
        Str_c_node * new_node = (Str_c_node *)newmem(sizeof(Str_c_node));
        new_node->string = (char*)newmem(((int)strlen(without)+1) * sizeof(char));
        
        strcpy(new_node->string, without);
        p_on_table = new_node->string;
        new_node->next = str_const_table[pos];
        str_const_table[pos] = new_node;
    }
    return p_on_table;
}

/**
 * @brief Insert a string in the string table without the first and last character
 * @param s The string to insert.
 * @return the pointer to ther string in the string table.
 */
char * insert_strWclean(char * s){
    char * without = s+1;
    without[strlen(without)-1]=0;
    
    return insert_str_c(without);
}

/**
 * @brief get the pointer to the string in string table
 * @param s to search
 * @return the pointer to ther string in the string table, null otherwise.
 */
char * get_str_c(char * s){
    int pos = hash_str_c(s);
    Str_c_node * node = str_const_table[pos];
    while (node) {
        if (strcmp(node->string, s) == 0) {
            return node->string;
        }
        node = node->next;
    }
    return NULL;
}

/**
 * @brief hash function of string table
 * @param s the string
 * @return the hash value.
 */
int hash_str_c(char * s){
    int i, h = 0;
    for(i = 0; s[i] != '\0'; i++){
        h = abs(((h << STR_SHIFT) + s[i]) % STR_CONST_DIM);
    }
    return h;
}

/**
 * @brief this function frees all string table
 */
void free_str_c_table(){
    int i=0;
    for (i=0; i<STR_CONST_DIM; i++) {
        if (str_const_table[i]!=NULL) {
            Str_c_node * newhead = str_const_table[i]->next;
            while (newhead) {
                freemem(str_const_table[i]->string, (int)strlen(str_const_table[i]->string)+1);
                freemem((char *)str_const_table[i], sizeof(Str_c_node));
                str_const_table[i] = newhead;
                newhead = newhead->next;
            }
            freemem(str_const_table[i]->string, (int)strlen(str_const_table[i]->string)+1);
            freemem((char *)str_const_table[i], sizeof(Str_c_node));
        }
    }
    freemem((char*)str_const_table, sizeof(Str_c_node*)*STR_CONST_DIM);
}

void print_str_c_table(){
    int i;
    Str_c_node * temp;
    printf("-------------Str_c_node\n");
    for (i=0; i<STR_CONST_DIM; i++){
        temp = str_const_table[i];
        while (temp != NULL) {
            printf("%3d. ", i);
            printf("[%s]", temp->string);
            printf("\t%p\n", (temp->string));
            temp = temp->next;
        }
    }
    printf("-------------Str_c_node end\n");
}


void machine_error(char * msg){
    fprintf(stderr, "Machine error: %s\n", msg);
    exit(EXIT_FAILURE);
}
