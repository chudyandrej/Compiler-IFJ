
#include "parser.h"

int start_syntax_analyz(){

    Token *new_token = next_token();
    switch(new_token->type){
        case KW_AUTO:
        case KW_DOUBLE:
        case KW_INT:
        case KW_STRING:
            free(new_token);
            return dec_function();
        case END_OF_FILE:
            free(new_token);
            errorMessage_syntax("Program file is empty !");
            return 1;
        default:
            free(new_token);
            errorMessage_syntax("Error on global scope !");
            return 1;
    }
}

int dec_function(){
    Token *new_token = next_token();
    if (new_token->type == KIN_IDENTIFIER){
        free(new_token);
        if ((new_token = next_token())->type == KIN_L_ROUNDBRACKET){
            free(new_token);
            if(parameters_declar() == 0){
                new_token = next_token();
                if (new_token->type == KIN_SEMICOLON){
                    free(new_token);
                    //generacia instrukcnej sady;
                    return 0;
                }
                else if(new_token->type == KIN_L_BRACE) {
                    free(new_token);
                    //generacia instrukcnej sady;
                    return body_funcion();
                }
                else {
                    errorMessage_syntax("Declaration function !");
                    return 1;
                }
            }
        }
    }
    errorMessage_syntax("Declaration function !");
    return 1;
}

int body_funcion(){
    while(true) {
        Token *new_token = next_token();
        switch(new_token->type){
            case KW_CIN:
                free(new_token);
                if (cin_cout(KIN_SCIN) == 0){continue;} else{return 1;}
            case KW_COUT:
                free(new_token);
                if (cin_cout(KIN_SCOUT) == 0){continue;} else{return 1;}
            case KW_IF:
                free(new_token);
                if (if_statement() == 0){continue;} else{return 1;}
            case KW_FOR:
                free(new_token);
                if (for_statement() == 0){continue;} else{return 1;}
            case KW_RETURN:
                free(new_token);
                if(expression_process(KIN_SEMICOLON) == 0){continue;} else{return 1;}
            case KW_SORT:
            case KW_LENGTH:
                free(new_token);
                if ((new_token = next_token())->type == KIN_L_ROUNDBRACKET && parameters_used() == 1 ){
                    free(new_token);
                    if((new_token = next_token())->type == KIN_SEMICOLON){
                        free(new_token);
                        continue;
                    }
                }
                return 1;
            case KW_FIND:
            case KW_CONCAT:
                free(new_token);
                if ((new_token=next_token())->type == KIN_L_ROUNDBRACKET && parameters_used() == 2 ){
                    free(new_token);
                    if((new_token=next_token())->type == KIN_SEMICOLON){
                        free(new_token);
                        continue;
                    }
                }
                return 1;
            case KW_SUBSTR:
                free(new_token);
                if ((new_token=next_token())->type == KIN_L_ROUNDBRACKET && parameters_used() == 3 ){
                    free(new_token);
                    if((new_token=next_token())->type == KIN_SEMICOLON){
                        free(new_token);
                        continue;
                    }
                }
                return 1;
            case KIN_IDENTIFIER:
                free(new_token);
                if (assing_funcCall() == 0){continue;} else{return 1;}
            case KW_AUTO:   case KW_DOUBLE:  case KW_INT:  case KW_STRING:      //all datatypes
                free(new_token);
                if (dec_variable() == 0) {continue;} else {return 1;}
            case KIN_R_BRACE:
                free(new_token);
                return 0;
            default:
                free(new_token);
                errorMessage_syntax(" Body of function !");
                return 1;
        }
    }
}

int for_statement() {
    Token *new_token;
    if ((new_token=next_token())->type == KIN_L_ROUNDBRACKET){
        free(new_token);
        new_token= next_token();
        if((new_token->type >= KW_AUTO) && (new_token->type <= KW_STRING)) {
            free(new_token);
            if (((new_token=next_token())->type == KIN_IDENTIFIER) && (assing_funcCall() == 0)) {
                free(new_token);
                if (expression_process(KIN_SEMICOLON) == KIN_SEMICOLON && (new_token = next_token())->type == KIN_IDENTIFIER) {
                    free(new_token);
                    if (assing_funcCall() == 0 && (new_token = next_token())->type == KIN_L_BRACE) {
                        free(new_token);
                        return body_funcion();

                    }
                }
            }
        }
        else if (new_token->type == KIN_IDENTIFIER) {
            free(new_token);
            if ((assing_funcCall() == 0) && (expression_process(KIN_SEMICOLON) == 0)){
                if((new_token=next_token())->type == KIN_IDENTIFIER) {
                    free(new_token);
                    if (assing_funcCall() == 0 && (new_token = next_token())->type == KIN_L_BRACE) {
                        free(new_token);
                        return body_funcion();
                    }
                }
            }
        }
    }
    errorMessage_syntax("Error in for statement !");
    return 1;
}

