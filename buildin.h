/*
 * File: buildin.h
 *
 * Description: All build in functions that are not required in ial.c
 * Authors: Vaclav Kondula
 */

#ifndef BUILDIN_H_INCLUDED
#define BUILDIN_H_INCLUDED
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "tac.h"
#include "ial.h"
#include "scanner.h"


char * substr(char * str, int i, int n);
int isBuildIn(struct Operation *rec);
int buildInOp(struct Operation *rec, tBSTPtr my_ST, int op);


#endif //BUILDIN_H_INCLUDED