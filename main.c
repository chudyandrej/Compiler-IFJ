
#include "syntax_checker.h"


FILE *fp;

int main() {

    fp = fopen("/Users/andrejchudy/Workspace/Compiler-IFJ/test.cpp","r");
    int exit_code = start_syntax_analyz();
    fclose(fp);

    return exit_code;
}
