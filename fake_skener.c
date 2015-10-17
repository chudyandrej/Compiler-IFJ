//
// Created by andrej on 15.10.2015.
//



#include "fake_skener.h"
#include "syntax_checker.h"


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
    add_to_queue(")", ')');
    add_to_queue("a", 'n');
    add_to_queue("a", 'o');
    add_to_queue("a", ')');
    add_to_queue("a", 'n');
    add_to_queue("a", 'o');
    add_to_queue("a", 'n');
    add_to_queue("a", '(');
    add_to_queue(",", ',');
    add_to_queue("a", 'i');
    add_to_queue("(", '(');
    add_to_queue("a", 'i');
    add_to_queue(";", ';');
    add_to_queue("a", 'i');
    add_to_queue("<<", TT_SCOUT);
    add_to_queue("mata", 'i');
    add_to_queue("<<", TT_SCOUT);
    add_to_queue("cout", C_COUT);
    add_to_queue(";", ';');
    add_to_queue("mata", 'i');
    add_to_queue(">>", TT_SCIN);
    add_to_queue("cin", C_CIN);

    add_to_queue(";", ';');
    add_to_queue(")", ')');
    add_to_queue("a", 'i');
    add_to_queue(",", ',');
    add_to_queue("a", 'i');
    add_to_queue(",", ',');
    add_to_queue("d", 'i');
    add_to_queue("(", '(');
    add_to_queue("substr", C_SUBSTR);

    add_to_queue(";", ';');
    add_to_queue(")", ')');
    add_to_queue("d", 'i');
    add_to_queue("(", '(');
    add_to_queue("length", C_LENGTH);

    add_to_queue("{", '{');
    add_to_queue(")", ')');
    add_to_queue("(", '(');
    add_to_queue("main", 'i');
    add_to_queue("int", 't');
}

token_stract* next_new_token(){
    if (head.tqh_first != NULL) {
        token_stract *token_quar;
        token_quar = head.tqh_first;
        TAILQ_REMOVE(&head, head.tqh_first, entries);
        return token_quar;
    }
    return NULL;

}
