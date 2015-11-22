#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "ial.h"
#include "tac.h"
#include "stack.h"

extern struct tBST Func; //globalni tabulka funkci
struct TMPRecord** working_tmp; //tmp promene aktualni funce
unsigned int working_size = 0;

void * extendTmp(void *ptr, unsigned int *size);
int countingOp(struct Operation *rec, tBSTPtr my_ST, int scope);
int assignmentOp(struct Operation *rec, tBSTPtr my_ST, int scope);
int initOp(struct Operation *rec, tBSTPtr my_ST, int scope);
int coutOp(struct Operation *rec, tBSTPtr my_ST);
int cinOp(struct Operation *rec, tBSTPtr my_ST);
int dereference(struct Operation *rec, tBSTPtr my_ST, int address_number, struct TMPRecord * dereferenced);
int pushOp(struct Operation *rec, tBSTPtr my_ST, int scope, tDLList * my_push);
int to_double(struct TMPRecord * tmp);
int function(char * name, tBSTPtr my_ST, union Address * ret);
void store_tmp(struct TMPRecord * tmp, int i);

int interpret(){
	if (GSTAllDef(Func.Root) != 1) return 3;
	BSTFind(&Func, "main");
	if (BSTActive(&Func)!=1) return 3;
	union Address ret;
	struct tBST ST;
	BSTInit(&ST);
	return function("main", &ST, &ret);
}

int function(char * name, tBSTPtr my_ST, union Address * ret){
	int out; // osetreni semantickych chyb
	int scope = 0; //aktualni zanoreni 
	unsigned int tmp_size = 4; //velikost pole pro TMP promene
	struct TMPRecord** TableRecords; // pole pro TMP promene
	TableRecords = malloc(sizeof(struct TMPRecord *) * tmp_size);
	working_tmp = TableRecords; // nastavit globalni na aktualni
	working_size = tmp_size;
	BSTFind(&Func, name);
	tDLList * my_tac = GSTCopyTAC(&Func); //stahnout zasobnik 3AC
	tDLList my_push; // zasobnik na pushovani pro volani funkci
	init_list(&my_push);
	activate_first(my_tac);
	while(is_active(my_tac)){ //cyklit nad vsem 3AC instrukcemi
		struct Operation *rec = copy_active(my_tac);
		TokenKind instruction = rec->inst;
		if((instruction>=3)&&(instruction<=17)){
			out = countingOp(rec, my_ST, scope);
		}
		else if (instruction==SCOPE_UP) scope++;
		else if (instruction==SCOPE_DOWN) scope--;
		else if (instruction==KIN_ASSIGNEMENT){
			out = assignmentOp(rec, my_ST, scope);
		}
		else if (instruction==TAC_PUSH){
			out = pushOp(rec, my_ST, scope, &my_push);
		}
		else if (instruction==TAC_INIT){
			out = initOp(rec, my_ST, scope);
		}
		else if (instruction==KIN_SCOUT){
			out = coutOp(rec, my_ST);
		} 
		else if (instruction==KIN_SCIN){
			out = cinOp(rec, my_ST);
		} 
		else if (instruction==TAC_EMPTY);
		else{
			/*TO DO : 
			KIN_PLUSPLUS,
			KIN_MINUSMINUS, 
            TAC_RETURN,
            TAC_CALL,
            TAC_GOTO_UNCOND,
            TAC_GOTO_COND,
			*/
			printf("IN PROGRESS!: %d\n", instruction);
		}
		shift_active(my_tac);
		if (out) {printf("out fuu: %d\n", out); return out;}
	}
	return out;
}


int countingOp(struct Operation *rec, tBSTPtr my_ST, int scope){
	int out;
	struct TMPRecord * operand1 = malloc(sizeof(struct TMPRecord));
	struct TMPRecord * operand2 = malloc(sizeof(struct TMPRecord));
	struct TMPRecord * target = malloc(sizeof(struct TMPRecord));
	target->t=DOUBLE;
	out = dereference(rec, my_ST, 1, operand1);
	if (out!=0) return out;
	out = to_double(operand1);
	if (out!=0) return out;
	out = dereference(rec, my_ST, 2, operand2);
	if (out!=0) return out;
	out = to_double(operand2);
	if (out!=0) return out;
	switch(rec->inst){
		case KIN_PLUS:
			target->value.d = operand1->value.d + operand2->value.d;
			break;
		case KIN_MINUS:
			target->value.d = operand1->value.d - operand2->value.d;
			break;
		case KIN_MUL:
			target->value.d = operand1->value.d * operand2->value.d;
			break;
		case KIN_DIV:
			target->value.d = operand1->value.d / operand2->value.d;
			break;
		case KIN_EQ:
			target->value.d = operand1->value.d == operand2->value.d;
			break;
		case KIN_SMALLER:
			target->value.d = operand1->value.d < operand2->value.d;
			break;
		case KIN_GREATER:
			target->value.d = operand1->value.d > operand2->value.d;
			break;
		case KIN_SMALLER_EQ:
			target->value.d = operand1->value.d <= operand2->value.d;
			break;
		case KIN_GREATER_EQ:
			target->value.d = operand1->value.d >= operand2->value.d;
			break;
		case KIN_NOT_EQ:
			target->value.d = operand1->value.d != operand2->value.d;
			break;
		default:
			out = 4;
	}
	if ((operand1->t == INT) && (operand2->t == INT)){
		target->value.i = (int)target->value.d;
		target->t = INT;
	}
	if (rec->t_t==VARIABLE){
		BSTFind(my_ST, rec->t.variable);
		if (!BSTActive(my_ST)) return 3;
		out = LSTSet(my_ST, target);
	} 
	else if (rec->t_t==TMP){
		store_tmp(target, rec->t.tmp);
	}
	else return 10;
	return out;
}


