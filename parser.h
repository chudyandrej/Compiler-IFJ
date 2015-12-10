/*
 * File: parser.h
 *
 * Description: Syntax analyzer head file
 * Authors: Chudý Andrej, Kopec Martin
 *
 * Created: 2015/10/20
 * Last time modified: 2015/11/21
 */

#ifndef IJF_PARSER_H
#define IJF_PARSER_H


#include <stdbool.h>
#include "scanner.h" 
#include "stack.h"
#include "err.h"
#include "ial.h"
#include "garbage.h"
#include "precedence.h"
#include "tac.h"

extern unsigned int tmp_counter;
extern tDLList *tac_stack;
extern union Address fake;
int start_syntax_analyz();
int dec_function(unsigned int type_func);
int parameters_declar(unsigned int type_func, char **types, char **names);
int body_function();
int assing_exp(Token *token_var);
int dec_variable(TokenKind type);
int cin();
int cout();
int if_statement();
int parameters_used();
int for_statement();
void ap_type(char **types,unsigned int type);
Token *next_token();
Type translate(TokenKind type);

extern struct tBST Func;
extern Token *token_predict;

#endif //IJF_PARSER_H
