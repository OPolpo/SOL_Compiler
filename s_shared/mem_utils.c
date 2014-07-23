/**
 * @author Andrea Bocchese
 * @author Ilaria Martinelli
 * @brief This file contains the function alloc and free the memory extended in order to take track of how much memory is allocated and freed
 */


#include "mem_utils.h"

long size_allocated=0, size_deallocated=0;

/**
 * @brief The function reports the error message on the stderr.
 * @param msg the message.
 */
void mem_error(char * msg){
    fprintf(stderr, "Mem error: %s\n", msg);
    exit(EXIT_FAILURE);
}

void * newmem(int size) {
    void *p;
    if((p = calloc(1,size)) == NULL) mem_error("Failure in memory allocation");
    size_allocated += size;
    return p;
}

void freemem(void *p, int size) {
    free(p);
    size_deallocated += size;
}
