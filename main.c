
#include "syntax_checker.h"


FILE *fp;

int main(int argc, char *argv[]) {

    fp = fopen("/home/andrej/ClionProjects/Compiler-IFJ/over_program.cpp","r");
    start_syntax_analyz();
    fclose(fp);


    return 0;
}
