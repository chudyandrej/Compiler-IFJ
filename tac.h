//
// Created by Andrej Oliver Chudý on 13/11/15.
//

#ifndef IJF_TAC_H
#define IJF_TAC_H


#include "stack.h"

/*
***************************************************************
** 3 adresne instrukce.
* Autor: xkondu00, Vaclav Kondula
* Popis: obsahuje struktury a funkce nad 3AC
* zasobnikova struktura prejata z stack.h
***************************************************************
*/


typedef enum Instruction {
    MAT_ADD,                /* + */
    MAT_DIFF,               /* - */
    MAT_MUL,                /* * */
    MAT_DIV,                /* / */
    COMP_ASSIGNEMENT,        /* = */
    COMP_GREATER,           /* > */
    COMP_GREATER_E,          /* >= */
    COMP_SMALLER,           /* < */
    COMP_SMALLER_EQ,        /* <= */
    COMP_EQ,                /* == */
    COMP_NOT_EQ,            /* != */
    IO_SCOUT,               /* << */
    IO_SCIN,                /* >> */
    GOTO_UNCOND,
    GOTO_COND,

    FCE_PUSH,
    FCE_RETURNED,
    SCOPE_UP,
    SCOPE_DOWN,
    INIT,
    RETURN
};

typedef enum Type {
    INT = 0,
    DOUBLE,
    STRING,
    AUTO,
    TMP,
    FUNCION,
    BUILD_IN,
    LABEL,
    OPERATOR,
    VARIABLE,
    EMPTY,

};

typedef union Address{
    int i;
    double d;
    char *s;
    unsigned int tmp;
    char * fce;
    char build_in;
    char *variable;
    enum sTokenKind op;
    unsigned int label;
};

typedef struct Operation{
     enum Instruction ction_inst;
     union Address t;
     union Address op1;
     union Address op2;
     enum Type pe_t_t;
     enum Type t_op1;
     enum Type t_op2;
    unsigned int label;
} *tOperation;


#endif //IJF_TAC_H