int if_statement(){
    Token *new_token;
    if(((new_token=next_token())->type == KIN_L_ROUNDBRACKET) && (expression_process(KIN_R_ROUNDBRACKET)== KIN_R_ROUNDBRACKET)){
        free(new_token);
       if((new_token=next_token())->type == KIN_L_BRACE && (body_funcion() == 0)) {
           free(new_token);
           if ((new_token = next_token())->type == KW_ELSE) {
               free(new_token);
               if ((new_token = next_token())->type == KIN_L_BRACE) {
                   free(new_token);
                   return body_funcion();
               }
           }
       }
    }
    errorMessage_syntax("Bad syntax in \"if\"");
    return 1;
}

int cin_cout(enum sTokenKind operator){         //treba spravit specificky cout
    Token *new_token;
    if ((new_token=next_token())->type ==  operator){
        free(new_token);
        while(true){
            new_token = next_token();
            if (new_token->type == KIN_IDENTIFIER) {
                Token *new_token2 = next_token();
                if (new_token2->type == operator) {
                    free(new_token);
                    free(new_token2);
                    continue;
                }
                else if(new_token2->type == KIN_SEMICOLON){     //kontrola
                    free(new_token);
                    free(new_token2);
                    return 0;
                }
                else {
                    errorMessage_syntax("Cin or cout command!");
                    return 1;
                }
            }
            free(new_token);
            break;
        }
    }
    errorMessage_syntax("Cin or cout operator!");
    return 1;
}

int assing_funcCall(){
    Token *new_token = next_token();
    if ((new_token->type == KIN_L_ROUNDBRACKET) && (parameters_used() != 100)) {        //func call
        free(new_token);
        if((new_token=next_token())->type == KIN_SEMICOLON){
            free(new_token);
            return 0;
        }
    }
    else if (new_token->type == KIN_ASSIGNEMENT){       //assing var
        free(new_token);
        return (expression_process(KIN_SEMICOLON) == KIN_SEMICOLON)? 0 : 1;
    }
    errorMessage_syntax("Assing function!");
    return 1;
}

int dec_variable(){
    Token *new_token;
    if ((new_token=next_token())->type == KIN_IDENTIFIER) {
        free(new_token);
        new_token = next_token();
        if (new_token->type == KIN_SEMICOLON) {
            free(new_token);
            return 0;
        }
        else if(new_token->type == KIN_ASSIGNEMENT){
            free(new_token);
            if(expression_process(KIN_SEMICOLON) == KIN_SEMICOLON){
                return 0;
            }
            else{
                errorMessage_syntax("Bad syntax in assingment");
                return 1;
            }
        }
    }
    errorMessage_syntax("Declaration variable");
    return 1;
}

int parameters_declar(){
    Token *new_token= next_token();
    if(new_token->type == KIN_R_ROUNDBRACKET) {
        free(new_token);
        return 0;
    }                                                /*means no parameters*/
    while(true) {
        if((new_token->type >= KW_AUTO) && (new_token->type <= KW_STRING)){
            free(new_token);
            if ((new_token=next_token())->type == KIN_IDENTIFIER) {
                free(new_token);
                new_token = next_token();
                if (new_token->type == KIN_R_ROUNDBRACKET){
                    free(new_token);
                    return 0;
                }
                else if (new_token->type == KIN_COMMA) {
                    free(new_token);
                    new_token = next_token();
                    continue;
                }
            }
        }
        errorMessage_syntax("Declaration parameters!");
        return 1;
    }
}

int parameters_used(){
    int counter_of_arguments = 0;
    while(true) {
        counter_of_arguments++;
        int exit_code_value = expression_process(KIN_COMMA);
        if(exit_code_value == KIN_COMMA) {
            continue;
        }
        else if(exit_code_value == KIN_R_ROUNDBRACKET){
            return counter_of_arguments;
        }
        errorMessage_syntax("Parameter used!");
        return 100;
    }
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

Token *next_token(){

    Token *new_token = token_predict;
    token_predict = get_token(fp);
    if(new_token == NULL ){
        //odalocujeme
        errorMessage_internal("Malloc error");
        exit(INTER_ERR);
    } else if(new_token->type == KIN_UNKNOWN){
        errorMessage_lexical("");
        //odalocujeme
        exit(LEX_ERR);
    }
    else{
        printf("Token: %d precedence_token: %d\n",new_token->type,token_predict->type);
        return new_token;
    }
}