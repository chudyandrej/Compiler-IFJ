#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ial.h"
#include "tac.h"
#include "stack.h"

struct tBST Func;


void init();
void fillM(tDLList * m);
void fillA(tDLList * a);

int main(){
    init();
    int out =  interpret();
    printf("///////////////\n");
    printf("END!\n");
    return out;
}

void init(){
    int out;
    BSTInit(&Func);
    char * m_key = malloc(sizeof(char)*5);
    char * a_key = malloc(sizeof(char)*5);
    char * m_params = malloc(sizeof(char)*2);
    char * a_params = malloc(sizeof(char)*4);
    char * m_names = malloc(sizeof(char)*1);
    char * a_names = malloc(sizeof(char)*4);
    strcpy(m_key, "main");
    strcpy(a_key, "fooo");
    strcpy(m_params, "v");
    strcpy(m_params, "iid");
    strcpy(m_names, "");
    strcpy(a_names,"a b");
    tDLList * a = malloc(sizeof(tDLList));
    tDLList * m = malloc(sizeof(tDLList));
    fillM(m);
    fillA(a);
    BSTAdd(&Func, m_key);
    BSTAdd(&Func, a_key);


    BSTFind(&Func, m_key);
    out = GSTDeclare(&Func, m_params, m_names);
    printf("Declare main: %d\n", out);
    out = GSTDefine(&Func, m);
    printf("Define main: %d\n", out);


    BSTFind(&Func, a_key);
    out = GSTDeclare(&Func, a_params, a_names);
    printf("Declare fooo: %d\n", out);
    out = GSTDefine(&Func, a);
    printf("Define fooo: %d\n", out);


    out = GSTAllDef(Func.Root);
    printf("///////////////\n");

}

void fillM(tDLList * m){
    // int ahoj = (4+9)*5
    init_list(m);
    tOperation o1 = malloc(sizeof(struct Operation));
    o1->inst = TAC_INIT;
    o1->t.variable = malloc(sizeof(char)*5);
    strcpy(o1->t.variable, "ahoj");
    o1->t_t=VARIABLE;
    o1->t_op1=INT;


    tOperation o2 = malloc(sizeof(struct Operation));
    o2->inst = KIN_PLUS;
    o2->t.tmp = 0;
    o2->t_t=TMP;
    o2->t_op1=INT;
    o2->t_op2=INT;
    o2->op1.i = 4;
    o2->op2.i = 9;


    tOperation o3 = malloc(sizeof(struct Operation));
    o3->inst = KIN_MUL;
    o3->t.tmp = 1;
    o3->t_t=TMP;
    o3->t_op1=TMP;
    o3->t_op2=INT;
    o3->op1.tmp = 0;
    o3->op2.i = 5;


    tOperation o4 = malloc(sizeof(struct Operation));
    o4->inst = KIN_ASSIGNEMENT;
    o4->t.variable = malloc(sizeof(char)*5);
    strcpy(o4->t.variable, "ahoj");
    o4->t_t=VARIABLE;
    o4->t_op1=TMP;
    o4->op1.tmp=1;
    insert_last(m, o1);
    insert_last(m, o2);
    insert_last(m, o3);
    insert_last(m, o4);
}

void fillA(tDLList * a){

}