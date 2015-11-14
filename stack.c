//
// Created by Andrej Oliver Chudý on 07/11/15.
//

#include <stdbool.h>
#include "stack.h"


/**
 * Function initialize list to NULL
 * @param L pointer to the list
 */
void init_list(tDLList *L){

    L->First=NULL;
    L->Last=NULL;
    L->Active=NULL;
    L->LastNode=NULL;
    L->LastToken=NULL;
}

/**
 * Function frees all elements in list and set list to NULL
 * @param L pointer to the list
 */
void dispose_list(tDLList *L){

    tDLElemPtr temp;

    while(L->First != NULL){

        temp=L->First->rptr;
        free(L->First);
        L->First=temp;
    }

    L->Active = NULL;
    L->Last = NULL;
}

/**
 * Function create new element and inserts its as last element
 * @param L    pointer to the list
 * @param data pointer to data to be stored
 */
void insert_last(tDLList *L, void *data){

    tDLElemPtr new = malloc(sizeof(struct tDLElem));

    if (new == NULL){
        data = NULL;
    }
    else{

        new->data=data;

        if(L->First == NULL){   /*Empty list*/
            L->First=new;
            L->Last=new;
            new->rptr=NULL;
            new->lptr=NULL;
        }
        else {                  /*One or more elements in list*/
            new->lptr=L->Last;
            L->Last->rptr=new;
            new->rptr=NULL;
            L->Last=new;
        }
    }
}

void insert_last_desc(tDLList *L, void *data,bool NODE ){
    insert_last(L,data);
    if(NODE == true){
        L->LastNode = L->Last;
    }
    else{
        L->LastToken = L->Last;
    }
}

/**
 * Function frees last element in list if exists
 * @param L pointer to the list
 */
void delete_last(tDLList *L){

    if(L->First != NULL){   /*Non-empty list*/

        if(L->Last == L->Active){
            L->Active = NULL;
        }

        tDLElemPtr temp;

        if(L->First == L->Last){    /*Just one element in list*/
            free(L->First);
            L->First=NULL;
            L->Last=NULL;
        }
        else{                       /*Two or more elements in list*/

            temp=L->Last->lptr;
            free(L->Last);
            L->Last=temp;
            L->Last->rptr=NULL;
        }
    }
}

/**
 * Function creates new element and if acitve exists new element
 * is inserted behind active element, otherwise does nothing
 * @param L    pointer to the list
 * @param data pointer to data to be stored
 */
void post_insert(tDLList *L, void *data){

    if(L->Active != NULL){     /*Exist active element*/

        tDLElemPtr new = malloc(sizeof(struct tDLElem));
        if(new == NULL){
            data = NULL;
        }
        else{
            new->data=data;
            new->lptr=L->Active;
            new->rptr=L->Active->rptr;
            L->Active->rptr=new;
            if(new->rptr != NULL){      /*Active element wasn't last one*/
                new->rptr->lptr=new;
            }
            else{                       /*Active element was last => now L->Last must point to new element */
                L->Last=new;
            }
        }
    }
}


void preinsert_lastNode(tDLList *L, void *data){

    if(L->LastNode != NULL){     /*Exist active element*/

        tDLElemPtr new = malloc(sizeof(struct tDLElem));
        if(new == NULL){
            data = NULL;
        }
        else{
            new->data=data;
            new->lptr=L->LastNode->lptr;
            new->rptr=L->LastNode;
            L->LastNode->lptr=new;
            if(new->lptr != NULL){      /*Active element wasn't last one*/
                new->lptr->rptr=new;
            }
            else{                       /*Active element was last => now L->Last must point to new element */
                L->First=new;
            }
        }
    }
}
/**
 * Function activates last element in list
 * If list is empty, Active still points to NULL
 * @param L   pointer to the list
 */
void activate_last(tDLList *L){

    L->Active = L->Last;
}

/**
 * Function shifts active element to the right
 * If active doesn't exist, function does nothing
 * @param L   pointer to the list
 */
void shift_active_right(tDLList *L){

    if(L->Active != L->Last && L->Active != NULL){
        L->Active = L->Active->rptr;
    }
}

/**
 * Function returns void pointer on Last element
 * @param  L pointer to the list
 * @return   void pointer on last element, if list
 *           is empty, return NULL
 */
void *copy_last(tDLList *L){

    if(L->Last != NULL){
        return L->Last->data;
    }
    else {
        return NULL;
    }
}
