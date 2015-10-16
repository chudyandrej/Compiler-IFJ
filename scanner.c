/* 
 * File: scanner.c
 *
 * Description: lexical analyzer
 * Authors: Bayer Jan, Kopec Maros
 *
 * Created: 2015/10/6
 * Last time modified: 2015/10/16
 */

#define DATA_TYPES_SIZE 4
#define COMMANDS_SIZE 11
#define SUCCESS 0
#define FAIL 1

#include <stdio.h>
#include <stdlib.h>
#include "scanner.h"


/*
 * Function: get_token
 * Author: Bayer Jan, Kopec Maros
 * Description: Scan input for next token
 * 
 * type: tsToken
 * param 'FILE * fp': file descriptor for input
 * returns: next token
 */
tsToken * get_token(FILE * fp) {
    tsState state = S_START;
    string * str_tmp;
    tsToken * token;
    if ((token = malloc(sizeof(tsToken))) == NULL) {
        return NULL;
    }
    if ((str_tmp = malloc(sizeof(string))) == NULL) {
        return NULL;
    }
    if (str_init(str_tmp)) {
        free(token);
        return NULL;
    }
    
    * token = (tsToken){ 0 };
    char c;
    
    while(1) {
        c = getc(fp);
        
        switch(state) {
            case S_START:
                if (isspace(c)) state = S_START;
                else if (isalpha(c) || c == '_') {
                    state = S_IDENTIFIER;
                    if (str_add_char(str_tmp, c)) {
                        cleanup(token, str_tmp);
                    }
                }
                //else if (isdigit(c)) state = S_INT;
                //else if (c == '"') state = S_STRING;
                else if (c == '/') state = S_SLASH;
                //else if (c == '<') state = S_LESSER;
                //else if (c == '>') state = S_BIGGER;
                //else if (c == '=') state = S_EQUAL;

                //else if (c == '*') token.type = KIN_MUL; return token;
                //else if (c == '-') token.type = KIN_MINUS; return token;
                //else if (c == '+') token.type = KIN_PLUS; return token;
                //else if (c == '{') token.type = KIN_LEFTBRACE; return token;
                //else if (c == '}') token.type = KIN_RIGHTBRACE; return token;
                //else if (c == '(') token.type = KIN_LEFTROUNDBRACKET; return token;
                //else if (c == ')') token.type = KIN_RIGHTROUNDBRACKET; return token;
                //else if (c == ';') token.type = KIN_SEMICOLON; return token;
                //else if (c == ':') token.type = KIN_COLON; return token;
                //else if (c == ',') token.type = KIN_COMMA; return token;
                else if (c == EOF) {
                    cleanup(token, str_tmp);
                }
                /*END_OF_FILE nie je zadefinovany*/
            break;
            
            //case S_EQUAL:
            //    if (c == '=') token.type = KIN_EQUAL; return token;
            //    else  ungetc(c,fp); token.type =KIN_ASSIGNEMENT return token;
            //break;
            //
            //case S_LESSER:
            //    if (c == '<') token.type = KIN_SCIN; return token;
            //    else if (c == '=') token.type = KIN_LESSEOREQUAL return token;
            //    else  ungetc(c,fp); token.type = KIN_LESSER return token;
            //break;
            //
            //case S_BIGGER:
            //    if (c == '>') /*addToChar(c,str);*/  return KIN_SCOUT;
            //    else if (c == '=') /*addToChar(c,str);*/ return KIN_BIGGEROREQUAL;
            //    else /*addToChar(c,str);*/ ungetc(c,fp); return KIN_BIGGER;
            //break;
            
            case S_SLASH:
                if (c == '*') state = S_COMMENT_BLOCK;
                else if (c == '/') state = S_COMMENT_LINE;
                else if (isspace(c)) state = S_SLASH;
                else if (isalnum(c)) {
                    token->type = KIN_OPERATOR;
                    if (copy_char_to_token(token, c)) {
                        cleanup(token, str_tmp);
                    }
                    str_free(str_tmp);
                    return token;
                }
                else {
                    token->type = KIN_UNKNOWN;
                    str_free(str_tmp);
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
            //   else if (isspace(c) || c == EOF) return KIN_INT;
                // pridat jeste nacteni do stringu
            //    else if (c == '.') state = S_DOUBLE;
            //   else return KIN_UNKOWN;
            //break;
            
            //case S_DOUBLE:
            //    if (isdigit(c)) {
            //        //add_to_string(c,str);
            //        state = S_DOUBLE;
            //    }
            //    else if (c == 'e' || c == 'E') state = S_DOUBLE_EXP;
            //    else if (! isdigit(c)) return KIN_UNKOWN;
            //   else if (isspace(c))
                //pridat nacteni do stringu
            //break;

            
            case S_COMMENT_LINE:
                if (c == '\n') state = S_START;
            break;
            
            case S_COMMENT_BLOCK:
                if (c == '*') {
                    if (getc(fp) == '/') {
                        state = S_START;
                    }
                    else
                        ungetc(c, fp);
                }
            break;

            case S_IDENTIFIER:
                if (isalnum(c) || c == '_') {
                    state = S_IDENTIFIER;
                    if (str_add_char(str_tmp, c)) {
                        cleanup(token, str_tmp);
                    }
                }
                else {
                    ungetc(c, fp);
                    if (str_find(str_tmp, data_types, DATA_TYPES_SIZE)) {
                        token->type = KIN_DATA_TYPE;
                        if (copy_str_to_token(token, str_tmp)) {
                            cleanup(token, str_tmp);
                        }
                        str_free(str_tmp);
                        return token;
                    }
                    else if (str_find(str_tmp, commands, COMMANDS_SIZE)) {
                        token->type = KIN_COMMAND;
                        if (copy_str_to_token(token, str_tmp)) {
                            cleanup(token, str_tmp);
                        }
                        str_free(str_tmp);
                        return token;
                    }
                    token->type = KIN_IDENTIFIER;
                    if (copy_str_to_token(token, str_tmp)) {
                        cleanup(token, str_tmp);
                    }
                    str_free(str_tmp);
                    return token;
                }
            break;
        }
    }
}


/*
 * Function: copy_str_to_token
 * Author: Kopec Maros
 * Description: copy string to token string value
 * 
 * type: int
 * param 'tsToken *t': pointer to token to which string will be copied
 * param 'string *s': pointer to string that will be copied
 * returns: FAIL (1) if malloc fails to allocate memory else SUCCESS (0)
 */
int copy_str_to_token(tsToken *t, string *s)
/* prekopiruje retezec s do t->s */
{
    int length = s->length;
    if ((t->str = (char*) malloc(length+1)) == NULL)
        return FAIL;
    strcpy(t->str, s->str);
    return SUCCESS;
}


/*
 * Function: copy_char_to_token
 * Author: Kopec Maros
 * Description: copy one character to string value of token
 * 
 * type: int
 * param 'tsToken * t': pointer to token to which character will be copied
 * param 'char c': character that will be copied
 * returns: FAIL (1) if malloc fails to allocate memory else SUCCESS (0)
 */
int copy_char_to_token(tsToken *t, char c)
/* prekopiruje znak c do t->s */
{
    if ((t->str = (char*) malloc(sizeof(char))) == NULL)
        return FAIL;
    t->str[0] = c;
    return SUCCESS;
}


/*
 * Function: cleanup
 * Author: Kopec Maros
 * Description: free memory after malloc if error is detected
 * 
 * type: tsToken
 * param 'tsToken * t': pointer to token that will be free'd
 * param 'string * s' pointer to string that will be free'd
 * returns: NULL
 */
tsToken * cleanup(tsToken * t, string * s) {
    str_free(s);
    free(t);
    return NULL;
}

char * data_types[] = {
    "auto",
    "double",
    "int",
    "string"
};

char * commands[] = {
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
    "sort"
};