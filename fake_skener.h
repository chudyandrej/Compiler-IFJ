//
// Created by andrej on 15.10.2015.
//

#ifndef INTERPRET_FAKE_SKENER_H
#define INTERPRET_FAKE_SKENER_H


#include <sys/queue.h>
#include "string.h"
#include <stdlib.h>
#include <stdio.h>

TAILQ_HEAD(tailhead, token) head;

typedef struct token {
    char Lexeme[10];
    char category[10];
    TAILQ_ENTRY(token) entries;
} token_stract;

int fuck();
void add_to_queue(char* Lexeme, char* category);
void set_quae();
token_stract* next_token();


#endif //INTERPRET_FAKE_SKENER_H
