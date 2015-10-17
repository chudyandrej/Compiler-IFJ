
#include "syntax_checker.h"


FILE *fp;

int main(int argc, char *argv[]) {

    fp = fopen("/home/andrej/ClionProjects/Compiler-IFJ/test.cpp","r");
    int exitcode = start_syntax_analyz();
    fclose(fp);


    return exitcode;
}
