//
// Created by andrej on 15.10.2015.
//

#include <stdbool.h>
#include "syntax_checker.h"

int start_syntax_analyz(){
    if(next_token()->category == TYPE){
        return dec_function();
    }else{
        errorMessage("Error on global scope !");
        return 1;
    }
}

int dec_function(){

    if (strcmp(next_token()->category, "id") == 0){
        if (strcmp(next_token()->Lexeme, "(") == 0){
            if(parameters() == 0){
                char *lexem = next_token()->Lexeme;
                if (strcmp(lexem, ";") == 0){ return 0;}
                else if(strcmp(lexem, "{")  == 0){return body_funcion();}
                else {
                    errorMessage("Error on declaration function !");
                    return 1;
                }
            }
        }
    }
    errorMessage("Error on declaration function !");
    return 1;
}

int command(token_stract *new_token){

    char *catedory = new_token->Lexeme;
    if (strcmp(catedory, "cin") == 0) { return cin_cout(">>"); }
    else if(strcmp(catedory, "cout") == 0){ return cin_cout("<<"); }
    else if(strcmp(catedory, "for") == 0){ return for_statement(); }
    else if(strcmp(catedory, "if") == 0){ return if_statement(); }
    else if(strcmp(catedory, "sort") == 0){ return one_par_command(); }
    else if(strcmp(catedory, "find") == 0){ return two_par_command(); }
    else if(strcmp(catedory, "concat") == 0){ return two_par_command(); }
    else if(strcmp(catedory, "substr") == 0){ return three_par_command(); }
    else if(strcmp(catedory, "length") == 0){ return one_par_command(); }
    else if(strcmp(catedory, "return") == 0){ return one_par_command(); }

    return 1;
}

int for_statement(){
    if (strcmp(next_token()->Lexeme, "(")){
        token_stract *new_token = next_token();
        if(strcmp(new_token->category, "type")){
            if(strcmp(new_token->category, "id")){
                if(assing() == 0){
                    if(value() == 0){
                        if(strcmp(new_token->category, "id")){
                            return assing();
                        }
                    }
                }
            }
        }
        else if(strcmp(new_token->category, "id")){
            if(assing() == 0){
                if(value() == 0){
                    if(strcmp(new_token->category, "id")){
                        return assing();
                    }
                }
            }
        }
    }
    return 1;
}

int if_statement(){

    if (strcmp(next_token()->Lexeme, "(")){
        if (value() == 0){
            if (strcmp(next_token()->Lexeme, "{")){
                if (body_funcion() == 0){
                    if (strcmp(next_token()->Lexeme, "else") == 0){
                        if (strcmp(next_token()->Lexeme, "{") == 0){
                            if (body_funcion() == 0){
                                return 0;
                            }
                        }
                    }
                }
            }
        }
    }
    return 1;

}

int cin_cout(const char* op){
    if (strcmp(next_token()->Lexeme, op) == 0){
        while(true){
            if (strcmp(next_token()->category, "id") == 0) {
                token_stract *new_token = next_token();
                if (strcmp(new_token->Lexeme, op) == 0) {continue; }
                else if(strcmp(new_token->Lexeme, ";") == 0){return 0;}
                else {
                    errorMessage("error in cin or cout command!");
                    return 1;
                }
            }
        }
    }
    errorMessage("error in cin or cout operator!");
    return 1;
}

int assing(){

    char *lexem = next_token()->Lexeme;
    if (strcmp(lexem, "(") == 0) {
        if (parameters_used() == 0) {
            if (strcmp(next_token()->Lexeme, ";") == 0) {
                return 0;
            }
        }
    }
    else if (strcmp(lexem, "=") == 0) {
        if (value() == 0) {
            return 0;           //kontrola bidkociarky
        }
    }
    return 1;

}

int dec_variable(){

    if (strcmp(next_token()->category, "id") == 0) {
        if (strcmp(next_token()->Lexeme, ";") == 0) {
            return 0;
        }
        else if(strcmp(next_token()->Lexeme, "=") == 0){
            return value();
        }
    }
    return 1;
}

int body_funcion(){
    while(true) {
        token_stract *new_token = next_token();
        if (strcmp(new_token->category, "command") == 0) {
            if (command(new_token) == 0){ continue; } else{ return 1; }
        }
        else if(strcmp(new_token->category, "id") == 0){
            if (assing() == 0){ continue; } else{ return 1; }
        }
        else if(strcmp(new_token->category, "type") == 0){
            if (dec_variable() == 0) { continue; } else { return 1; }
        }
        else if(strcmp(new_token->Lexeme, "}") == 0){
            return 0;
        }
        else{
            return 1;
        }
    }
}

