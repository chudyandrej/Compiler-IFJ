
#include "garbage.h"
#include <string.h>
#include <stdio.h>

tDLList *G = NULL;
   




int main(){
    
    if((G= malloc(sizeof(tDLList))) != NULL){
        init_list(G);
    }

    int *b1 = gc_malloc(sizeof(int));
    int *b2 = gc_malloc(sizeof(int));
    int *b3 = gc_malloc(sizeof(int));
    int *b4 = gc_malloc(sizeof(int));
    int *b5 = gc_malloc(sizeof(int));
    int *b6 = gc_malloc(sizeof(int));
    int *b7 = gc_malloc(sizeof(int));
    int *b8 = gc_malloc(sizeof(int));
    char *b9 = gc_malloc(sizeof(char));
    int *b10 = gc_malloc(sizeof(int));
    int *b11 = gc_malloc(sizeof(int));
    double *b12 = gc_malloc(sizeof(double)*5);
    int *b13 = gc_malloc(sizeof(int));
    int *b14 = gc_malloc(sizeof(int));
    int *b15 = gc_malloc(sizeof(int));
    int *b16 = gc_malloc(sizeof(int));
    int *b17 = gc_malloc(sizeof(int));
    int *b18 = gc_malloc(sizeof(int));
    int *b19 = gc_malloc(sizeof(int));
    int *b20 = gc_malloc(sizeof(int)*2);
    int *b21 = gc_malloc(sizeof(int));
    int *b22 = gc_malloc(sizeof(int));
    int *b23 = gc_malloc(sizeof(int));
    int *b24 = gc_malloc(sizeof(int));
    int *b25 = gc_malloc(sizeof(int));
    int *b26 = gc_malloc(sizeof(int));
    int *b27 = gc_malloc(sizeof(int));
    int *b28 = gc_malloc(sizeof(int));
    int *b29 = gc_malloc(sizeof(int));
    int *b30 = gc_malloc(sizeof(int));
    gc_free(b1);
    gc_free(b30);
    gc_free(b25);
    gc_free(b18);
    gc_free(b14);
    gc_free(b4);
    gc_free(b6);
    gc_free_all();
    printf("length of log after gc_free_all: %d\n", length_list(G));

    char *b31 = gc_malloc(sizeof(char)*2+1);
    int *b32 = gc_malloc(sizeof(int));
    printf("length of log: %d\n", length_list(G));
    strcpy(b31, "bl");
    b31 = gc_realloc(b31, sizeof(char)*100);
    strcpy(b31, "blablsdfadsfasdfasdfsafsadsfsadfsfssf");
    printf("length of log after realloc: %d\n", length_list(G));

    gc_free_all();
    printf("length of log after gc_free_all: %d\n", length_list(G));

    free(G);
    return 0;
}