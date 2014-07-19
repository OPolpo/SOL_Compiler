#ifndef S_MEMUTILS
#define S_MEMUTILS

#include <stdio.h>
#include <stdlib.h>

void *newmem(int size);
void freemem(void *p, int size);

#endif