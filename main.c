#include "precedence.h"
#include "parser.h"


FILE *fp;


int main(int argc, char *argv[]) {

    fp = fopen(argv[1],"r");
    int exit_code = start_syntax_analyz();
    fclose(fp);

    return exit_code;
}