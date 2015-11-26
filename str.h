

#ifndef IJF_STR_H
#define IJF_STR_H

#include <string.h>
#include <stdlib.h>
typedef struct
{
    char* str;		// misto pro dany retezec ukonceny znakem '\0'
    int length;		// skutecna delka retezce
    int alloc_size;	// velikost alokovane pameti
} string;

enum {NOTFOUND = -1};

int str_init(string *s);
void str_free(string *s);

void str_clear(string *s);
int str_add_char(string *s1, char c);
int str_copy_string(string *s1, string *s2);
int str_cmp_string(string *s1, string *s2);
int str_cmp_const_str(string *s1, char *s2);

char *str_get_str(string *s);
int str_get_length(string *s);

//Added by Kopec Maros
int str_find(string *s1, char **s2, int size);
#include "garbage.h"
#endif //IJF_STR_H
