

#include "err.h"

void handle_err(tErrCode code) {
    if(code==1) errorMessage_lexical("1");
    else if (code==2) errorMessage_syntax("2");
    else if (code<=10) errorMessage_semantic("semantic error");
    else errorMessage_internal("");
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
