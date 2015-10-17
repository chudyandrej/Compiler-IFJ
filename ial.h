/*
* Soubor s implementacemi algoritmu pro predmet IAL
* Verze zadani: a/2/i
* a) Knuth-Morris-Prattuv algoritmus
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
#ifndef IAL_H_INCLUDED
#define IAL_H_INCLUDED

typedef struct {
    tBSTEPtr Root;  // koren
    tBSTEPtr Act;   // aktivni
} tBST;


typedef struct {
    char * key;     // klic pro porovnavani
    void * data;    // pointer na data:
                        //globalni TS: struktura tFunc
                        //lokalni TS: DL list, nebo NULL, kdyz neexistuje
    struct tBSTEPtr *lptr;   // levy element
    struct tBSTEPtr *rptr;   // pravy element
} *tBSTEPtr;

void BSTInit (tBST *);
void BSTAdd (tBST *, char *); //zaroven oznaci pridane za Aktualni
void BSTFind (tBST *, char *);
int BSTActive (tBST *); //return 0 kdzy neaktivni


/***************/
/* Globalni TS */
/***************/
typedef struct tFunc{
    char * params; 
        // v = void
        // d = double
        // a = auto
        // i = int
        // s = string
        // prvni je navratova hodnota, zbyvajici parametry v danem poradi
    int def; //BOOL definovano, default = False
    int order_def; //poradi definice 
    int order_dec; //poradi dkelarace
    void * TAC; //pointer na list 3AC 
    /*! zmenit void az bude 3AC*/
} *tFuncPtr;

void GSTDispose(tBST *);
int GSTAllDef(tBST *); // return 0 kdyz vsechny deklarovane funkce byly definovany
int GSTDeclare(tBST *, char *, int); //return 0 kdyz parametry odpovidaji (nebo byly prazdne)
int GSTDefine(tBST *, void * TAC, int); //vzdy volat declare pred define!!!
                                        //return 0 kdyz jde o prvni definici


/**************/
/* Lokalni TS */
/**************/
/*! osetrit datatype auto*/
typedef struct tVarEl{
    int scope; //zanoreni, pri volani funkce zacina na 0
    char datatype; // obdobne jako u params v tFunc
    void * value; // NULL if not defined
    tVarElPtr * ptr;    //pointer na pravy
                        //pri deklaraci se pridavaji prvky ZLEVA
} *tVarElPtr;

void LSTDispose (tBST *);
int LSTAdd (tBST *, char type, int scope); //return 0 kdyz nebylo v zanorenim doposud definovano
int LSTSet (tBST *, char type, void *); //return 0 kdyz nedoslo k nekompatibilite typu
void LSTLeaveScope (tBTS *, int scope); 
        //odstrani vsechny lokalni promene z daneho zanoreni

#endif IAL_H_INCLUDED