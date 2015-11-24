

#ifndef IJF_GARBAGE_H
#define IJF_GARBAGE_H

#include "stack.h"

extern  tDLList *garbage;

void *gc_find(void *ptr);
void *gc_malloc(size_t size);
void *gc_realloc(void *ptr, size_t size);
void gc_free(void *ptr);
void gc_free_all();

void gc_delete_element(void *element);
void gc_insert_last(void *data);
void gc_delete_last();

#endif //IJF_GARBAGE_H
