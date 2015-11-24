/*
* Soubor s implementacemi algoritmu pro predmet IAL
* Verze zadani: a/2/i
* a) Knuth-Morris-Prattuv algoritmus
* 2) Heap sort
* I) Tabulka symbolu pomoci binarniho stronu 
*/

#ifndef IAL_H_INCLUDED
#define IAL_H_INCLUDED
#include "tac.h"
#include "stack.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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
typedef struct tBSTElem {
    char * key;     // klic pro porovnavani
    void * data;    // pointer na data:
                        //globalni TS: struktura tFunc
                        //lokalni TS: DL list, nebo NULL, kdyz neexistuje
    struct tBSTElem *lptr;   // levy element
    struct tBSTElem *rptr;   // pravy element
} *tBSTEPtr;

typedef struct tBST{
    tBSTEPtr Root;  // koren
    tBSTEPtr Act;   // aktivni
} *tBSTPtr;

void BSTInit (tBSTPtr);
void BSTAdd (tBSTPtr, char *); //zaroven oznaci pridane za Aktualni
void BSTFind (tBSTPtr, char *);
int BSTActive (tBSTPtr); //return 0 kdzy neaktivni

/*
** Pridani noveho symbolu: **
BSTFind(&T, key);
if( ! BSTActive) BSTAdd(&T. key);
*/

/***************/
/* Globalni TS */
/***************/
typedef struct tFunc{
    char * params; 
        // d = double
        // i = int
        // s = string
        // prvni je navratova hodnota, zbyvajici parametry v danem poradi
    void * TAC; //pointer na list 3AC 
    char * names; //rozdelene mezerami
    /*! zmenit void az bude 3AC*/
} *tFuncPtr;

void * GSTCopyTAC(tBSTPtr);
void GSTDispose(tBSTPtr);
int GSTAllDef(tBSTEPtr rootPtr); // return 1 kdyz vsechny deklarovane funkce byly definovany
int GSTDeclare(tBSTPtr, char * params, char * names); //return 0 kdyz parametry odpovidaji (nebo byly prazdne)
int GSTDefine(tBSTPtr, void * TACs); //vzdy volat declare pred define!!!
                                        //return 0 kdyz jde o prvni definici


/**************/
/* Lokalni TS */
/**************/
/*! osetrit datatype auto*/

typedef struct tVar{
    int scope; //zanoreni, pri volani funkce zacina na 0
    char assigned;
    struct TMPRecord value; 
    struct tVar * ptr;    //pointer na pravy
                        //pri deklaraci se pridavaji prvky ZLEVA
} *tVarPtr;

void LSTDispose (tBSTPtr);
int LSTAdd (tBSTPtr, Type type, int scope); //return 0 kdyz nebylo v zanorenim doposud definovano
int LSTSet (tBSTPtr, struct TMPRecord * v); //return 0 kdyz nedoslo k nekompatibilite typu
                            //vzdy nastavuje aktivni prvek v poslednim zanoreni!!
void LSTLeaveScope (tBSTEPtr root, int scope); 
        //odstrani vsechny lokalni promene z daneho zanoreni
int LSTGet (tBSTPtr, struct TMPRecord * v);

/**************/
/*  BUILD IN  */
/**************/

int find(char *s, char *search);
void swap(char *s, int i, int j);
void sift_down(char *s, int start, int length);
void sort(char *s);
char * substr(char * str, int i, int n);

#endif //IAL_H_INCLUDED