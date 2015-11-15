#include "precedence.h"


FILE *fp;


int main(int argc, char *argv[]) {

    fp = fopen(argv[1],"r");
    int exit_code = expression_process(KIN_SEMICOLON);
    fclose(fp);

    return exit_code;
}