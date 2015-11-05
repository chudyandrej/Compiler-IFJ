#include <stdio.h>
#include "err.h"

/*
 * Function: handle_err
 * Author: Kopec Maros
 * Description: Handle error code and print massage to standard error output
 * 
 * type: int
 * returns: erroe code
 */
int handle_err(tErrCode code) {
    fprintf(stderr, "ERROR MESSAGE");
    return code;
}