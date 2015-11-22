#include "parser.h"
#include "bst.h"

FILE *fp;

Token *token_predict = NULL;
struct tBST Func;
tDLList *tac_stack;

void print_instuctions();

int main(int argc, char *argv[]) {
    BSTInit(&Func);
    fp = fopen(argv[1],"r");
    token_predict = get_token(fp);
    int exit_code = start_syntax_analyz();

    fclose(fp);
    printf("\n############---FINAL RESULTS---############\n");
    printf("NO. instructions: %d \n", length_list(tac_stack));
    printf("exit_code: %d  !!!!\n", exit_code);
    
    print_instuctions();

    return exit_code;
}


int is_active(tDLList *S){
   
    return S->Active==NULL?0:1;
}
void activate_first(tDLList *S){
    
    S->Active = S->First;
}
void *copy_active(tDLList *S){
   
    if(S->Active != NULL){
        return S->Active->data;
    }
    else {
        return NULL;
    }
}
void shift_active(tDLList *S){
    
    if(S->Active != NULL){
        S->Active = S->Active->rptr;
    }
}

   
void print_instuctions(){

    printf("\nVYPIS INSTRUKCII:------------------\n");
   
    activate_first(tac_stack);
   
    while(is_active(tac_stack)){ //cyklit nad vsem 3AC instrukcemi
       
        struct Operation *rec = copy_active(tac_stack);
        TokenKind instruction = rec->ction_inst;
        //printf("INSTRUKCE CISLO: %d\n", instruction);
        switch (instruction){

            case KIN_PLUS:
                printf("ADD\n");                
                break; 
            case KIN_MINUS:
                printf("MINUS\n");
                break;
            case KIN_MUL:
                printf("MUL\n");
                break;  
            case KIN_DIV:
                printf("DIV\n");
                break;  
            case KIN_PLUSPLUS:
                printf("PLUSPLUS\n");
                break;  
            case KIN_MINUSMINUS:              
                printf("MINUSMINUS\n");
                 break;  
            default:
                printf("unknown operation\n");
        }        
        shift_active(tac_stack);
    }

}