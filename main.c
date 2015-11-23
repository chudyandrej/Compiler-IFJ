

#include "parser.h"

FILE *fp;

Token *token_predict = NULL;
struct tBST Func;
tDLList *tac_stack;
tDLList *garbage = NULL;

union Address fake;

void print_instuctions();

int main(int argc, char *argv[]) {
    BSTInit(&Func);
    if((garbage= malloc(sizeof(tDLList))) == NULL){
        errorMessage_internal("Malloc garbage collector");
        return 1;
    }
    init_list(garbage);
    fp = fopen(argv[1],"r");
    token_predict = get_token(fp);
    int exit_code = start_syntax_analyz();

    fclose(fp);
    if(exit_code == 0) {
        printf("\n############---FINAL RESULTS---############\n");
        printf("NO. instructions: %d \n", length_list(tac_stack));
        printf("exit_code: %d  !!!!\n", exit_code);

        print_instuctions();
    }
    free(garbage);
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
            case TAC_GOTO_UNCOND:
                printf("TAC_GOTO_UNCOND\n");
                break;
            case TAC_GOTO_COND:
                printf("TAC_GOTO_UNCOND\n");
                break;
            case TAC_PUSH:
                printf("TAC_PUSH\n");
                break;
            case TAC_INIT:
                printf("TAC_INIT\n");
                break;
            case TAC_CALL:
                printf("TAC_CALL\n");
                break;
            case TAC_RETURN:
                printf("TAC_RETURN\n");
                break;
            case SCOPE_UP:
                printf("SCOPE_UP\n");
                break;
            case SCOPE_DOWN:
                printf("SCOPE_DOWN\n");
                break;
            case TAC_EMPTY:
                printf("TAC_EMPTY\n");
                break;
            case KIN_ASSIGNEMENT:
                printf("KIN_ASSIGNEMENT\n");
                break;
            case KIN_SCIN:
                printf("KIN_SCIN\n");
                break;
            case KIN_SCOUT:
                printf("KIN_SCOUT\n");
                break;
            case KIN_SMALLER:
                printf("KIN_SMALLER\n");
                break;
            case KIN_GREATER:
                printf("KIN_GREATER\n");
                break;
            case KIN_SMALLER_EQ:
                printf("KIN_SMALLER_EQ\n");
                break;
            case KIN_GREATER_EQ:
                printf("KIN_GREATER_EQ\n");
                break;
            case KIN_NOT_EQ:
                printf("KIN_NOT_EQ\n");
                break;
            case KIN_UNARYMINUS:
                printf("KIN_UNARYMINUS\n");
                break;
            default:
                printf("unknown operation %d \n", instruction);
        }        
        shift_active(tac_stack);
    }

}
