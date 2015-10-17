/* 
 * File: scanner.h
 *
 * Description: header file for lexical analyzer
 * Authors: Bayer Jan, Kopec Maros
 *
 * Created: 2015/10/6
 * Last time modified: 2015/10/17
 */

#ifndef SCANNER_H_INCLUDED
#define SCANNER_H_INCLUDED

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
    /* 100 */   KIN_ASSIGNEMENT = 100,      /* = */
    /* 101 */   KIN_GREATER,                /* > */
    /* 102 */   KIN_GREATER_EQ,             /* >= */
    /* 103 */   KIN_SMALLER,                /* < */
    /* 104 */   KIN_SMALLER_EQ,             /* <= */
    /* 105 */   KIN_EQ,                     /* == */
    /* 106 */   KIN_NOT_EQ,                 /* != */
    /* 107 */   KIN_SCOUT,                  /* << */
    /* 108 */   KIN_SCIN,                   /* >> */
    /* 109 */   KIN_L_ROUNDBRACKET,         /* ( */
    /* 110 */   KIN_R_ROUNDBRACKET,         /* ) */
    /* 111 */   KIN_L_BRACE,                /* { */
    /* 112 */   KIN_R_BRACE,                /* } */
    /* 113 */   KIN_SEMICOLON,              /* ; */
    /* 114 */   KIN_IDENTIFIER,             /* identifier */
    /*################## KEYWORDS ########################*/
    /* 115 */   KW_AUTO,                   /* data type */
    /* 116 */   KW_DOUBLE,                 /* data type */
    /* 117 */   KW_INT,                    /* data type */
    /* 118 */   KW_STRING,                 /* data type */
    /* 119 */   KW_CIN,                    /* command */
    /* 120 */   KW_COUT,                   /* command */
    /* 121 */   KW_ELSE,                   /* command */
    /* 122 */   KW_FOR,                    /* command */
    /* 123 */   KW_IF,                     /* command */
    /* 124 */   KW_RETURN,                 /* command */
    /* 125 */   KW_LENGTH,                 /* command */
    /* 126 */   KW_SUBSTR,                 /* command */
    /* 127 */   KW_CONCAT,                 /* command */
    /* 128 */   KW_FIND,                   /* command */
    /* 129 */   KW_SORT,                   /* command */
    /*###################################################*/
    /* 130 */   KIN_PLUS,                   /* + */
    /* 131 */   KIN_PLUSPLUS,               /* ++ */
    /* 132 */   KIN_MINUS,                  /* - */
    /* 133 */   KIN_MINUSMINUS,             /* -- */
    /* 134 */   KIN_DIV,                    /* / */
    /* 135 */   KIN_MUL,                    /* * */
    /* 136 */   KIN_TEXT,                   /* String value "example" */
    /* 137 */   KIN_NUM_INT,                /* range: <−32767, +32767>  */
    /* 138 */   KIN_NUM_DOUBLE,             /* 42.42; 4e-2 */
    /* 139 */   KIN_COMMA,                  /* , */
    /* 140 */   KIN_UNKNOWN,                /*LEX_ERR*/
    /* 141 */   END_OF_FILE,                /* EOF */
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
    /* 210 */   S_COMMENT_LINE,             
    /* 211 */   S_COMMENT_BLOCK,
    /* 212 */   S_PUNCT,            /* punctuation character */
    /* 213 */   S_NUMBER
}State;


/*
 * Typedef: Token
 * Author: Bayer Jan
 * Description: struct for token
 * item 'string': text part of token (represents name)
 * item 'tTokenType': stores which type of token it is 
 */
typedef struct sToken {
    TokenKind type;
    char * str;
}Token;

int isoperator(char c);
int isINT(char * c);
int copy_carray_to_token(Token *t, char *s);
int copy_char_to_token(Token *t, char c);
int copy_str_to_token(Token *t, string *s);
Token * get_token(FILE * fp);
Token * cleanup();

#endif // SCANNER_H_INCLUDED
