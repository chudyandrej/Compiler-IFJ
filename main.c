#include "precedence.h"


FILE *fp;


int main() {


   fp = fopen("/Users/andrejchudy/Workspace/Compiler-IFJ/test.txt","r");
   int exit_code = expression_process(KIN_SEMICOLON);
    fclose(fp);

    return exit_code;
}