int parameters(){
    token_stract *new_token = next_token();
    if(strcmp(new_token->Lexeme,")") == 0) {
        return 0;
    }                                                /*means no parameters*/
    while(true) {
        if(strcmp(new_token->category, "type") == 0){
            if(strcmp(new_token->category, "ID") == 0) {
                new_token = next_token();
                if (strcmp(new_token->Lexeme, ")") == 0) {
                    return 0;
                }
                else if (strcmp(new_token->Lexeme, ",") == 0) {
                    continue;
                }
            }
        }
        return 1;
    }
}

int parameters_used(){

    if(strcmp(next_token()->Lexeme,")")==0) {
        return 0;
    }                                                /*means no parameters*/
    while(true) {
        if (value() == 0) {
            char *lexeme = next_token()->Lexeme;
            if (strcmp(lexeme, ")") == 0) {
                return 0;
            }
            else if (strcmp(lexeme, ",") == 0) {
                continue;
            }
        }
    }
}

int one_par_command(){
    if (strcmp(next_token()->Lexeme, "(") == 0){
        if (strcmp(next_token()->category, "id") == 0){
            if (strcmp(next_token()->Lexeme, ")") == 0){
                if (strcmp(next_token()->Lexeme, ";") == 0){ //gun
                    return 0;
                }
            }
        }
    }
    return 1;
}

int two_par_command(){
    if (strcmp(next_token()->Lexeme, "(") ==0){
        if (strcmp(next_token()->category, "id") == 0) {
            if (strcmp(next_token()->category, ",") == 0) {
                if (strcmp(next_token()->category, "id") == 0) {
                    if (strcmp(next_token()->Lexeme, ")") == 0) {
                        if (strcmp(next_token()->Lexeme, ";") == 0) {
                            return 0;
                        }
                    }
                }
            }
        }
    }
    return 1;
}

int three_par_command(){
    if (strcmp(next_token()->Lexeme, "(") == 0){
        if (strcmp(next_token()->category, "id") == 0) {
            if (strcmp(next_token()->category, ",") == 0) {
                if (strcmp(next_token()->category, "id") == 0) {
                    if (strcmp(next_token()->category, ",") == 0) {
                        if (strcmp(next_token()->category, "id") == 0) {
                            if (strcmp(next_token()->Lexeme, ")") == 0) {
                                if (strcmp(next_token()->Lexeme, ";") == 0) {
                                    return 0;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return 1;
}

int value(){
    token_stract *token;
    int bracket_number=0;

    while(1){
        if((token = next_token())==NULL) return 0;

        if(strcmp(token->Lexeme, "(")==0){
            bracket_number++;
            token = next_token();
        }
        else if(strcmp(token->Lexeme, ")")==0){
            bracket_number--;

            if(bracket_number==-1) { return 0; }

            if((token = next_token())==NULL) return 0;
        }

        if(strcmp(token->category, "number") && strcmp(token->category, "text")){
            if(strcmp(token->category, "id")){         /*not id and must be there*/
                return 1;
            }
            else{                                    /*it's var or calling function*/
                token = next_token();

                if(strcmp(token->Lexeme, ")")==0){
                    bracket_number--;
                    if(bracket_number==-1) { return 0; }
                    token = next_token();
                }
                if(strcmp(token->Lexeme, ";")==0) { return 0; }
                if(strcmp(token->category, "operator")==0) { continue; }

                if(strcmp(token->Lexeme, "(")==0){         /*it's function*/
                    if(parameters_used()==0){
                        token = next_token();
                        if(strcmp(token->Lexeme, ")")==0){
                            bracket_number--;
                            if(bracket_number==-1) { return 0; }
                            token = next_token();
                        }
                        if(strcmp(token->Lexeme, ";")==0) { return 0; }
                        if(strcmp(token->category, "operator")==0){
                            continue;
                        }
                        else{
                            return 1;
                        }
                    }
                }
                else{
                    return 1;
                }
            }
        }  /*if number or string the next token must be operator, ; , ) */
        printf("%s\n", token->Lexeme);
        token = next_token();

        if(strcmp(token->Lexeme, ")")==0){
            bracket_number--;
            if(bracket_number==-1) { return 0; }

            if((token = next_token())==NULL) return 0;
            printf("tu%d%s\n",bracket_number, token->Lexeme);
        }
        if(strcmp(token->category, "operator")==0) { continue; }
        if(strcmp(token->Lexeme, ";")==0) { return 0; }
        printf("tu\n");
    }
    return 0;
}

void errorMessage(const char *mesasge , const char *lexem){
    printf("############ SYNTAX ERROR ############ \n");
    printf("Error message: %s \n",mesasge);
    printf("\n \t%s \n",lexem);
}