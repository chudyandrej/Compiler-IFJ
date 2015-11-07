//
// Created by Andrej Oliver Chudý on 07/11/15.
//

#include <stdbool.h>
#include "precedence.h"
#include "stack.h"


const char PrecedentTable [18][18] ={
                                // n_i   n_d   +    -   *   /  ++  --  ID   (   )  ==   <   >  <=  >=  =! $
        [KIN_NUM_INT]           = { 0 ,  0  ,'>' ,'>','>','>','>','>', 0 , 0 ,'>','>','>','>','>','>','>','>'},
        [KIN_NUM_DOUBLE]        = { 0 ,  0  ,'>' ,'>','>','>','>','>', 0 , 0 ,'>','>','>','>','>','>','>','>'},
        [KIN_PLUS]              = {'<', '<' ,'>' ,'>','<','<','<','<','<','<','>','>','>','>','>','>','>','>'},
        [KIN_MINUS]             = {'<', '<' ,'>' ,'>','<','<','<','<','<','<','>','>','>','>','>','>','>','>'},
        [KIN_MUL]               = {'<', '<' ,'>' ,'>','>','>','<','<','<','<','>','>','>','>','>','>','>','>'},
        [KIN_DIV]               = {'<', '<' ,'>' ,'>','>','>','<','<','<','<','>','>','>','>','>','>','>','>'},
        [KIN_PLUSPLUS]          = {'<', '<' ,'>' ,'>','>','>','>','>','>','<','>','>','>','>','>','>','>','>'},
        [KIN_MINUSMINUS]        = {'<', '<' ,'>' ,'>','>','>','>','>','>','<','>','>','>','>','>','>','>','>'},
        [KIN_IDENTIFIER]        = { 0 ,  0  ,'>' ,'>','>','>','>','>', 0 , 0 ,'>','>','>','>','>','>','>','>'},
        [KIN_L_ROUNDBRACKET]    = {'<', '<' ,'<' ,'<','<','<','<','<','<','=','>','>','>','>','>','>','>','>'},
        [KIN_R_ROUNDBRACKET]    = { 0 ,  0  ,'>' ,'>','>','>','>','>', 0 , 0 ,'>','>','>','>','>','>','>','>'},
        [KIN_EQ]                = {'<', '<' ,'<' ,'<','<','<','<','<','<','<','>','>','>','>','>','>','>','>'},
        [KIN_SMALLER]           = {'<', '<' ,'<' ,'<','<','<','<','<','<','<','>','>','>','>','>','>','>','>'},
        [KIN_GREATER]           = {'<', '<' ,'<' ,'<','<','<','<','<','<','<','>','>','>','>','>','>','>','>'},
        [KIN_SMALLER_EQ]        = {'<', '<' ,'<' ,'<','<','<','<','<','<','<','>','>','>','>','>','>','>','>'},
        [KIN_GREATER_EQ]        = {'<', '<' ,'<' ,'<','<','<','<','<','<','<','>','>','>','>','>','>','>','>'},
        [KIN_NOT_EQ]            = {'<', '<' ,'<' ,'<','<','<','<','<','<','<','>','>','>','>','>','>','>','>'},
        [KIN_DOLLAR]            = {'<', '<' ,'<' ,'<','<','<','<','<','<','<', 0 ,'<','<','<','<','<','<', 0 },
};

tDLList* init_stack(){
    tDLList *Stack = malloc(sizeof(tDLList));
    dTreeElementPtr new_element;
    init_list(Stack);
    new_element = create_tree_element(NULL);
    new_element->token = create_fake_token(KIN_DOLLAR);
    insert_last(Stack,new_element);
    return Stack;
}

Token* create_fake_token(unsigned int type){
    Token *init_token = malloc(sizeof(Token));
    init_token->type = type;
    init_token->str = NULL;
    return init_token;
}

dTreeElementPtr create_tree_element(Token *token){
    dTreeElementPtr new_element = malloc(sizeof(struct dTreeElement));
    new_element->token = token;
    new_element->description = 0;
    new_element->l = NULL;
    new_element->m = NULL;
    new_element->r = NULL;
    return new_element;
}

void element_reduct(tDLList *Stack){
    while(((dTreeElementPtr)Stack->Last->data)->description != STOPPER){



    }
}

int expression_process(){
    tDLList *Stack = init_stack();
    int TOP_stack =((dTreeElementPtr) copy_last(Stack))->token->type;
    Token *new_token = get_token(fp);
    switch(PrecedentTable[TOP_stack][new_token->type]){
        case '<':
            if(Stack->LastNode == Stack->Last){
                preinsert_lastNode(Stack,create_fake_token(STOPPER));
                insert_last_desc(Stack,create_tree_element(new_token),TOKEN);
            }
            else{
                insert_last_desc(Stack,create_tree_element(create_fake_token(STOPPER)),TOKEN);
                insert_last_desc(Stack,create_tree_element(new_token),TOKEN);
            }
            break;
        case '>':

        case '=':
            insert_last_desc(Stack,create_tree_element(new_token),TOKEN);
            break;
        default:
            printf("Error pri precedencnej tabulke");
            return 100;
    }


    return 1;
}
