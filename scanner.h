/* 
 * File: scanner.h
 *
 * Description: header file for lexical analyzer
 * Authors: Bayer Jan, Kopec Maros
 *
 * Created: 2015/10/6
 * Last time modified: 2015/10/16
 */

#ifndef SCANNER_H_INCLUDED
#define SCANNER_H_INCLUDED

#include <string.h>
#include <ctype.h>
#include "stable.h"
#include "str.h"

extern char * data_types[];
extern char * commands[];
extern char garbage[];
extern FILE * fp; /* Pointer to the source file */

enum {APOSTROPH = 39};

/*
 * Typedef: tsTokenKind
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
    /* 109 */   KIN_LEFTROUNDBRACKET,       /* ( */    
    /* 110 */   KIN_RIGHTROUNDBRACKET,      /* ) */    
    /* 111 */   KIN_LEFTBRACE,              /* { */    
    /* 112 */   KIN_RIGHTBRACE,             /* } */    
    /* 113 */   KIN_SEMICOLON,              /* ; */    
    /* 114 */   KIN_INT,                    /* data type */
    /* 115 */   KIN_STRING,                 /* data type */
    /* 116 */   KIN_DOUBLE,                 /* data type */
    /* 117 */   KIN_AUTO,                   /* data type */
    /* 118 */   KIN_IDENTIFIER,             /* identifier */
    /* 119 */   KIN_CIN,                    /* command */
    /* 120 */   KIN_COUT,                   /* command */
    /* 121 */   KIN_ELSE,                   /* command */
    /* 122 */   KIN_FOR,                    /* command */
    /* 123 */   KIN_IF,                     /* command */
    /* 124 */   KIN_RETURN,                 /* command */
    /* 125 */   KIN_LENGTH,                 /* command */
    /* 126 */   KIN_SUBSTR,                 /* command */
    /* 127 */   KIN_CONCAT,                 /* command */
    /* 128 */   KIN_FIND,                   /* command */
    /* 129 */   KIN_SORT,                   /* command */
    /* 130 */   KIN_PLUS,                   /* + */
    /* 131 */   KIN_PLUSPLUS,               /* ++ */
    /* 132 */   KIN_MINUS,                  /* - */
    /* 133 */   KIN_MINUSMINUS,             /* -- */
    /* 134 */   KIN_DIV,                    /* / */
    /* 135 */   KIN_MUL,                    /* * */
    /* 136 */   KIN_TEXT,                   /* String value "example" */
    /* 137 */   KIN_UNKNOWN,                /*LEX_ERR*/                   
    /* 138 */   END_OF_FILE                 /* EOF */                    

}tsTokenKind;



/*
 * Typedef: tsToken
 * Author: Bayer Jan
 * Description: struct for token
 * item 'string': text part of token (represents name)
 * item 'tTokenType': stores which type of token it is 
 */
typedef struct sToken {
    tsTokenKind type;
    char * str;
}tsToken;


/*
 * Typedef: tsState
 * Author: Bayer Jan
 * Description: states for finite-state machine
 */
typedef enum sState {
    /* 200 */   S_START = 200,              
    /* 201 */   S_LESSER,                   
    /* 202 */   S_BIGGER,                   
    /* 203 */   S_EQUAL,                    
    /* 204 */   S_PLUS,                     
    /* 205 */   S_MINUS,                    
    /* 206 */   S_SLASH,
    /* 207 */   S_STAR,         /* '*' */                 
    /* 208 */   S_SCREAMER,     /* '!' */     
    /* 209 */   S_IDENTIFIER,               
    /* 210 */   S_TEXT,                     
    /* 211 */   S_COMMENT_LINE,             
    /* 212 */   S_COMMENT_BLOCK,
    /* 213 */   S_PUNCT     /* punctuation character */
}tsState;


int copy_char_to_token(tsToken *t, char c);
int copy_str_to_token(tsToken *t, string *s);
tsToken * get_token(FILE * fp);
tsToken * cleanup();

#endif // SCANNER_H_INCLUDED
