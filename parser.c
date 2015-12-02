#include "parser.h"

unsigned int label = 1;

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
                    return  exit_code;
                }
            case END_OF_FILE:
                if(func_found){ return 0; }
                else{
                    errorMessage_syntax("EMPTY file cannot be interpreted!");
                    gc_free_all();
                    return  SYN_ERR; //2
                }
            default:
                gc_free_all();
                errorMessage_syntax("Global scope !");
                return  SYN_ERR; //2
        }
    }
}

int dec_function(unsigned int type_func){
    Token *func_name = next_token();
    Token *new_token;
    char *data_types = NULL;
    char *names = NULL;
    int exit_code;

    if (func_name->type == KIN_IDENTIFIER){
        if ((new_token = next_token())->type == KIN_L_ROUNDBRACKET){
            gc_free(new_token);
            if((exit_code=parameters_declar(type_func, &data_types, &names)) == 0){
               // fprintf(stderr,"\ntyps: %s\n",data_types);      //debug
               // fprintf(stderr,"\nnames: %s\n",names);          //debug
                new_token = next_token();
                BSTFind(&Func,func_name->str);
                if ( ! BSTActive(&Func)) {
                    BSTAdd(&Func, func_name->str);
                }
                if (GSTDeclare(&Func, data_types, names)){       
                    errorMessage_semantic("WRONG arguments of function!");
                    return PROGRAM_SEM_ERR; //3
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
                    exit_code = body_function();
                    if (!exit_code){
                        BSTFind(&Func, func_name->str);
                        exit_code = GSTDefine(&Func, tac_stack);
                        if(exit_code){
                            errorMessage_semantic("Redefinition of function!");
                            return PROGRAM_SEM_ERR; //3
                        }
                    }
                    gc_free(func_name);
                    if(exit_code == TYPE_COMP_SEM_ERR){errorMessage_semantic("WRONG number of function arguments!");}
                    return exit_code;
                }
                else {
                    errorMessage_syntax("WRONG function declaration!");
                    return SYN_ERR; //2
                }
            }else{return SYN_ERR;} //wrong declaration of function parameters 
        }
    }
    errorMessage_syntax("WRONG function declaration!");
    return SYN_ERR; //2
}

int body_function(){
   // fprintf(stderr,"scope UP body !!!  ##\n");          //debug
    gen_instructions(SCOPE_UP, fake, fake, fake, EMPTY, EMPTY, EMPTY);
    Token *token_var;
    Token *new_token;
    dTreeElementPtr end_node = NULL;
    enum sTokenKind operator_history;
    int exit_code;
    while(true) {
        new_token = next_token();        
        switch(new_token->type){
            case KW_CIN:
                gc_free(new_token);
                if ((exit_code=cin()) == 0){continue;} else{return exit_code;}
            case KW_COUT:
                gc_free(new_token);
                if ((exit_code=cout()) == 0){continue;} else{return exit_code;}
            case KW_IF:
                gc_free(new_token);
                if ((exit_code=if_statement()) == 0){continue;} else{return exit_code;}
            case KW_FOR:
                gc_free(new_token);
                if ((exit_code=for_statement()) == 0){continue;} else{return exit_code;}
            
            case KW_RETURN:
                gc_free(new_token);
                if((exit_code=expression_process(KIN_SEMICOLON, &end_node)) == KIN_SEMICOLON){
                    if(end_node == NULL || end_node->description == D_DOLLAR){return SYN_ERR;}    //D_DOLLAR when no expression occured
                    gen_instructions(TAC_RETURN,end_node->data,fake,fake,end_node->type,EMPTY,EMPTY);
                    gc_free(end_node);
                    continue;
                } else{
                    errorMessage_syntax("BAD SYNTAX in RETURN function");
                    return (exit_code == TYPE_COMP_SEM_ERR) ? TYPE_COMP_SEM_ERR : SYN_ERR;
                }

            case KIN_IDENTIFIER:
                token_var = new_token;      //save token with ID
                if(token_predict->type == KIN_MINUSMINUS || token_predict->type == KIN_PLUSPLUS){
                    operator_history = (new_token=next_token())->type;
                    gc_free(new_token); 
                    if(new_token->type == KIN_SEMICOLON){
                        union Address tmp;
                        tmp.variable = new_token->str;
                        gen_instructions(operator_history,tmp,fake,fake,VARIABLE,EMPTY,EMPTY);
                        gc_free(new_token);
                        continue;
                    }
                    else{
                        return SYN_ERR; //dealocation in syntax_analyzer
                    }
                }
                else if ((exit_code=assing_exp(token_var)) == 0){continue;} else{return exit_code;}

            case KIN_PLUSPLUS:
            case KIN_MINUSMINUS:
                operator_history = new_token->type;
                gc_free(new_token);
                if((token_var=next_token())->type == KIN_IDENTIFIER){
                    // token_var - info about identifier
                    if((new_token=next_token())->type == KIN_SEMICOLON){
                        union Address tmp;
                        tmp.variable = token_var->str;
                        gen_instructions(operator_history,tmp,fake,fake,VARIABLE,EMPTY,EMPTY);
                        gc_free(token_var);
                        gc_free(new_token);
                        continue;
                    }
                    return SYN_ERR;
                }
                return SYN_ERR;

            case KW_AUTO:   //declaration ID to auto must be followed by initialization 
                if((new_token=next_token())->type == KIN_IDENTIFIER){
                    union Address tmp;
                    tmp.variable = new_token->str;
                    gen_instructions(TAC_INIT,tmp, fake, fake, VARIABLE,AUTO,EMPTY);
                    if((exit_code=assing_exp(new_token)) == 0){continue;}else{return exit_code;}
                    gc_free(new_token);
                }
                else{return VAR_TYPE_ERR;} //5

            case KW_DOUBLE:  case KW_INT:  case KW_STRING:
                if ((exit_code=dec_variable(new_token->type)) == 0) {
                    gc_free(new_token);
                    continue;}
                else {return exit_code;}

            case KIN_L_BRACE:
                if((exit_code=body_function()) == 0){continue;}else{return exit_code;}
            case KIN_R_BRACE:
                //fprintf(stderr,"scope down body!!!  ##\n");     //debug
                gc_free(new_token);
                gen_instructions(SCOPE_DOWN, fake, fake, fake, EMPTY, EMPTY, EMPTY);
                return 0;

            default:
                errorMessage_syntax(" Body of function !");
                return SYN_ERR; //2 //dealocation in syntax_analyzer
        }
    }
}

