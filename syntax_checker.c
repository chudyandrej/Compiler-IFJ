//
// Created by andrej on 15.10.2015.
//

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
                        return 0;
                    }
                    else if(strcmp(token->Lexeme, "{"){
                        return body_funcion();
                    }else
                        return 1;
                }
            }
        }
    }
    return 1;

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