//
// Created by andrej on 15.10.2015.
//

#ifndef INTERPRET_SYNTAX_CHECKER_H
#define INTERPRET_SYNTAX_CHECKER_H

#include "fake_skener.h"

int start_syntax_analyz();
int dec_funcion();
int check_type(const char *lexem);
int var_declaration(void);
int parameters();
int body_funcion();
int command();
int assing();
int dec_variable();
int one_par_command();
int two_par_command();
int three_par_command();
#endif //INTERPRET_SYNTAX_CHECKER_H
