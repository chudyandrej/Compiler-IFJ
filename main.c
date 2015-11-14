#include "precedence.h"

FILE *fp;
tDLList *THC;
unsigned int tmp_counter = 0;
int main() {
    init_list(THC);



    fp = fopen("/Users/andrejchudy/Workspace/Compiler-IFJ/test.txt","r");
    int exit_code = expression_process(KIN_SEMICOLON);
    fclose(fp);

    return exit_code;
}
