#include "mem_utils.h"

long size_allocated=0, size_deallocated=0;

void mem_error(char * msg){
    fprintf(stderr, "Mem error: %s\n", msg);
    exit(EXIT_FAILURE);
}

void * newmem(int size) {
    printf("\n+%d\n", size);
    void *p;
    if((p = calloc(1,size)) == NULL) mem_error("Failure in memory allocation");
    size_allocated += size;
    return p;
}

void freemem(char *p, int size) {
    printf("\n-%d\n", size);
    free(p);
    size_deallocated += size;
}

