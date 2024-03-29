
#ifndef IJF_ERR_H
#define IJF_ERR_H

#include <stdio.h>

typedef enum ErrCode {
        LEX_ERR = 1,    // 1 - chyba v programu v rámci lexikální analýzy
                        // (chybná struktura aktuálního lexému).
        SYN_ERR,
                        /*  2 - chyba v programu v rámci syntaktické analýzy
                         * (chybná syntaxe struktury programu).*/

        PROGRAM_SEM_ERR,
                        /*  3 - sémantická chyba v programu – nedefinovaná funkce/promenná,
                         *  pokus o redefinici funkce/promenné, atd.*/

        TYPE_COMP_SEM_ERR,
                            /*  4 - sémantická chyba typové kompatibility v aritmetických,
                            * retezcových a relacních výrazech, príp. špatný pocet
                            * ci typ parametru u volání funkce.*/

        VAR_TYPE_ERR,
                    /*  5 - chyba pri odvozování datového typu promenné.*/

            OTHER_SEM_ERR,
    /* 6 - ostatní sémantické chyby*/

            NUM_ERR,
    /*  7 - behová chyba pri nacítání císelné hodnoty ze vstupu.*/

            NONINIT_VAR_ERR,
    /*  8 - behová chyba pri práci s neinicializovanou promennou.*/

            ZERO_DIV_ERR,
    /*  9 - behová chyba delení nulou.*/

            OTHER_RUN_ERR,
    /*  10 - ostatní behové chyby.*/

            INTER_ERR = 99
    /*  99 - interní chyba interpretu tj. neovlivnená vstupním programem
     *  (napr. chyba alokace pameti, chyba pri otvírání souboru
     *  s rídicím programem, špatné parametry príkazové rádky atd.)*/
}tErrCode;

int handle_err(tErrCode code);
void errorMessage_syntax(const char *message );
void errorMessage_lexical(const char *message );
void errorMessage_internal(const char *message );
void errorMessage_semantic(const char *message );
#endif //IJF_ERR_H
