#ifndef SCANNER_H_INCLUDED
#define SCANNER_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include "stable.h"
#include "str.h"
extern FILE * fp; //Pointer to the source file


typedef enum TokenType {
    TT_PLUS = 100, //                 +
    TT_MINUS,//                 -
    TT_MUL,//                 *
    TT_DIV,  //                 +
    TT_BIGGER,//                >
    TT_LESSER,//                <
    TT_BIGGEROREQUAL,//         >=
    TT_LESSEOREQUAL,//          <=
    TT_EQUAL,//                 ==
    TT_NOTEQUAL,//              !=
    TT_ASSIGNEMENT,//           =
    TT_SCOUT,//                 <<
    TT_SCIN,//                  >>

    TT_LEFTROUNDBRACKET,//      (
    TT_RIGHTROUNDBRACKET,//     )
    TT_LEFTBRACE,//             {
    TT_RIGHTBRACE,//            }

    TT_SEMICOLON,//             ;
    TT_COLON,//                 :
    TT_COMMA,//                 ,



    TT_IDENTIFIER,
    TT_INT,
    TT_DOUBLE,
    TT_STRING,

    TT_TEXT,

    TT_UNKOWN,

}tTokenType;

typedef struct token {
    string * str;
    tTokenType type;
}tToken;

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
    S_SLASH,
}sState;

const char *key_words[] = {
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
    "sort",
};

FILE * open_source(char * path);
tTokenType  get_next_token(FILE * fp, tSymbolTable * st, tSymbolTableItem * ptr);

#endif // SCANNER_H_INCLUDED
