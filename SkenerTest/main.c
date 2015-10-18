#include <stdio.h>
#include "scanner.h"



int main(int argc, char *argv[]){
    FILE *fp;
    fp = fopen(argv[1], "r");
    Token *token;
    
    while((token=get_token(fp))->type != END_OF_FILE){
        printf("%d \n", token->type);    
    }
    fclose(fp);
    return 0;
}
