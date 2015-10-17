//
// Created by andrej on 15.10.2015.
//

#ifndef INTERPRET_SYNTAX_CHECKER_H
#define INTERPRET_SYNTAX_CHECKER_H

#include "fake_skener.h"
#include "stack.h"



Stack *bracket_stack ;
int start_syntax_analyz();
int dec_function();

int parameters();
int body_funcion();

int assing();
int dec_variable();
int one_par_command();
int two_par_command();
int three_par_command();
int cin_cout(int op);
int if_statement();
int parameters_used();
int for_statement();
int value();
void errorMessage(const char *mesasge );
int next_token();


int bracket(int token);

typedef enum TokenType {
    TT_ASSIGNEMENT = '=',
    TT_SCOUT = 1995, //'<<',
    TT_SCIN = 1996, //'>>',
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
    TYPE = 't',
    END_OF_FILE = 'e',
    C_CIN = 1000,
    C_COUT = 1001,
    C_RETURN = 1002,
    C_IF = 1003,
    C_FOR = 1004,
    C_LENGTH = 1005,
    C_SUBSTR = 1006,
    C_CONCAT = 1007,
    C_FIND = 1008,
    C_SORT = 1009,
    C_ELSE = 1010
};
#endif //INTERPRET_SYNTAX_CHECKER_H
