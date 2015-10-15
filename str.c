/* 
 * File: str.c
 *
 * Description: simple string library
 * Authors: Brno University of Technology
 */

//jednoducha knihovna pro praci s nekonecne dlouhymi retezci
#include <string.h>
#include <malloc.h>
#include "str.h"
#include "err.h"

#define STR_LEN_INC 8
// konstanta STR_LEN_INC udava, na kolik bytu provedeme pocatecni alokaci pameti
// pokud nacitame retezec znak po znaku, pamet se postupne bude alkokovat na
// nasobky tohoto cisla

#define STR_ERROR   1
#define STR_SUCCESS 0

int str_init(string *s)
// funkce vytvori novy retezec
{
   if ((s->str = (char*) malloc(STR_LEN_INC)) == NULL)
      return STR_ERROR;
   s->str[0] = '\0';
   s->length = 0;
   s->alloc_size = STR_LEN_INC;
   return STR_SUCCESS;
}

void str_free(string *s)
// funkce uvolni retezec z pameti
{
   free(s->str);
}

void str_clear(string *s)
// funkce vymaze obsah retezce
{
   s->str[0] = '\0';
   s->length = 0;
}

int str_add_char(string *s1, char c)
// prida na konec retezce jeden znak
{
   if (s1->length + 1 >= s1->alloc_size)
   {
      // pamet nestaci, je potreba provest realokaci
      if ((s1->str = (char*) realloc(s1->str, s1->length + STR_LEN_INC)) == NULL)
         return STR_ERROR;
      s1->alloc_size = s1->length + STR_LEN_INC;
   }
   s1->str[s1->length] = c;
   s1->length++;
   s1->str[s1->length] = '\0';
   return STR_SUCCESS;
}

int str_copy_string(string *s1, string *s2)
// prekopiruje retezec s2 do s1
{
   int newLength = s2->length;
   if (newLength >= s1->alloc_size)
   {
      // pamet nestaci, je potreba provest realokaci
      if ((s1->str = (char*) realloc(s1->str, newLength + 1)) == NULL)
         return STR_ERROR;
      s1->alloc_size = newLength + 1;
   }
   strcpy(s1->str, s2->str);
   s1->length = newLength;
   return STR_SUCCESS;
}

int str_cmp_string(string *s1, string *s2)
// porovna oba retezce a vrati vysledek
{
   return strcmp(s1->str, s2->str);
}

int str_cmp_const_str(string *s1, char* s2)
// porovna nas retezec s konstantnim retezcem
{
   return strcmp(s1->str, s2);
}

char *str_get_str(string *s)
// vrati textovou cast retezce
{
   return s->str;
}

int str_get_length(string *s)
// vrati delku daneho retezce
{
   return s->length;
}

/*
 * Wrapper for handling error
 */
int str_init_wr(string *s)
// funkce vytvori novy retezec
{
    if(str_init(s)) {
        /*INTER_ERR nie je zadefinovany*/
        return INTER_ERR;
    }
}

/*
 * Wrapper for handling error
 */
int str_add_char_wr(string *s, char c)
{
    if(str_add_char(s, c)) {
        str_free(s);
        /*INTER_ERR nie je zadefinovany*/
        return INTER_ERR;
    }
}

/*
 * Wrapper for handling error
 */
int str_copy_string_wr(string *s1, string *s2)
{
    if(str_copy_string(s1, s2)) {
        str_free(s1);
        /*INTER_ERR nie je zadefinovany*/
        return INTER_ERR;
    }
}