#include <stdlib.h>
#include <stdio.h>
#include "stack.h"

int main(){
    tDLList *L = malloc(sizeof(tDLList));


    init_list(L);
    printf("init done\n");
    dispose_list(L);
    printf("dispose empty list done\n");
    int *a=malloc(sizeof(int));
    void *b;
    int *c=malloc(sizeof(int));
    *a=9;
   
    insert_last(L, a);
    printf("insert last done\n");
    printf("last item is: %d\n\n",  *(int*)copy_last(L) );
    
    *c=10;
    activate_last(L);
    post_insert(L, c);
    printf("activate last and post insert done\n");
    printf("last item is: %d\n",  *(int*)copy_last(L) );
    insert_last(L, a);
    printf("insert last done\n");
    printf("last item is: %d\n\n",  *(int*)copy_last(L) );
    
    *c=54;
    shift_active_right(L);
    printf("shift activate done\n");
    post_insert(L, c);
    printf("post insert done\n");
    printf("last item is: %d\n\n",  *(int*)copy_last(L) );

    delete_last(L);
    printf("delete last complete\n");
    printf("last item is: %d\n",  *(int*)copy_last(L) );

    dispose_list(L);
    free(L);
    free(a);
    free(c);
    return 0;
}
