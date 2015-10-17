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
* Globalni BST: elementy typu tVarEl
* Lokalni BST: kazde element je double-linked list
***************************************************************
*/


/***********/
/* SDILENE */
/***********/
#include "ial.h"
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
    node->key = malloc(strlen(key));
    if (node->key==NULL) {
        interError();
        return;
    }
    strcpy(node->key, key);
    node->lptr=NULL;
    node->rptr=NULL;

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

void GSTDispose(tBSTPtr T){return ;}
int GSTAllDef(tBSTPtr T){return 0;}  
int GSTDeclare(tBSTPtr T, char * params, int dec){return 0;}  
int GSTDefine(tBSTPtr T, void * TAC, int def){return 0;}  
                                         


/**************/
/* Lokalni TS */
/**************/

void LSTDispose (tBSTPtr T){return ;}
int LSTAdd (tBSTPtr T, char type, int scope){return 0;}  
int LSTSet (tBSTPtr T, char type, void * value){return 0;}  
void LSTLeaveScope (tBSTPtr T, int scope){return ;} 
