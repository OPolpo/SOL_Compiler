//http://en.wikipedia.org/wiki/List_of_prime_numbers
#include "hash_table.h"

int hash (char* id){
	int i, h = 0; 
	for(i = 0; id[i] != '\0'; i++){
		h = ((h << SHIFT) + id[i]) % TOT;
	}
	return h;
}

void insert(Phash_node p, Phash_node * table){
    int pos = hash(p->name);
    p->next = table[pos];
    table[pos] = p;
}

Phash_node getNode(char * id, Phash_node * table){
    int pos = hash(id);
    Phash_node node = table[pos];
    while(node){
        if (strcmp(node->name,id) == 0) {
            return node;
        }
        node = node->next;
    }
}

Phash_node * new_hash_table(){
    Phash_node * table = (Phash_node *) malloc(TOT * sizeof(Phash_node));
    memset(table, '\0', TOT * sizeof(Phash_node)); // NULL == '\0'
    return table;
}

void print_func_node(Phash_node node){
    printf("[%d] %s | ", node->oid, node->name);
    if (node->class_node == CLFUNC) {
        printf("FUNC ");
    }
    (node->schema != NULL)? printf("ok schema |"): printf("no schema |");
    (node->locenv != NULL)? printf("ok env |"): printf("no env |");
    printf ("%d :", node->formals_num);
    
    Formal * f = node->formal;
    while (f!= NULL) {
        printf ("[%d] ", f->formal->oid);
    }
    printf ("\n");
}

/*
 typedef struct shash_node{
 char * name;
 int oid;
 Class class_node; //TYPE, VAR, CONST, FUNC, PAR
 Pschema schema;
 struct shash_node ** locenv;
 int formals_num;
 Formal * formal;
 struct shash_node * next;
 } Hash_node, * Phash_node;
*/