#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "ial.h"
#include "tac.h"
#include "stack.h"
#include "tmp.h"

extern struct tBST Func; //globalni tabulka funkci
struct TMPRecord** working_tmp; //tmp promene aktualni funce
struct TMPRecord** working_push;
unsigned int working_size = 0;
unsigned int working_push_size = 0;

void * extendTmp(void *ptr, unsigned int *size);
int countingOp(struct Operation *rec, tBSTPtr my_ST, int scope);
int assignmentOp(struct Operation *rec, tBSTPtr my_ST, int scope);
int initOp(struct Operation *rec, tBSTPtr my_ST, int scope);
int coutOp(struct Operation *rec, tBSTPtr my_ST);
int cinOp(struct Operation *rec, tBSTPtr my_ST);
int dereference(struct Operation *rec, tBSTPtr my_ST, int address_number, struct TMPRecord * dereferenced);
int pushOp(struct Operation *rec, tBSTPtr my_ST, int scope, tDLList * my_push);
int to_double(struct TMPRecord * tmp);
int function(char * name, tBSTPtr my_ST, struct TMPRecord * ret);
void store_tmp(struct TMPRecord * tmp, int i);
int condition(struct Operation *rec, tBSTPtr my_ST, int * jump);
void jumpOp(struct Operation *rec, tBSTPtr my_ST, tDLList * my_tac);
int returnOp(struct Operation *rec, tBSTPtr my_ST, struct TMPRecord * ret, char c);
enum Type convert(char c);
int unaryminusOp(struct Operation *rec, tBSTPtr my_ST);
int unaryOp(struct Operation *rec, tBSTPtr my_ST, int con);
void store_push(struct TMPRecord * tmp);
int isBuildIn(struct Operation *rec);
int buildInOp(struct Operation *rec, tBSTPtr my_ST, int op);
int funcOp(struct Operation *rec, tBSTPtr my_ST);
void decrease_push();

int interpret(){
	if (GSTAllDef(Func.Root) != 1) return 3;
	BSTFind(&Func, "main");
	if (BSTActive(&Func)!=1) return 3;
	struct TMPRecord ret;
	struct tBST ST;
	BSTInit(&ST);
	return function("main", &ST, &ret);
}

