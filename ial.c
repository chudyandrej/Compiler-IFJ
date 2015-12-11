/*
* Implementations of algarithms for IAL class
* Assigment version: a/2/i
* a) Knuth-Morris-Pratt algoritm
* 2) Heap sort
* I) Symbol table using BST
*/

/*
***************************************************************
** Symbol table using BInary search tree.
* Author: xkondu00, Vaclav Kondula
* Descripiton: Both global and local sumbol table shares some
strcuctures and operations. Other operations are implemented 
specificly for one ot another.
* Global BST: elements of type tVar
* Local BST: every node in Local BST is a stack
***************************************************************
*/


/***********/
/* SHARED */
/***********/
#include "ial.h"

// initialize a new BST, must be empty
void BSTInit (tBSTPtr T){
    T->Root = NULL;
    T->Act = NULL;
}

void BSTInsert(tBSTEPtr node, tBSTPtr T){
    if(T->Act==NULL) return;
    int comp = strcmp(node->key,T->Act->key);
    if(comp<0){ // follow left pointer
        if(T->Act->lptr==NULL) { 
            T->Act->lptr=node;
            T->Act = T->Act->lptr;
        }else {
            T->Act = T->Act->lptr;
            BSTInsert(node, T);
        }
    }
    else { // folow right pointer
        if(T->Act->rptr==NULL) { 
            T->Act->rptr=node;
            T->Act = T->Act->rptr;
        }else {
            T->Act = T->Act->rptr;
            BSTInsert(node, T);
        }
    }
}

// add new node, uses BSTInsert
void BSTAdd (tBSTPtr T, char * key){
    tBSTEPtr node = gc_malloc(sizeof(struct tBSTElem));
    node->key = key;
    node->lptr=NULL;
    node->rptr=NULL;
    node->data=NULL;

    T->Act = T->Root;
    BSTInsert(node, T);
    if (T->Root==NULL){
        T->Root = node;
        T->Act = T->Root;
    }
    
} 

//set Act (active) pointer to found node
void BSTFind (tBSTPtr T, char * key){
    T->Act = NULL;
    tBSTEPtr tmp = T->Root;
    while(tmp!=NULL){
        int comp = strcmp(key,tmp->key);
        if(comp<0) tmp = tmp->lptr;
        else if(comp>0) tmp = tmp->rptr;
        else {
            T->Act = tmp;
            tmp = NULL;
        }
    }
}

int BSTActive (tBSTPtr T){
    return T->Act!=NULL?1:0;
}


/***************/
/* Globalni TS */
/***************/
void funcFree(tBSTEPtr ptr){
    if(ptr==NULL) return;
    funcFree(ptr->lptr);
    funcFree(ptr->rptr);
    if(ptr->data!=NULL){
        gc_free(((struct tFunc *)ptr->data)->params);
        gc_free(((struct tFunc *)ptr->data)->TAC);
        gc_free(ptr->data);
    }
    gc_free(ptr->key);
    gc_free(ptr);
}

// free all nodes in a tree
void GSTDispose(tBSTPtr T){
    funcFree(T->Root);
    T->Root=NULL;
    T->Act=NULL;
}

// cheks whether all functions are defined
int GSTAllDef(tBSTEPtr tmp){
    if(tmp==NULL) return 1;
    if(tmp->data==NULL) return 2;
    if(((struct tFunc *)tmp->data)->TAC==NULL) return 0;
    int a = GSTAllDef(tmp->lptr);
    if(a!=1) return a;
    a = GSTAllDef(tmp->rptr);
    return a;
}  

// declare function
int GSTDeclare(tBSTPtr T, char * params, char * names){
    if(T->Act==NULL) return 2;
    if(T->Act->data==NULL){ // function didn't exist before
        tFuncPtr node = gc_malloc(sizeof(struct tFunc));
        node->params = params;
        node->names = names;
        node->TAC = NULL;
        T->Act->data = node;
    } else { // checks whether last prototype of function is same as this one
        if(strcmp(params,((struct tFunc *)T->Act->data)->params)!=0)
            return 1;
        if ((names==NULL)||(((struct tFunc *)T->Act->data)->names==NULL)){
            if ((names==NULL)&&(((struct tFunc *)T->Act->data)->names==NULL)) return 0;
            else return 1;
        }
        if(strcmp(names,((struct tFunc *)T->Act->data)->names)!=0)
            return 1;
    }
    return 0;
}
// stores stack of 3AC to global ST 
int GSTDefine(tBSTPtr T, void * TAC){
    if(T->Act==NULL || T->Act->data==NULL) return 2;
    if(((struct tFunc *)T->Act->data)->TAC != NULL) return 1;
    ((struct tFunc *)T->Act->data)->TAC = TAC;
    return 0;
}

// returns pointer to a 3AC stack of active function 
void * GSTCopyTAC(tBSTPtr T){
    return ((struct tFunc *)T->Act->data)->TAC;
}  
                                         


/**************/
/* Lokalni TS */
/**************/
void scopeFree(tVarPtr ptr){
    if (ptr==NULL) return;
    scopeFree(ptr->ptr);
    gc_free(ptr);
}

void varFree(tBSTEPtr ptr){
    if(ptr==NULL) return;
    varFree(ptr->lptr);
    varFree(ptr->rptr);
    scopeFree(ptr->data);
    gc_free(ptr);
}

// dispose all nodes and all stacks of a tree
void LSTDispose (tBSTPtr T){
    varFree(T->Root);
    T->Root=NULL;
    T->Act=NULL;
}

