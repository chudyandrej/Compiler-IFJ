
#include "syntax_checker.h"

FILE *fp;

int main(int argc, char *argv[]){
    if(argc == 0){
	printf("give argument!\n");
    return 1;	
    }

    fp = fopen(argv[1],"r");
    int exit_code = start_syntax_analyz();
    fclose(fp);

    return exit_code;
}
