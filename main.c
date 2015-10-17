/* 
 * File: main.c
 *
 * Authors: 
 *
 * Created: 2015/10/6
 * Last time modified: 2015/10/17
 */

#include <stdio.h>
#include <stdlib.h>
#include "scanner.h"

/*
char * data[] = {
    "auto",
    "double",
    "int",
    "string"
};
 */ 


int main() {
    
    FILE * fp;
    fp=fopen("/home/maros/NetBeansProjects/scanner/test.ifj15", "r");
    int t;
    
    //char * string = "AHOJ";
    //Token * token = get_token(fp);
    
    //printf("%d\n", KIN_UNKNOWN);
    //printf("%s",get_token(fp)->str);
    //printf("%s",string);
    
    while ((t = get_token(fp)->type) != END_OF_FILE) {
        printf("%d\n",t);
    }
    //printf("%s",data[0]);
    //printf("\n#################################################################");
    
    fclose(fp);
    //return  token->type;
    return (EXIT_SUCCESS);
}

