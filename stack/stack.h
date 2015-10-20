#ifndef INTERPRET_STACK_H
#define INTERPRET_STACK_H

#include<stdio.h>
#include<stdlib.h>

typedef struct tDLElem{
	int *data; 
    struct tDLElem *lptr;  
    struct tDLElem *rptr; 
} *tDLElemPtr;

typedef struct{
	tDLElemPtr First;
	tDLElemPtr Last;
	tDLElemPtr Active;
} tDLList;

void init_list(tDLList *L);
void dispose_list(tDLList *L);
void insert_last(tDLList *L, void *data);
void delete_last(tDLList *L);
void post_insert(tDLList *L, void *data);
void activate_last(tDLList *L);
void shift_active_right(tDLList *L);
void copy_last(tDLList *L, void *data);



#endif //INTERPRET_STACK_H