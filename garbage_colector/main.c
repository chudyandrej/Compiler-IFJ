
#include "garbage.h"

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
    int *b9 = gc_malloc(sizeof(int));
    int *b10 = gc_malloc(sizeof(int));
    int *b11 = gc_malloc(sizeof(int));
    int *b12 = gc_malloc(sizeof(int));
    int *b13 = gc_malloc(sizeof(int));
    int *b14 = gc_malloc(sizeof(int));
    int *b15 = gc_malloc(sizeof(int));
    int *b16 = gc_malloc(sizeof(int));
    int *b17 = gc_malloc(sizeof(int));
    int *b18 = gc_malloc(sizeof(int));
    int *b19 = gc_malloc(sizeof(int));
    int *b20 = gc_malloc(sizeof(int));
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
  
    

    free(G);
    return 0;
}