/*
 * File: scanner.c
 *
 * Description: lexical analyzer
 * Authors: Bayer Jan, Kopec Maros
 *
 * Created: 2015/10/6
 * Last time modified: 2015/10/17
 */

#define DATA_TYPES_SIZE 4
#define COMMANDS_SIZE 11
#define KEYWORDS_SIZE 15
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
 * type: Token
 * param 'FILE * fp': file descriptor for input
 * returns: pointer to the next token
 */
Token * get_token(FILE * fp) {
    State state = S_START;
    string * str_tmp;
    Token * token;
    if ((token = malloc(sizeof(Token))) == NULL) {
        return NULL;
    }
    if ((str_tmp = malloc(sizeof(string))) == NULL) {
        return NULL;
    }
    if (str_init(str_tmp)) {
        free(token);
        return NULL;
    }

    token->str = NULL;
    token->type = KIN_UNKNOWN;
    char c;

    while(1) {
        c = getc(fp);

        switch(state) {
        /* ######################## S_START ################################# */
            case S_START:
                if (isspace(c))    state = S_START;
                else if (isalpha(c) || c == '_') {
                    state = S_IDENTIFIER;
                    if (str_add_char(str_tmp, c)) {
                        cleanup(token, str_tmp);
                    }
                }
                else if (isdigit(c))    state = S_NUMBER;
                else if (c == EOF)      cleanup(token, str_tmp);
                else if (c == '(') {
                    token->type = KIN_L_ROUNDBRACKET;
                    cleanup(NULL, str_tmp);
                    return token;
                }
                else if (c == ')') {
                    token->type = KIN_R_ROUNDBRACKET;
                    cleanup(NULL, str_tmp);
                    return token;
                }
                else if (c == '{') {
                    token->type = KIN_L_BRACE;
                    cleanup(NULL, str_tmp);
                    return token;
                }
                else if (c == '}') {
                    token->type = KIN_R_BRACE;
                    cleanup(NULL, str_tmp);
                    return token;
                }
                else if (c == ',') {
                    token->type = KIN_COMMA;
                    cleanup(NULL, str_tmp);
                    return token;
                }
                else if (c == '*') {
                    token->type = KIN_MUL;
                    cleanup(NULL, str_tmp);
                    return token;
                }
                else if (c == '/')    state = S_SLASH;
                else if (c == '+')    state = S_PLUS;
                else if (c == '-')    state = S_MINUS;
                else if (c == '>')    state = S_GREATER;
                else if (c == '<')    state = S_SMALLER;
                else if (c == '=')    state = S_EQUAL;
                else if (c == '!')    state = S_SCREAMER;
                else if (c == '"')    state = S_TEXT;
                else {
                    token->type = KIN_UNKNOWN;
                    return token;
                }
            break;

        /* ######################### S_SLASH ################################ */
            case S_SLASH:
                if (c == '*') {
                    state = S_COMMENT_BLOCK;
                    break;
                }
                else if (c == '/') {
                    state = S_COMMENT_LINE;
                    break;
                }
                else if (isalnum(c) || isspace(c) || c == '_') {
                    token->type = KIN_DIV;
                    //if (copy_char_to_token(token, c)) {
                    //    cleanup(token, str_tmp);
                    //}
                    str_free(str_tmp);
                    return token;
                }
                else {
                    token->type = KIN_UNKNOWN;
                    str_free(str_tmp);
                    return token;
                }
            break;

        /* ######################### S_PLUS ################################ */
        case S_PLUS:
            if (c == '+') {
                token->type = KIN_PLUSPLUS;
                cleanup(NULL, str_tmp);
                return token;
            }
            else if (isalnum(c) || isspace(c) || c == '_') {
                token->type = KIN_PLUS;
                //if (copy_char_to_token(token, c)) {
                //    cleanup(token, str_tmp);
                //}
                    str_free(str_tmp);
                    return token;
            }
            else {
                    token->type = KIN_UNKNOWN;
                    str_free(str_tmp);
                    return token;
                }
        break;

        /* ######################### S_MINUS ################################ */
        case S_MINUS:
            if (c == '-') {
                token->type = KIN_MINUSMINUS;
                cleanup(NULL, str_tmp);
                return token;
            }
            else if (isalnum(c) || isspace(c) || c == '_') {
                token->type = KIN_MINUS;
                //if (copy_char_to_token(token, c)) {
                //    cleanup(token, str_tmp);
                //}
                str_free(str_tmp);
                return token;
            }
            else if (isdigit(c)) {
                state = S_NUMBER;
                if (str_add_char(str_tmp, c)) {
                    cleanup(token, str_tmp);
                }
            }
            else {
                    token->type = KIN_UNKNOWN;
                    str_free(str_tmp);
                    return token;
                }
        break;


        /* ######################### S_GREATER ############################## */
        case S_GREATER:
            if (c == '>') {
                token->type = KIN_CIN;
                cleanup(NULL, str_tmp);
                return token;
            }
            else if (c == '=') {
                token->type = KIN_GREATER_EQ;
                cleanup(NULL, str_tmp);
                return token;
            }
            else if (isalnum(c) || isspace(c) || c == '_') {
                token->type = KIN_GREATER;
                //if (copy_char_to_token(token, c)) {
                //    cleanup(token, str_tmp);
                //}
                str_free(str_tmp);
                return token;
            }
            else {
                    token->type = KIN_UNKNOWN;
                    str_free(str_tmp);
                    return token;
                }
        break;

        /* ######################### S_SMALLER ############################## */
        case S_SMALLER:
            if (c == '<') {
                token->type = KIN_COUT;
                cleanup(NULL, str_tmp);
                return token;
            }
            else if (c == '=') {
                token->type = KIN_SMALLER_EQ;
                cleanup(NULL, str_tmp);
                return token;
            }
            else if (isalnum(c) || isspace(c) || c == '_') {
                token->type = KIN_SMALLER;
                //if (copy_char_to_token(token, c)) {
                //    cleanup(token, str_tmp);
                //}
                str_free(str_tmp);
                return token;
            }
            else {
                    token->type = KIN_UNKNOWN;
                    str_free(str_tmp);
                    return token;
                }
        break;

        /* ########################## S_EQUAL ############################### */
        case S_EQUAL:
            if (c == '=') {
                token->type = KIN_EQ;
                cleanup(NULL, str_tmp);
                return token;
            }
            else if (isalnum(c) || isspace(c) || c == '_') {
                token->type = KIN_ASSIGNEMENT;
                //if (copy_char_to_token(token, c)) {
                //    cleanup(token, str_tmp);
                //}
                str_free(str_tmp);
                return token;
            }
            else {
                    token->type = KIN_UNKNOWN;
                    str_free(str_tmp);
                    return token;
                }
        break;

        /* ######################### S_SCREAMER ############################# */
        case S_SCREAMER:
            if (c == '=') {
                token->type = KIN_NOT_EQ;
                cleanup(NULL, str_tmp);
                return token;
            }
            else {
                    token->type = KIN_UNKNOWN;
                    str_free(str_tmp);
                    return token;
                }
        break;

        /* ########################### S_TEXT ############################### */
        /* Zatial nerozoznava escape sekvencie ani backslash tvary */
        case S_TEXT:
            ungetc(c, fp);
            while ((c = getc(fp)) != '"') {
                if (str_add_char(str_tmp, c)) {
                    cleanup(token, str_tmp);
                }
            }
            if (copy_str_to_token(token, str_tmp)) {
                cleanup(token, str_tmp);
            }
            str_free(str_tmp);
            return token;
        break;


        /* ######################## S_NUMBER ################################ */
        case S_NUMBER:
            if (isdigit(c)) {
                if (str_add_char(str_tmp, c)) {
                    cleanup(token, str_tmp);
                }
            }
            else if (c == 'e' || c == 'E') {
                if (str_add_char(str_tmp, c)) {
                    cleanup(token, str_tmp);
                }
                token->type = KIN_FLOAT_NUMBER;
                state = S_NUMBER_E;
            }
            else if (c == '.') {
                if (str_add_char(str_tmp, c)) {
                    cleanup(token, str_tmp);
                }
                token->type = KIN_FLOAT_NUMBER;
                state = S_NUMBER_F;
            }
            else {
                token->type = KIN_NUMBER;
                if (copy_str_to_token(token, str_tmp)) {
                    cleanup(token, str_tmp);
                }
                str_free(str_tmp);
                return token;
            }
        break;

        /* ######################## S_NUMBER_E ############################## */
        case S_NUMBER_E:
            if (c == 'e' || c == 'E' || c == '.') {
                token->type = KIN_UNKNOWN;
                str_free(str_tmp);
                return token;
            }
            if (isdigit(c)) {
                if (str_add_char(str_tmp, c)) {
                    cleanup(token, str_tmp);
                }
            }
            else {
                if (copy_str_to_token(token, str_tmp)) {
                    cleanup(token, str_tmp);
                }
                str_free(str_tmp);
                return token;
            }
        break;

        /* ######################## S_NUMBER_F ############################## */
        case S_NUMBER_F:
            if (c == '.') {
                token->type = KIN_UNKNOWN;
                str_free(str_tmp);
                return token;
            }
            if (isdigit(c)) {
                if (str_add_char(str_tmp, c)) {
                    cleanup(token, str_tmp);
                }
            }
            else if (c == 'e' || c == 'E') {
                if (str_add_char(str_tmp, c)) {
                    cleanup(token, str_tmp);
                }
                token->type = KIN_FLOAT_NUMBER;
                state = S_NUMBER_E;
            }
            else {
                if (copy_str_to_token(token, str_tmp)) {
                    cleanup(token, str_tmp);
                }
                str_free(str_tmp);
                return token;
            }
        break;

        /* ##################### S_COMMENT_LINE ############################# */
            case S_COMMENT_LINE:
                if (c == '\n') state = S_START;
            break;

        /* ##################### S_COMMENT_BLOCK ############################ */
            case S_COMMENT_BLOCK:
                if (c == '*') {
                    if (getc(fp) == '/') {
                        state = S_START;
                    }
                    else
                        ungetc(c, fp);
                }
            break;

        /* ####################### S_IDENTIFIER ############################# */
            case S_IDENTIFIER:
                if ((isalnum(c) || c == '_') && str_add_char(str_tmp, c) )
                    cleanup(token, str_tmp);
                else if (isspace(c)) {
                    ungetc(c, fp);
                    int position;
                    if ((position = (str_find(str_tmp, keywords, KEYWORDS_SIZE))) != NOTFOUND) {
                        token->type = KW_AUTO + position;
                        if (copy_carray_to_token(token, keywords[position])) {
                            cleanup(token, str_tmp);
                        }
                        str_free(str_tmp);
                        return token;
                    }
                    else {
                        token->type = KIN_IDENTIFIER;
                        if (copy_str_to_token(token, str_tmp)) {
                            cleanup(token, str_tmp);
                        }
                        str_free(str_tmp);
                        return token;
                    }
                }
                else {
                    token->type = KIN_UNKNOWN;
                    str_free(str_tmp);
                    return token;
                }
            break;
        }
    }
}


