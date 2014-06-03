//http://en.wikipedia.org/wiki/List_of_prime_numbers
#include "hash_table.h"

int hash (char* id){
	int i, h = 0; 
	for(i = 0; id[i] != '\0'; i++){
		h = ((h << SHIFT) + id[i]) % TOT;
	}
	return h;
}

void insert(Phash_node p){
    int pos = hash(p->id);
    p->next = table[pos];
    table[pos] = p;
}

Phash_node getNode(char * id){
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
    Phash_node * table = (Phash_node *) malloc(TOT * sizeof(Phash_node *));
    memset(table, NULL, TOT * sizeof(Phash_node));
    return table;
}

