#include "parser.h"

int start_syntax_analyz(){
    bool func_found = false;
    Token *new_token;
    int exit_code;
    while(true) {
        new_token = next_token();
        switch (new_token->type) {
            case KW_AUTO:
            case KW_DOUBLE:
            case KW_INT:
            case KW_STRING:
                func_found = true;
                exit_code = dec_function(new_token->type);
                gc_free(new_token);
                if(exit_code == 0) {
                    continue;
                }
                else{
                    gc_free_all();
                    return  1;
                }
            case END_OF_FILE:
                if(func_found){ return 0; }
                else{
                    errorMessage_syntax("EMPTY file cannot be interpreted!");
                    gc_free_all();
                    return 1;
                }
            default:
                gc_free_all();
                errorMessage_syntax("Global scope !");
                return 1;
        }
    }
}

int dec_function(unsigned int type_func){
    Token *func_name = next_token();
    Token *new_token;
    char *data_types = NULL;
    char *names = NULL;
    if (func_name->type == KIN_IDENTIFIER){
        if ((new_token = next_token())->type == KIN_L_ROUNDBRACKET){
            gc_free(new_token);
            if(parameters_declar(type_func, &data_types, &names) == 0){
                printf("\ntyps: %s\n",data_types);
                printf("\nnames: %s\n",names);
                new_token = next_token();
                BSTFind(&Func,func_name->str);
                if ( ! BSTActive(&Func)) {
                    BSTAdd(&Func, func_name->str);
                }
                if (GSTDeclare(&Func, data_types, names)){
                    errorMessage_semantic("neznamy error BST");
                    return 1;
                }
                if (new_token->type == KIN_SEMICOLON){
                    gc_free(new_token);
                    gc_free(func_name);
                    return 0;
                }
                else if(new_token->type == KIN_L_BRACE) {
                    gc_free(new_token);
                    tac_stack = gc_malloc(sizeof(struct tDLList));
                    init_list(tac_stack);
                    int ret_val = body_funcion();
                    if (!ret_val){
                        BSTFind(&Func, func_name->str);
                        ret_val = GSTDefine(&Func, tac_stack);
                    }
                    //erroro dvoch body
                    gc_free(func_name);
                    return ret_val;
                }
                else {
                    errorMessage_syntax("Declaration function !");
                    return 1;
                }
            }
        }
    }
    errorMessage_syntax("Declaration function !");
    return 1;
}

int body_funcion(){
    while(true) {
        Token *new_token = next_token();
        dTreeElementPtr end_node = NULL;
        int operator_history;
        switch(new_token->type){
            case KW_CIN:
                gc_free(new_token);
                if (cin() == 0){continue;} else{return 1;}
            case KW_COUT:
                gc_free(new_token);
                if (cout() == 0){continue;} else{return 1;}
            case KW_IF:
                gc_free(new_token);
                if (if_statement() == 0){continue;} else{return 1;}
            case KW_FOR:
                gc_free(new_token);
                if (for_statement() == 0){continue;} else{return 1;}
            case KW_RETURN:
                gc_free(new_token);
                if(expression_process(KIN_SEMICOLON, end_node) == KIN_SEMICOLON){
                    gc_free(end_node);
                    continue;
                } else{return 1;}
            case KIN_IDENTIFIER:
                gc_free(new_token);
                if(token_predict->type == KIN_MINUSMINUS || token_predict->type == KIN_PLUSPLUS){
                    gc_free(next_token());  //useless token
                    new_token = next_token();
                    if(new_token->type == KIN_SEMICOLON){
                        //make instruction
                     /*   dTreeElementPtr new_element = gc_malloc(sizeof(struct dTreeElement));
                        if(new_element == NULL) {  return 1;}
                        new_element->type = VARIABLE;
                        new_element->data.variable = new_token->str;
                        new_element->description = KIN_IDENTIFIER;
                        gen_instruction(op,new_element->data,new_element->data,VARIABLE,EMPTY);*/
                        gc_free(new_token);
                        continue;
                    }
                    else{
                        gc_free(new_token);
                        return 1;
                    }
                }
                else if (assing_exp() == 0){continue;} else{return 1;}
            case KIN_PLUSPLUS:
            case KIN_MINUSMINUS:
                operator_history = new_token->type;
                gc_free(new_token);
                if((new_token=next_token())->type == KIN_IDENTIFIER){
                    // new_token - info about identifier
                    Token *temp_token;
                    if((temp_token=next_token())->type == KIN_SEMICOLON){
                        /*dTreeElementPtr new_element = gc_malloc(sizeof(struct dTreeElement));
                        if(new_element == NULL) {  return 1;}
                        new_element->type = VARIABLE;
                        new_element->data.variable = new_token->str;
                        new_element->description = KIN_IDENTIFIER;
                        gen_instruction(op,new_element->data,new_element->data,EMPTY, VARIABLE);*/
                        gc_free(new_token);
                        gc_free(temp_token);
                        continue;
                    }
                    return 1;
                }
                return 1;
            case KW_AUTO:   case KW_DOUBLE:  case KW_INT:  case KW_STRING:      //all datatypes
                gc_free(new_token);
                if (dec_variable() == 0) {continue;} else {return 1;}
            case KIN_R_BRACE:
                gc_free(new_token);
                return 0;
            default:
                gc_free(new_token);
                errorMessage_syntax(" Body of function !");
                return 1;
        }
    }
}

