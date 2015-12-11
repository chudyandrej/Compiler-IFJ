#ifndef TAC_H_INCLUDED
#define TAC_H_INCLUDED

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "scanner.h"
#include "stack.h"
#include "garbage.h"

/*
***************************************************************
** 3 adress instruction.
* Autor: Vaclav Kondula
* Popis: structures and functions of 3 adress code
* stack was used from stack.c
* instruction names are in scanner.h
***************************************************************
*/

typedef enum Type {
    INT,
    DOUBLE,
    STRING,
    AUTO,
    VARIABLE,
    TMP,
    FUNCION,
    BUILD_IN,
    LABEL,
    EMPTY
} Type;

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
    TokenKind  inst;
    union Address t;
    union Address op1;
    union Address op2;
    Type t_t;
    Type t_op1;
    Type t_op2;
    unsigned int label;
} *tOperation;

struct TMPRecord{
    union Address value;
    Type t;
};

int interpret();
void decrease_push();
void store_tmp(struct TMPRecord * tmp, int i);
#include "ial.h"
#include "buildin.h"
#endif //TAC_H_INCLUDED