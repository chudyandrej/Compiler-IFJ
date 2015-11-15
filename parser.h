//
// Created by Andrej Oliver Chudý on 07/11/15.
//

#ifndef IJF_PARSER_H
#define IJF_PARSER_H


#include "scanner.h"
#include "stack.h"
#include <stdbool.h>
#include "err.h"




int start_syntax_analyz();
int dec_function();
int parameters_declar();
int body_funcion();
int assing_funcCall();
int dec_variable();
int cin_cout(enum sTokenKind operator);
int if_statement();
int parameters_used();
int for_statement();

void errorMessage_syntax(const char *message );
void errorMessage_lexical(const char *message );
void errorMessage_internal(const char *message );
Token *next_token();



int bracket(int token);

#endif //IJF_PARSER_H
