
#include "garbage.h"


void gc_delete_last(){

    if(garbage->First != NULL){           /*Non-empty list*/

        if(garbage->Last == garbage->Active){
            garbage->Active = NULL;
        }
        tDLElemPtr temp;
        if(garbage->First == garbage->Last){    /*Just one element in list*/
            free(garbage->First);
            garbage->First=NULL;
            garbage->Last=NULL;
        }
        else{                       /*Two or more elements in list*/

            temp=garbage->Last->lptr;
            free(garbage->Last);
            garbage->Last=temp;
            garbage->Last->rptr=NULL;
        }
    }
}

void gc_insert_last(void *data){

    tDLElemPtr new = malloc(sizeof(struct tDLElem));

    if (new == NULL){
        gc_free_all();      //free all
        fprintf(stderr,"INTERNAL ERROR: malloc failed");
        exit(99);
    }
    else{

        new->data=data;

        if(garbage->First == NULL){   //Empty list
            garbage->First=new;
            garbage->Last=new;
            new->rptr=NULL;
            new->lptr=NULL;
        }
        else {                  //One or more elements in list
            new->lptr=garbage->Last;
            garbage->Last->rptr=new;
            new->rptr=NULL;
            garbage->Last=new;
        }
    }
}

void gc_delete_element(void *element){

    tDLElemPtr temp = garbage->Last;

    while(temp != NULL){
        if(temp == element){            //delete this one
            if(temp->rptr != NULL && temp->lptr != NULL){
                temp->rptr->lptr = temp->lptr;
                temp->lptr->rptr = temp->rptr;
            }
            else if(temp->rptr != NULL){
                temp->rptr->lptr = temp->lptr;
                garbage->First = temp->rptr;

            }
            else if(temp->lptr != NULL){
                temp->lptr->rptr = temp->rptr;
                garbage->Last = temp->lptr;
            }
            free(temp);
            break;

        }
        else{
            temp = temp->lptr;
        }
    }
}

void *gc_malloc(size_t size){

    void *ptr = malloc(size);
    if(ptr == NULL){
        gc_free_all();      //free all
        fprintf(stderr,"INTERNAL ERROR: malloc failed");
        exit(99);
    }

    gc_insert_last(ptr);    //if failed, exits in gc_insert_last func
    return ptr;

}

void *gc_realloc(void *ptr, size_t size){
    void *temp = ptr;
    //realloc feature size = 0, is for now not implemented

    ptr = realloc(ptr, size);
    if(ptr == NULL){
        //deletes origin pointer from log,
        // but it won't be freed, cause, it is maybe already freed by realloc
        gc_delete_element(gc_find(temp));
        gc_free_all();      //free all
        fprintf(stderr,"INTERNAL ERROR: malloc failed");
        exit(99);
    }
    if(ptr != temp){
        if(temp != NULL){ gc_delete_element(gc_find(temp)); }  //remove and free from log
        gc_insert_last(ptr);    //if failed, exits in gc_insert_last func
    }

    return ptr;
}



void gc_free(void *ptr){

    free(ptr);                //free usefull data
    gc_delete_element(gc_find(ptr));      //remove and free from log

}

void *gc_find(void *ptr){

    tDLElemPtr temp = garbage->Last;

    while(temp != NULL){
        if(temp->data == ptr){
            return temp;
        }
        temp = temp->lptr;
    }
    return NULL;
}


void gc_free_all(){

    while( copy_last(garbage) != NULL ){
        free(copy_last(garbage)); //free usefull data
        gc_delete_last();     //free structure from G
    }
}
