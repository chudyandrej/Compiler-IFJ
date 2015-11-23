

#ifndef IJF_TAC_H
#define IJF_TAC_H


#include "scanner.h"

/*
***************************************************************
** 3 adresne instrukce.
* Autor: xkondu00, Vaclav Kondula
* Popis: obsahuje struktury a funkce nad 3AC
* zasobnikova struktura prejata z stack.h
***************************************************************
*/



enum Type {
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
    EMPTY

};

union Address{
    int i;
    double d;
    char *s;
    unsigned int tmp;
    char * fce;
    char build_in;
    char *variable;
    unsigned int label;
};

typedef struct Operation{
     enum sTokenKind ction_inst;
     union Address t;
     union Address op1;
     union Address op2;
     enum Type t_t;
     enum Type t_op1;
     enum Type t_op2;
    unsigned int label;
} *tOperation;


#endif //IJF_TAC_H
