//
// Created by Andrej Oliver Chudý on 07/11/15.
//

#ifndef IJF_PARSER_H
#define IJF_PARSER_H


#include "scanner.h"
#include "stack.h"
#include <stdbool.h>




int start_syntax_analyz();
int dec_function();

int parameters();
int body_funcion();
int assing(int PREDICT_EXIT);
int dec_variable();
int cin_cout(int op);
int if_statement();
int parameters_used();
int for_statement();
int value();
void errorMessage(const char *mesasge );
int next_token();
int value_identifier();
int value_operator();
int value_number_func();
int value_call_bracket(int PREDICT_EXIT);


int bracket(int token);

#endif //IJF_PARSER_H
