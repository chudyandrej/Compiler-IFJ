#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ial.h"


int main(){
    tBSTPtr T = malloc(sizeof(struct tBST));
    BSTInit(T);
    printf("**INIT**\n");
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
    printf("END\n");
    return 0;
}