// Initialize variable in a given scope
int LSTAdd (tBSTPtr T, enum Type type, int scope){
    if (T->Act->data!=NULL)
        if(((struct tVar *)T->Act->data)->scope == scope) return 3;
    tVarPtr node = gc_malloc(sizeof(struct tVar));
    node->scope = scope;
    node->assigned = 0;
    node->value.t = type;
    tVarPtr tmp = T->Act->data;
    T->Act->data = node;
    node->ptr = tmp;
    return 0;
}  

// set variable in the highest posible scope and retypes if necessary
int LSTSet (tBSTPtr T, struct TMPRecord * v){
    tVarPtr tmp = T->Act->data;
    if (tmp==NULL)
        return 3;
    if (tmp->value.t == STRING){ // if variable is string accept only string
        if (v->t == STRING) tmp->value.value = v->value;
        else return 4;
    }
    else if(tmp->value.t == DOUBLE){ //if duoble
        if (v->t == DOUBLE) tmp->value.value = v->value; // store double
        else if (v->t == INT) tmp->value.value.d = (double) v->value.i; // retype from integer
        else return 4;
    }
    else if(tmp->value.t == INT){ // if int
        if (v->t == INT) tmp->value.value = v->value; // store int
        else if (v->t == DOUBLE) tmp->value.value.i = (int) v->value.d; //retype form double
        else return 4;
    }
    else if(tmp->value.t == AUTO){ // if auto, store anything and set type of variable
        tmp->value.value = v->value;
        tmp->value.t = v->t;
    }
    else return 4;
    tmp->assigned = 1;
    return 0;
}  

// remove all variables in given scope
void LSTLeaveScope (tBSTEPtr ptr, int scope){
    if(ptr==NULL) return;
    if(ptr->data!=NULL)
        if(((struct tVar *)ptr->data)->scope == scope){
            tVarPtr tmp = ptr->data;
            ptr->data = ((struct tVar *)ptr->data)->ptr;
            gc_free(tmp);
        }
    LSTLeaveScope(ptr->lptr, scope);
    LSTLeaveScope(ptr->rptr, scope);
    return ;
} 

// saves value to a pointer and return 0 if success or exit error_code
int LSTGet (tBSTPtr T, struct TMPRecord * v){
    tVarPtr tmp = T->Act->data;
    if (tmp==NULL){ // variable wasn't defined 
        return 3;
    }
    if(tmp->assigned == 0){ // variable wasn't declared
        v->t = tmp->value.t;
        return 8;
    }
    v->value = tmp->value.value;
    v->t = tmp->value.t;
    return 0;
}

/***************/
/*  BUILD IN   */
/***************/

/**
 * Knuth-Morris-Pratt algorithm for string matching
 * @param  s      string array which is searched
 * @param  search string array which is used to search in s
 * @return        starting index of search string in s array (indexed from 0) 
 */
int find(char *s, char *search){       
    int pattern_length = strlen(search);    
    
    if (pattern_length == 0) { return 0; }
    
    int table[pattern_length];
    table[0] = 0;       /*must be zero*/
    int i=0; 
    int j=0; 
    
    /*fill table with values*/
    for(j = 1; j < pattern_length; j++){
        if(search[j] == search[i]){
            i++;
            table[j] = i;
        }
        else{
            while(i > 0 && search[i] != search[j]){
                i--;
            }
            if(i == 0 &&  search[i] == search[j]){
                table[j] = 1;
                i = 1;
            }
            else{
                table[j]=i;
            }     
        }
    }
    
    
    int source_length=strlen(s);
    j=0;
    
    for(i=0; i < source_length; i++){
        if(search[j] ==  s[i]){
            j++;
            if(j==pattern_length){      /*found search in s*/
                return i-j+1;
            }
        }
        else{
            if(j==0) continue;      /*go again and compare next char of string with pattern*/
            j=table[j-1];           /*save number according the table to j and */
            i--;                    /*compare again the same char of string with new char of pattern*/
        }        
    }    
    
    return -1;    
}

/**
 * Swap two array items given by indexes
 * @param  s pointer to char array
 * @param  i index the first item in array 's' to exchange
 * @param  j index the second item in array 's' to exchange
 */
void swap(char *s, int i, int j){
    char swap;
    swap = s[i];
    s[i] = s[j];
    s[j] = swap;
}


/**
 * Reorder array (tree) by sorting heap tree
 * @param  s        pointer to char array
 * @param  start    index of nodeon which level is sift executed
 * @param  length   actual length of array 's'
 */
void sift_down(char *s, int start, int length) {
    int parent = start;

    while ( 2*parent + 1 < length) {
        int child = 2*parent + 1;
        if (child + 1 < length) { /* have both right and left childs */
            if (s[child] < s[child+1]) {
                child += 1; /* set bigger child to be swapped unit */
            }
        }
        if (s[parent] < s[child]) {
            swap(s, parent, child); /* swap if child is bigger then parent */
            parent = child;
        }
        else {
            break;
        }
    }
}


/**
 * Sort characters in array with heap-sort algoritm
 * @param  s        pointer to char array
 * @param  length   actual length of array 's'
 */
void sort(char *s) {
    int start, end;
    int length = strlen(s);

    /* heapify => make heap tree*/
    for (start = (length - 2)/2; start >= 0; start--) {
        sift_down(s, start, length);
    }

    for (end = length -1; end > 0; end--) {
        swap(s, end, 0);
        sift_down(s, 0, end);
    }
}