/*
 * Function: copy_carray_to_token
 * Author: Kopec Maros
 * Description: copy array of characters to token string value
 *
 * type: int
 * param 'Token *t': pointer to token to which string will be copied
 * param 'cahr *s': pointer to string that will be copied
 * returns: FAIL (1) if malloc fails to allocate memory else SUCCESS (0)
 */
int copy_carray_to_token(Token *t, char *s)
/* prekopiruje retezec s do t->s */
{
    int length = strlen(s);
    if ((t->str = (char*) malloc(length+1)) == NULL)
        return FAIL;
    strcpy(t->str, s);
    return SUCCESS;
}


/*
 * Function: copy_str_to_token
 * Author: Kopec Maros
 * Description: copy string to token string value
 *
 * type: int
 * param 'Token *t': pointer to token to which string will be copied
 * param 'string *s': pointer to string that will be copied
 * returns: FAIL (1) if malloc fails to allocate memory else SUCCESS (0)
 */
int copy_str_to_token(Token *t, string *s)
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
 * param 'Token * t': pointer to token to which character will be copied
 * param 'char c': character that will be copied
 * returns: FAIL (1) if malloc fails to allocate memory else SUCCESS (0)
 */
int copy_char_to_token(Token *t, char c)
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
 * type: Token
 * param 'Token * t': pointer to token that will be free'd
 * param 'string * s' pointer to string that will be free'd
 * returns: NULL
 */
Token * cleanup(Token * t, string * s) {
    if (s != NULL) str_free(s);
    if (t != NULL) free(t);
    return NULL;
}


char * keywords[] = {
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
    "sort"
};
