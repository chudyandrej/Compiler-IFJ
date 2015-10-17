//
// Created by andrej on 15.10.2015.
//

#include <stdbool.h>
#include "syntax_checker.h"



int start_syntax_analyz(){

    bracket_stack= stackCreate() ;

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
            else{
                break;
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
    int status_bracket = 1;
    stackPush(bracket_stack, status_bracket);
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
    switch(new_token){
        case IDENTIFIER: {
            new_token = next_token();
            switch (new_token) {
                case TT_LEFTROUNDBRACKET:
                    return parameters_used();
                case TT_RIGHTROUNDBRACKET:
                    return bracket(new_token);
                case TT_COMMA:
                    return TT_COMMA;
                case OPERATOR:
                    return value();
                default:
                    errorMessage("Error hned po volani identyfikatora");
                    return 1;
            }
        }
        case NUMBER: {
            new_token = next_token();
            switch (new_token) {
                case TT_COMMA:
                    return 0;
                case OPERATOR:
                    return value();

                case TT_LEFTROUNDBRACKET:
                case TT_RIGHTROUNDBRACKET:
                    return bracket(new_token);
                default:
                    errorMessage("Error hned po volani number");
                    return 1;
            }
        }
        case OPERATOR: {
            return  value();

        }
        case TT_LEFTROUNDBRACKET:
        case TT_RIGHTROUNDBRACKET:
            return bracket(new_token);
        case TT_COMMA:
            return TT_COMMA;
        default:
            errorMessage("error pri volani vsetkeho ");
            printf("%d",new_token);
            return 1;
    }
}

int bracket(int token){
    int counter_bracket = 0;
    switch(token){
        case TT_LEFTROUNDBRACKET:
            counter_bracket = stackPop(bracket_stack);
            counter_bracket++;
            stackPush(bracket_stack, counter_bracket);
            return value();
        case TT_RIGHTROUNDBRACKET:
            counter_bracket = stackPop(bracket_stack);
            printf("\n%d\n",counter_bracket);
            counter_bracket--;
            if(counter_bracket == 0){
                return TT_RIGHTROUNDBRACKET;
            }
            else {
                stackPush(bracket_stack, counter_bracket);
                token = next_token();
                switch(token){
                    case OPERATOR:
                        return value();
                    case TT_RIGHTROUNDBRACKET:
                    case TT_LEFTROUNDBRACKET:
                        return bracket(token);
                    default:
                        errorMessage("error pri operacii za zatvorkou ");
                        return 1;
                }
            }
        default:
            errorMessage("error zatvoriek");
            return 1;
    }
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
