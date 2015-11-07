//
// Created by Andrej Oliver Chudý on 07/11/15.
//

#ifndef IJF_PRECEDENCE_H
#define IJF_PRECEDENCE_H


#include "stack.h"
#include "scanner.h"


const char PrecedentTable[18][18];

typedef struct dTreeElement{
    unsigned int description;
    unsigned int rule;
    Token *token;             /*points to any data*/
    struct dTreeElement *l;   /*points to left element*/
    struct dTreeElement *r;   /*points to right element*/
} *dTreeElementPtr;

typedef enum sElement {
    /* 10 */   NODE = 10,
    /* 11 */   TOKEN,
    /* 11 */   STOPPER,
}Element;

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

dTreeElementPtr create_tree_element(Token *token);
int expression_process();
Token* create_fake_token(unsigned int type);
int rules( dTreeElementPtr p1, dTreeElementPtr p2, dTreeElementPtr p3);

#endif //IJF_PRECEDENCE_H
