//
// Created by Andrej Oliver Chudý on 07/11/15.
//

#ifndef IJF_SCANNER_H
#define IJF_SCANNER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "str.h"

extern char * keywords[];
extern FILE * fp; /* Pointer to the source file */

enum {TRUE = 1, FALSE = 0};


/*
 * Typedef: TokenKind
 * Author: Bayer Jan, Kopec Maros
 * Description: Type of tokens, which can be found
 */
typedef enum sTokenKind {
    /*################### OPERATORS VALUE ###################*/
    /* 0 */    KIN_NUM_INT = 0,          /* range: <−32767, +32767>  */
    /* 1 */    KIN_NUM_DOUBLE,             /* 42.42; 4e-2 */
    /* 2 */    KIN_TEXT,                   /* String value "example" */
    /* 3 */    KIN_PLUS,                   /* + */
    /* 4 */    KIN_MINUS,                  /* - */
    /* 5 */    KIN_MUL,                    /* * */
    /* 6 */    KIN_DIV,                    /* / */
    /* 7 */    KIN_PLUSPLUS,               /* ++ */
    /* 8 */    KIN_MINUSMINUS,             /* -- */
    /* 9 */    KIN_IDENTIFIER,             /* identifier */
    /* 10 */   KIN_L_ROUNDBRACKET,         /* ( */
    /* 11 */   KIN_R_ROUNDBRACKET,         /* ) */
    /* 12 */   KIN_EQ,                     /* == */
    /* 13 */   KIN_SMALLER,                /* < */
    /* 14 */   KIN_GREATER,                /* > */
    /* 15 */   KIN_SMALLER_EQ,             /* <= */
    /* 16 */   KIN_GREATER_EQ,             /* >= */
    /* 17 */   KIN_NOT_EQ,                 /* != */
    /*################# DERIVATE ELEMENT ##################*/
    /* 18 */   D_DOLLAR,
    /* 19 */   D_STOPER,
    /* 20 */   D_TMP,
    /* 21 */   D_NODE,
    /*######################################################*/
    /* 22 */   KIN_ASSIGNEMENT,            /* = */
    /* 23 */   KIN_SCOUT,                  /* << */
    /* 24 */   KIN_SCIN,                   /* >> */
    /* 25 */   KIN_L_BRACE,                /* { */
    /* 26 */   KIN_R_BRACE,                /* } */
    /* 27 */   KIN_SEMICOLON,              /* ; */
    /*################## KEYWORDS ########################*/
    /* 28 */   KW_AUTO,                   /* data type */
    /* 29 */   KW_DOUBLE,                 /* data type */
    /* 30 */   KW_INT,                    /* data type */
    /* 31 */   KW_STRING,                 /* data type */
    /* 32 */   KW_CIN,                    /* command */
    /* 33 */   KW_COUT,                   /* command */
    /* 34 */   KW_ELSE,                   /* command */
    /* 35 */   KW_FOR,                    /* command */
    /* 36 */   KW_IF,                     /* command */
    /* 37 */   KW_RETURN,                 /* command */
    /* 38 */   KW_LENGTH,                 /* command */
    /* 39 */   KW_SUBSTR,                 /* command */
    /* 40 */   KW_CONCAT,                 /* command */
    /* 41 */   KW_FIND,                   /* command */
    /* 42 */   KW_SORT,                   /* command */
    /*###################################################*/
    /* 43 */   KIN_COMMA,                  /* , */
    /* 44 */   KIN_UNKNOWN,                /*LEX_ERR*/
    /* 45 */   END_OF_FILE,                /* EOF */
/*#################### TAC ELEMENT #####################*/
            TAC_GOTO_UNCOND,
            TAC_GOTO_COND,
            TAC_PUSH,
            TAC_INIT,
            TAC_CALL,
            TAC_RETURN,
            SCOPE_UP,
            SCOPE_DOWN,
            TAC_EMPTY,
            KIN_UNARYMINUS

}TokenKind;


/*
 * Typedef: State
 * Author: Bayer Jan
 * Description: states for finite-state machine
 */
typedef enum sState {
    /* 200 */   S_START = 200,
    /* 201 */   S_SMALLER,
    /* 202 */   S_GREATER,
    /* 203 */   S_EQUAL,
    /* 204 */   S_PLUS,
    /* 205 */   S_MINUS,
    /* 206 */   S_SLASH,
    /* 207 */   S_SCREAMER,         /* '!' */
    /* 208 */   S_IDENTIFIER,
    /* 209 */   S_TEXT,
    /* 210 */   S_TEXT_ESC,
    /* 211 */   S_COMMENT_LINE,
    /* 212 */   S_COMMENT_BLOCK,
    /* 213 */   S_NUMBER,
}State;


/*
 * Typedef: Token
 * Author: Bayer Jan
 * Description: struct for token
 * item 'string': text part of token (represents name)
 * item 'tTokenType': stores which type of token it is
 */
typedef struct sToken {
    enum sTokenKind type;
    char * str;
}Token;

int isoperator(char c);
int copy_carray_to_token(Token *t, char *s);
int copy_char_to_token(Token *t, char c);
int copy_str_to_token(Token *t, string *s);
Token * get_token(FILE * fp);
Token * cleanup(Token * t, string * s);

#endif //IJF_SCANNER_H
