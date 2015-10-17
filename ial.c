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

void BSTInit (tBST * T){return ;}
void BSTAdd (tBST * T, char * key){return ;} 
void BSTFind (tBST * T, char * key){return ;}
int BSTActive (tBST * T){return 0;}


/***************/
/* Globalni TS */
/***************/

void GSTDispose(tBST * T){return ;}
int GSTAllDef(tBST * T){return 0;}  
int GSTDeclare(tBST * T, char * params, int dec){return 0;}  
int GSTDefine(tBST * T, void * TAC, int def){return 0;}  
                                         


/**************/
/* Lokalni TS */
/**************/

void LSTDispose (tBST * T){return ;}
int LSTAdd (tBST * T, char type, int scope){return 0;}  
int LSTSet (tBST * T, char type, void * value){return 0;}  
void LSTLeaveScope (tBST * T, int scope){return ;} 
