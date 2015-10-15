/* 
 * File: scanner.h
 *
 * Description: header file for lexical analyzer
 * Authors: Bayer Jan, Kopec Maros
 *
 * Created: 2015/10/6
 * Last time modified: 2015/10/15
 */

#ifndef SCANNER_H_INCLUDED
#define SCANNER_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "stable.h"
#include "str.h"
extern FILE * fp; /* Pointer to the source file */

/*
 * Typedef: tTokenType
 * Author: Bayer Jan
 * Description: Type of tokens, which can be found
 */
typedef enum TokenType {
    TT_PLUS = 100,      /* + */
    TT_MINUS,           /* - */
    TT_MUL,             /* * */
    TT_DIV,             /* / */
    TT_BIGGER,          /* > */
    TT_LESSER,          /* < */
    TT_BIGGEROREQUAL,   /* >= */
    TT_LESSEOREQUAL,    /* <= */
    TT_EQUAL,           /* == */
    TT_NOTEQUAL,        /* != */
    TT_ASSIGNEMENT,     /* = */
    TT_SCOUT,           /* << */
    TT_SCIN,            /* >> */

    TT_LEFTROUNDBRACKET,    /* ( */
    TT_RIGHTROUNDBRACKET,   /* ) */
    TT_LEFTBRACE,           /* { */
    TT_RIGHTBRACE,          /* } */

    TT_SEMICOLON,   /* ; */
    TT_COLON,       /* : */
    TT_COMMA,       /* . */



    TT_IDENTIFIER,
    TT_KEYWORD,

    TT_TEXT,

    TT_UNKOWN,
    END_OF_FILE

}tTokenType;


/*
 * Typedef: tToken
 * Author: Bayer Jan
 * Description: struct for token
 * item 'string': text part of token (represents name)
 * item 'tTokenType': stores which type of token it is 
 */
typedef struct token {
    string * str;
    tTokenType type;
}tToken;


/*
 * Typedef: tState
 * Author: Bayer Jan
 * Description: states for finite-state machine
 */
typedef enum State {
    S_START = 200,

    S_LESSER,
    S_BIGGER,
    S_EQUAL,
    S_PLUS,

    S_IDENTIFIER,
    S_INT,
    S_DOUBLE,
    S_DOUBLE_EXP,
    S_STRING,

    S_COMMENT_LINE,
    S_COMMENT_BLOCK,
    S_SLASH
}sState;

/*
 * keywords
 * Author: Bayer Jan
 * Description: defines keywords for lang IFJ15
 */
char * keywords[] = {
    "auto",
    "cin",
    "cout",
    "double",
    "else",
    "for",
    "if",
    "int",
    "return",
    "string",
    "length",
    "substr",
    "concat",
    "find",
    "sort"
};


char * check_keywords(string * str);
tToken get_token(FILE * fp);

#endif // SCANNER_H_INCLUDED
