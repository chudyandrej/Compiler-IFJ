#include "parser.h"
#include "bst.h"

FILE *fp;

Token *token_predict = NULL;
struct tBST Func;
tDLList *tac_stack;

int main(int argc, char *argv[]) {
    BSTInit(&Func);
    fp = fopen(argv[1],"r");
    token_predict = get_token(fp);
    int exit_code = start_syntax_analyz();

    fclose(fp);
    printf("exit_code: %d\n!!!!#####", exit_code);
    
    return exit_code;
}