int for_statement() {
    Token *new_token;
    dTreeElementPtr end_node = NULL;
    if ((new_token=next_token())->type == KIN_L_ROUNDBRACKET){
        gc_free(new_token);
        new_token= next_token();
        if((new_token->type >= KW_AUTO) && (new_token->type <= KW_STRING)) {
            gc_free(new_token);
            if (((new_token=next_token())->type == KIN_IDENTIFIER) && (assing_exp() == 0)) {
                gc_free(new_token);
                if (expression_process(KIN_SEMICOLON, end_node) == KIN_SEMICOLON && (new_token = next_token())->type == KIN_IDENTIFIER) {
                    gc_free(new_token);
                    if (assing_exp() == 0 && (new_token = next_token())->type == KIN_L_BRACE) {
                        gc_free(new_token);
                        return body_funcion();

                    }
                }
            }
        }
        else if (new_token->type == KIN_IDENTIFIER) {
            gc_free(new_token);

            if ((assing_exp() == 0) && (expression_process(KIN_SEMICOLON, end_node) == 0)){
                if((new_token=next_token())->type == KIN_IDENTIFIER) {
                    gc_free(new_token);
                    if (assing_exp() == 0 && (new_token = next_token())->type == KIN_L_BRACE) {
                        gc_free(new_token);
                        return body_funcion();
                    }
                }
            }
        }
    }
    errorMessage_syntax("Error in for statement !");
    return 1;
}

int if_statement(){
    Token *new_token;
    dTreeElementPtr end_node = NULL;
    if(((new_token=next_token())->type == KIN_L_ROUNDBRACKET) && (expression_process(KIN_R_ROUNDBRACKET, end_node)== KIN_R_ROUNDBRACKET)){
        gc_free(new_token);
       if((new_token=next_token())->type == KIN_L_BRACE && (body_funcion() == 0)) {
           gc_free(new_token);
           if ((new_token = next_token())->type == KW_ELSE) {
               gc_free(new_token);
               if ((new_token = next_token())->type == KIN_L_BRACE) {
                   gc_free(new_token);
                   return body_funcion();
               }
           }
       }
    }
    errorMessage_syntax("Bad syntax in \"if\"");
    return 1;
}

int cin(){
    Token *new_token;
    if ((new_token=next_token())->type == KIN_SCIN){
        gc_free(new_token);
        while(true){
            if((new_token = next_token())->type == KIN_IDENTIFIER){
                gc_free(new_token);
                new_token = next_token();
                if(new_token->type == KIN_SEMICOLON){
                    gc_free(new_token);
                    return 0;
                }
                if(new_token->type == KIN_SCIN){
                    gc_free(new_token);
                    continue;
                }
                else{
                    gc_free(new_token);
                }
            }
            break;
        }      
    }
    errorMessage_syntax("Cin operator!");
    return 1;
}

