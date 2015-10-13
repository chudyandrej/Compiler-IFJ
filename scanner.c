#include "scanner.h"

int main() {
}


tToken  get_next_token(FILE * fp) {
    sState state = S_START;
    tToken token;
    token.str = NULL;
    string str;
    char c;

    while(1) {
        c = getc(fp);
        switch(state) {
            case S_START:
                if (isspace(c)) state = S_START;
                else if (isalpha(c)) state = S_IDENTIFIER;
                else if (isdigit(c)) state = S_INT;
                else if (c == '"') state = S_STRING;
                else if (c == '/') state = S_SLASH;
                else if (c == '<') state = S_LESSER;
                else if (c == '>') state = S_BIGGER;
                else if (c == '=') state = S_EQUAL;

                else if (c == '*') token.type = TT_MUL; return token;
                else if (c == '-') token.type = TT_MINUS; return token;
                else if (c == '+') token.type = TT_PLUS; return token;
                else if (c == '{') token.type = TT_LEFTBRACE; return token;
                else if (c == '}') token.type = TT_RIGHTBRACE; return token;
                else if (c == '(') token.type = TT_LEFTROUNDBRACKET; return token;
                else if (c == ')') token.type = TT_RIGHTROUNDBRACKET; return token;
                else if (c == ';') token.type = TT_SEMICOLON; return token;
                else if (c == ':') token.type = TT_COLON; return token;
                else if (c == ',') token.type = TT_COMMA; return token;
            break;
            case S_EQUAL:
                if (c == '=') token.type = TT_EQUAL; return token;
                else  ungetc(c,fp); token.type =TT_ASSIGNEMENT return token;
            break;
            case S_LESSER:
                if (c == '<') token.type = TT_SCIN; return token;
                else if (c == '=') token.type = TT_LESSEOREQUAL return token;
                else  ungetc(c,fp); token.type = TT_LESSER return token;
            break;
            case S_BIGGER:
                if (c == '>') /*addToChar(c,str);*/  return TT_SCOUT;
                else if (c == '=') /*addToChar(c,str);*/ return TT_BIGGEROREQUAL;
                else /*addToChar(c,str);*/ ungetc(c,fp); return TT_BIGGER;
            break;
            case S_SLASH:
                if (c == '*') state = S_COMMENT_BLOCK;
                else if (c == '/') state = S_COMMENT_LINE;
                else if (isspace(c)) state = S_SLASH;
                else if (isdigit(c)) token.type = TT_DIVreturn TT_DIV;
                else return TT_UNKOWN;
            break;
            /*case S_STRING:
                if (c == '\\')
                else {
                    /*add_to_str(c,str)

                }
            break;*/

            case S_INT:
                if (isdigit(c)) state = S_INT;
                else if (isspace(c) || c == EOF) return TT_INT;
                // pridat jeste nacteni do stringu
                else if (c == '.') state = S_DOUBLE;
                else return TT_UNKOWN;

            break;
            case S_DOUBLE:
                if (isdigit(c)) {
                    //add_to_string(c,str);
                    state = S_DOUBLE;
                }
                else if (c == 'e' || c == 'E') state = S_DOUBLE_EXP;
                else if (! isdigit(c)) return TT_UNKOWN;
                else if (isspace(c))
                //pridat nacteni do stringu
            break;

            case S_COMMENT_LINE:
                if (c == '*') state = S_COMMENT_BLOCK;
                //if (c == '/')
            break;
            case S_COMMENT_BLOCK:
                if ( c == '*' && ( c = getc(fp)) == '/') state = S_START;
            break;

            case S_IDENTIFIER:
                if (isalnum(c) || c == '_') /*add_to_str(c,str)*/ state = S_IDENTIFIER;
                else
            break;

        }
    }
}