int for_statement() {

    gen_instructions(SCOPE_UP, fake, fake, fake, EMPTY, EMPTY, EMPTY);
    Token *new_token;
    char *var_name;
    union Address cond;
    cond.label = label;
    union Address uncond;
    uncond.label = label + 1;
    union Address incre;
    incre.label = label + 2;
    union Address skip;
    skip.label = label + 3;
    label = label + 4;
    dTreeElementPtr end_node = NULL;
    int exit_code;

    if ((new_token=next_token())->type == KIN_L_ROUNDBRACKET){
        gc_free(new_token);
        if((token_predict->type >= KW_AUTO) && (token_predict->type <= KW_STRING)){//for(int ID=.., ...) = definition
            new_token = next_token();
            if ((exit_code=dec_variable(new_token->type)) != 0){ return exit_code;}
            gc_free(new_token);
        }
        else if(token_predict->type == KIN_IDENTIFIER){                            //for(ID=.., ...) = inicialization
            new_token = next_token();
            if((exit_code=assing_exp(new_token)) != 0){ return exit_code;}
            gc_free(new_token);
        }
        else{ errorMessage_syntax("ERROR in definition part of FOR statement!"); return SYN_ERR;}
        gen_label(incre.label);
        if ((exit_code=expression_process(KIN_SEMICOLON, &end_node)) == KIN_SEMICOLON) {       //expression part of for statement
            if(end_node != NULL && end_node->description != D_DOLLAR) {
                gen_instructions(TAC_GOTO_COND, cond, end_node->data, fake, LABEL, end_node->type, EMPTY);
                gen_instructions(TAC_GOTO_UNCOND, skip, fake, fake, LABEL, EMPTY, EMPTY);
                gc_free(end_node);
            }else{return SYN_ERR;}

            gen_label(uncond.label);
            //new_token = token_predict;
            var_name = token_predict->str; //save name of ID, cause new_token can be free in expression in some cases
            exit_code = expression_process(KIN_R_ROUNDBRACKET,&end_node);      //command part of for statement
            if(exit_code == KIN_ASSIGNEMENT){
                exit_code = expression_process(KIN_R_ROUNDBRACKET,&end_node);
                if(exit_code != KIN_R_ROUNDBRACKET || end_node == NULL || end_node->description == D_DOLLAR){
                    return (exit_code == TYPE_COMP_SEM_ERR) ? TYPE_COMP_SEM_ERR : SYN_ERR;
                }
                union Address tmp;
                tmp.variable = var_name;
                gen_instructions(KIN_ASSIGNEMENT, tmp, end_node->data, fake, VARIABLE, end_node->type, EMPTY);
                gc_free(end_node);
            }else if(exit_code == TYPE_COMP_SEM_ERR){return TYPE_COMP_SEM_ERR;}

            if ((new_token = next_token())->type == KIN_L_BRACE){
                gc_free(new_token);
                gen_instructions(TAC_GOTO_UNCOND, incre, fake, fake, LABEL, EMPTY, EMPTY);
                gen_label(skip.label);
                exit_code = body_function();
                gen_instructions(TAC_GOTO_UNCOND, uncond, fake, fake, LABEL, EMPTY, EMPTY);
                gen_label(cond.label);
                gen_instructions(SCOPE_DOWN, fake, fake, fake, EMPTY, EMPTY, EMPTY);
                return exit_code;
            }
        }else if(exit_code == TYPE_COMP_SEM_ERR){return TYPE_COMP_SEM_ERR;}
    }
    errorMessage_syntax("Error in for statement !");
    return SYN_ERR; //2
}

