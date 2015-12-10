/*
 * File: precedence.c
 *
 * Description: Expressions parser
 * Authors: Chudý Andrej, Kopec Martin
 *
 * Created: 2015/11/02
 * Last time modified: 2015/11/27
 */

#include "precedence.h"
unsigned int tmp_counter = 0;

const char PrecedentTable [19][19] ={
                            // n_i  n_d  text +    -   *   /  ++  --   ID  (   )   ==   <   >  <=  >=  =!  $
    [KIN_NUM_INT]           = { 0 ,  0  , 0 ,'>' ,'>','>','>','>','>', 0 , 0 ,'>','>','>','>','>','>','>','>'},
    [KIN_NUM_DOUBLE]        = { 0 ,  0  , 0 ,'>' ,'>','>','>','>','>', 0 , 0 ,'>','>','>','>','>','>','>','>'},
    [KIN_TEXT]              = { 0 ,  0  , 0 ,'>' ,'>','>','>','>','>', 0 , 0 ,'>','>','>','>','>','>','>','>'},
    [KIN_PLUS]              = {'<', '<' ,'<','>' ,'>','<','<','<','<','<','<','>','>','>','>','>','>','>','>'},
    [KIN_MINUS]             = {'<', '<' ,'<','>' ,'>','<','<','<','<','<','<','>','>','>','>','>','>','>','>'},
    [KIN_MUL]               = {'<', '<' ,'<','>' ,'>','>','>','<','<','<','<','>','>','>','>','>','>','>','>'},
    [KIN_DIV]               = {'<', '<' ,'<','>' ,'>','>','>','<','<','<','<','>','>','>','>','>','>','>','>'},
    [KIN_PLUSPLUS]          = {'<', '<' ,'<','>' ,'>','>','>','>','>','<','<','>','>','>','>','>','>','>','>'},
    [KIN_MINUSMINUS]        = {'<', '<' ,'<','>' ,'>','>','>','>','>','<','<','>','>','>','>','>','>','>','>'},
    [KIN_IDENTIFIER]        = { 0 ,  0  , 0 ,'>' ,'>','>','>','>','>', 0 , 0 ,'>','>','>','>','>','>','>','>'},
    [KIN_L_ROUNDBRACKET]    = {'<', '<' ,'<','<' ,'<','<','<','<','<','<','<','=','<','<','<','<','<','<', 0 },
    [KIN_R_ROUNDBRACKET]    = { 0 ,  0  , 0 ,'>' ,'>','>','>','>','>', 0 , 0 ,'>','>','>','>','>','>','>','>'},
    [KIN_EQ]                = {'<', '<' ,'<','<' ,'<','<','<','<','<','<','<','>','>','>','>','>','>','>','>'},
    [KIN_SMALLER]           = {'<', '<' ,'<','<' ,'<','<','<','<','<','<','<','>','>','>','>','>','>','>','>'},
    [KIN_GREATER]           = {'<', '<' ,'<','<' ,'<','<','<','<','<','<','<','>','>','>','>','>','>','>','>'},
    [KIN_SMALLER_EQ]        = {'<', '<' ,'<','<' ,'<','<','<','<','<','<','<','>','>','>','>','>','>','>','>'},
    [KIN_GREATER_EQ]        = {'<', '<' ,'<','<' ,'<','<','<','<','<','<','<','>','>','>','>','>','>','>','>'},
    [KIN_NOT_EQ]            = {'<', '<' ,'<','<' ,'<','<','<','<','<','<','<','>','>','>','>','>','>','>','>'},
    [D_DOLLAR]              = {'<', '<' ,'<','<' ,'<','<','<','<','<','<','<','!' ,'<','<','<','<','<','<', 0},
};

