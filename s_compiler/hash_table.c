/**
 * @author Andrea Bocchese
 * @author Ilaria Martinelli
 * @brief This contains all function to manage the hash table.
 */

//http://en.wikipedia.org/wiki/List_of_prime_numbers
#include "hash_table.h"
#include "parser/parser.h"
#include "semantic.h"

int hash (char* id){
    int i, h = 0;
    for(i = 0; id[i] != '\0'; i++){
        h = ((h << SHIFT) + id[i]) % TOT;
    }
    return h;
}

int insert(Phash_node p, Phash_node * table){
    if (getNode(p->name, table) != NULL) {
        return 0;
    }
    int pos = hash(p->name);
    p->next = table[pos];
    table[pos] = p;
    return 1;
}

int insert_func(Phash_node p, Phash_node f_parent){
    int not_used;
    if (find_visible_node(p->name, f_parent, &not_used) != NULL) {
        return 0;
    }
    return insert(p, f_parent->aux->locenv);
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
    return NULL;
}

Phash_node find_visible_node(char * id, Phash_node func_node, int * offset){
    Phash_node f = func_node;
    *offset=0;
    while (f != NULL) {
        Phash_node node = getNode(id, f->aux->locenv);
        if (node != NULL) {
            return node;
        }
        if (f->father==NULL){
            if (strcmp(f->name,id) == 0) {
                return f;
            }
        }
        f = f->father;
        (*offset)++;
    }
    return NULL;
}

Phash_node * new_hash_table(){
    Phash_node * table = (Phash_node *) calloc(TOT, sizeof(Phash_node));
    return table;
}

void print_func_node(Phash_node node){
    printf("[%d] %s | ", node->oid, node->name);
    if (node->class_node == CLFUNC) {
        printf("FUNC ");
        (node->schema != NULL)? printf("ok schema |"): printf("no schema |");
        ((node->aux)->locenv != NULL)? printf("ok env |"): printf("no env |");
        printf ("%d :", (node->aux)->formals_num);
        
        Formal * f = (node->aux)->formal;
        while (f!= NULL) {
            printf ("[%d]", f->formal->oid);
            printf ("(%s) ", f->formal->name);
            f = f->next;
        }
        if(node->father)
            printf(" | F_oid: %d", node->father->oid);
        else
            printf(" | F_oid: NULL");
        printf ("((%d))", (node->aux)->num_obj);
    }
    printf ("\n");
}

void print_generic_node(Phash_node node){
    printf("[%d] %s | ", node->oid, node->name);
    
    switch (node->class_node) {
        case CLTYPE:
            printf("TYPE ");
            break;
        case CLVAR:
            printf("VAR ");
            break;
        case CLCONST:
            printf("CONST ");
            break;
        case CLPAR:
            printf("PAR ");
            break;
        case CLCOUNT:
            printf("COUNT ");
            break;
        default:
            printf("Unexpected CLASS ");
            break;
    }
    (node->schema != NULL)? printf("ok schema |"): printf("no schema |");
    
    printf ("\n");
}

void print_hash_content(Phash_node * table){
    int i;
    Phash_node temp;
    printf("-------------local environment\n");
    for (i=0; i<TOT; i++){
        temp = table[i];
        while (temp != NULL) {
            printf("%3d. ", i);
            print_generic_node(temp);
            if(temp->schema != NULL){
                printSchema(temp->schema, "   ");
            }
            temp = temp->next;
        }
    }
    printf("-------------end\n");
}
