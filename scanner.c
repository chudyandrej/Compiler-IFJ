/* 
 * File: scanner.c
 *
 * Description: lexical analyzer
 * Authors: Bayer Jan, Kopec Maros
 *
 * Created: 2015/10/6
 * Last time modified: 2015/10/17
 */

#include <stdio.h>
#include <stdlib.h>
#include "scanner.h"

#define DATA_TYPES_SIZE 4
#define COMMANDS_SIZE 11
#define KEYWORDS_SIZE 15
#define SUCCESS 0
#define FAIL 1

int count_e;
int enable_op;
int count_dot;

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
    
    * token = (Token){ 0 };
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
                else if (isdigit(c)) {
                    state = S_NUMBER;
                    count_e = FALSE;
                    enable_op = 0;
                    count_dot = 0;
                }
                else if (c == EOF) {
                    token->type = END_OF_FILE;
                    cleanup(NULL, str_tmp);
                    return token;
                }
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
                else if (c == ';') {
                    token->type = KIN_SEMICOLON;
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
                else if (c == '.')    state = S_NUMBER;
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
                else if (isoperator(c)) {
                    ungetc(c, fp);
                    token->type = KIN_DIV;
                    if (copy_str_to_token(token, str_tmp)) {
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
            
        /* ########################## S_PLUS ################################ */
        case S_PLUS:
            if (c == '+') {
                token->type = KIN_PLUSPLUS;
                cleanup(NULL, str_tmp);
                return token;
            }
            else if (isalnum(c) || isspace(c) || c == '_') {
                ungetc(c, fp);
                token->type = KIN_PLUS;
                //if (copy_char_to_token(token, c)) {
                //    cleanup(token, str_tmp);
                //}
                    str_free(str_tmp);
                    return token;
            }
            else if (isoperator(c)) {
                    ungetc(c, fp);
                    token->type = KIN_PLUS;
                    if (copy_str_to_token(token, str_tmp)) {
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
        
        /* ######################### S_MINUS ################################ */
        case S_MINUS:
            if (c == '-') {
                token->type = KIN_MINUSMINUS;
                cleanup(NULL, str_tmp);
                return token;
            }
            else if (isalnum(c) || isspace(c) || c == '_') {
                ungetc(c, fp);
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
            else if (isoperator(c)) {
                    ungetc(c, fp);
                    token->type = KIN_MINUS;
                    if (copy_str_to_token(token, str_tmp)) {
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
        
        
        /* ######################### S_GREATER ############################## */
        case S_GREATER:
            if (c == '>') {
                token->type = KIN_SCIN;
                cleanup(NULL, str_tmp);
                return token;
            }
            else if (c == '=') {
                token->type = KIN_GREATER_EQ;
                cleanup(NULL, str_tmp);
                return token;
            }
            else if (isalnum(c) || isspace(c) || c == '_') {
                ungetc(c, fp);
                token->type = KIN_GREATER;
                //if (copy_char_to_token(token, c)) {
                //    cleanup(token, str_tmp);
                //}
                str_free(str_tmp);
                return token;
            }
            else if (isoperator(c)) {
                    ungetc(c, fp);
                    token->type = KIN_GREATER;
                    if (copy_str_to_token(token, str_tmp)) {
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
        
        /* ######################### S_SMALLER ############################## */
        case S_SMALLER:
            if (c == '<') {
                token->type = KIN_SCOUT;
                cleanup(NULL, str_tmp);
                return token;
            }
            else if (c == '=') {
                token->type = KIN_SMALLER_EQ;
                cleanup(NULL, str_tmp);
                return token;
            }
            else if (isalnum(c) || isspace(c) || c == '_') {
                ungetc(c, fp);
                token->type = KIN_SMALLER;
                //if (copy_char_to_token(token, c)) {
                //    cleanup(token, str_tmp);
                //}
                str_free(str_tmp);
                return token;
            }
            else if (isoperator(c)) {
                    ungetc(c, fp);
                    token->type = KIN_SMALLER;
                    if (copy_str_to_token(token, str_tmp)) {
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
        
        /* ########################## S_EQUAL ############################### */
        case S_EQUAL:
            if (c == '=') {
                token->type = KIN_EQ;
                cleanup(NULL, str_tmp);
                return token;
            }
            else if (isalnum(c) || isspace(c) || c == '_') {
                ungetc(c, fp);
                token->type = KIN_ASSIGNEMENT;
                //if (copy_char_to_token(token, c)) {
                //    cleanup(token, str_tmp);
                //}
                str_free(str_tmp);
                return token;
            }
            else if (isoperator(c)) {
                    ungetc(c, fp);
                    token->type = KIN_ASSIGNEMENT;
                    if (copy_str_to_token(token, str_tmp)) {
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
            if (isdigit(c) || (enable_op && (c == '+' || c == '-'))) {
                if ((c == '+' || c == '-')) {
                    if (isdigit(getc(fp))) {
                        ungetc(c, fp);
                    }
                    else {
                        token->type = KIN_UNKNOWN;
                        cleanup(NULL, str_tmp);
                        return token;
                    }
                }
                enable_op = FALSE;
                if (str_add_char(str_tmp, c)) {
                    cleanup(token, str_tmp);
                }
            }
            else if (c == '.') {
                if (++count_dot > 1  || count_e != 0) {
                    token->type = KIN_UNKNOWN;
                    cleanup(NULL, str_tmp);
                    return token;
                }
                token->type = KIN_NUM_DOUBLE;
                enable_op = FALSE;
            }
            else if (c == 'e' || c == 'E') {
                if (++count_e > 1) {
                    token->type = KIN_UNKNOWN;
                    cleanup(NULL, str_tmp);
                    return token;
                }
                token->type = KIN_NUM_DOUBLE;
                enable_op = TRUE;
            }
            else if (isoperator(c)) {
                ungetc(c, fp);
                if (copy_str_to_token(token, str_tmp)) {
                    cleanup(token, str_tmp);
                }
                if ((count_e == 0) && (count_dot == 0)) {
                    if (isINT(token->str))   token->type = KIN_NUM_INT;
                    else                     token->type = KIN_NUM_DOUBLE;
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
                if (isalnum(c) || c == '_') {
                    state = S_IDENTIFIER;
                    if (str_add_char(str_tmp, c)) {
                        cleanup(token, str_tmp);
                    }
                }
                else if (isspace(c) || isoperator(c)) {
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
 * Function: isINT
 * Author: Kopec Maros
 * Description: Check if number in string is in range of INT
 * 
 * type: int
 * param 'char c': character that will be check'd
 * returns: TRUE (1) if is in range, else FALSE (0)
 */
int isINT(char * c)
{
    int l = strlen(c);
    int i;
    
    /* 6, because INT has range <−32767, +32767> including '-' */
    if (c[0] == '-') {
        if ( l > 6) return FALSE;
        for (i = 0; i < l; ++i) {
            if (i == 1) {
                if(c[i] > '3') return FALSE;
            }
            else if (i == 2) {
                if(c[i] > '2') return FALSE;
            }
            else if (i == 3) {
                if(c[i] > '7') return FALSE;
            }
            else if (i == 4) {
                if(c[i] > '6') return FALSE;
            }
            else {
                if(c[i] > '7') return FALSE;
            }
        }
    }
    else {
        if (l > 5) return FALSE;
        for (i = 0; i < l; ++i) {
            if (i == 0) {
                if(c[i] > '3') return FALSE;
            }
            else if (i == 1) {
                if(c[i] > '2') return FALSE;
            }
            else if (i == 2) {
                if(c[i] > '7') return FALSE;
            }
            else if (i == 3) {
                if(c[i] > '6') return FALSE;
            }
            else {
                if(c[i] > '7') return FALSE;
            }
        }
    }
    
    return TRUE;
}


/*
 * Function: isoperator
 * Author: Kopec Maros
 * Description: Check if character is valid operator or command symbol
 * 
 * type: int
 * param 'char c': character that will be compared
 * returns: TRUE (1) if found, else FALSE (0)
 */
int isoperator(char c)
{
    int i;
    char operators[] = {';', '*', '/', '+', '-', '>', '<', '=', '!', '(', ')',
                        '{', '}', ',', EOF};
    for (i = 0; i<strlen(operators); i++) {
        if (c == operators[i])
            return TRUE;
    }
    return FALSE;
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