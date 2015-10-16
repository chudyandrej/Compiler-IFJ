//
// Created by andrej on 15.10.2015.
//



#include "fake_skener.h"

void add_to_queue(char* Lexeme, int category) {
    struct token *new_token;
    new_token = malloc(sizeof(struct token));
    if (new_token) {
        strcpy(new_token->Lexeme, Lexeme);
        new_token->category = category;
    }
    TAILQ_INSERT_HEAD(&head, new_token, entries);
}

void set_quae() {
    TAILQ_INIT(&head);
    add_to_queue("}", '}');
    add_to_queue(";", ';');
    add_to_queue("mata", 'i');
    add_to_queue(">>", '>>');
    add_to_queue("cin", 'c');
    add_to_queue("{", '{');
    add_to_queue(")", ')');
    add_to_queue("(", '(');
    add_to_queue("main", 'i');
    add_to_queue("int", 't');
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
