#include "stack.h"


void init_list(tDLList *L){

	L->First=NULL;
	L->Last=NULL;
	L->Active=NULL;
}

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

void activate_last(tDLList *L){

	L->Active = L->Last;
}

void shift_active_right(tDLList *L){
	
	if(L->Active != L->Last && L->Active != NULL){
		L->Active = L->Active->rptr;
	}
}

void copy_last(tDLList *L, void *data){
	
	data=L->Last->data;
	
}
