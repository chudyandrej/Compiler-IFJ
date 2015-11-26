/*
* Soubor s implementacemi algoritmu pro predmet IAL
* Verze zadani: a/2/i
* a) Knuth-Morris-Pratt ˚uv algoritmus
* 2) Heap sort
* I) Tabulka symbolu pomoci binarniho stronu 
*/

/*
***************************************************************
** Tabulka symbolu pomoci binarniho vyhledavaciho stromu.
* Autor: xkondu00, Vaclav Kondula
* Popis: Globalni tabulka symbolu (funkci) a jednotlive lokalni 
tabulky spolu sdileji zakladni strukturu a funkce 
nad nimi (pridani, vyhledavani ...)
* Globalni BST: elementy typu tVar
* Lokalni BST: kazde element je double-linked list
***************************************************************
*/


/***********/
/* SDILENE */
/***********/
#include "ial.h"


void BSTInit (tBSTPtr T){
    T->Root = NULL;
    T->Act = NULL;
}

void BSTInsert(tBSTEPtr node, tBSTPtr T){
    if(T->Act==NULL) return;
    int comp = strcmp(node->key,T->Act->key);
    if(comp<0){
        if(T->Act->lptr==NULL) { 
            T->Act->lptr=node;
            T->Act = T->Act->lptr;
        }else {
            T->Act = T->Act->lptr;
            BSTInsert(node, T);
        }
    }
    else {
        if(T->Act->rptr==NULL) { 
            T->Act->rptr=node;
            T->Act = T->Act->rptr;
        }else {
            T->Act = T->Act->rptr;
            BSTInsert(node, T);
        }
    }
}

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

void GSTDispose(tBSTPtr T){
    funcFree(T->Root);
    T->Root=NULL;
    T->Act=NULL;
}
int GSTAllDef(tBSTEPtr tmp){
    if(tmp==NULL) return 1;
    if(tmp->data==NULL) return 2;
    if(((struct tFunc *)tmp->data)->TAC==NULL) return 0;
    int a = GSTAllDef(tmp->lptr);
    if(a!=1) return a;
    a = GSTAllDef(tmp->rptr);
    return a;
}  
int GSTDeclare(tBSTPtr T, char * params, char * names){
    if(T->Act==NULL) return 2;
    if(T->Act->data==NULL){
        tFuncPtr node = gc_malloc(sizeof(struct tFunc));
        node->params = params;
        node->names = names;
        node->TAC = NULL;
        T->Act->data = node;
    } else {
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
int GSTDefine(tBSTPtr T, void * TAC){
    if(T->Act==NULL || T->Act->data==NULL) return 2;
    if(((struct tFunc *)T->Act->data)->TAC != NULL) return 1;
    ((struct tFunc *)T->Act->data)->TAC = TAC;
    return 0;
}

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
    gc_free(ptr->key);
    gc_free(ptr);
}

void LSTDispose (tBSTPtr T){
    varFree(T->Root);
    T->Root=NULL;
    T->Act=NULL;
}

int LSTAdd (tBSTPtr T, enum Type type, int scope){
    if (T->Act->data!=NULL)
        if(((struct tVar *)T->Act->data)->scope == scope) return 1;
    tVarPtr node = gc_malloc(sizeof(struct tVar));
    node->scope = scope;
    node->assigned = 0;
    node->value.t = type;
    tVarPtr tmp = T->Act->data;
    T->Act->data = node;
    node->ptr = tmp;
    return 0;
}  
int LSTSet (tBSTPtr T, struct TMPRecord * v){
    tVarPtr tmp = T->Act->data;
    if (tmp==NULL)
        return 3;
    if (tmp->value.t == STRING){
        if (v->t == STRING) tmp->value.value = v->value;
        else return 4;
    }
    else if(tmp->value.t == DOUBLE){
        if (v->t == DOUBLE) tmp->value.value = v->value;
        else if (v->t == INT) tmp->value.value.d = (double) v->value.i;
        else return 4;
    }
    else if(tmp->value.t == INT){
        if (v->t == INT) tmp->value.value = v->value;
        else if (v->t == DOUBLE) tmp->value.value.i = (int) v->value.d;
        else return 4;
    }
    else if(tmp->value.t == AUTO){
        tmp->value.value = v->value;
        tmp->value.t = v->t;
    }
    else return 4;
    tmp->assigned = 1;
    return 0;
}  

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

int LSTGet (tBSTPtr T, struct TMPRecord * v){
    tVarPtr tmp = T->Act->data;
    if (tmp==NULL){
        return 8    ;
    }
    if(tmp->assigned == 0){
        v->t = tmp->value.t;
        return 2;
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

char * substr(char * str, int i, int n){
    int l = strlen(str);
    char * out = NULL;
    if (i<0);
    else if (i==l){
        out = gc_malloc(sizeof(1));
        out[0] = '\0';
    } 
    else if (i<l){
        int end;
        if (l>n+i) end = n+i;
        else end = l;
        out = gc_malloc(sizeof(end-i+1));
        memcpy(out, &str[i], end-i);
    }
    return out;
}