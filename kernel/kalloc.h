#ifndef KALLOC_H
#define KALLOC_H

#include "types.h"

// Function declarations
void kinit(void);
void freerange(void *pa_start, void *pa_end);
void kfree(void *pa);
void *kalloc(void);
uint get_freemem(void);

#endif // KALLOC_H
