/*
 * File: buildin.c
 *
 * Description: All build in functions that are not required in ial.c
 * Authors: Vaclav Kondula
 */

#include "buildin.h"
extern struct TMPRecord** working_push;
extern unsigned int push_size;

#define ESC_HEX_MAX 2
#define ESC_BINARY_MAX 8
#define ESC_OCTAL_MAX 3

/**
 * Return a substring defined by:
 * @param  str pointer to char array
 * @param  i index of a first char
 * @param  n length of returned string
 */
char * substr(char * str, int i, int n){
    int l = strlen(str);
    char * out = NULL;
    if (i<0);
    else if (i==l){
        out = gc_malloc(1);
        out[0] = '\0';
    } 
    else if (i<l){
        int end;
        if (l>(n+i)) end = n+i;
        else end = l;
        out = gc_malloc(end-i+1);
        memcpy(out, &str[i], end-i);
        out[end-i]='\0';
    }
    return out;
}



int isBuildIn(struct Operation *rec){
/*
0 || not build in
1 || int length(string s) 
2 || string substr(string s, int i, int n) 
3 || string concat(string s1, string s2) 
4 || int find(string s, string search)
5 || string sort(string s) 
*/
    char * str = rec->op1.fce;
    if (strcmp(str, "length")==0)
        return 1;
    else if (strcmp(str, "substr")==0)
        return 2;
    else if (strcmp(str, "concat")==0)
        return 3;
    else if (strcmp(str, "find")==0)
        return 4;
    else if (strcmp(str, "sort")==0)
        return 5;   
    return 0;

}

int buildInOp(struct Operation *rec, tBSTPtr my_ST, int op){
    int out = 0;
    struct TMPRecord * target = gc_malloc(sizeof(struct TMPRecord));
    if (op==1){
        //length
        struct TMPRecord * operand1;
        operand1 = working_push[push_size-1];
        decrease_push();
        if (operand1->t==STRING){
            target->t=INT;
            target->value.i=strlen(operand1->value.s);
        } 
        else out = 4;
    }
    else if (op==2){
        //substr
        struct TMPRecord * operand3;
        operand3 = working_push[push_size-1];
        decrease_push();
        struct TMPRecord * operand2;
        operand2 = working_push[push_size-1];
        decrease_push();
        struct TMPRecord * operand1;
        operand1 = working_push[push_size-1];
        decrease_push();
        if (operand2->t==DOUBLE){
            operand2->value.i = (int)operand2->value.d;
            operand2->t=INT;
        }
        if (operand3->t==DOUBLE){
            operand3->value.i = (int)operand3->value.d;
            operand3->t=INT;
        }
        if ((operand1->t==STRING)&&(operand2->t==INT)&&(operand3->t==INT)){
            target->t=STRING;
            target->value.s = substr(operand1->value.s, operand2->value.i, operand3->value.i);
            if (target->value.s == NULL) out = 10;
        }
        else out = 4;
    }
    else if (op==3){
        //concat
        struct TMPRecord * operand2;
        operand2 = working_push[push_size-1];
        decrease_push();
        struct TMPRecord * operand1;
        operand1 = working_push[push_size-1];
        decrease_push();
        if ((operand1->t==STRING)&&(operand2->t==STRING)){
            int a = strlen(operand1->value.s);
            int b = strlen(operand2->value.s);
            target->value.s = gc_malloc(a+b+1);
            target->t = STRING;
            strcpy(target->value.s, operand1->value.s);
            strcpy(&target->value.s[a], operand2->value.s);
        }
        else out = 4;
    }
    else if (op==4){
        //find
        struct TMPRecord * operand2;
        operand2 = working_push[push_size-1];
        decrease_push();
        struct TMPRecord * operand1;
        operand1 = working_push[push_size-1];
        decrease_push();
        if ((operand1->t==STRING)&&(operand2->t==STRING)){
            target->t=INT;
            target->value.i = find(operand1->value.s, operand2->value.s);
        }
        else out = 4;
    }
    else if (op==5){
        // sort
        struct TMPRecord * operand1;
        operand1 = working_push[push_size-1];
        decrease_push();
        if (operand1->t==STRING){
            target->t=STRING;
            int a = strlen(operand1->value.s);
            target->value.s = gc_malloc(a + 1);
            strcpy(target->value.s, operand1->value.s);
            sort(target->value.s);
        }
        else return 4;
    }
    else out = 10;
    if (out) return out;
    if (rec->t_t==VARIABLE){
        BSTFind(my_ST, rec->t.variable);
        if (!BSTActive(my_ST)) return 3;
        out = LSTSet(my_ST, target);
    } 
    else if (rec->t_t==TMP){
        store_tmp(target, rec->t.tmp);
    }
    return out;
}