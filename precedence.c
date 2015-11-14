//
// Created by Andrej Oliver Chudý on 07/11/15.
//

#include <stdbool.h>
#include "precedence.h"
#include "stack.h"


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
    [D_DOLLAR]              = {'<', '<' ,'<','<' ,'<','<','<','<','<','<','<', 0 ,'<','<','<','<','<','<', 0 },
};

tDLList* init_stack(){
    tDLList *Stack = malloc(sizeof(tDLList));   //malloc stack
    init_list(Stack);                           //init stack
    dTreeElementPtr new_element;
    new_element = create_stack_element(D_DOLLAR, NULL);
    insert_last(Stack,new_element);
    return Stack;
}


dTreeElementPtr load_unterm_char(dTreeElementPtr new_element, enum sTokenKind description) {
    new_element->description = description;     //operator  //stopper
    new_element->type = EMPTY;
    return new_element;
}
dTreeElementPtr load_token_node(dTreeElementPtr new_element, Token *token){

    switch (token->type) {
        case KW_STRING:
            new_element->type = STRING;
            new_element->data.s = token->str;
            new_element->description = KIN_TEXT;
            return new_element;
        case KW_DOUBLE:
            new_element->type = DOUBLE;
            new_element->data.d = strtod(token->str, NULL);
            new_element->description = KIN_NUM_DOUBLE;
            return new_element;
        case KW_INT:
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
            printf("Chyba pri vytvarani elementu,create_terminal");
            return NULL;
    }
}
dTreeElementPtr create_stack_element(enum sTokenKind description, Token *token) {

    dTreeElementPtr new_element = malloc(sizeof(struct dTreeElement));
    if (new_element == NULL) {
        printf("Malloc ERROR");
        return NULL;
    }
    if(token == NULL){      //stoper, dollar
        return load_unterm_char(new_element, description);
    }
    else if ((token->type >= KIN_NUM_INT && token->type <= KIN_TEXT) || token->type == KIN_IDENTIFIER) { //cislo,string
        dTreeElementPtr exit_data = load_token_node(new_element, token);
        free(token);
        return exit_data;
    }
    else {
        dTreeElementPtr returned_value = load_unterm_char(new_element, token->type);
        free(token);
        return returned_value;
    }
}

void gen_instruction(enum Instruction ction_inst, union Address op1,union Address op2,enum Type t_op1,enum Type t_op2){

    tOperation instruction = malloc(sizeof(struct Operation));

    instruction->ction_inst = ction_inst;
    instruction->op1 = op1;
    instruction->op2 = op2;
    instruction->t_op1 = t_op1;
    instruction->t_op2 = t_op2;
    instruction->pe_t_t = TMP;
    instruction->t.tmp = tmp_counter;
    tmp_counter++;
    insert_last(THC, instruction);

}


