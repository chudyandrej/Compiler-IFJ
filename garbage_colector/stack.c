//
// Created by Andrej Oliver Chudý on 07/11/15.
//

#include "stack.h"


/**
 * Function initialize list to NULL
 * @param L pointer to the list
 */
void init_list(tDLList *L){

    L->First=NULL;
    L->Last=NULL;
    L->Active=NULL;

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


int length_list(tDLList *L){
    int count = 0;
    tDLElemPtr temp = L->First;

    while(temp != NULL){
        count++;
        temp = temp->rptr;
    }
    return count;
}

/*
tDLElemPtr find_last(tDLList *L, bool NODE){

    tDLElemPtr temp = L->Last;

    while(temp != NULL){

        if(!((((dTreeElementPtr) temp->data)->description == D_NODE) ^ NODE)){
            return temp;
        }
        else{
            temp = temp->lptr;
        }
    }

    return NULL;
}

void preinsert_lastNode(tDLList *L, void *data){

    tDLElemPtr last_node = find_last(L, TRUE);

    if( last_node != NULL){

        tDLElemPtr new = malloc(sizeof(struct tDLElem));
        if(new == NULL){
            data = NULL;
        }
        else{
            new->data = data;
            new->lptr = last_node->lptr;
            new->rptr = last_node;
            last_node->lptr = new;
            if(new->lptr != NULL){      //Last node element wasn't last one
                new->lptr->rptr = new;
            }
            else{                       //Last Node element was last => now L->Last must point to new element 
                L->First = new;
            }
        }
    }
}
*/
void delete_element(tDLList *L, tDLElemPtr element){

    tDLElemPtr temp = L->Last;

    while(temp != NULL){
        if(temp == element){            //delete this one
            if(temp->rptr != NULL && temp->lptr != NULL){
                temp->rptr->lptr = temp->lptr;
                temp->lptr->rptr = temp->rptr;
            }
            else if(temp->rptr != NULL){
                temp->rptr->lptr = temp->lptr;
                L->First = temp->rptr;

            }
            else if(temp->lptr != NULL){
                temp->lptr->rptr = temp->rptr;
                L->Last = temp->lptr;
            }
            free(temp);
            break;

        }
        else{
            temp = temp->lptr;
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
