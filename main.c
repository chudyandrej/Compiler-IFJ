#include "precedence.h"
#include "parser.h"


FILE *fp;

Token *token_predict = NULL;

int main(int argc, char *argv[]) {

    fp = fopen(argv[1],"r");
    token_predict = get_token(fp);
    int exit_code = expression_process(KIN_SEMICOLON);
   //  = start_syntax_analyz();

   fclose(fp);

    return exit_code ;
}