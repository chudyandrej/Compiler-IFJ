#include <mach/task.h>
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
        if (next_token() == KIN_L_ROUNDBRACKET){
            if(parameters() == 0){
                new_token = next_token();
                if (new_token == KIN_SEMICOLON){ return 0;}
                else if(new_token == KIN_L_BRACE) {
                    int exit_code = body_funcion();
                    if (exit_code == 0) { return 0; }
                    errorMessage("Error in body of program !");
                    return 1;
                }
                else {
                    errorMessage("Error during declaration function !");
                    return 1;
                }
            }
        }
    }
    errorMessage("Error during declaration function !");
    return 1;
}

int body_funcion(){
    while(true) {
        int  new_token = next_token();
        switch(new_token){
            case KW_CIN:
                if (cin_cout(KIN_SCIN) == 0){ continue;} else{ return 1;}
            case KW_COUT:
                if (cin_cout(KIN_SCOUT) == 0){ continue;} else{ return 1;}
            case KW_IF:
                if (if_statement() == 0){ continue;} else{ return 1;}
            case KW_FOR:
                if (for_statement() == 0){ continue;} else{ return 1;}
            case KW_RETURN:
                if(expression_process(KIN_SEMICOLON) == KIN_SEMICOLON){ continue;} else{ return 1;}
            case KW_SORT:
            case KW_LENGTH:
                if (next_token() == KIN_L_ROUNDBRACKET &&
                    parameters_used() == 1 && next_token() == KIN_SEMICOLON){continue;} else{return 1;}
            case KW_FIND:
            case KW_CONCAT:
                if (next_token() == KIN_L_ROUNDBRACKET &&
                    parameters_used() == 2  && next_token() == KIN_SEMICOLON){continue;} else{ return 1;}
            case KW_SUBSTR:
                if (next_token() == KIN_L_ROUNDBRACKET &&
                    parameters_used() == 3  && next_token() == KIN_SEMICOLON){continue;} else{ return 1;}
            case KIN_IDENTIFIER:
                if (assing(KIN_SEMICOLON) == 0){ continue; } else{ return 1; }
            case KW_AUTO:   case KW_DOUBLE:  case KW_INT:  case KW_STRING:      //all datatypes
                if (dec_variable() == KIN_SEMICOLON) { continue; } else { return 1; }
            case KIN_R_BRACE:
                return 0;
            default:
                errorMessage("Error in body of program !");
                return 1;
        }
    }
}

int for_statement() {
    if (next_token() == KIN_L_ROUNDBRACKET) {
        int new_token = next_token();
        if((new_token == KW_AUTO) || (new_token == KW_DOUBLE) || (new_token == KW_INT) || (new_token == KW_STRING)) {
            if ((next_token() == KIN_IDENTIFIER) && (assing(KIN_SEMICOLON) == 0) && (expression_process(KIN_SEMICOLON) == KIN_SEMICOLON) &&     //treba zment
                (next_token() == KIN_IDENTIFIER) && (assing(KIN_R_ROUNDBRACKET) == 0)&& next_token() == KIN_L_BRACE){
                return body_funcion();
            }
        }
        else if (new_token == KIN_IDENTIFIER) {
            if ((assing(KIN_SEMICOLON) == 0)&& (expression_process(KIN_SEMICOLON) == KIN_SEMICOLON) && (next_token() == KIN_IDENTIFIER) &&
                (assing(KIN_R_ROUNDBRACKET) == 0) && next_token() == KIN_L_BRACE) {
                return body_funcion();
            }
        }
    }
    errorMessage("Error in for statement !");
    return 1;
}

int if_statement(){
    if((next_token()== KIN_L_ROUNDBRACKET) && (expression_process(KIN_R_ROUNDBRACKET)== 0) &&
       (next_token()== KIN_L_BRACE) && (body_funcion() == 0)){
        if ((next_token() == KW_ELSE) && (next_token() == KIN_L_BRACE)){
            return body_funcion();
        }
    }
    return 1;
}

int cin_cout(int op){
    if (next_token() ==  op){
        while(true){
            if (next_token() == KIN_IDENTIFIER) {
                int new_token = next_token();
                if (new_token == op) {continue; }
                else if(new_token == KIN_SEMICOLON){return 0;}
                else {
                    errorMessage("error in cin or cout command!");
                    return 1;
                }
            }
            else{
                break;
            }
        }
    }
    errorMessage("Error in cin or cout operator!");
    return 1;
}

int assing(int PREDICT_EXIT){
    int new_token = next_token();
    if ((new_token == KIN_L_ROUNDBRACKET) && (parameters_used() != 100) && (next_token() == KIN_SEMICOLON)) {
        return 0;
    }
    else if (new_token == KIN_ASSIGNEMENT){
        return expression_process(KIN_SEMICOLON);
    }
    errorMessage("Error in assing function!");
    return 1;
}


int dec_variable(){
    if (next_token() == KIN_IDENTIFIER) {
        int new_token = next_token();
        if (new_token == KIN_SEMICOLON) {
            return 0;
        }
        else if(new_token == KIN_ASSIGNEMENT){
            return expression_process(KIN_SEMICOLON);
        }
    }
    errorMessage("Error in declaration variable");
    return 1;
}

int parameters(){
    int new_token = next_token();
    if(new_token == KIN_R_ROUNDBRACKET) {
        return 0;
    }                                                /*means no parameters*/
    while(true) {
        if((new_token == KW_AUTO) || (new_token == KW_DOUBLE) || (new_token == KW_INT) || (new_token == KW_STRING)) {
            if (next_token() == KIN_IDENTIFIER) {
                new_token = next_token();
                if (new_token == KIN_R_ROUNDBRACKET) {
                    return 0;
                }
                else if (new_token == KIN_COMMA) {
                    new_token = next_token();
                    continue;
                }
            }
        }
        errorMessage("Error in declaration parameters!");
        return 1;
    }
}

int parameters_used(){

    int counter_of_arguments = 0;

    while(true) {
        counter_of_arguments++;
        int exit_code_value = expression_process(KIN_SEMICOLON);
        if(exit_code_value == KIN_COMMA) {
            continue;
        }
        else if(exit_code_value == KIN_R_ROUNDBRACKET){
            return counter_of_arguments;
        }
        errorMessage("Error in parameter used!");
        return 100;
    }
}


void errorMessage_syntax(const char *message ){
    fprintf(stderr,"############ SYNTAX ERROR ###########\n");
    fprintf(stderr,"Error message: %s \n",message);
    fprintf(stderr,"#####################################\n");

}
void errorMessage_lexical(const char *message ){
    fprintf(stderr,"############ LEXICAL ERROR ##########\n");
    fprintf(stderr,"Error message: %s \n",message);
    fprintf(stderr,"#####################################\n");

}
void errorMessage_internal(const char *message ){
    fprintf(stderr,"############ INTERNAL ERROR #########\n");
    fprintf(stderr,"Error message: %s \n",message);
    fprintf(stderr,"#####################################\n");

}

Token *next_token(){

    Token *new_token = get_token(fp);
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
        return new_token;
    }
}