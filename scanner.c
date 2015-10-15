/* 
 * File: scanner.c
 *
 * Description: lexical analyzer
 * Authors: Bayer Jan, Kopec Maros
 *
 * Created: 2015/10/6
 * Last time modified: 2015/10/15
 */

#define KEYWORDS_COUNT 15
#include "scanner.h"
#include "err.h"



/*
 * Function: get_token
 * Author: Bayer Jan, Kopec Maros
 * Description: Scan input for next token
 * 
 * type: tToken
 * param 'FILE * fp': file descriptor for input
 * returns: next token
 */
tToken get_token(FILE * fp) {
    sState state = S_START;
    string * str_tmp;
    tToken token;
    char c;

    while(1) {
        c = getc(fp);
        
        switch(state) {
            case S_START:
                if (isspace(c)) state = S_START;
                else if (isalpha(c) || c == '_') {
                    state = S_IDENTIFIER;
                    str_init_wr(str_tmp);
                    str_add_char_wr(str_tmp, c);
                }
                //else if (isdigit(c)) state = S_INT;
                //else if (c == '"') state = S_STRING;
                else if (c == '/') state = S_SLASH;
                //else if (c == '<') state = S_LESSER;
                //else if (c == '>') state = S_BIGGER;
                //else if (c == '=') state = S_EQUAL;

                //else if (c == '*') token.type = TT_MUL; return token;
                //else if (c == '-') token.type = TT_MINUS; return token;
                //else if (c == '+') token.type = TT_PLUS; return token;
                //else if (c == '{') token.type = TT_LEFTBRACE; return token;
                //else if (c == '}') token.type = TT_RIGHTBRACE; return token;
                //else if (c == '(') token.type = TT_LEFTROUNDBRACKET; return token;
                //else if (c == ')') token.type = TT_RIGHTROUNDBRACKET; return token;
                //else if (c == ';') token.type = TT_SEMICOLON; return token;
                //else if (c == ':') token.type = TT_COLON; return token;
                //else if (c == ',') token.type = TT_COMMA; return token;
                else if (c == EOF) return END_OF_FILE;
                /*END_OF_FILE nie je zadefinovany*/
            break;
            
            //case S_EQUAL:
            //    if (c == '=') token.type = TT_EQUAL; return token;
            //    else  ungetc(c,fp); token.type =TT_ASSIGNEMENT return token;
            //break;
            //
            //case S_LESSER:
            //    if (c == '<') token.type = TT_SCIN; return token;
            //    else if (c == '=') token.type = TT_LESSEOREQUAL return token;
            //    else  ungetc(c,fp); token.type = TT_LESSER return token;
            //break;
            //
            //case S_BIGGER:
            //    if (c == '>') /*addToChar(c,str);*/  return TT_SCOUT;
            //    else if (c == '=') /*addToChar(c,str);*/ return TT_BIGGEROREQUAL;
            //    else /*addToChar(c,str);*/ ungetc(c,fp); return TT_BIGGER;
            //break;
            
            case S_SLASH:
                if (c == '*') state = S_COMMENT_BLOCK;
                else if (c == '/') state = S_COMMENT_LINE;
                else if (isspace(c)) state = S_SLASH;
                else if (isalnum(c)) {
                    token->type = TT_DIV;
                    return token;
                }
                else {
                    token->type = TT_UNKNOWN;
                    return token;
                }
            break;
            
            /*case S_STRING:
                if (c == '\\')
                else {
                    /*add_to_str(c,str)

                }
            break;*/

            //case S_INT:
            //    if (isdigit(c)) state = S_INT;
            //   else if (isspace(c) || c == EOF) return TT_INT;
                // pridat jeste nacteni do stringu
            //    else if (c == '.') state = S_DOUBLE;
            //   else return TT_UNKOWN;
            //break;
            
            //case S_DOUBLE:
            //    if (isdigit(c)) {
            //        //add_to_string(c,str);
            //        state = S_DOUBLE;
            //    }
            //    else if (c == 'e' || c == 'E') state = S_DOUBLE_EXP;
            //    else if (! isdigit(c)) return TT_UNKOWN;
            //   else if (isspace(c))
                //pridat nacteni do stringu
            //break;

            case S_COMMENT_LINE:
                if (c == '*') state = S_COMMENT_BLOCK;
                if (c == '\n') state = S_START;
            break;
            
            case S_COMMENT_BLOCK:
                if (c == '*') {
                    if (getc(fp) == '/') {
                        state = S_START;
                    }
                }
            break;

            case S_IDENTIFIER:
                if (isalnum(c) || c == '_') {
                    state = S_IDENTIFIER;
                    str_add_char_wr(str_tmp, c);
                }
                else {
                    ungetc(c, fp);
                    if (check_keywords(str_tmp) == '\0') {
                        token->type = TT_IDENTIFIER;
                        return token;
                    }
                    token->type = TT_KEYWORD;
                    str_copy_string_wr(token->str, str_tmp);
                    return token;
                }
            break;
        }
    }
}


/*
 * Function: check_keywords
 * Author: Kopec Maros
 * Description: Compare string with keywords from scanner.h
 * 
 * type: char * string
 * param 'string * str': compared string
 * returns: If matches keyword, returns that keyword, else '\0'
 */
char * check_keywords(string * str) {
    int i = 0;
    for (i; i<KEYWORDS_COUNT; i++) {
        if (!str_cmp_const_str(str, keywords[i])) {
            return keywords[i];
        }
    }
    return '\0';
}