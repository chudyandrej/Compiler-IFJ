

#include "err.h"

int handle_err(tErrCode code) {
    fprintf(stderr, "ERROR MESSAGE");
    return code;
}

void errorMessage_syntax(const char *message ){
    fprintf(stderr,"############### SYNTAX ERROR ##############\n");
    fprintf(stderr,"Error message: %s \n",message);
    fprintf(stderr,"###########################################\n");

}
void errorMessage_lexical(const char *message ){
    fprintf(stderr,"################ LEXICAL ERROR #############\n");
    fprintf(stderr,"Error message: %s \n",message);
    fprintf(stderr,"############################################\n");

}
void errorMessage_internal(const char *message ){
    fprintf(stderr,"############### INTERNAL ERROR ############\n");
    fprintf(stderr,"Error message: %s \n",message);
    fprintf(stderr,"###########################################\n");

}
void errorMessage_semantic(const char *message ){
    fprintf(stderr,"############### SEMANTIC ERROR ############\n");
    fprintf(stderr,"Error message: %s \n",message);
    fprintf(stderr,"###########################################\n");

}