int if_statement(){

    int exit_code;
    Token *new_token;
    union Address cond;
    cond.label = label;
    union Address uncond;
    uncond.label = label + 1;
    label = label + 2;
    dTreeElementPtr end_node = NULL;

    if(((new_token=next_token())->type == KIN_L_ROUNDBRACKET) && ((exit_code=expression_process(KIN_R_ROUNDBRACKET, &end_node)) == KIN_R_ROUNDBRACKET)){
        if(exit_code == TYPE_COMP_SEM_ERR){return TYPE_COMP_SEM_ERR;} //4
        if(end_node != NULL) {
            gen_instructions(TAC_GOTO_COND, cond, end_node->data, fake, LABEL, end_node->type, EMPTY);
            gc_free(new_token);
            gc_free(end_node);
        }
        if((new_token = next_token())->type == KIN_L_BRACE && ((exit_code=body_function()) == 0)) {
           gen_instructions(TAC_GOTO_UNCOND, uncond, fake, fake, LABEL, EMPTY,EMPTY);
           gen_label(cond.label);
           gc_free(new_token);
           if ((new_token = next_token())->type == KW_ELSE) {
               gc_free(new_token);
               if ((new_token = next_token())->type == KIN_L_BRACE) {
                   gc_free(new_token);
                   int exit_code =  body_function();
                   gen_label(uncond.label);
                   return exit_code;
               }
           }
       }
    }
    errorMessage_syntax("Bad syntax in \"if\"");
    return exit_code;
}

int cin(){
    Token *new_token;
    if ((new_token=next_token())->type == KIN_SCIN){
        gc_free(new_token);
        while(true){
            if((new_token = next_token())->type == KIN_IDENTIFIER){
                union Address data;
                data.variable = new_token->str;
                gen_instructions(KIN_SCIN, data , fake, fake, VARIABLE, EMPTY, EMPTY);
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
                else{       //v podstate nemusi tu byt, ci?
                    gc_free(new_token);
                }
            }
            break;
        }      
    }
    errorMessage_syntax(" WRONG cin operator!");
    return SYN_ERR;
}

int cout(){
    Token *new_token;
    int ret_code=SYN_ERR; //in case, first if statement is false
    dTreeElementPtr end_node = NULL;
    if ((new_token=next_token())->type == KIN_SCOUT){ 
        gc_free(new_token);
        while(true){
            ret_code = expression_process(KIN_SEMICOLON, &end_node);
            if(end_node != NULL && end_node->description != D_DOLLAR) {
               // fprintf(stderr,"COUT%d\n", end_node->type);             //debug
                gen_instructions(KIN_SCOUT, end_node->data, fake, fake, end_node->type, EMPTY, EMPTY);
                gc_free(end_node);
            }else{break;}   //if end_node == D_DOLLAR => no expression -> ERROR
            if(ret_code == KIN_SEMICOLON){return 0; }
            else if(ret_code == KIN_SCOUT){continue; }
            else{ break; }
        }
    }
    errorMessage_syntax("WRONG cout operator!"); 
    return ret_code;
}