void gen_tnp(enum sTokenKind inst, union Address op1, union Address op2, Type t_op1, Type t_op2){
    tOperation instruction = gc_malloc(sizeof(struct Operation));
    instruction->inst = inst;
    instruction->op1 = op1;
    instruction->op2 = op2;
    instruction->t_op1 = t_op1;
    instruction->t_op2 = t_op2;
    instruction->t_t = TMP;
    instruction->t.tmp = tmp_counter;
    instruction->label = 0;
    tmp_counter++;
    insert_last(tac_stack, instruction);
}
void gen_instructions(enum sTokenKind inst,union Address t, union Address op1, union Address op2,Type t_t, Type t_op1, Type t_op2){
    tOperation instruction = gc_malloc(sizeof(struct Operation));
    instruction->inst = inst;
    instruction->op1 = op1;
    instruction->op2 = op2;
    instruction->t_op1 = t_op1;
    instruction->t_op2 = t_op2;
    instruction->t_t = t_t;
    instruction->t = t;
    instruction->label = 0;
    insert_last(tac_stack, instruction);
}
void gen_label(unsigned int lab){
    tOperation instruction = gc_malloc(sizeof(struct Operation));
    instruction->inst = TAC_EMPTY;
    instruction->op1.fce = NULL;
    instruction->op2.fce = NULL;
    instruction->t_op1 = EMPTY;
    instruction->t_op2 = EMPTY;
    instruction->t_t = EMPTY;
    instruction->t.fce = NULL;
    instruction->label = lab;
    insert_last(tac_stack, instruction);
}

tDLList* init_stack(){
    tDLList *Stack = gc_malloc(sizeof(struct tDLList));   //malloc stack
    init_list(Stack);                                    //init stack
    dTreeElementPtr new_element;
    new_element = create_stack_element(D_DOLLAR, NULL);
    insert_last(Stack,new_element);
    return Stack;
}
dTreeElementPtr load_nonterm_char(dTreeElementPtr new_element, enum sTokenKind description) {
    new_element->description = description;     //operator  //stopper
    new_element->type = EMPTY;
    return new_element;
}
dTreeElementPtr load_token(dTreeElementPtr new_element, Token *token){

    switch (token->type) {
        case KIN_TEXT:
            new_element->type = STRING;
            new_element->data.s = token->str;
            new_element->description = KIN_TEXT;
            return new_element;
        case KIN_NUM_DOUBLE:
            new_element->type = DOUBLE;
            new_element->data.d = strtod(token->str, NULL);
            new_element->description = KIN_NUM_DOUBLE;
            return new_element;
        case KIN_NUM_INT:
            new_element->type = INT;
            new_element->data.i = (int) strtol(token->str, NULL, 10);
            new_element->description = KIN_NUM_INT;
            return new_element;
        case KIN_IDENTIFIER:
            new_element->type = VARIABLE;
            new_element->data.variable = token->str;
            new_element->description = KIN_IDENTIFIER;
            return new_element;
        default:
            return NULL;
    }
}
dTreeElementPtr create_stack_element(enum sTokenKind description, Token *token) {
    dTreeElementPtr new_element = gc_malloc(sizeof(struct dTreeElement));
    if (new_element == NULL) {
        fprintf(stderr,"Malloc ERROR");
        return NULL;
    }
    if(description == D_TMP){
        new_element->type = TMP ;
        new_element->data.tmp = tmp_counter - 1;
        new_element->description = KIN_NUM_INT;
            return new_element;
    }
    else if(token == NULL){      //stoper, dollar
        return load_nonterm_char(new_element, description);
    }
    else if ((token->type >= KIN_NUM_INT && token->type <= KIN_TEXT) || token->type == KIN_IDENTIFIER) { //cislo,string,var
        dTreeElementPtr loaded_element = load_token(new_element, token);
        gc_free(token);
        return loaded_element;
    }
    else {      //operator
        dTreeElementPtr loaded_element = load_nonterm_char(new_element, token->type);
        gc_free(token);
        return loaded_element;
    }

}
int rules( dTreeElementPtr p1, dTreeElementPtr p2, dTreeElementPtr p3){
    if(p3 != NULL) { // mame tri argumenty
        switch (p2->description) {
            case KIN_PLUS:
            case KIN_MINUS:
            case KIN_MUL:
            case KIN_DIV:
            case KIN_EQ:
            case KIN_NOT_EQ:
            case KIN_SMALLER:
            case KIN_GREATER:
            case KIN_SMALLER_EQ:
            case KIN_GREATER_EQ:
                if (p1->description == D_NODE && p3->description == D_NODE) {
                   
                    gen_tnp(p2->description, p3->data, p1->data, p3->type, p1->type);
                    return 2;   //number of useless tokens in stack
                } break;
            case D_NODE:
                if (p1->description == KIN_R_ROUNDBRACKET && p3->description == KIN_L_ROUNDBRACKET) {
                    return 2;
                }
                break;
            default:
                return -1; //error
        }

    }
    else if(p2 != NULL){    // rules 3-6
        switch(p1->description){        //E++/E--
            case KIN_PLUSPLUS:
            case KIN_MINUSMINUS:
                if(p2->description == D_NODE){
                    gen_tnp(p1->description, p2->data, p2->data, p2->type, EMPTY);
                    return 1;
                } break;
            default:
                break;
        }
        switch(p2->description){    //++E/--E
            case KIN_PLUSPLUS:
            case KIN_MINUSMINUS:
                if(p1->description == D_NODE){
                    gen_instructions(p2->description, p1->data, fake, fake, p1->type, EMPTY, EMPTY);
                    return 5;       //special situation
                } break;
            case KIN_MINUS:
                if(p1->description == D_NODE){
                    gen_tnp(KIN_UNARYMINUS, p1->data,fake, p1->type, EMPTY);
                    return 1;
                } break;

            default:
                return -1;
        }
    }
    else{       // rules 1-2
        switch (p1->description){
            case KIN_NUM_INT:
            case KIN_NUM_DOUBLE:
            case KIN_TEXT:
            case KIN_IDENTIFIER:
                return 0;   //do not delete element in stack, just create node
            default:
                return -1;
        }
    }
    return -1;
}
int element_reduct(tDLList *Stack){
    dTreeElementPtr elements[3] = {NULL,NULL,NULL};
    tDLElemPtr new = Stack->Last;
    int i;
    for(i =0; i<=4; i++){
        if(((dTreeElementPtr)new->data)->description == D_STOPER){
            gc_free(new->data);
            delete_element(Stack,new);
            break;
        }
        if (i >= 3){
            return -1;           
        }
        elements[i] = (dTreeElementPtr) new->data;
        new = new->lptr;
    }
    int exit_code = rules(elements[0],elements[1],elements[2]);
    if(exit_code == 0){
        ((dTreeElementPtr)Stack->Last->data)->description = D_NODE;
        return 0;
    }
    else if(exit_code == 5){      //exit_code 5, delete element --/++
        gc_free(elements[1]);
        delete_element(Stack, Stack->Last->lptr);
        return 0;
    }
    else if(exit_code >= 1 && exit_code <= 2){
        for (i = 0; i < exit_code; i++) {
            gc_free(copy_last(Stack));
            delete_last(Stack);
        }
        ((dTreeElementPtr)Stack->Last->data)->description = D_NODE;
        ((dTreeElementPtr)Stack->Last->data)->data.tmp = tmp_counter - 1 ;
        ((dTreeElementPtr)Stack->Last->data)->type = TMP;
        return 0;
    }
    else {
        return -1; 
    }
}

