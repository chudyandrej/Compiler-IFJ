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
void errorMessage(const char *mesasge);
#endif //INTERPRET_SYNTAX_CHECKER_H
