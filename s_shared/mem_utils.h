#ifndef S_MEMUTILS
#define S_MEMUTILS

#include <stdio.h>
#include <stdlib.h>

void *newmem(int size);
void freemem(char *p, int size);

#endif