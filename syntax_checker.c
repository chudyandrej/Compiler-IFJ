//
// Created by andrej on 15.10.2015.
//

#include <stdbool.h>
#include "syntax_checker.h"

int start_syntax_analyz(){
    int new_token = next_token();
    if(new_token == TYPE){
        return dec_function();
    }else{
        errorMessage("Error on global scope !");
        return 1;
    }
}

int dec_function(){
    int new_token = next_token();
    if (new_token == IDENTIFIER){
        if (next_token() == TT_LEFTROUNDBRACKET){
            if(parameters() == 0){
                new_token = next_token();
                if (new_token == TT_SEMICOLON){ return 0;}
                else if(new_token == TT_LEFTBRACE){return body_funcion();}
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
            case C_CIN:
                if (cin_cout(TT_SCIN) == 0){ continue;} else{ return 1;}
            case C_COUT:
                if (cin_cout(TT_SCOUT) == 0){ continue;} else{ return 1;}
            case C_IF:
                if (if_statement() == 0){ continue;} else{ return 1;}
            case C_FOR:
                if (for_statement() == 0){ continue;} else{ return 1;}
            case C_SORT:
            case C_RETURN:
            case C_LENGTH:
                if (one_par_command() == 0){ continue;} else{ return 1;}
            case C_FIND:
            case C_CONCAT:
                if (two_par_command() == 0){ continue;} else{ return 1;}
            case C_SUBSTR:
                if (three_par_command() == 0){ continue;} else{ return 1;}
            case IDENTIFIER:
                if (assing() == 0){ continue; } else{ return 1; }
            case TYPE:
                if (dec_variable() == TT_SEMICOLON) { continue; } else { return 1; }
            case TT_RIGHTBRACE:
                return 0;
            default:
                errorMessage("Error in body of program !");
                return 1;
        }
    }
}

int for_statement() {
    if (next_token() == TT_LEFTROUNDBRACKET) {
        int new_token = next_token();
        if (new_token == TYPE) {
            if ((next_token() == IDENTIFIER) && (assing() == TT_SEMICOLON) && (value() == TT_SEMICOLON) &&
                    (next_token() == IDENTIFIER) && (assing() == TT_RIGHTROUNDBRACKET)&& next_token() == TT_LEFTBRACE){
                return body_funcion();
            }
        }
        else if (new_token == IDENTIFIER) {
            if ((assing() ==TT_SEMICOLON) && (value() == TT_SEMICOLON) && (next_token() == IDENTIFIER) &&
                    (assing() == TT_RIGHTROUNDBRACKET) && next_token() == TT_LEFTBRACE) {
                return body_funcion();
            }
        }
    }
    errorMessage("Error in for statement !");
    return 1;
}

int if_statement(){
    if((next_token()==TT_LEFTROUNDBRACKET) && (value()==TT_RIGHTROUNDBRACKET) &&
            (next_token()==TT_LEFTBRACE) && (body_funcion() == 0)){
        if ((next_token() == C_ELSE) && (next_token() == TT_LEFTBRACE)){
            return body_funcion();
        }
    }
    return 1;
}

int cin_cout(int op){
    if (next_token() ==  op){
        while(true){
            if (next_token() == IDENTIFIER) {
                int new_token = next_token();
                if (new_token == op) {continue; }
                else if(new_token == TT_SEMICOLON){return 0;}
                else {
                    errorMessage("error in cin or cout command!");
                    return 1;
                }
            }
        }
    }
    errorMessage("Error in cin or cout operator!");
    return 1;
}

int assing(){
    int new_token = next_token();
    if ((new_token == TT_LEFTROUNDBRACKET) && (parameters_used() == 0) && (next_token() == TT_SEMICOLON)) {
        return 0;
    }
    else if (new_token == TT_ASSIGNEMENT) {
        return value();           //kontrola bidkociarky
    }
    errorMessage("Error in assing function!");
    return 1;
}

int dec_variable(){
    if (next_token() == IDENTIFIER) {
        int new_token = next_token();
        if (new_token == TT_SEMICOLON) {
            return 0;
        }
        else if(new_token == TT_ASSIGNEMENT){
            return value();
        }
    }
    errorMessage("Error in declaration variable");
    return 1;
}



int parameters(){
    int new_token = next_token();
    if(new_token == TT_RIGHTROUNDBRACKET) {
        return 0;
    }                                                /*means no parameters*/
    while(true) {
        if((new_token == TYPE) && (next_token() == IDENTIFIER)) {
            new_token = next_token();
            if (new_token == TT_RIGHTROUNDBRACKET) {
                return 0;
            }
            else if (new_token == TT_COMMA) {
                continue;
            }
        }
        errorMessage("Error in declaration parameters!");
        return 1;
    }
}

int parameters_used(){
    if(next_token() == TT_RIGHTROUNDBRACKET) {
        return 0;
    }                                                /*means no parameters*/
    while(true) {
        int exit_code_value = value();
        if(exit_code_value == TT_COMMA) {
            continue;
        }
        else if(exit_code_value == TT_RIGHTROUNDBRACKET){
            return 0;
        }
        errorMessage("Error in parameter used!");
        return 1;
    }
}

int one_par_command(){
    if((next_token() == TT_LEFTROUNDBRACKET) && (next_token() == IDENTIFIER )&& (next_token() == TT_RIGHTROUNDBRACKET)){
        if (next_token() == TT_SEMICOLON) {
            return 0;
        }
    }
    errorMessage("Error in one parameter func!");
    return 1;
}

int two_par_command(){
    if((next_token() == TT_LEFTROUNDBRACKET) && (next_token()== IDENTIFIER) && (next_token() == TT_COMMA) &&
            (next_token()== IDENTIFIER) && (next_token() == TT_RIGHTROUNDBRACKET) && (next_token() == TT_SEMICOLON)){
        return 0;
    }
    errorMessage("Error in two parameter func!");
    return 1;
}

int three_par_command(){
    if((next_token() == TT_LEFTROUNDBRACKET) && (next_token() == IDENTIFIER) && (next_token() == TT_COMMA) &&
            (next_token() == IDENTIFIER) && (next_token() == TT_COMMA) && (next_token()== IDENTIFIER) &&
            (next_token() == TT_RIGHTROUNDBRACKET) && (next_token() == TT_SEMICOLON)){
        return 0;
    }
    errorMessage("Error in three parameter func!");
    return 1;
}

int value(){
    int new_token = next_token();
    if(new_token == IDENTIFIER){
        new_token = next_token();
        switch (new_token){
            case TT_LEFTROUNDBRACKET:
                if(parameters_used() == 0) {
                    return val_par();
                }
            case OPERATOR:
                return val_par();
            case TT_COMMA:
                return TT_COMMA;
            case TT_RIGHTROUNDBRACKET:
                return 0;
            default:
                return 1;
        }
    }
    else if(new_token == NUMBER){
        int counter_bracket = 0;
        while(true) {

            new_token = next_token();
            switch(new_token){
                case NUMBER:
                    if(next_token() == OPERATOR){
                        continue;
                    }
                case TT_LEFTROUNDBRACKET:
                    counter_bracket++;
                    continue;
                case TT_RIGHTROUNDBRACKET:
                    counter_bracket--;
                    if(counter_bracket == -1){
                        return TT_RIGHTROUNDBRACKET;
                    }
                    continue;
                case TT_COMMA:
                    if(counter_bracket == 0){
                        return TT_COMMA;
                    }
                    return 1;
                case TT_SEMICOLON:
                    if(counter_bracket == 0){
                        return TT_SEMICOLON;
                    }
                    return 1;
                default:
                    errorMessage("Error in value!");
                    return 1;
            }
        }
    }

}

int val_par() {
    int new_token = next_token();
    if (new_token == TT_COMMA) {
        return TT_COMMA;
    }
    else if (new_token == TT_RIGHTROUNDBRACKET){
        return TT_RIGHTROUNDBRACKET;
    }
    else if (new_token == OPERATOR){
        return val_par();
    }
    return 1;
}


void errorMessage(const char *mesasge ){
    printf("############ SYNTAX ERROR ############ \n");
    printf("Error message: %s \n",mesasge);

}
int next_token(){
    token_stract *new_token = next_new_token();
    if(new_token != NULL) {
        if (new_token->category != END_OF_FILE) {
            return new_token->category;
        }
    }

    return END_OF_FILE;
}
