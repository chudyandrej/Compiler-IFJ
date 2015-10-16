//
// Created by andrej on 15.10.2015.
//



#include "fake_skener.h"

void add_to_queue(char* Lexeme, char* category) {
    struct token *new_token;
    new_token = malloc(sizeof(struct token));
    if (new_token) {
        strcpy(new_token->Lexeme, Lexeme);
        strcpy(new_token->category, category);
    }
    TAILQ_INSERT_HEAD(&head, new_token, entries);
}

void set_quae() {
    TAILQ_INIT(&head);
    add_to_queue("}", "ble");
    add_to_queue(";", "ble");
    add_to_queue("mata", "id");
    add_to_queue(">>", "bla");
    add_to_queue("cin", "command");
    add_to_queue("{", "ble");
    add_to_queue(")", "bla");
    add_to_queue("(", "bla");
    add_to_queue("main", "id");
    add_to_queue("int", "type");
}

token_stract* next_token(){
    if (head.tqh_first != NULL) {
        token_stract *token_quar;
        token_quar = head.tqh_first;
        TAILQ_REMOVE(&head, head.tqh_first, entries);
        return token_quar;
    }
    return NULL;

}
