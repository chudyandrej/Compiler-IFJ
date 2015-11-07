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
    new_element->description = (token == NULL)? NODE : TOKEN;
    new_element->l = NULL;
    new_element->m = NULL;
    new_element->r = NULL;
    return new_element;
}

int rules( dTreeElementPtr p1, dTreeElementPtr p2, dTreeElementPtr p3){
    if(p3 != NULL){ // rules 7-17
        if(p2->description == TOKEN) {
            switch (p2->token->type) {
                case KIN_PLUS:
                    if(p1->description == NODE && p3->description == NODE) {return RULE_7;} break;
                case KIN_MINUS:
                    if(p1->description == NODE && p3->description == NODE) {return RULE_8;} break;
                case KIN_MUL:
                    if(p1->description == NODE && p3->description == NODE) {return RULE_9;} break;
                case KIN_DIV:
                    if(p1->description == NODE && p3->description == NODE) {return RULE_10;} break;
                case KIN_EQ:
                    if(p1->description == NODE && p3->description == NODE) {return RULE_12;} break;
                case KIN_NOT_EQ:
                    if(p1->description == NODE && p3->description == NODE) {return RULE_13;} break;
                case KIN_SMALLER:
                    if(p1->description == NODE && p3->description == NODE) {return RULE_14;} break;
                case KIN_GREATER:
                    if(p1->description == NODE && p3->description == NODE) {return RULE_15;} break;
                case KIN_SMALLER_EQ:
                    if(p1->description == NODE && p3->description == NODE) {return RULE_16} break;
                case KIN_GREATER_EQ:
                    if(p1->description == NODE && p3->description == NODE) {return RULE_17;} break;
                default:
                    break;
            }
        }
        else if(p2->description == NODE){
            if(p1->description == TOKEN && p3->description == TOKEN){
                return RULE_11;
            }
        }
        printf("chyba pri uplatnovani pravidiel troch\n");
        return 100;
    }
    else if(p2 != NULL){ // rules 3-6
        if(p1->description == TOKEN){
            switch(p1->token->type){
                case KIN_PLUSPLUS:
                    if(p2->description == NODE){ return RULE_4;} break;
                case KIN_MINUSMINUS:
                    if(p2->description == NODE){ return RULE_6;} break;
                default:
                    break;
            }
        }
        else if(p2->description == TOKEN){
            switch(p2->token->type){
                case KIN_PLUSPLUS:
                    if(p1->description == NODE){ return RULE_3;} break;
                case KIN_MINUSMINUS:
                    if(p1->description == NODE){ return RULE_5;} break;
                default:
                    break;
            }
        }
        printf("chyba pri uplatnovani pravidiel dvoch\n");
        return 100;
    }
    else{   // rules 1-2
        if(p1->description == TOKEN){
            switch (p1->token->type){
                case KIN_NUM_INT:
                case KIN_NUM_DOUBLE:
                case KW_STRING:
                    return RULE_2;
                case KIN_IDENTIFIER:
                    return RULE_1;
                default:
                    break;
            }
        }
        printf("chyba pri uplatnovani pravidiel jednej\n");
        return 100;
    }


}

void element_reduct(tDLList *Stack){
    int i = 0;
    dTreeElementPtr elemnts[3]={NULL,NULL,NULL};
    for(i = 0; i<3; i++){
        dTreeElementPtr pop_element = ((dTreeElementPtr) pop(Stack));
        if(pop_element->description != STOPPER){
            elemnts[i] = pop_element;

        }
        else{
            free(pop_element);
            int selected_rule = rules(elemnts[0],elemnts[1],elemnts[2]);
            dTreeElementPtr new_node = create_tree_element(NULL);
            new_node->description = NODE;
            switch(selected_rule){
                case RULE_1:case RULE_2:
                    new_node->

            }
        }
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
