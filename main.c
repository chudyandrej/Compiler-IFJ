#include "parser.h"

FILE *fp;

Token *token_predict = NULL;
struct tBST Func;
tDLList *tac_stack;
tDLList *garbage = NULL;
union Address fake;
void foobar(struct Operation * rec);
void print_instuctions();

int main(int argc, char *argv[]) {
    printf("%d\n", argc);
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
        printf("//////////////\n");
        exit_code = interpret();
        printf("//////////////\n");
        printf("konec: %d\n", exit_code);
    }
    free(garbage);
    return exit_code;
}

   
void print_instuctions(){

    printf("\nVYPIS INSTRUKCII:------------------\n");
    activate_first(tac_stack);
    while(is_active(tac_stack)){ //cyklit nad vsem 3AC instrukcemi
        struct Operation *rec = copy_active(tac_stack);
        TokenKind instruction = rec->inst;
        //printf("INSTRUKCE CISLO: %d\n", instruction);
        switch (instruction){
            case KIN_PLUS:
                printf("ADD");                
                break; 
            case KIN_MINUS:
                printf("MINUS");
                break;
            case KIN_MUL:
                printf("MUL");
                break;  
            case KIN_DIV:
                printf("DIV");
                break;  
            case KIN_PLUSPLUS:
                printf("PLUSPLUS");
                break;  
            case KIN_MINUSMINUS:              
                printf("MINUSMINUS");
                 break;
            case TAC_GOTO_UNCOND:
                printf("TAC_GOTO_UNCOND");
                break;
            case TAC_GOTO_COND:
                printf("TAC_GOTO_UNCOND");
                break;
            case TAC_PUSH:
                printf("TAC_PUSH");
                break;
            case TAC_INIT:
                printf("TAC_INIT");
                break;
            case TAC_CALL:
                printf("TAC_CALL");
                break;
            case TAC_RETURN:
                printf("TAC_RETURN");
                break;
            case SCOPE_UP:
                printf("SCOPE_UP");
                break;
            case SCOPE_DOWN:
                printf("SCOPE_DOWN");
                break;
            case TAC_EMPTY:
                printf("TAC_EMPTY");
                break;
            case KIN_ASSIGNEMENT:
                printf("KIN_ASSIGNEMENT");
                break;
            case KIN_SCIN:
                printf("KIN_SCIN");
                break;
            case KIN_SCOUT:
                printf("KIN_SCOUT");
                break;
            case KIN_SMALLER:
                printf("KIN_SMALLER");
                break;
            case KIN_GREATER:
                printf("KIN_GREATER");
                break;
            case KIN_SMALLER_EQ:
                printf("KIN_SMALLER_EQ");
                break;
            case KIN_GREATER_EQ:
                printf("KIN_GREATER_EQ");
                break;
            case KIN_NOT_EQ:
                printf("KIN_NOT_EQ");
                break;
            case KIN_UNARYMINUS:
                printf("KIN_UNARYMINUS");
                break;
            default:
                printf("unknown operation %d ", instruction);
        }
        foobar(rec);
        if ((int)rec->label>0) printf("         >>>%d", (int)rec->label);
            
        printf("\n");
        shift_active(tac_stack);
    }

}

void foobar(struct Operation * rec){
    switch (rec->t_t){
        case TMP:
                printf("  TMP %d, ", rec->t.tmp);
                break;                    
        case INT:
                printf("  INT %d, ", rec->t.i);
                break;
        case DOUBLE:
                printf("  DOUBLE %g, ", rec->t.d);
                break;
        case VARIABLE:
                printf("  VAR %s, ", rec->t.variable);
                break;
        case STRING:
                printf("  STR %s, ", rec->t.s);
                break;
        case FUNCION:
                printf("  FCE %s, ", rec->t.fce);
                break;
        case LABEL:
                printf("  LABEL %d, ", (int)rec->t.label);
                break;
        default:
                printf(", ");
                break;            
    }
        switch (rec->t_op1){
        case TMP:
                printf("TMP %d, ", rec->op1.tmp);
                break;                    
        case INT:
                printf("INT %d, ", rec->op1.i);
                break;
        case DOUBLE:
                printf("DOUBLE %g, ", rec->op1.d);
                break;
        case VARIABLE:
                printf("VAR %s, ", rec->op1.variable);
                break;
        case STRING:
                printf("STR %s, ", rec->op1.s);
                break;
        case FUNCION:
                printf("FCE %s, ", rec->op1.fce);
                break;
        default:
                printf(", ");
                break;            
    }
        switch (rec->t_op2){
        case TMP:
                printf("TMP %d, ", rec->op2.tmp);
                break;                    
        case INT:
                printf("INT %d, ", rec->op2.i);
                break;
        case DOUBLE:
                printf("DOUBLE %g, ", rec->op2.d);
                break;
        case VARIABLE:
                printf("VAR %s, ", rec->op2.variable);
                break;
        case STRING:
                printf("STR %s, ", rec->op2.s);
                break;
        case FUNCION:
                printf("FCE %s, ", rec->op2.fce);
                break;
        default:
                printf(", ");
                break;            
    }
}