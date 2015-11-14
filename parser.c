//
// Created by Andrej Oliver Chudý on 07/11/15.
//

#include "parser.h"
//
// Created by andrej on 15.10.2015.
//



int start_syntax_analyz(){


    int new_token = next_token();
    switch(new_token){
        case KW_AUTO:
        case KW_DOUBLE:
        case KW_INT:
        case KW_STRING:
            return dec_function();
        case END_OF_FILE:
            errorMessage("Program file is empty !");
            return 1;
        default:
            errorMessage("Error on global scope !");
            return 1;
    }
}

int dec_function(){
    int new_token = next_token();
    if (new_token == KIN_IDENTIFIER){
        if (next_token() == KIN_L_ROUNDBRACKET){
            if(parameters() == 0){
                new_token = next_token();
                if (new_token == KIN_IDENTIFIER){
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
                if(value() == KIN_SEMICOLON){ continue;} else{ return 1;}
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
                if (dec_variable() == 0) { continue; } else { return 1; }
            case KIN_PLUSPLUS:

            case KIN_R_BRACE:
                return 0;
            default:
                errorMessage("Error in body of program !");
                return 1;
        }
    }
}

int unary_operation(int operator){





}

int for_statement() {
    if (next_token() == KIN_L_ROUNDBRACKET) {
        int new_token = next_token();
        if((new_token == KW_AUTO) || (new_token == KW_DOUBLE) || (new_token == KW_INT) || (new_token == KW_STRING)) {
            if ((next_token() == KIN_IDENTIFIER) && (assing(KIN_SEMICOLON) == 0) && (value() == KIN_SEMICOLON) &&
                (next_token() == KIN_IDENTIFIER) && (assing(KIN_R_ROUNDBRACKET) == 0)&& next_token() == KIN_L_BRACE){
                return body_funcion();
            }
        }
        else if (new_token == KIN_IDENTIFIER) {
            if ((assing(KIN_SEMICOLON) == 0)&& (value() == KIN_SEMICOLON) && (next_token() == KIN_IDENTIFIER) &&
                (assing(KIN_R_ROUNDBRACKET) == 0) && next_token() == KIN_L_BRACE) {
                return body_funcion();
            }
        }
    }
    errorMessage("Error in for statement !");
    return 1;
}

int if_statement(){
    if((next_token()== KIN_L_ROUNDBRACKET) && (value_call_bracket(KIN_R_ROUNDBRACKET)== 0) &&
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
        return value_call_bracket(PREDICT_EXIT);
    }
    else if (new_token == KIN_PLUSPLUS){
        //generácia inštrukcii
    }
    else if (new_token == KIN_MINUSMINUS){
        //generácia inštrukcii
    }
    errorMessage("Error in assing function!");
    return 1;
}

int value_call_bracket(int PREDICT_EXIT){
    int status_bracket = 1;
    if(value() == PREDICT_EXIT){
        return 0;                               //kontrola bidkociarky
    }
    return 1;
}


int dec_variable(){
    if (next_token() == KIN_IDENTIFIER) {
        int new_token = next_token();
        if (new_token == KIN_SEMICOLON) {
            return 0;
        }
        else if(new_token == KIN_ASSIGNEMENT){
            return value();
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
    int status_bracket = 1;
    int counter_of_arguments = 0;
    while(true) {
        counter_of_arguments++;
        int exit_code_value = value();
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

int value() {

    return 1;
}


void errorMessage(const char *mesasge ){
    printf("############ SYNTAX ERROR ############ \n");
    printf("Error message: %s \n",mesasge);

}

int next_token(){
    Token *new_token = get_token(fp);
    if(new_token != NULL) {
        if (new_token->type != END_OF_FILE) {
            printf("%d, ",new_token->type);
            return new_token->type;
        }
    }
    return END_OF_FILE;
}
