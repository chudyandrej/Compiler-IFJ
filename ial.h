/*
* Implementations of algarithms for IAL class
* Assigment version: a/2/i
* a) Knuth-Morris-Pratt algoritm
* 2) Heap sort
* I) Symbol table using BST
*/

#ifndef IAL_H_INCLUDED
#define IAL_H_INCLUDED
#include "stack.h"
#include "garbage.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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
typedef struct tBSTElem {
    char * key;     // key for comparing
    void * data;    // data pointer:
                        //global ST: struktura tFunc
                        //local ST: DL list, or NULL
    struct tBSTElem *lptr;   // left element
    struct tBSTElem *rptr;   // right element
} *tBSTEPtr;

typedef struct tBST{
    tBSTEPtr Root;  // root element
    tBSTEPtr Act;   // active element
} *tBSTPtr;

void BSTInit (tBSTPtr);
void BSTAdd (tBSTPtr, char *); //it also mark added as active
void BSTFind (tBSTPtr, char *); // set Act to found node or NULL if not found
int BSTActive (tBSTPtr); //return 0 if not active

/***************/
/* Globalni TS */
/***************/
typedef struct tFunc{
    char * params; 
        // d = double
        // i = int
        // s = string
        // a = auto
        // first is a return vlaue,m others are parameter types
    void * TAC; //pointer to a stack of 3AC 
    char * names; //names of parameters separated by spaces
} *tFuncPtr;

#include "tac.h"
void * GSTCopyTAC(tBSTPtr);
void GSTDispose(tBSTPtr);
int GSTAllDef(tBSTEPtr rootPtr); // return 1 if all declared functions were also defined
int GSTDeclare(tBSTPtr, char * params, char * names); //return 0 if params match or are none
int GSTDefine(tBSTPtr, void * TACs); //always call declare before define!!!
                                        //return 0 if it's first definition


/**************/
/* Lokalni TS */
/**************/

typedef struct tVar{
    int scope; //defines in whitch scope was value initialize
    char assigned;
    struct TMPRecord value; 
    struct tVar * ptr;    //pointer to right
} *tVarPtr;

void LSTDispose (tBSTPtr);
int LSTAdd (tBSTPtr, Type type, int scope); //return 0 if there was no variabl of same name in given scope
int LSTSet (tBSTPtr, struct TMPRecord * v); //return 0 if types match or could be retyped
void LSTLeaveScope (tBSTEPtr root, int scope); 
        //remove all values in given scope
int LSTGet (tBSTPtr, struct TMPRecord * v);

/**************/
/*  BUILD IN  */
/**************/

int find(char *s, char *search); // return valus is the index of 1st character in 1st match
                                // or -1 if not found
void swap(char *s, int i, int j);
void sift_down(char *s, int start, int length);
void sort(char *s);

#endif //IAL_H_INCLUDED