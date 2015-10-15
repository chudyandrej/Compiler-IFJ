/* 
 * File: str.h
 *
 * Description: header file for simple string library
 * Authors: Brno University of Technology
 */

//hlavickovy soubor pro praci s nekonecne dlouhymi retezci

typedef struct
{
  char* str;		// misto pro dany retezec ukonceny znakem '\0'
  int length;		// skutecna delka retezce
  int alloc_size;	// velikost alokovane pameti
} string;


int str_init(string *s);
void str_free(string *s);

void str_clear(string *s);
int str_add_char(string *s1, char c);
int str_copy_string(string *s1, string *s2);
int str_cmp_string(string *s1, string *s2);
int str_cmp_const_str(string *s1, char *s2);

char *str_get_str(string *s);
int str_get_length(string *s);

int str_init_wr(string *s);
int str_add_char_wr(string *s, char c);
int str_copy_string_wr(string *s1, string *s2);
