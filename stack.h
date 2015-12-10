/*
 * File: stack.c
 *
 * Description: Malloc Garbage Collector
 * Authors: Kondula Václav, Kopec Martin
 *
 * Created: 2015/10/24
 * Last time modified: 2015/11/25
 */

#ifndef INTERPRET_STACK_H
#define INTERPRET_STACK_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

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

#include "garbage.h"
void init_list(tDLList *L);
void dispose_list(tDLList *L);
void insert_last(tDLList *L, void *data);
void delete_last(tDLList *L);
void post_insert(tDLList *L, void *data);
void activate_last(tDLList *L);
void *copy_last(tDLList *L);
void delete_element(tDLList *L, tDLElemPtr element);
int length_list(tDLList *L);
/////////////
void activate_first(tDLList *L);
int is_active(tDLList *L);
void *copy_active(tDLList *L);
void shift_active(tDLList *L);


#endif //INTERPRET_STACK_H