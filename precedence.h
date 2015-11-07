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
    Token *token;             /*points to any data*/
    struct dTreeElement *l;   /*points to left element*/
    struct dTreeElement *r;   /*points to right element*/
    struct dTreeElement *m;   /*points to right element*/
} *dTreeElementPtr;

typedef enum sElement {
    /* 10 */   NODE = 10,
    /* 11 */   TOKEN,
    /* 11 */   STOPPER,
}Element;


dTreeElementPtr create_tree_element(Token *token);
int expression_process();
Token* create_fake_token(unsigned int type);


#endif //IJF_PRECEDENCE_H
