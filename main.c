#include "parser.h"
#include "err.h"

FILE *fp;
Token *token_predict = NULL;
struct tBST Func;
tDLList *tac_stack;
tDLList *garbage = NULL;
union Address fake;
void foobar(struct Operation * rec);
void print_instuctions();

int main(int argc, char *argv[]) {
    
    if (argc == 1)
    {
        errorMessage_internal("No argument");
        return INTER_ERR;
    }
    else if (argc > 2)
    {
        errorMessage_internal("Too many arguments");
        return INTER_ERR;
    }
    
    BSTInit(&Func);
    if((garbage = malloc(sizeof(tDLList))) == NULL){
        errorMessage_internal("Malloc garbage collector");
        return INTER_ERR;
    }
    init_list(garbage);
    if ((fp = fopen(argv[1],"r")) == NULL ) {
        errorMessage_internal("Can not open file. Make sure the path is right, or that you have rights to read");
        return INTER_ERR;
    }
    token_predict = get_token(fp);
    int exit_code = start_syntax_analyz();

    fclose(fp);
    if(exit_code == 0) {
        exit_code = interpret();
        if(exit_code) handle_err(exit_code);
    }
    gc_free_all();
    free(garbage);
    return exit_code;
}
