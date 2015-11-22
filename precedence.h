//
// Created by Andrej Oliver Chudý on 07/11/15.
//

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


typedef enum {
    /* 1 */  RULE_1 = 1, /* E -> ID    */
    /* 2 */  RULE_2,     /* E -> NUMB, STRING */
    /* 3 */  RULE_3,     /* E -> ++E    */
    /* 4 */  RULE_4,     /* E -> E++    */
    /* 5 */  RULE_5,     /* E -> --E    */
    /* 6 */  RULE_6,     /* E -> E--    */
    /* 7 */  RULE_7,     /* E -> E + E  */
    /* 8 */  RULE_8,     /* E -> E - E  */
    /* 9 */  RULE_9,     /* E -> E * E  */
    /* 10 */ RULE_10,    /* E -> E / E  */
    /* 11 */ RULE_11,    /* E -> (E)    */
    /* 12 */ RULE_12,    /* E -> E == E */
    /* 13 */ RULE_13,    /* E -> E != E */
    /* 14 */ RULE_14,    /* E -> E < E  */
    /* 15 */ RULE_15,    /* E -> E > E  */
    /* 16 */ RULE_16,    /* E -> E <= E */
    /* 17 */ RULE_17,    /* E -> E >= E */

}PrecedenceRules;

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
