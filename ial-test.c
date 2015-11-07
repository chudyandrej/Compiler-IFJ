#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ial.h"

void global();
void local();

int main(){
    global();
    local();
    printf("END\n");
    return 0;
}

void local(){
    printf("\n\n\n**INIT LOCAL SYMBOL TABLE**\n");
    union Value v;
    union Value ptr;
    tBSTPtr T = malloc(sizeof(struct tBST));
    BSTInit(T);
    printf("active: %d\n", BSTActive(T));
    BSTAdd(T, "FML");
    printf("add FML\n");
    BSTAdd(T, "FOO");
    printf("add FOO\n");
    BSTAdd(T, "BAR");
    printf("add BAR\n");
    printf("Act: %s\n", T->Act->key);
    printf("add int BAR scope 0: %d\n", LSTAdd(T, 'i', 0));
    v.i = 6;
    printf("set double BAR scope 0 to 6: %d\n", LSTSet(T, 'i', v));
    printf("add double BAR scope 1: %d\n", LSTAdd(T, 'd', 1));
    printf("add incorrectly int BAR scope 1: %d\n", LSTAdd(T, 'i', 1));
    v.d = 5.0;
    printf("set double BAR scope 1 to 5.0: %d\n", LSTSet(T, 'd', v));
    v.d = 3.0;
    printf("re-set double BAR scope 1 to 3.0: %d\n", LSTSet(T, 'd', v));
    printf("Return value: %c\n", LSTGet(T, &ptr));
    printf("Value of BAR: %g\n", ptr.d);
    v.i = 4;
    printf("incorrect set double BAR scope 1 to 3 (int): %d\n", LSTSet(T, 'i', v));
    printf("Return value: %c\n", LSTGet(T, &ptr));
    printf("Value of BAR: %g\n", ptr.d);
    LSTLeaveScope(T->Root, 1);
    printf("Leave scope 1\n");
    BSTFind(T, "BAR");
    printf("act->key: %s\n", T->Act->key);
    printf("Return value: %c\n", LSTGet(T, &ptr));
    printf("Value of BAR: %d\n", ptr.i);
    LSTDispose (T);
    free(T);
}


void global(){ //tested, everything passed
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
    printf("declare FOO,vi,1: %d\n", GSTDeclare(T, "vi"));
    BSTFind(T, "BAR");
    printf("declare BAR,ii,2: %d\n", GSTDeclare(T, "ii"));
    BSTFind(T, "FML");
    printf("declare FML,iii,3: %d\n", GSTDeclare(T, "iii"));
    BSTFind(T, "FN");
    printf("declare FN,ii,4: %d\n", GSTDeclare(T, "ii"));
    BSTFind(T, "FML");
    printf("re-declare correctlty FML,iii,5: %d\n", GSTDeclare(T, "iii"));
    printf("re-declare incorrectly FML,isi,6: %d\n", GSTDeclare(T, "isi"));
    printf("everything defined: %d\n", GSTAllDef(T->Root));
    BSTFind(T, "FNO");
    printf("declare not added FNO,ii,7: %d\n", GSTDeclare(T, "ii"));
    
    char * a =malloc(10);
    char * b =malloc(10);
    char * c =malloc(10);
    char * d =malloc(10);

    BSTFind(T, "FML");
    GSTDeclare(T, "iii");
    printf("define FML,iii,8: %d\n", GSTDefine(T, a));

    BSTFind(T, "FN");
    GSTDeclare(T, "ii");
    printf("define FN,ii,9: %d\n", GSTDefine(T, b));

    BSTFind(T, "BAR");
    GSTDeclare(T, "ii");
    printf("define BAR,ii,10: %d\n", GSTDefine(T, c));

    printf("everything defined: %d\n", GSTAllDef(T->Root));

    BSTFind(T, "FOO");
    GSTDeclare(T, "vi");
    printf("define FOO,vi,11: %d\n", GSTDefine(T, d));

    BSTFind(T, "FOO");
    GSTDeclare(T, "vi");
    printf("re-define FOO,vi,12: %d\n", GSTDefine(T, d));

    printf("everything defined: %d\n", GSTAllDef(T->Root));
    printf("dispose Tree\n");
    GSTDispose(T);
    free(T);
}