int call_function(Token *id_token){
    int number_of_params=0;
    char *params=NULL;
    switch(id_token->type){
        case KW_SORT:
        case KW_LENGTH:
            if(parameters_used() == 1){
                union Address tmp;
                tmp.fce = id_token->str;
                gen_tnp(TAC_CALL,tmp,fake,FUNCION,EMPTY);
                return 0;
            }else{return TYPE_COMP_SEM_ERR;}//4
        case KW_FIND:
        case KW_CONCAT:
            if(parameters_used() == 2){
                union Address tmp;
                tmp.fce = id_token->str;
                gen_tnp(TAC_CALL,tmp,fake,FUNCION,EMPTY);
                return 0;
            }else{return TYPE_COMP_SEM_ERR;}
        case KW_SUBSTR:
            if(parameters_used() == 3){
                union Address tmp;
                tmp.fce = id_token->str;
                gen_tnp(TAC_CALL,tmp,fake,FUNCION,EMPTY);
                return 0;
            }else{return TYPE_COMP_SEM_ERR;}
        case KIN_IDENTIFIER:            //user's function
            if((number_of_params= parameters_used()) != -1){
                BSTFind(&Func, id_token->str);  //check if function is declared if so, set active
                if ( ! BSTActive(&Func) ){      
                    errorMessage_semantic("UNDEFINED function");
                    return PROGRAM_SEM_ERR; //3
                }
                params = ((struct tFunc *)Func.Act->data)->params;
                if(strlen(params)-1 != (size_t)number_of_params){return TYPE_COMP_SEM_ERR;}
                union Address tmp;
                tmp.fce = id_token->str;
                gen_tnp(TAC_CALL,tmp,fake,FUNCION,EMPTY);
                return 0;
            }else{return SYN_ERR;}

        default:
            return SYN_ERR; //2
    }
}

