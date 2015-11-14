#include "precedence.h"

FILE *fp;
tDLList *THC;
unsigned int tmp_counter = 0;
int main() {
    init_list(THC);

   expression_process();

   // fp = fopen("/Users/andrejchudy/Workspace/Compiler-IFJ/test.cpp","r");
    /*int exit_code = start_syntax_analyz();
    fclose(fp);*/

    return 1;
}
