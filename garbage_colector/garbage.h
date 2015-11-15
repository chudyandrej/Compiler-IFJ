
#ifndef GARBAGE_H
#define GARBAGE_H

#include "stack.h"

extern  tDLList *G;

void *find(void *ptr);
void *gc_malloc(size_t size);
void *gc_free(void *ptr);
void gc_free_all();

void gc_delete_element(void *element);
void gc_insert_last(void *data);
void gc_delete_last();

#endif //GARBAGE_H
