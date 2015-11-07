//
// Created by Andrej Oliver Chudý on 07/11/15.
//

#include "err.h"

int handle_err(tErrCode code) {
    fprintf(stderr, "ERROR MESSAGE");
    return code;
}