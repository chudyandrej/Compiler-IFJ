/*
 * File: main.c
 *
 * Authors:
 *
 * Created: 2015/10/6
 * Last time modified: 2015/10/17
 */

#include <stdio.h>
#include <stdlib.h>
#include "scanner.h"

/*
char * data[] = {
    "auto",
    "double",
    "int",
    "string"
};
 */


int main() {
    char * token_kinds[] = {
       "=",
       ">",
       ">=",
       "<",
       "<=",
       "==",
       "!=",
       "<<",
       ">>",
       "(",
       ")",
       "{",
       "}",
       ";",
       "ID",
       "auto",
       "double",
       "int",
       "string",
       "cin",
       "cout",
       "else",
       "for",
       "if",
       "return",
       "length",
       "substr",
       "concat",
       "find",
       "sort",
       "+",
       "++",
       "-",
       "--",
       "/",
       "*",
       "string_val",
       "int",
       "num_double",
       ",",
       "LEX_ERR",
       "EOF",
 };

    FILE * fp;
    fp=fopen("test.ifj15", "r");
    Token * t;

    //char * string = "AHOJ";
    //Token * token = get_token(fp);

    //printf("%d\n", KIN_UNKNOWN);
    //printf("%s",get_token(fp)->str);
    //printf("%s",string);

    while ((t = get_token(fp)) && t->type!= END_OF_FILE) {
        //printf("%s(%d) ",token_kinds[t-100],t);
        printf("%s ",token_kinds[t->type-100]);
        if (t->type == KIN_TEXT) printf("\":%s\" ",t->str);
    }

    //printf("%s",data[0]);
    //printf("\n#################################################################");

    fclose(fp);
    //return  token->type;
    return (EXIT_SUCCESS);
}

