/*
 * File: scanner.c
 *
 * Description: lexical analyzer
 * Authors: Bayer Jan, Kopec Maros
 *
 * Created: 2015/10/6
 * Last time modified: 2015/10/19
 */

#include "scanner.h"

#define ESC_HEX_MAX 2
#define ESC_BINARY_MAX 8
#define ESC_OCTAL_MAX 3
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
    int ch; //help variable for escape sequence processing
    if ((token = gc_malloc(sizeof(Token))) == NULL) {
        return NULL;
    }
    if ((str_tmp = gc_malloc(sizeof(string))) == NULL) {
        return NULL;
    }
    if (str_init(str_tmp)) {
        gc_free(token);
        return NULL;
    }

    * token = (Token){ 0 };
    char c;
    char tc; /* temporary character */


    while(1) {
        c =(char)getc(fp);
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
                    if (str_add_char(str_tmp, c)) {
                        cleanup(token, str_tmp);
                    }
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
                else {
                    ungetc(c, fp);
                    token->type = KIN_DIV;
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
                else {
                    ungetc(c, fp);
                    token->type = KIN_PLUS;
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
                else {
                    ungetc(c, fp);
                    token->type = KIN_MINUS;
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
                else {
                    ungetc(c, fp);
                    token->type = KIN_GREATER;
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
                else {
                    ungetc(c, fp);
                    token->type = KIN_SMALLER;
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
                else {
                    ungetc(c, fp);
                    token->type = KIN_ASSIGNEMENT;
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

            case S_TEXT:
                if (c == EOF) {
                    token->type = KIN_UNKNOWN;
                    str_free(str_tmp);
                    return token;
                }
                else if (c == '\\') state = S_TEXT_ESC;
                else if(c != '"') {
                    if (str_add_char(str_tmp, c)) {
                        cleanup(token, str_tmp);
                    }
                }
                else {
                    token->type = KIN_TEXT;
                    if (copy_str_to_token(token, str_tmp)) {
                        cleanup(token, str_tmp);
                    }
                    str_free(str_tmp);
                    return token;
                }
                break;

                /* ######################## S_TEXT_ESC ############################## */

            case S_TEXT_ESC:
                switch(c) {
                case '"':
                    ch = '\"';
                    break;
                case 'n':
                    ch = '\n';
                    break;
                case 't':
                    ch = '\t';
                    break;
                case '\\':
                    ch = '\\';
                    break;
                case 'x':
                case 'X':
                    ch = escape_check(HEXA,ESC_HEX_MAX);
                    break;
                case 'b':
                    ch = escape_check(BINARY,ESC_BINARY_MAX);
                    break;
                case '0':
                    ch = escape_check(OCTAL,ESC_OCTAL_MAX);
                    break;
                default:
                    token->type = KIN_UNKNOWN;
                    cleanup(NULL, str_tmp);
                    return token;
                };
                if (ch < 1 || ch > 255) {
                    token->type = KIN_UNKNOWN;
                    cleanup(NULL, str_tmp);
                    return token;
                }
                if (str_add_char(str_tmp, (char)ch)) cleanup(token, str_tmp);
                state = S_TEXT;
                break;


                /* ######################## S_NUMBER ################################ */
            case S_NUMBER:
                if (isdigit(c) || (enable_op && (c == '+' || c == '-'))) {
                    if ((c == '+' || c == '-')) {
                        if (!isdigit(c = (char) getc(fp))) {
                            ungetc(c, fp);
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
                    if (!isdigit(tc =(char)getc(fp)) || (++count_dot > 1 || count_e != 0 )) {
                        token->type = KIN_UNKNOWN;
                        cleanup(NULL, str_tmp);
                        return token;
                    }
                    ungetc(tc,fp);
                    enable_op = FALSE;
                    if (str_add_char(str_tmp, c)) {
                        cleanup(token, str_tmp);
                    }
                }
                else if ( c == 'e' || c == 'E') {
                    if (++count_e > 1 || (!isdigit(tc = (char)getc(fp)) && tc != '+' && tc != '-')) {
                        ungetc(tc, fp);
                        token->type = KIN_UNKNOWN;
                        cleanup(NULL, str_tmp);
                        return token;
                    }
                    ungetc(tc, fp);
                    enable_op = TRUE;
                    if (str_add_char(str_tmp, c)) {
                       cleanup(token, str_tmp);
                    }
                }
                else if (isoperator(c) || isspace(c)) {
                    ungetc(c, fp);
                    if (copy_str_to_token(token, str_tmp)) {
                        cleanup(token, str_tmp);
                    }
                    if ((count_e == 0) && (count_dot == 0)) {
                        token->type = KIN_NUM_INT;
                    }
                    else token->type = KIN_NUM_DOUBLE;
                    str_free(str_tmp);
                    return token;
                }
                else {
                    printf("AHOJ------------------\n");
                    token->type = KIN_UNKNOWN;
                    cleanup(NULL, str_tmp);
                    return token;
                }
                break;

                /* ##################### S_COMMENT_LINE ############################# */
            case S_COMMENT_LINE:
                if (c == '\n') state = S_START;
                else if (c == EOF) {
                    token->type = KIN_UNKNOWN;
                    return token;
                }
                break;

                /* ##################### S_COMMENT_BLOCK ############################ */
            case S_COMMENT_BLOCK:
                if (c == '*') {
                    if ((c = (char) getc(fp)) == '/') {
                        state = S_START;
                    }
                    else
                        ungetc(c, fp);
                }
                else if (c == EOF) {
                    token->type = KIN_UNKNOWN;
                    return token;
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
                else {
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
                break;
        }
    }
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
    char operators[] = {';', '*', '/', '+', '-', '>', '<', '=', '!', '(', ')',
                        '{', '}', ',', EOF, '\0'};
    for (int i = 0; (unsigned)i < strlen(operators); i++) {
        if (c == operators[i])
            return TRUE;
    }
    return FALSE;
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
    int length = (int)strlen(s);
    if ((t->str = (char*) gc_malloc(((size_t)length+1))) == NULL)
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
    if ((t->str = (char*) gc_malloc((size_t)length+1)) == NULL)
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
    if ((t->str = (char*) gc_malloc(sizeof(char))) == NULL)
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
    if (t != NULL) gc_free(t);
    return NULL;
}

int escape_check(char base, int max_size) {
    char tmp_array[max_size];
    for (int counter = 0; counter < max_size;counter++) {
        tmp_array[counter] = (char) getc(fp);
        if (base == BINARY && (tmp_array[counter] != '0' && tmp_array[counter] != '1')) {
            ungetc(tmp_array[counter],fp);
            return FAIL;
        }
        else if (base == HEXA && (!isxdigit(tmp_array[counter]))) {
            ungetc(tmp_array[counter],fp);
            return FAIL;
        }
        else if (base == OCTAL && (tmp_array[counter] < '0' || tmp_array[counter] > '7')) {
            ungetc(tmp_array[counter],fp);
            return FAIL;
        }
    }
    tmp_array[max_size] = '\0';
    return strtol(tmp_array,NULL,base);
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
