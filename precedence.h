
#ifndef IJF_PRECEDENCE_H
#define IJF_PRECEDENCE_H


#include "stack.h"
#include "parser.h"
#include "scanner.h"
#include "tac.h"

extern union Address fake;
const char PrecedentTable[19][19];

typedef struct dTreeElement{
    enum sTokenKind description;
    enum Type type;
    union Address data;
} *dTreeElementPtr;

dTreeElementPtr clean_stack(tDLList *Stack,  bool correct_end);
dTreeElementPtr load_token(dTreeElementPtr new_element, Token *token);
dTreeElementPtr create_stack_element(enum sTokenKind description, Token *token);
dTreeElementPtr load_nonterm_char(dTreeElementPtr new_element, enum sTokenKind description);

void gen_label(unsigned int lab);
void gen_tnp(enum sTokenKind, union Address op1, union Address op2, enum Type t_op1, enum Type t_op2);
void gen_instructions(enum sTokenKind ction_inst,union Address t, union Address op1, union Address op2,enum Type t_t, enum Type t_op1, enum Type t_op2);

int rules( dTreeElementPtr p1, dTreeElementPtr p2, dTreeElementPtr p3);
int expression_process(enum sTokenKind end_char, dTreeElementPtr *final_node);

#endif //IJF_PRECEDENCE_H