int expression_process(enum sTokenKind end_char, dTreeElementPtr *final_node){
    tDLList *Stack = init_stack();
    bool load_new_tag = true;
    Token *new_token = NULL;
    int exit_char = KIN_R_ROUNDBRACKET;     //default value
    int exit_code;
    while(true) {
        int top_token = ((dTreeElementPtr)(find_last(Stack,false))->data)->description;
        if(load_new_tag == true) {
            new_token = next_token();
        }
        if((new_token->type == KIN_IDENTIFIER || (new_token->type >= KW_LENGTH && new_token->type <= KW_SORT)) &&
                token_predict->type == KIN_L_ROUNDBRACKET){                    //calling function in expession
            gc_free(next_token());
            if((exit_code=call_function(new_token)) != 0){ *final_node = clean_stack(Stack, false); return exit_code;}  //2,3 or 4
            insert_last(Stack, create_stack_element(D_STOPER, NULL));
            insert_last(Stack, create_stack_element(D_TMP, NULL));
            continue;
        }
        load_new_tag = true;

        if((new_token->type == end_char && new_token->type != KIN_R_ROUNDBRACKET) || new_token->type == END_OF_FILE ||
                new_token->type == KIN_COMMA || new_token->type == KIN_SCOUT || new_token->type == KIN_ASSIGNEMENT){
            if(new_token->type == KIN_ASSIGNEMENT){
                if( (((dTreeElementPtr)Stack->Last->data)->description != KIN_IDENTIFIER) || length_list(Stack) != 3) {
                    *final_node = clean_stack(Stack, false); 
                    return SYN_ERR; //2
                }                
            }
            exit_char = new_token->type;
            new_token->type = D_DOLLAR;
        }

        int exit_code;
        if(new_token->type > D_DOLLAR){
            *final_node = clean_stack(Stack, false);
            return SYN_ERR; //2
        }

        switch (PrecedentTable[top_token][new_token->type]) {
            case '<':
                if (Stack->Last == find_last(Stack,true)) {
                    preinsert_lastNode(Stack, create_stack_element(D_STOPER, NULL));
                    insert_last(Stack, create_stack_element(new_token->type, new_token));
                }
                else {
                    insert_last(Stack, create_stack_element(D_STOPER, NULL));
                    insert_last(Stack, create_stack_element(new_token->type, new_token));
                }
                continue;
            case '>':
                exit_code = element_reduct(Stack);
                if(exit_code == 0){
                    load_new_tag = false;
                    continue;
                }else {*final_node = clean_stack(Stack, false); return SYN_ERR;} //2
            case '=':
                insert_last(Stack, create_stack_element(new_token->type, new_token));
                continue;
            case '!':
                *final_node = clean_stack(Stack, true);
                return exit_char;
            default:
                /*error in precedence table, because needed rule doesn't exist -> means
                2 situations -> 1. it is an error
                             -> 2. everything was already reduced*/
                if(length_list(Stack) == 2 &&
                    ((dTreeElementPtr)(find_last(Stack,false))->data)->description == D_DOLLAR) {
                    *final_node = clean_stack(Stack, true);
                    return exit_char;
                }
                *final_node = clean_stack(Stack, false);
                return SYN_ERR; //2
        }
    }
}

dTreeElementPtr clean_stack(tDLList *Stack,  bool correct_end){
    dTreeElementPtr ptr = (correct_end) ? Stack->Last->data : NULL;
    dispose_list(Stack);
    return ptr;
}

tDLElemPtr find_last(tDLList *L, bool NODE){

    tDLElemPtr temp = L->Last;

    while(temp != NULL){

        if(!((((dTreeElementPtr) temp->data)->description == D_NODE) ^ NODE)){
            return temp;
        }
        else{
            temp = temp->lptr;
        }
    }

    return NULL;
}


void preinsert_lastNode(tDLList *L, void *data){

    tDLElemPtr last_node = find_last(L, true);

    if( last_node != NULL){

        tDLElemPtr new = gc_malloc(sizeof(struct tDLElem));
        if(new == NULL){
            data = NULL;
        }
        else{
            new->data = data;
            new->lptr = last_node->lptr;
            new->rptr = last_node;
            last_node->lptr = new;
            if(new->lptr != NULL){      /*Last node element wasn't last one*/
                new->lptr->rptr = new;
            }
            else{                       /*Last Node element was last => now L->Last must point to new element */
                L->First = new;
            }
        }
    }
}