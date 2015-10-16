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
extern FILE * fp; /* Pointer to the source file */


/*
 * Typedef: tsTokenKind
 * Author: Bayer Jan, Kopec Maros
 * Description: Type of tokens, which can be found
 */
typedef enum sTokenKind {
    /* 100 */   KIN_ASSIGNEMENT = 100,        /* = */
    /* 101 */   KIN_SCOUT,                  /* << */    
    /* 102 */   KIN_SCIN,                   /* >> */    

    /* 103 */   KIN_LEFTROUNDBRACKET,    /* ( */    
    /* 104 */   KIN_RIGHTROUNDBRACKET,   /* ) */    
    /* 105 */   KIN_LEFTBRACE,           /* { */    
    /* 106 */   KIN_RIGHTBRACE,          /* } */    

    /* 107 */   KIN_SEMICOLON,   /* ; */    
    /* 108 */   KIN_COLON,       /* : */    
    /* 109 */   KIN_COMMA,       /* . */    

    /* 110 */   KIN_DATA_TYPE,   /* int, double, string */      
    /* 111 */   KIN_NUMBER,                                     
    /* 112 */   KIN_IDENTIFIER,                                 
    /* 113 */   KIN_COMMAND,     /* cin, cout, for etc. */      
    /* 114 */   KIN_OPERATOR,    /* + - * / etc. */             
    /* 115 */   KIN_TEXT,        /* String value "example" */   
    /* 116 */   KIN_UNKNOWN,      /*LEX_ERR*/                   
    /* 117 */   END_OF_FILE                                     

}tsTokenKind;



/*
 * Typedef: tsToken
 * Author: Bayer Jan
 * Description: struct for token
 * item 'string': text part of token (represents name)
 * item 'tTokenType': stores which type of token it is 
 */
typedef struct stoken {
    tsTokenKind type;
    char * str;
    double value;
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
    /* 207 */   S_SCREAMER,     /* ! */     
    /* 208 */   S_IDENTIFIER,               
    /* 209 */   S_TEXT,                     
    /* 210 */   S_COMMENT_LINE,             
    /* 211 */   S_COMMENT_BLOCK             
}tsState;


int copy_char_to_token(tsToken *t, char c);
int copy_str_to_token(tsToken *t, string *s);
tsToken * get_token(FILE * fp);
tsToken * cleanup();

#endif // SCANNER_H_INCLUDED
