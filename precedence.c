//
// Created by Andrej Oliver Chudý on 07/11/15.
//


#include "precedence.h"



const char PrecedentTable [19][19] ={
                            // n_i  n_d  text +    -   *   /  ++  --   ID  (   )   ==   <   >  <=  >=  =!  $
    [KIN_NUM_INT]           = { 0 ,  0  , 0 ,'>' ,'>','>','>','>','>', 0 , 0 ,'>','>','>','>','>','>','>','>'},
    [KIN_NUM_DOUBLE]        = { 0 ,  0  , 0 ,'>' ,'>','>','>','>','>', 0 , 0 ,'>','>','>','>','>','>','>','>'},
    [KIN_TEXT]              = { 0 ,  0  , 0 ,'>' ,'>','>','>','>','>', 0 , 0 ,'>','>','>','>','>','>','>','>'},
    [KIN_PLUS]              = {'<', '<' ,'<','>' ,'>','<','<','<','<','<','<','>','>','>','>','>','>','>','>'},
    [KIN_MINUS]             = {'<', '<' ,'<','>' ,'>','<','<','<','<','<','<','>','>','>','>','>','>','>','>'},
    [KIN_MUL]               = {'<', '<' ,'<','>' ,'>','>','>','<','<','<','<','>','>','>','>','>','>','>','>'},
    [KIN_DIV]               = {'<', '<' ,'<','>' ,'>','>','>','<','<','<','<','>','>','>','>','>','>','>','>'},
    [KIN_PLUSPLUS]          = {'<', '<' ,'<','>' ,'>','>','>','>','>','>','<','>','>','>','>','>','>','>','>'},
    [KIN_MINUSMINUS]        = {'<', '<' ,'<','>' ,'>','>','>','>','>','>','<','>','>','>','>','>','>','>','>'},
    [KIN_IDENTIFIER]        = { 0 ,  0  , 0 ,'>' ,'>','>','>','>','>', 0 , 0 ,'>','>','>','>','>','>','>','>'},
    [KIN_L_ROUNDBRACKET]    = {'<', '<' ,'<','<' ,'<','<','<','<','<','<','<','=','<','<','<','<','<','<', 0 },
    [KIN_R_ROUNDBRACKET]    = { 0 ,  0  , 0 ,'>' ,'>','>','>','>','>', 0 , 0 ,'>','>','>','>','>','>','>','>'},
    [KIN_EQ]                = {'<', '<' ,'<','<' ,'<','<','<','<','<','<','<','>','>','>','>','>','>','>','>'},
    [KIN_SMALLER]           = {'<', '<' ,'<','<' ,'<','<','<','<','<','<','<','>','>','>','>','>','>','>','>'},
    [KIN_GREATER]           = {'<', '<' ,'<','<' ,'<','<','<','<','<','<','<','>','>','>','>','>','>','>','>'},
    [KIN_SMALLER_EQ]        = {'<', '<' ,'<','<' ,'<','<','<','<','<','<','<','>','>','>','>','>','>','>','>'},
    [KIN_GREATER_EQ]        = {'<', '<' ,'<','<' ,'<','<','<','<','<','<','<','>','>','>','>','>','>','>','>'},
    [KIN_NOT_EQ]            = {'<', '<' ,'<','<' ,'<','<','<','<','<','<','<','>','>','>','>','>','>','>','>'},
    [D_DOLLAR]              = {'<', '<' ,'<','<' ,'<','<','<','<','<','<','<','!' ,'<','<','<','<','<','<', 0 },
};

void gen_instruction(enum Instruction ction_inst, union Address op1,union Address op2,enum Type t_op1,enum Type t_op2){
    tOperation instruction = malloc(sizeof(struct Operation));
    instruction->ction_inst = ction_inst;
    instruction->op1 = op1;
    instruction->op2 = op2;
    instruction->t_op1 = t_op1;
    instruction->t_op2 = t_op2;
    instruction->pe_t_t = TMP;
    //instruction->t.tmp = tmp_counter;
    //tmp_counter++;
    // insert_last(THC, instruction);

}
void gen_unary_instruction(enum Instruction ction_inst, union Address op1,union Address t,enum Type t_op1,enum Type pe_t_t){
    tOperation instruction = malloc(sizeof(struct Operation));
    instruction->ction_inst = ction_inst;
    instruction->op1 = op1;
    instruction->t_op1 = t_op1;
    instruction->t_op2 = EMPTY;
    instruction->pe_t_t = TMP;
    // instruction->t.tmp = tmp_counter;
    //  tmp_counter++;
    //  insert_last(THC, instruction);

}


