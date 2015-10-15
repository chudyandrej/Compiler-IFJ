//
// Created by andrej on 15.10.2015.
//

#include <stdbool.h>
#include "syntax_checker.h"

int start_syntax_analyz(){

    token_stract* token  = next_token();
    if (check_type(token->Lexeme) == 0){
        return (dec_funcion() == 0)? 0:1;
    }else{

        return 1;

    }
}

int dec_funcion(){

    if (strcmp(next_token()->category, "id")){
        if (strcmp(next_token()->Lexeme, "(")){
            if(parameters() == 0){
                if (strcmp(next_token()->Lexeme, ")")){
                    token_stract* token = next_token();
                    if (strcmp(token->Lexeme, ";")){
                        return 0;}
                    else if(strcmp(token->Lexeme, "{")){
                        return body_funcion();
                    }else
                        return 1;
                }
            }
        }
    }
    return 1;
}
int command(){

    char *catedory = next_token()->Lexeme;
    if (strcmp(catedory, "cin")) { }
    else if(strcmp(catedory, "cout")){ }
    else if(strcmp(catedory, "for")){    }
    else if(strcmp(catedory, "if")){    }
    else if(strcmp(catedory, "sort")){ return one_par_command(); }
    else if(strcmp(catedory, "find")){ return two_par_command(); }
    else if(strcmp(catedory, "concat")){ return two_par_command(); }
    else if(strcmp(catedory, "substr")){ return three_par_command(); }
    else if(strcmp(catedory, "length")){ return one_par_command(); }
    else if(strcmp(catedory, "return")){ return one_par_command(); }

    return 1;

}


int assing(){

}
int dec_variable(){

}




int body_funcion(){
    while(true) {
        char *catedory = next_token()->category;
        if (strcmp(catedory, "command")) {
            if (command() == 0){ continue; } else{ return 1; }
        }
        else if(strcmp(catedory, "id")){
            if (assing() == 0){ continue; } else{ return 1; }
        }
        else if(strcmp(catedory, "type")){
            if (dec_variable() == 0) { continue; } else { return 1; }
        }
        else if(strcmp(catedory, "}")){
            return 0;
        }
        else{
            return 1;
        }
    }
}



int check_type(const char *lexem){

    if (strcmp(lexem, "int") == 0){
        return 0;
    }
    else if(strcmp(lexem, "double") == 0){
        return 0;
    }
    else if(strcmp(lexem, "auto") == 0){
        return 0;
    }
    else if(strcmp(lexem, "string") == 0){
        return 0;
    }
    else {
        return 1;
    }

}

int var_declaration(void){
    
    token_stract *token = next_token();  /*add to queue?*/
    if (token->category != "id") return 1;    
    
    token = next_token();               /*add to queue?*/
    if (token->Lexeme == ";" ){
        return 0;
    }
    else if (token->Lexeme == "="){
        value();
    }
    else {
        return 1;
    }
}

int parameters(){
    token_stract *token = next_token();  /*add to queue?*/
     
    if(check_type(token->Lexeme)) return 1;           /*type returns 1 when error*/
    
    token = next_token();               /*add to queue?*/
    if(token->category != "id") return 1;
    
    token = next_token();               /*add to queue?*/
    if(token->Lexeme == ")") return 0;
    if(token->Lexeme == ","){
        if(parameters()) return 1;
        return 0;
    }
    return 0;
    
}

int one_par_command(){
    if (strcmp(next_token()->Lexeme, "(")){
        if (strcmp(next_token()->category, "id")){
            if (strcmp(next_token()->Lexeme, ")")){
                if (strcmp(next_token()->Lexeme, ";")){ //gun
                    return 0;
                }
            }
        }
    }
    return 1;
}

int two_par_command(){
    if (strcmp(next_token()->Lexeme, "(")){
        if (strcmp(next_token()->category, "id")) {
            if (strcmp(next_token()->category, ",")) {
                if (strcmp(next_token()->category, "id")) {
                    if (strcmp(next_token()->Lexeme, ")")) {
                        if (strcmp(next_token()->Lexeme, ";")) {
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
    if (strcmp(next_token()->Lexeme, "(")){
        if (strcmp(next_token()->category, "id")) {
            if (strcmp(next_token()->category, ",")) {
                if (strcmp(next_token()->category, "id")) {
                    if (strcmp(next_token()->category, ",")) {
                        if (strcmp(next_token()->category, "id")) {
                            if (strcmp(next_token()->Lexeme, ")")) {
                                if (strcmp(next_token()->Lexeme, ";")) {
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