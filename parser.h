
#ifndef IJF_PARSER_H
#define IJF_PARSER_H


#include "scanner.h" 
#include "stack.h"
#include <stdbool.h>
#include "err.h"
#include "ial.h"
#include "garbage.h"

extern tDLList *tac_stack;
extern union Address fake;
int start_syntax_analyz();
int dec_function(unsigned int type_func);
int parameters_declar(unsigned int type_func, char **types, char **names);
int body_funcion();
int assing_exp(Token *token_var);
int dec_variable(enum sTokenKind type);
int cin();
int cout();
int if_statement();
int parameters_used();
int for_statement();
void ap_type(char **types,unsigned int type);
Token *next_token();
enum Type translate(enum sTokenKind type);

extern struct tBST Func;
extern Token *token_predict;

#endif //IJF_PARSER_H