void printf_stack(tDLList *Stack){
    tDLElemPtr new_help = Stack->First;
    new_help = Stack->First;
    printf("\n####### STACK ##########\n");
    printf("######## %p ########\n",&Stack);
    while(new_help != NULL){
        printf("%d\n",((dTreeElementPtr) new_help->data)->description);
        new_help = new_help->rptr;
    }
    printf("########################\n");
}
tDLList* init_stack(){
    tDLList *Stack = malloc(sizeof(struct tDLList));   //malloc stack
    init_list(Stack);                           //init stack
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
            new_element->data.i = atoi(token->str);
            new_element->description = KIN_NUM_INT;
            return new_element;
        case KIN_IDENTIFIER:
            new_element->type = VARIABLE;
            new_element->data.variable = token->str;
            new_element->description = KIN_IDENTIFIER;
            return new_element;
        default:
            printf("\nload token !!!!!!!!!!!!\n");
            return NULL;
    }
}
dTreeElementPtr create_stack_element(enum sTokenKind description, Token *token) {
    dTreeElementPtr new_element = malloc(sizeof(struct dTreeElement));
    if (new_element == NULL) {
        printf("Malloc ERROR");
        return NULL;
    }
    if(description == D_TMP){
        new_element->type = TMP;
        //new_element->data.tmp =
        new_element->description = KIN_NUM_INT;
            return new_element;
    }
    else if(token == NULL){      //stoper, dollar
        return load_nonterm_char(new_element, description);
    }
    else if ((token->type >= KIN_NUM_INT && token->type <= KIN_TEXT) || token->type == KIN_IDENTIFIER) { //cislo,string,var
        dTreeElementPtr loaded_element = load_token(new_element, token);
        free(token);
        return loaded_element;
    }
    else {      //operator
        dTreeElementPtr loaded_element = load_nonterm_char(new_element, token->type);
        free(token);
        return loaded_element;
    }

}
int rules( dTreeElementPtr p1, dTreeElementPtr p2, dTreeElementPtr p3){
    if(p3 != NULL) { // mame tri argumenty
        switch (p2->description) {
            case KIN_PLUS:
                if (p1->description == D_NODE && p3->description == D_NODE) {
               //     gen_instruction(MAT_ADD,p1->data,p3->data,p1->type,p3->type);
                    return 2;
                } break;
            case KIN_MINUS:
                if (p1->description == D_NODE && p3->description == D_NODE) {
               //     gen_instruction(MAT_DIFF,p1->data,p3->data,p1->type,p3->type);
                    return 2;
                } break;
            case KIN_MUL:
                if (p1->description == D_NODE && p3->description == D_NODE) {
                 //   gen_instruction(MAT_MUL,p1->data,p3->data,p1->type,p3->type);
                    return 2;
                } break;
            case KIN_DIV:
                if (p1->description == D_NODE && p3->description == D_NODE) {
                //    gen_instruction(MAT_DIV,p1->data,p3->data,p1->type,p3->type);
                    return 2;
                } break;
            case KIN_EQ:
                if (p1->description == D_NODE && p3->description == D_NODE) {
                //    gen_instruction(COMP_EQ,p1->data,p3->data,p1->type,p3->type);
                    return 2;
                } break;
            case KIN_NOT_EQ:
                if (p1->description == D_NODE && p3->description == D_NODE) {
                 //   gen_instruction(COMP_NOT_EQ,p1->data,p3->data,p1->type,p3->type);
                    return 2;
                } break;
            case KIN_SMALLER:
                if (p1->description == D_NODE && p3->description == D_NODE) {
                   // gen_instruction(COMP_SMALLER,p1->data,p3->data,p1->type,p3->type);
                    return 2;
                } break;
            case KIN_GREATER:
                if (p1->description == D_NODE && p3->description == D_NODE) {
                   // gen_instruction(COMP_GREATER,p1->data,p3->data,p1->type,p3->type);
                    return 2;
                } break;
            case KIN_SMALLER_EQ:
                if (p1->description == D_NODE && p3->description == D_NODE) {
                   // gen_instruction(COMP_SMALLER_EQ,p1->data,p3->data,p1->type,p3->type);
                    return 2;
                } break;
            case KIN_GREATER_EQ:
                if (p1->description == D_NODE && p3->description == D_NODE) {
                   // gen_instruction(COMP_GREATER_EQ, p1->data, p3->data, p1->type, p3->type);
                    return 2;
                } break;
            case D_NODE:
                if (p1->description == KIN_R_ROUNDBRACKET && p3->description == KIN_L_ROUNDBRACKET) {
                    return 2;
                }
                break;
            default:
               printf("Error: rules tri argumenty");
                return 100;
        }

    }
    else if(p2 != NULL){    // rules 3-6
        switch(p1->description){
            case KIN_PLUSPLUS:
                if(p2->description == D_NODE){
                    return 1;
                } break;
            case KIN_MINUSMINUS: if(p2->description == D_NODE){
                    return 1;
                } break;
            default:
                printf("Ruls");
                break;
        }
        switch(p2->description){
            case KIN_PLUSPLUS: if(p1->description == D_NODE){
                    return 1;
                } break;
            case KIN_MINUSMINUS: if(p1->description == D_NODE){
                    return 1;
                } break;
            default:
                printf("chyba pri uplatnovani pravidiel dvoch\n");
                return 100;
        }
    }
    else{       // rules 1-2
        switch (p1->description){
            case KIN_NUM_INT:
            case KIN_NUM_DOUBLE:
            case KIN_TEXT:
            case KIN_IDENTIFIER:
                return 0;
            default:
                printf("chyba pri uplatnovani pravidiel jednej\n");
                return 100;
        }
    }
    printf("Mrda mi v halve: rules - od vrchu: %d %d %d\n", p1->description,
        p2->description, p3->description);
    return 100;
}
int element_reduct(tDLList *Stack){
    dTreeElementPtr elements[3] = {NULL,NULL,NULL};
    tDLElemPtr new = Stack->Last;
    int i;
    for(i =0; i<=4; i++){
        if(((dTreeElementPtr)new->data)->description == D_STOPER){
            free(new->data);
            delete_element(Stack,new);
            break;
        }
        if (i >= 3){
            return 100;           //ak nepojde matova ruka je v ohni
        }
        elements[i] = (dTreeElementPtr) new->data;
        new = new->lptr;
    }
    int exit_code = rules(elements[0],elements[1],elements[2]);
    if(exit_code == 0){
        ((dTreeElementPtr)Stack->Last->data)->description = D_NODE;
        return 0;
    }
    else if(exit_code >= 1 && exit_code <= 2){
        for (i = 0; i < exit_code; i++) {
            free(copy_last(Stack));
            delete_last(Stack);
        }
        ((dTreeElementPtr)Stack->Last->data)->description = D_NODE;
        //((dTreeElementPtr)Stack->Last->data)->data.tmp = tmp_counter - 1 ;
        ((dTreeElementPtr)Stack->Last->data)->type = TMP;
        return 0;
    }
    else {
      //  printf("Mrdka\n");
        return 100;
    }
}

