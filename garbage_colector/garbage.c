#include "garbage.h"

 
void gc_delete_last(){

    if(G->First != NULL){           /*Non-empty list*/

        if(G->Last == G->Active){
            G->Active = NULL;
        }

        tDLElemPtr temp;

        if(G->First == G->Last){    /*Just one element in list*/
            free(G->First);
            G->First=NULL;
            G->Last=NULL;
        }
        else{                       /*Two or more elements in list*/

            temp=G->Last->lptr;
            free(G->Last);
            G->Last=temp;
            G->Last->rptr=NULL;
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

        if(G->First == NULL){   //Empty list
            G->First=new;
            G->Last=new;
            new->rptr=NULL;
            new->lptr=NULL;
        }
        else {                  //One or more elements in list
            new->lptr=G->Last;
            G->Last->rptr=new;
            new->rptr=NULL;
            G->Last=new;
        }
    }
}

void gc_delete_element(void *element){

    tDLElemPtr temp = G->Last;

    while(temp != NULL){
        if(temp == element){            //delete this one
            if(temp->rptr != NULL && temp->lptr != NULL){
                temp->rptr->lptr = temp->lptr;
                temp->lptr->rptr = temp->rptr;
            }
            else if(temp->rptr != NULL){
                temp->rptr->lptr = temp->lptr;
                G->First = temp->rptr;

            }
            else if(temp->lptr != NULL){
                temp->lptr->rptr = temp->rptr;
                G->Last = temp->lptr;
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
        gc_delete_element(find(temp)); 
        gc_free_all();      //free all
        fprintf(stderr,"INTERNAL ERROR: malloc failed");
        exit(99);
    }
    if(ptr != temp){
        if(temp != NULL){ gc_delete_element(find(temp)); }  //remove and free from log
        gc_insert_last(ptr);    //if failed, exits in gc_insert_last func
    }
   
    return ptr;
}



void *gc_free(void *ptr){

    free(ptr);                //free usefull data
    gc_delete_element(find(ptr));      //remove and free from log

}

void *find(void *ptr){

    tDLElemPtr temp = G->Last;

    while(temp != NULL){
        if(temp->data == ptr){
            return temp;
        }
        temp = temp->lptr;
    }
    return NULL;
}


void gc_free_all(){

    while( copy_last(G) != NULL ){
        free(copy_last(G)); //free usefull data
        gc_delete_last();     //free structure from G
    }
}
 