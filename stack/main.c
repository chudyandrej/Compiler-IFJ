#include <stdlib.h>
#include <stdio.h>
#include "stack.h"

int main(){
	tDLList L;


	init_list(&L);
	printf("init done\n");
	dispose_list(&L);
	printf("dispose empty list done\n");
	int *a=malloc(sizeof(int));
	int *b=malloc(sizeof(int));
	int *c=malloc(sizeof(int));
	*a=9;
	*c=10;
	insert_last(&L, a);
	printf("insert last done\n");
	copy_last(&L, b);
	printf("last item is: %d\n", *b );

	//activate_last(&L);
	//post_insert(&L, c);

	//copy_last(&L, b);
	//printf("last item is: %d\n", *b);


	dispose_list(&L);
	free(a);
	free(b);
	free(c);
	return 0;
}