int assing_exp(Token *token_var){       //token_var -> name of destination variable
    Token *new_token = next_token();
    dTreeElementPtr end_node = NULL;
    if (new_token->type == KIN_ASSIGNEMENT){
        gc_free(new_token);
        int exit_code = expression_process(KIN_SEMICOLON, &end_node);
        exit_code = (exit_code == KIN_SEMICOLON && end_node != NULL)? 0 : exit_code;
        if( !exit_code ) {
            union Address tmp;
            tmp.variable = token_var->str;
           // fprintf(stderr,"variable: %s!!!\n", tmp.variable);     //debug
            gen_instructions(KIN_ASSIGNEMENT, tmp, end_node->data, fake, VARIABLE, end_node->type, EMPTY);
            gc_free(end_node);
        }
        return exit_code;
    }
   // printf("%d assignement\n", new_token->type);
    errorMessage_syntax("WRONG assignement!");
    return SYN_ERR; //2
}

Type translate(enum sTokenKind type){
    switch(type){
        case KW_INT:
            return INT;
        case KW_DOUBLE:
            return DOUBLE;
        case KW_STRING:
            return STRING;
        case KW_AUTO:           
            return AUTO;
        default:
            //fprintf(stderr,"hope never happen\n");              //just debug
            return AUTO;             //this situation never happen
    }
}

int dec_variable(enum sTokenKind type){
    Token *new_token;
    if ((new_token=next_token())->type == KIN_IDENTIFIER) {
        union Address tmp;
        tmp.variable = new_token->str;
        gen_instructions(TAC_INIT,tmp, fake, fake, VARIABLE,translate(type),EMPTY);
        if (token_predict->type == KIN_SEMICOLON) {
            gc_free(next_token());
            gc_free(new_token);
            return 0;
        }
        else if(token_predict->type == KIN_ASSIGNEMENT){
            int exit_code;
            if((exit_code=assing_exp(new_token)) != 0){return exit_code;}
            gc_free(new_token);
            return 0;
        }
    }
    errorMessage_syntax("WRONG variable declaration");
    return SYN_ERR; //2
}

void ap_type(char **types,unsigned int type){

    switch (type){
        case KW_INT:
            strcat(*types,"i\0");
            break;
        case KW_DOUBLE:
            strcat(*types,"d\0");
            break;
        case KW_AUTO:               //should not be error?
            strcat(*types,"a\0");
            break;
        case KW_STRING:
            strcat(*types,"s\0");
            break;
        default:
            break;
    }

    *types = gc_realloc(*types, strlen(*types)+sizeof(char)*2);
    if (*types == NULL){errorMessage_internal("Realloc failed");}
}

int parameters_declar(unsigned int type_func, char **types, char **names){
    *types = gc_malloc(sizeof(char)*2);
    strcpy(*types, "");
    ap_type(types, type_func);
    Token *new_token= next_token();
    if(new_token->type == KIN_R_ROUNDBRACKET) {
        gc_free(new_token);
        return 0;
    }                                               
    while(true) {
        if((new_token->type >= KW_AUTO) && (new_token->type <= KW_STRING)){
            ap_type(types, new_token->type);
            gc_free(new_token);
            if ((new_token=next_token())->type == KIN_IDENTIFIER) {
                if (*names == NULL){
                    *names = new_token->str;
                }
                else{
                    *names = gc_realloc(*names, strlen(*names)+strlen(new_token->str)*sizeof(char)+2);
                    strcat(*names, " \0");
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
        errorMessage_syntax("WRONG declaration of function parameter!");
        return 1; 
    }
}

int parameters_used(){
    int counter_of_arguments = 0;
    dTreeElementPtr end_node = NULL;
    while(true) {
        counter_of_arguments++;
        int exit_code_value = expression_process(KIN_COMMA, &end_node);
        if(end_node != NULL){
            if(end_node->description == D_DOLLAR){return 0;} // zero number of arguments
            else{
                gen_instructions(TAC_PUSH, end_node->data, fake, fake, end_node->type, EMPTY, EMPTY);
                gc_free(end_node);
            }
        }
        if(exit_code_value == KIN_COMMA) {
            continue;
        }
        else if(exit_code_value == KIN_R_ROUNDBRACKET){
            return counter_of_arguments;
        }
        errorMessage_syntax("WRONG syntax of function arguments!");
        return -1;
    }
}

Token *next_token(){
    Token *new_token = token_predict;
    token_predict = get_token(fp);
    if(new_token == NULL ){
        errorMessage_internal("Malloc error");
        fclose(fp);
        gc_free_all();
        free(garbage);
        exit(INTER_ERR);
    } else if(new_token->type == KIN_UNKNOWN){
        errorMessage_lexical("BAD SYNTAX");
        fclose(fp);
        gc_free_all();
        free(garbage);
        exit(LEX_ERR);
    }
    else{
        if(new_token->type == KIN_SEMICOLON){
            //tmp_counter = 0;
        }
        return new_token;
    }
}