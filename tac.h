#ifndef TAC_H_INCLUDED
#define TAC_H_INCLUDED

#include "stack.h"
#include "ial.h"

/*
***************************************************************
** 3 adresne instrukce.
* Autor: xkondu00, Vaclav Kondula
* Popis: obsahuje struktury a funkce nad 3AC
* zasobnikova struktura prejata z stack.h
***************************************************************
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
}TokenKind;


enum Type {
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
    TokenKind  inst; //! importovat ze scanneru
    union Address t;
    union Address op1;
    union Address op2;
    enum Type t_t;
    enum Type t_op1;
    enum Type t_op2;
    unsigned int label;
} *tOperation;

struct TMPRecord{
    union Address value;
    enum Type t;
};

int interpret();

#endif //TAC_H_INCLUDED