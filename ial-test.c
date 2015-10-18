#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ial.h"


int main(){
    tBSTPtr T = malloc(sizeof(struct tBST));
    BSTInit(T);
    printf("**INIT GLOBAL SYMBOL TABLE**\n");
    printf("active: %d\n", BSTActive(T));
    BSTAdd(T, "FML");
    printf("add FML\n");
    printf("Act: %s\n", T->Act->key);
    printf("Root: %s\n", T->Root->key);
    printf("active: %d\n", BSTActive(T));
    BSTAdd(T, "FOO");
    printf("add FOO\n");
    BSTAdd(T, "BAR");
    printf("add BAR\n");
    printf("Root->rptr: %s\n", T->Root->rptr->key);
    printf("Root->lptr: %s\n", T->Root->lptr->key);
    BSTAdd(T, "FN");
    printf("add FN\n");
    printf("Root->rptr->lptr: %s\n", T->Root->rptr->lptr->key);
    printf("find non-existing key\n");
    BSTFind(T, "FNO");
    printf("active: %d\n", BSTActive(T));
    printf("find FOO\n");
    BSTFind(T, "FOO");
    printf("active: %d\n", BSTActive(T));
    printf("Act: %s\n", T->Act->key);
    printf("**************\n");
    printf("everything defined: %d\n", GSTAllDef(T->Root));
    BSTFind(T, "FOO");
    printf("declare FOO,vi,1: %d\n", GSTDeclare(T, "vi", 1));
    BSTFind(T, "BAR");
    printf("declare BAR,ii,2: %d\n", GSTDeclare(T, "ii", 2));
    BSTFind(T, "FML");
    printf("declare FML,iii,3: %d\n", GSTDeclare(T, "iii", 3));
    BSTFind(T, "FN");
    printf("declare FN,ii,4: %d\n", GSTDeclare(T, "ii", 4));
    BSTFind(T, "FML");
    printf("re-declare correctlty FML,iii,5: %d\n", GSTDeclare(T, "iii", 5));
    printf("re-declare incorrectlty FML,isi,6: %d\n", GSTDeclare(T, "isi", 6));
    printf("everything defined: %d\n", GSTAllDef(T->Root));
    BSTFind(T, "FNO");
    printf("declare not added FNO,ii,7: %d\n", GSTDeclare(T, "ii", 7));
    
    char * a =malloc(10);
    char * b =malloc(10);
    char * c =malloc(10);
    char * d =malloc(10);

    BSTFind(T, "FML");
    GSTDeclare(T, "iii", 8);
    printf("define FML,iii,8: %d\n", GSTDefine(T, a, 8));

    BSTFind(T, "FN");
    GSTDeclare(T, "ii", 9);
    printf("define FN,ii,9: %d\n", GSTDefine(T, b, 9));

    BSTFind(T, "BAR");
    GSTDeclare(T, "ii", 10);
    printf("define BAR,ii,10: %d\n", GSTDefine(T, c, 10));

    printf("everything defined: %d\n", GSTAllDef(T->Root));

    BSTFind(T, "FOO");
    GSTDeclare(T, "vi", 11);
    printf("define FOO,vi,11: %d\n", GSTDefine(T, d, 11));

    BSTFind(T, "FOO");
    GSTDeclare(T, "vi", 12);
    printf("re-define FOO,vi,12: %d\n", GSTDefine(T, d, 12));

    printf("everything defined: %d\n", GSTAllDef(T->Root));
    printf("dispose Tree\n");
    GSTDispose(T);
    free(T);
    printf("END\n");
    return 0;
}