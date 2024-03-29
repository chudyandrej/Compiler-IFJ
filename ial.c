

#include "ial.h"
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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int INTError = 0;
void interError(){
    INTError = 1;
    fprintf(stderr, "**Internal Error**\n not enough space for malloc\n");
}

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
    tBSTEPtr node = malloc(sizeof(struct tBSTElem));
    if (node==NULL) {
        interError();
        return;
    }
    node->key = malloc(strlen(key)+1);
    if (node->key==NULL) {
        interError();
        return;
    }
    strcpy(node->key, key);
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
        free(((struct tFunc *)ptr->data)->params);
        free(((struct tFunc *)ptr->data)->TAC);
        free(ptr->data);
    }
    free(ptr->key);
    free(ptr);
}

void GSTDispose(tBSTPtr T){
    funcFree(T->Root);
    T->Root=NULL;
    T->Act=NULL;
}
int GSTAllDef(tBSTEPtr tmp){
    if(tmp==NULL) return 1;
    if(tmp->data==NULL) return 2;
    if(((struct tFunc *)tmp->data)->TAC!=NULL) return 0;
    int a = GSTAllDef(tmp->lptr);
    if(a!=1) return a;
    a = GSTAllDef(tmp->rptr);
    return a;
}
int GSTDeclare(tBSTPtr T, char * params, char * names){
    if(T->Act==NULL) return 2;
    if(T->Act->data==NULL){
        tFuncPtr node = malloc(sizeof(struct tFunc));
        node->params = malloc(strlen(params)+1);
        if (node->params==NULL) {
            interError();
            return 2;
        }
        strcpy(node->params, params);
        node->TAC = NULL;
        T->Act->data = node;
    } else {
        if(strcmp(params,((struct tFunc *)T->Act->data)->params)!=0)
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



/**************/
/* Lokalni TS */
/**************/
void scopeFree(tVarPtr ptr){
    if (ptr==NULL) return;
    scopeFree(ptr->ptr);
    if ((ptr->datatype == 's') && (ptr->value.s != NULL))
        free(ptr->value.s);
    free(ptr);
}

void varFree(tBSTEPtr ptr){
    if(ptr==NULL) return;
    varFree(ptr->lptr);
    varFree(ptr->rptr);
    scopeFree(ptr->data);
    free(ptr->key);
    free(ptr);
}

void LSTDispose (tBSTPtr T){
    varFree(T->Root);
    T->Root=NULL;
    T->Act=NULL;
}

int LSTAdd (tBSTPtr T, char type, int scope){
    if (T->Act->data!=NULL)
    if(((struct tVar *)T->Act->data)->scope == scope) return 1;
    tVarPtr node = malloc(sizeof(struct tVar));
    if (node==NULL) {
        interError();
        return 2;
    }
    node->scope = scope;
    node->datatype = type;
    node->assigned = 0;
    node->value.s = NULL;
    tVarPtr tmp = T->Act->data;
    T->Act->data = node;
    node->ptr = tmp;
    return 0;
}
int LSTSet (tBSTPtr T, char type, union Value v){
    tVarPtr tmp = T->Act->data;
    if (tmp==NULL)
        return 1;
    if (tmp->datatype == 'a')
        tmp->datatype = type;
    if (tmp->datatype != type)
        //! pretypovani dodelam pozdeji
        return 2;
    if (tmp->datatype == 's')
    if (tmp->value.s != NULL)
        free (tmp->value.s);
    tmp->value = v;
    tmp->assigned = 1;
    return 0;
}

void LSTLeaveScope (tBSTEPtr ptr, int scope){
    if(ptr==NULL) return;
    if(ptr->data!=NULL)
    if(((struct tVar *)ptr->data)->scope == scope){
        tVarPtr tmp = ptr->data;
        ptr->data = ((struct tVar *)ptr->data)->ptr;
        if ((tmp->datatype == 's') && (tmp->value.s != NULL))
            free(tmp->value.s);
        free(tmp);
    }
    LSTLeaveScope(ptr->lptr, scope);
    LSTLeaveScope(ptr->rptr, scope);
    return ;
}

char LSTGet (tBSTPtr T, union Value * v){
    tVarPtr tmp = T->Act->data;
    if (tmp==NULL)
        return 1;
    if(tmp->assigned == 0)
        return 2;
    *v = tmp->value;
    return tmp->datatype;
}
