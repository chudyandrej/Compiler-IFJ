//
// Created by Andrej Oliver Chudý on 07/11/15.
//

#ifndef IJF_STACK_H
#define IJF_STACK_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "precedence.h"

typedef struct tDLElem{
    void *data;             /*points to any data*/
    struct tDLElem *lptr;   /*points to left element*/
    struct tDLElem *rptr;   /*points to right element*/

} *tDLElemPtr;

typedef struct tDLList{
    tDLElemPtr First;   /*points to firts element in the list*/
    tDLElemPtr Last;    /*points to last element in the list*/
    tDLElemPtr Active;  /*points to active element in the list*/
} tDLList;

void init_list(tDLList *L);
void dispose_list(tDLList *L);
void insert_last(tDLList *L, void *data);
void delete_last(tDLList *L);
void post_insert(tDLList *L, void *data);
void activate_last(tDLList *L);
void shift_active_right(tDLList *L);
void *copy_last(tDLList *L);
void preinsert_lastNode(tDLList *L, void *data);
void delete_element(tDLList *L, tDLElemPtr element);
int length_list(tDLList *L);



tDLElemPtr find_last(tDLList *L, bool NODE );

extern tDLList *tac_stack;

#endif //IJF_STACK_H