int cout(){
    Token *new_token;
    dTreeElementPtr end_node = NULL;
    if ((new_token=next_token())->type == KIN_SCOUT){ 
        gc_free(new_token);
        while(true){
            int ret_code = expression_process(KIN_SEMICOLON, end_node);
            gc_free(end_node);
            if(ret_code == KIN_SEMICOLON){return 0; }
            else if(ret_code == KIN_SCOUT){continue; }
            else{ break; }
        }
    }
    errorMessage_syntax("Cout operator!");
    return 1;
}

int assing_exp(){
    Token *new_token = next_token();
    dTreeElementPtr end_node = NULL;
    if (new_token->type == KIN_ASSIGNEMENT){       //assing var
        gc_free(new_token);
        int ret_code = (expression_process(KIN_SEMICOLON, end_node) == KIN_SEMICOLON)? 0 : 1;
        printf("tu: %d",ret_code);
        //gen_instrukcie
        gc_free(end_node);
        return ret_code;
    }
    errorMessage_syntax("Assing function!");
    return 1;
}

int dec_variable(){
    Token *new_token;
    dTreeElementPtr end_node = NULL;
    if ((new_token=next_token())->type == KIN_IDENTIFIER) {
        gc_free(new_token);
        new_token = next_token();
        if (new_token->type == KIN_SEMICOLON) {
            gc_free(new_token);
            return 0;
        }
        else if(new_token->type == KIN_ASSIGNEMENT){
            gc_free(new_token);
            if(expression_process(KIN_SEMICOLON, end_node) == KIN_SEMICOLON){
                //generacia instrukcie
                gc_free(end_node);
                return 0;
            }
            else{
                errorMessage_syntax("Bad syntax in assingment");
                return 1;
            }
        }
    }
    errorMessage_syntax("Declaration variable");
    return 1;
}

void ap_type(char **types,unsigned int type){

    switch (type){
        case KW_INT:
            strcat(*types,"i ");
            break;
        case KW_DOUBLE:
            strcat(*types,"d ");
            break;
        case KW_AUTO:
            strcat(*types,"a ");
            break;
        case KW_STRING:
            strcat(*types,"s ");
            break;
        default:
            break;
    }

    *types = gc_realloc(*types, sizeof(char)*2);
    if (*types == NULL){errorMessage_internal("Realloc");}

}

int parameters_declar(unsigned int type_func, char **types, char **names){
    *types = gc_malloc(sizeof(char)*2);
    strcpy(*types, "");
    ap_type(types, type_func);
    Token *new_token= next_token();
    if(new_token->type == KIN_R_ROUNDBRACKET) {
        gc_free(new_token);
        return 0;
    }                                                /*means no parameters*/
    while(true) {
        if((new_token->type >= KW_AUTO) && (new_token->type <= KW_STRING)){
            ap_type(types, new_token->type);
            gc_free(new_token);
            if ((new_token=next_token())->type == KIN_IDENTIFIER) {
                if (*names == NULL){
                    *names = new_token->str;
                }
                else{
                    *names = realloc(*names, strlen(new_token->str) * sizeof(char) + 2);
                    strcat(*names, " ");
                    strcat(*names, new_token->str);
                }
                gc_free(new_token);
                new_token = next_token();
                if (new_token->type == KIN_R_ROUNDBRACKET){
                    gc_free(new_token);
                    return 0;
                }
                else if (new_token->type == KIN_COMMA) {
                    gc_free(new_token);
                    new_token = next_token();
                    continue;
                }
            }
        }
        errorMessage_syntax("Declaration parameters!");
        return 1;
    }
}

int parameters_used(){
    int counter_of_arguments = 0;
    dTreeElementPtr end_node = NULL;
    while(true) {
        counter_of_arguments++;
        int exit_code_value = expression_process(KIN_COMMA, end_node);
        gc_free(end_node);
        if(exit_code_value == KIN_COMMA) {
            continue;
        }
        else if(exit_code_value == KIN_R_ROUNDBRACKET){
            return counter_of_arguments;
        }
        errorMessage_syntax("Parameter used!");
        return -1;
    }
}

Token *next_token(){
    Token *new_token = token_predict;
    token_predict = get_token(fp);
    if(new_token == NULL ){
        //odalocujeme
        errorMessage_internal("Malloc error");
        exit(INTER_ERR);
    } else if(new_token->type == KIN_UNKNOWN){
        errorMessage_lexical("");
        //odalocujeme
        exit(LEX_ERR);
    }
    else{
        return new_token;
    }
}