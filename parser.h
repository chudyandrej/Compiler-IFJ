//
// Created by Andrej Oliver Chudý on 07/11/15.
//

#ifndef IJF_PARSER_H
#define IJF_PARSER_H


#include "scanner.h" 
#include "stack.h"
#include <stdbool.h>
#include "err.h"

extern tDLList *tac_stack;

int start_syntax_analyz();
int dec_function(unsigned int type_func);
int parameters_declar(unsigned int type_func, char **types, char **names);
int body_funcion();
int assing_funcCall();
int dec_variable();
int cin_cout(enum sTokenKind operator);
int if_statement();
int parameters_used();
int for_statement();
void ap_type(char **types,unsigned int type);

void errorMessage_syntax(const char *message );
void errorMessage_lexical(const char *message );
void errorMessage_internal(const char *message );
Token *next_token();

extern struct tBST Func;
extern Token *token_predict;

#endif //IJF_PARSER_H
