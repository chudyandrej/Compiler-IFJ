//
// Created by andrej on 15.10.2015.
//

#ifndef INTERPRET_SYNTAX_CHECKER_H
#define INTERPRET_SYNTAX_CHECKER_H

#include "scanner.h"
#include "stack.h"



Stack *bracket_stack ;
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


#endif //INTERPRET_SYNTAX_CHECKER_H