int function(char * name, tBSTPtr my_ST, struct TMPRecord * ret){
	int out; // osetreni semantickych chyb
	int scope = 0; //aktualni zanoreni 
	unsigned int tmp_size = 4; //velikost pole pro TMP promene
	unsigned int push_size = 0;
	struct TMPRecord** TableRecords; // pole pro TMP promene
	struct TMPRecord** PushRecords; // pole pro TMP promene
	TableRecords = malloc(sizeof(struct TMPRecord *) * tmp_size);
	PushRecords = malloc(sizeof(struct TMPRecord *) * push_size);
	working_tmp = TableRecords; // nastavit globalni na aktualni
	working_size = tmp_size;
	working_push = PushRecords;
	working_push_size = push_size;
	BSTFind(&Func, name);
	char * params = ((struct tFunc *)Func.Act)->params;
	tDLList * my_tac = GSTCopyTAC(&Func); //stahnout zasobnik 3AC
	tDLList my_push; // zasobnik na pushovani pro volani funkci
	init_list(&my_push);
	activate_first(my_tac);
	while(is_active(my_tac)){ //cyklit nad vsem 3AC instrukcemi
		struct Operation *rec = copy_active(my_tac);
		TokenKind instruction = rec->inst;
		if(instruction==KIN_UNARYMINUS){
			out = unaryminusOp(rec, my_ST);
		}
		else if (instruction==KIN_PLUSPLUS){
			out = unaryOp(rec, my_ST, 1);
		}
		else if (instruction==KIN_MINUSMINUS){
			out = unaryOp(rec, my_ST, -1);
		}
		if((instruction>=3)&&(instruction<=17)){
			out = countingOp(rec, my_ST, scope);
		}
		else if (instruction==SCOPE_UP) scope++;
		else if (instruction==SCOPE_DOWN) {
			LSTLeaveScope(my_ST->Root, scope);
			scope--;
		}
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
		else if (instruction==TAC_GOTO_UNCOND){
			jumpOp(rec, my_ST, my_tac);
			continue;
		}
		else if (instruction==TAC_GOTO_COND){
			int jump = 0;
			out =  condition(rec, my_ST, &jump);
			if (jump){
				jumpOp(rec, my_ST, my_tac);
				continue;
			}
		}
		else if (instruction==TAC_RETURN){
			return returnOp(rec, my_ST, ret, params[0]);
		}
		else if (instruction==TAC_CALL){
			int op = isBuildIn(rec);
			if (op)
				out = buildInOp(rec, my_ST, op);
			else
				out = funcOp(rec, my_ST);
		}
		else {
			return 10; // neznama instrukce
		}
		shift_active(my_tac);
		if (out) {printf("out fuu: %d\n", out); return out;}
	}

	return 8; // end of non-void function
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
			if (operand2->value.d==0.0) return 9;
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
	int out = 0;
	struct TMPRecord * dereferenced = malloc(sizeof(struct TMPRecord));
	out = dereference(rec, my_ST, 0, dereferenced);
	if (!out){
		store_push(dereferenced);
	}
	return out;
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

void * extendPush(void *ptr, unsigned int size){
	// zmeni velikost zasobniku na push
	return realloc(ptr, sizeof(struct TMPRecord) * size);
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

void store_push(struct TMPRecord * tmp){
	working_push_size++;
	working_push = extendPush(working_tmp, working_push_size);
	working_push[working_push_size-1] = tmp;
}

int coutOp(struct Operation *rec, tBSTPtr my_ST){
	int out = 0;
	struct TMPRecord * dereferenced = malloc(sizeof(struct TMPRecord));
	out = dereference(rec, my_ST, 0, dereferenced);
	if (!out){
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
	}
	return out;
}

int cinOp(struct Operation *rec, tBSTPtr my_ST){
	int out = 0;
	if (rec->t_t!=VARIABLE) return 2;
	struct TMPRecord * dereferenced = malloc(sizeof(struct TMPRecord));
	out = dereference(rec, my_ST, 0, dereferenced);
	if (out!=8){
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

int condition(struct Operation *rec, tBSTPtr my_ST, int * jump){
	int out = 0;
	struct TMPRecord * dereferenced = malloc(sizeof(struct TMPRecord));
	out = dereference(rec, my_ST, 1, dereferenced);
	if (!out){
		if (dereferenced->t==STRING) return 6;
		else if (dereferenced->t==DOUBLE){
			if(dereferenced->value.d==0.0) *jump=1;
		}
		else if (dereferenced->t==INT){
			if(dereferenced->value.i==0) *jump=1;
		}
	}
	return out;
}

void jumpOp(struct Operation *rec, tBSTPtr my_ST, tDLList * my_tac){
	unsigned int label = rec->op1.label;
	activate_first(my_tac);
	while(is_active(my_tac)){
		struct Operation *rec = copy_active(my_tac);
		if (rec->label == label) return;
		shift_active(my_tac);
	}
}

int returnOp(struct Operation *rec, tBSTPtr my_ST, struct TMPRecord * ret, char c){
	int out = 0;
	struct TMPRecord * dereferenced = malloc(sizeof(struct TMPRecord));
	out = dereference(rec, my_ST, 0, dereferenced);
	if (!out){
		if(dereferenced->t==convert(c)){
			ret->value = dereferenced->value;
			ret->t = dereferenced->t;
		} 
		else if ((convert(c)==DOUBLE)&&(dereferenced->t==INT)){
			ret->value.d = (double)dereferenced->value.i;
			ret->t = DOUBLE;	
		}
		else if ((convert(c)==INT)&&(dereferenced->t==DOUBLE)){
			ret->value.i = (int)dereferenced->value.d;
			ret->t = INT;				
		}
		else return 4;
	}
	return out;
}

enum Type convert(char c){
	if (c=='d') return DOUBLE;
	else if (c=='i') return INT;
	else if (c=='s') return STRING;
	else return EMPTY; 
}

int unaryminusOp(struct Operation *rec, tBSTPtr my_ST){
	int out = 0;
	struct TMPRecord * dereferenced = malloc(sizeof(struct TMPRecord));
	out = dereference(rec, my_ST, 1, dereferenced);
	if (!out){
		if (dereferenced->t==DOUBLE) dereferenced->value.d = -dereferenced->value.d;
		else if (dereferenced->t==INT) dereferenced->value.i = -dereferenced->value.i;
		else return 4;
		if (rec->t_t==VARIABLE){
			BSTFind(my_ST, rec->t.variable);
			if (!BSTActive(my_ST)) return 3;
			out = LSTSet(my_ST, dereferenced);
		} else if (rec->t_t==TMP){
			store_tmp(dereferenced, rec->t.tmp);
		} 
		else return 10;
	}
	return out;
}

int unaryOp(struct Operation *rec, tBSTPtr my_ST, int con){
	int out = 0;
	struct TMPRecord * dereferenced = malloc(sizeof(struct TMPRecord));
	if (rec->t_op1==EMPTY){
		out = dereference(rec, my_ST, 0, dereferenced);
		if (dereferenced->t==DOUBLE) dereferenced->value.d = dereferenced->value.d + con;
		else if (dereferenced->t==INT) dereferenced->value.i = dereferenced->value.i + con;
		else return 4;
		if (rec->t_t==VARIABLE){
			BSTFind(my_ST, rec->t.variable);
			if (!BSTActive(my_ST)) return 3;
			out = LSTSet(my_ST, dereferenced);
		} 
		else return 4; // nebo 6
	}
	else{
		out = dereference(rec, my_ST, 1, dereferenced);
		if (dereferenced->t==DOUBLE) dereferenced->value.d = dereferenced->value.d + con;
		else if (dereferenced->t==INT) dereferenced->value.i = dereferenced->value.i + con;
		else return 4;
		if (rec->t_op1==VARIABLE){
			BSTFind(my_ST, rec->t.variable);
			if (!BSTActive(my_ST)) return 3;
			out = LSTSet(my_ST, dereferenced);
		} 
		else return 4; // nebo 6
		if (rec->t_t==TMP){
			if (dereferenced->t==DOUBLE) dereferenced->value.d = dereferenced->value.d - con;
			else if (dereferenced->t==INT) dereferenced->value.i = dereferenced->value.i - con;
			else return 4;
			store_tmp(dereferenced, rec->t.tmp);
		} else return 10;
	}
	return out;
}

int buildInOp(struct Operation *rec, tBSTPtr my_ST, int op){
	int out = 0;
	struct TMPRecord * target = malloc(sizeof(struct TMPRecord));
	if (op==1){
		//length
		struct TMPRecord * operand1;
		operand1 = working_push[working_push_size-1];
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
		operand3 = working_push[working_push_size-1];
		decrease_push();
		struct TMPRecord * operand2;
		operand2 = working_push[working_push_size-1];
		decrease_push();
		struct TMPRecord * operand1;
		operand1 = working_push[working_push_size-1];
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
		}
		else out = 4;
	}
	else if (op==3){
		//concat
		struct TMPRecord * operand2;
		operand2 = working_push[working_push_size-1];
		decrease_push();
		struct TMPRecord * operand1;
		operand1 = working_push[working_push_size-1];
		decrease_push();
		if ((operand1->t==STRING)&&(operand2->t==STRING)){
			int a = strlen(operand1->value.s);
			int b = strlen(operand2->value.s);
			target->value.s = malloc(a+b+1);
			target->t = STRING;
			strcpy(target->value.s, operand1->value.s);
			strcpy(&target->value.s[a], operand2->value.s);
		}
		else out = 4;
	}
	else if (op==4){
		//find
		struct TMPRecord * operand2;
		operand2 = working_push[working_push_size-1];
		decrease_push();
		struct TMPRecord * operand1;
		operand1 = working_push[working_push_size-1];
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
		operand1 = working_push[working_push_size-1];
		decrease_push();
		if (operand1->t==STRING){
			target->t=STRING;
			int a = strlen(operand1->value.s);
			target->value.s = malloc(a + 1);
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

int funcOp(struct Operation *rec, tBSTPtr my_ST){
	int out = 0;

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
	char * str = rec->t.fce;
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

void decrease_push(){
	working_push_size--;
	working_push = extendPush(working_tmp, working_push_size);
}
