/*
 * File: str.c
 *
 * Description: simple string library
 * Authors: Brno University of Technology
 */

//jednoducha knihovna pro praci s nekonecne dlouhymi retezci
#include <string.h>
#include <stdlib.h>
#include "str.h"

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

/*
 * Function: str_find
 * Author: Kopec Maros
 * Description: Find if string is in array of strings
 *
 * type: int
 * param 'string *s1': searched string
 * param 'char **s2': array of strings in which is searched
 * param 'int size': number of strings in array
 * returns: position if founded else NOTFOUND (-1) if string is not in array
 */
int str_find(string *s1, char **s2, int size)
{
    int i;
    for (i = 0; i<size; i++)
    {
        if (!strcmp(s1->str, s2[i]))
        {
            return i;
        }
    }
    return NOTFOUND;
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