int assignmentOp(struct Operation *rec, tBSTPtr my_ST, int scope){
	struct TMPRecord * dereferenced = malloc(sizeof(struct TMPRecord));
	int out = dereference(rec, my_ST, 1, dereferenced);
	if (out!=0) return out;
	if (rec->t_t==VARIABLE){
		BSTFind(my_ST, rec->t.variable);
		if (!BSTActive(my_ST)) return 3;
		out = LSTSet(my_ST, dereferenced);
	}
	else return 10; //neni duvod volat tuto FCI na TMP promene
	return out;
}

int pushOp(struct Operation *rec, tBSTPtr my_ST, int scope, tDLList * my_push){
	return 0;
}

int initOp(struct Operation *rec, tBSTPtr my_ST, int scope){
	int out;
	BSTFind(my_ST, rec->t.variable);
	if(!BSTActive(my_ST)) BSTAdd(my_ST, rec->t.variable);
	out = LSTAdd(my_ST, rec->t_op1, scope);
	return out;
}

int dereference(struct Operation *rec, tBSTPtr my_ST, int address_number, struct TMPRecord * dereferenced){
	struct TMPRecord * tmp = NULL;
	int i;
	int out = 0;
	if (address_number==0){
		dereferenced->value = rec->t;
		dereferenced->t = rec->t_t;
	} 
	else if(address_number==1){
		dereferenced->value = rec->op1;
		dereferenced->t = rec->t_op1;
	}
	else{
		dereferenced->value = rec->op2;
		dereferenced->t = rec->t_op2;
	}
	switch (dereferenced->t){
		case INT:
		case DOUBLE:
		case STRING:
			break;
		case TMP:
			i = dereferenced->value.tmp;
			dereferenced->value = working_tmp[i]->value;
			dereferenced->t = working_tmp[i]->t;
			break;
		case VARIABLE:
			tmp = malloc(sizeof(struct TMPRecord));
			BSTFind(my_ST, dereferenced->value.variable);
			if (!BSTActive(my_ST)) return 3;
			out = LSTGet(my_ST, tmp);
			dereferenced->value = tmp->value;
			dereferenced->t = tmp->t;
			free(tmp);
			break;
		default:
			return 10;
	}
	return out;
}

void * extendTmp(void *ptr, unsigned int *size){
	//working_tmp = extendTmp(working_tmp, &tmp_size);
	*size = *size * 2;
	ptr = realloc(ptr, sizeof(struct TMPRecord) * *size);
	return ptr;
}

int to_double(struct TMPRecord * tmp){
	if(tmp->t==STRING) return 4;
	else if(tmp->t==INT) {
		tmp->value.d=(double)tmp->value.i;
	}
	return 0;

}

void store_tmp(struct TMPRecord * tmp, int i){
	while (i>=working_size){
		working_tmp = extendTmp(working_tmp, &working_size);
	}
	working_tmp[i] = tmp;
}

int coutOp(struct Operation *rec, tBSTPtr my_ST){
	int out = 0;
	struct TMPRecord * dereferenced = malloc(sizeof(struct TMPRecord));
	out = dereference(rec, my_ST, 0, dereferenced);
	if (dereferenced->t==STRING){
		printf("%s", dereferenced->value.s);
	}
	else if (dereferenced->t==INT){
		printf("%d", dereferenced->value.i);
	}
	else if (dereferenced->t==DOUBLE){
		printf("%g", dereferenced->value.d);
	} 
	else out = 10;
	return out;
}

int cinOp(struct Operation *rec, tBSTPtr my_ST){
	int out = 0;
	if (rec->t_t!=VARIABLE) return 2;
	struct TMPRecord * dereferenced = malloc(sizeof(struct TMPRecord));
	out = dereference(rec, my_ST, 0, dereferenced);
	if (out!=1){
		char * str = malloc(1);
		char * ptr = NULL;
		char c;
		int count = 1;
		while(isspace(c=fgetc(stdin)));

		do {
			str = realloc(str, ++count);
			str[count-2] = c;
			str[count-1] = '\0';
		}
		while(!isspace(c=fgetc(stdin)));
		if(dereferenced->t==INT){
			dereferenced->value.i = (int) strtol(str, &ptr, 10);
			if (*ptr!='\0') return 7;
		}
		else if(dereferenced->t==DOUBLE){
			dereferenced->value.d = strtod(str, &ptr);
			if (*ptr!='\0') return 7;
		}
		else if(dereferenced->t==STRING){
			dereferenced->value.s = str;
		}
		else return 10;
		BSTFind(my_ST, rec->t.variable);
		if (!BSTActive(my_ST)) return 3;
		out = LSTSet(my_ST, dereferenced);

	}
	return out;
}