int rules( dTreeElementPtr p1, dTreeElementPtr p2, dTreeElementPtr p3){
    if(p3 != NULL) { // mame tri argumenty
        switch (p2->description) {
            case KIN_PLUS:
                if (p1->description == D_NODE && p3->description == D_NODE) {
                    gen_instruction(MAT_ADD,p1->data,p3->data,p1->type,p3->type);
                    return 2;
                } break;
            case KIN_MINUS:
                if (p1->description == D_NODE && p3->description == D_NODE) {
                    gen_instruction(MAT_DIFF,p1->data,p3->data,p1->type,p3->type);
                    return 2;
                } break;
            case KIN_MUL:
                if (p1->description == D_NODE && p3->description == D_NODE) {
                    gen_instruction(MAT_MUL,p1->data,p3->data,p1->type,p3->type);
                    return 2;
                } break;
            case KIN_DIV:
                if (p1->description == D_NODE && p3->description == D_NODE) {
                    gen_instruction(MAT_DIV,p1->data,p3->data,p1->type,p3->type);
                    return 2;
                } break;
            case KIN_EQ:
                if (p1->description == D_NODE && p3->description == D_NODE) {
                    gen_instruction(COMP_EQ,p1->data,p3->data,p1->type,p3->type);
                    return 2;
                } break;
            case KIN_NOT_EQ:
                if (p1->description == D_NODE && p3->description == D_NODE) {
                    gen_instruction(COMP_NOT_EQ,p1->data,p3->data,p1->type,p3->type);
                    return 2;
                } break;
            case KIN_SMALLER:
                if (p1->description == D_NODE && p3->description == D_NODE) {
                    gen_instruction(COMP_SMALLER,p1->data,p3->data,p1->type,p3->type);
                    return 2;
                } break;
            case KIN_GREATER:
                if (p1->description == D_NODE && p3->description == D_NODE) {
                    gen_instruction(COMP_GREATER,p1->data,p3->data,p1->type,p3->type);
                    return 2;
                } break;
            case KIN_SMALLER_EQ:
                if (p1->description == D_NODE && p3->description == D_NODE) {
                    gen_instruction(COMP_SMALLER_EQ,p1->data,p3->data,p1->type,p3->type);
                    return 2;
                } break;
            case KIN_GREATER_EQ:
                if (p1->description == D_NODE && p3->description == D_NODE) {
                    gen_instruction(COMP_GREATER_E,p1->data,p3->data,p1->type,p3->type);
                    return 2;
                } break;
            case D_NODE:
                if (p1->description == KIN_L_ROUNDBRACKET && p3->description == KIN_R_ROUNDBRACKET) {
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
            case KIN_PLUSPLUS: if(p2->description == D_NODE){
                    gen_instruction(COMP_GREATER_E,p1->data,NULL,p1->type,EMPTY);
                    return 1;} break;
            case KIN_MINUSMINUS: if(p2->description == D_NODE){

                    return 1;} break;
            default:
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
}

int element_reduct(tDLList *Stack){
    dTreeElementPtr elements[3] = {NULL,NULL,NULL};
    tDLElemPtr new = Stack->Last;
    for(int i =0; i<=4; i++){

        if(((dTreeElementPtr) new->data)->description == D_STOPER){
            delete_element(Stack,new);
            break;
        }
        if (i > 3){
            return 100;
        }
        elements[i] = (dTreeElementPtr) new->data;
        new = Stack->Last->lptr;
    }
    int exit_code = rules(elements[0],elements[1],elements[2]);
    if(exit_code == 0){
        ((dTreeElementPtr)Stack->Last->data)->description = D_NODE;
    }
    else {
        for (int i = 0; exit_code > i; i++) {
            delete_last(Stack);
        }
        ((dTreeElementPtr)Stack->Last->data)->description = D_NODE;
        ((dTreeElementPtr)Stack->Last->data)->data.tmp = tmp_counter - 1 ;
        ((dTreeElementPtr)Stack->Last->data)->type = TMP;
    }
}

int expression_process(enum sTokenKind end_char){

    tDLList *Stack = init_stack();
    while(true) {
        int top_token = ((dTreeElementPtr) find_last(Stack, false))->description;
        Token *new_token = get_token(fp);
        if(new_token->type == end_char && new_token->type != KIN_R_ROUNDBRACKET){
            new_token->type = D_DOLLAR;
        }
        switch (PrecedentTable[top_token][new_token->type]) {
            case '<':
                if (Stack->Last == find_last(Stack,true)) {
                    preinsert_lastNode(Stack, create_stack_element(D_STOPER, NULL));
                    insert_last(Stack, create_stack_element(new_token->type, new_token));
                    break;
                }
                else {
                    insert_last(Stack, create_stack_element(D_STOPER, NULL));
                    insert_last(Stack, create_stack_element(new_token->type, new_token));
                    break;
                }

            case '>':

                element_reduct(Stack);
                return 1;


            case '=':
                insert_last(Stack, create_stack_element(new_token->type, new_token));
                break;
            default:
                printf("Mrda mi v hlave");

        }
    }


}