int call_function(Token *id_token){

    switch(id_token->type){
        case KW_SORT:
        case KW_LENGTH:
            if(parameters_used() == 1){return 0;}else{return 1;}
        case KW_FIND:
        case KW_CONCAT:
            if(parameters_used() == 2){return 0;}else{return 1;}
        case KW_SUBSTR:
            if(parameters_used() == 3){return 0;}else{return 1;}
        case KIN_IDENTIFIER:

            if(parameters_used() != 100){ printf("ta som tu\n");return 0;}else{return 1;}

        default:
            return 1;

    }

}



int expression_process(enum sTokenKind end_char){
    tDLList *Stack = init_stack();

    bool load_new_tag = true;
    Token *new_token = NULL;
    int exit_char = KIN_R_ROUNDBRACKET;
    while(true) {
        printf_stack(Stack);
        int top_token = ((dTreeElementPtr)(find_last(Stack,false))->data)->description;
        if(load_new_tag == true) {
            new_token = next_token();
        }
        if((new_token->type == KIN_IDENTIFIER || (new_token->type >= KW_LENGTH && new_token->type <= KW_SORT)) &&
                token_predict->type == KIN_L_ROUNDBRACKET){
            printf("VNARAM\n");
            free(next_token());
            if(call_function(new_token) != 0){return 1;}
            insert_last(Stack, create_stack_element(D_STOPER, NULL));
            insert_last(Stack, create_stack_element(D_TMP, NULL));
            new_token=next_token();
            printf("VYNARAM\n");
            printf_stack(Stack);
        }
        load_new_tag = true;

        if((new_token->type == end_char && new_token->type != KIN_R_ROUNDBRACKET) || new_token->type == END_OF_FILE ||
                new_token->type == KIN_COMMA){
            exit_char = new_token->type;
            new_token->type = D_DOLLAR;
        }

        int exit_code;
        if(new_token->type > D_DOLLAR){ printf("Mrada mi v medziach: expression_process %d",new_token->type);return 1;}

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
                }
                if(exit_code == 100 && length_list(Stack) == 2 &&
                        ((dTreeElementPtr)(find_last(Stack,false))->data)->description == D_DOLLAR) {
                    dispose_list(Stack);
                    return end_char;
                }
                else{
                    dispose_list(Stack);
                    return 1;
                }
            case '=':
                insert_last(Stack, create_stack_element(new_token->type, new_token));
                continue;
            case '!':
                printf("\nvykricnik\n");
                    return exit_char;
            default:
                if(length_list(Stack) == 2 &&
                    ((dTreeElementPtr)(find_last(Stack,false))->data)->description == D_DOLLAR) {
                    dispose_list(Stack);
                     return exit_char;
                }
                printf("precedence table errror s tokenom: %d\n", new_token->type);
                dispose_list(Stack);
                return 1;

        }
    }


}

