//
// Created by andrej on 15.10.2015.
//

#ifndef INTERPRET_SYNTAX_CHECKER_H
#define INTERPRET_SYNTAX_CHECKER_H

#include "fake_skener.h"



int start_syntax_analyz();
int dec_function();

int parameters();
int body_funcion();
int command(token_stract *new_token);
int assing();
int dec_variable();
int one_par_command();
int two_par_command();
int three_par_command();
int cin_cout(const char* op);
int if_statement();
int parameters_used();
int for_statement();
int value();
void errorMessage(const char *mesasge , const char *lexem);

typedef enum TokenType {
    TT_ASSIGNEMENT = '=',
    TT_SCOUT = '<<',
    TT_SCIN = '>>',
    TT_LEFTROUNDBRACKET = '(',
    TT_RIGHTROUNDBRACKET = ')',
    TT_LEFTBRACE = '{',
    TT_RIGHTBRACE = '}',
    TT_SEMICOLON = ';',
    TT_COMMA = ',',
    IDENTIFIER = 'i',
    NUMBER = 'n',
    OPERATOR = 'o',
    STRING = 's',
    TYPE = 't'
}
#endif //INTERPRET_SYNTAX_CHECKER_H
