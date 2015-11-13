#ifndef TAC_H_INCLUDED
#define TAC_H_INCLUDED

#include "stack/stack.h"

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
    COMP_ASSIGNEMENT        /* = */
    COMP_GREATER,           /* > */
    COMP_GREATER_E          /* >= */
    COMP_SMALLER,           /* < */
    COMP_SMALLER_EQ,        /* <= */
    COMP_EQ,                /* == */
    COMP_NOT_EQ,            /* != */
    IO_SCOUT,               /* << */
    IO_SCIN,                /* >> */
    GOTO_UNCOND,
    GOTO_COND,
    FCE_PREPARE,
    FCE_PUSH,
    FCE_RETURNED,
    SCOPE_UP,
    SCOPE_DOWN,
    INIT,
    RETURN
};

typedef enum Type {
    INT,
    DOUBLE,
    STRING,
    VARIABLE,
    TMP,
    FUNCION,
    BUILD_IN,
    LABEL,
};

union Address{
    int i;
    double d;
    char *s;
    char *variable;
    unsigned int tmp;
    char * fce;
    char build_in;
    unsigned int label;
};

typedef struct Operation{
    Instruction inst;
    union Address t;
    union Address op1;
    union Address op2;
    Type t_t;
    Type t_op1;
    Type t_op2;
    unsigned int label;
} *tOperation;



#endif //TAC_H_INCLUDED