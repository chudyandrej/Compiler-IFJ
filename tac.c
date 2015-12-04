#include "tac.h"

extern struct tBST Func; //globalni tabulka funkci
struct TMPRecord** working_tmp; //tmp promene aktualni funce
struct TMPRecord** working_push;
unsigned int working_size = 0;
unsigned int push_size = 0;

void * extendTmp(void *ptr);
int countingOp(struct Operation *rec, tBSTPtr my_ST);
int assignmentOp(struct Operation *rec, tBSTPtr my_ST);
int initOp(struct Operation *rec, tBSTPtr my_ST, int scope);
int coutOp(struct Operation *rec, tBSTPtr my_ST);
int cinOp(struct Operation *rec, tBSTPtr my_ST);
int dereference(struct Operation *rec, tBSTPtr my_ST, int address_number, struct TMPRecord * dereferenced);
int pushOp(struct Operation *rec, tBSTPtr my_ST);
int to_double(struct TMPRecord * tmp);
int function(char * name, tBSTPtr my_ST, struct TMPRecord * ret);
int condition(struct Operation *rec, tBSTPtr my_ST, int * jump);
void jumpOp(struct Operation *rec, tDLList * my_tac);
int returnOp(struct Operation *rec, tBSTPtr my_ST, struct TMPRecord * ret, char c);
Type convert(char c);
int unaryminusOp(struct Operation *rec, tBSTPtr my_ST);
int unaryOp(struct Operation *rec, tBSTPtr my_ST, int con);
void store_push(struct TMPRecord * tmp);
int funcOp(struct Operation *rec, tBSTPtr my_ST, tDLList * my_tac);
char * get_name(char * names, int  i);

int interpret(){
	if (GSTAllDef(Func.Root) != 1) return 3;
	BSTFind(&Func, "main");
	if (BSTActive(&Func)!=1) return 3;
	working_push = gc_malloc(sizeof(struct TMPRecord *) * push_size);
	struct TMPRecord ret;
	struct tBST ST;
	BSTInit(&ST);
	return function("main", &ST, &ret);
}

int function(char * name, tBSTPtr my_ST, struct TMPRecord * ret){
	int out = 0; // osetreni semantickych chyb
	int scope = 0; //aktualni zanoreni 
	unsigned int tmp_size = 4; //velikost pole pro TMP promene
	struct TMPRecord** TableRecords; // pole pro TMP promene
	TableRecords = gc_malloc(sizeof(struct TMPRecord *) * tmp_size);
	working_tmp = TableRecords; // nastavit globalni na aktualni
	working_size = tmp_size;
	BSTFind(&Func, name);
	char * params = ((struct tFunc *)Func.Act->data)->params;
	tDLList * my_tac = GSTCopyTAC(&Func); //stahnout zasobnik 3AC
	tDLList my_push; // zasobnik na pushovani pro volani funkci
	init_list(&my_push);
	activate_first(my_tac);
	while(is_active(my_tac)){ 
			// cyklit nad vsem 3AC instrukcemi
			// pokud dojde na konec tak chyba neukonceni RETURNem
		struct Operation *rec = copy_active(my_tac);
		TokenKind instruction = rec->inst;
		//printf("inst:%d:\n", instruction);
		if(instruction==KIN_UNARYMINUS){
			out = unaryminusOp(rec, my_ST);
		}
		else if (instruction==KIN_PLUSPLUS){
			out = unaryOp(rec, my_ST, 1);
		}
		else if (instruction==KIN_MINUSMINUS){
			out = unaryOp(rec, my_ST, -1);
		}
		else if((instruction>=3)&&(instruction<=17)){
			out = countingOp(rec, my_ST);
		}
		else if (instruction==SCOPE_UP) scope++;
		else if (instruction==SCOPE_DOWN) {
			LSTLeaveScope(my_ST->Root, scope);
			scope--;
		}
		else if (instruction==KIN_ASSIGNEMENT){
			out = assignmentOp(rec, my_ST);
		}
		else if (instruction==TAC_PUSH){
			out = pushOp(rec, my_ST);
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
			jumpOp(rec, my_tac);
			continue;
		}
		else if (instruction==TAC_GOTO_COND){
			int jump = 0;
			out =  condition(rec, my_ST, &jump);
			if (jump){
				jumpOp(rec, my_tac);
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
				out = funcOp(rec, my_ST, my_tac);
		}
		else {
			return 10; // neznama instrukce
		}
		shift_active(my_tac);
		if (out) {
			//printf("out:%d:, inst:%d:\n", out, instruction);
			return out;
		}
	}

	return 8; // end of non-void function
}


int countingOp(struct Operation *rec, tBSTPtr my_ST){
	int out;
	struct TMPRecord * operand1 = gc_malloc(sizeof(struct TMPRecord));
	struct TMPRecord * operand2 = gc_malloc(sizeof(struct TMPRecord));
	struct TMPRecord * target = gc_malloc(sizeof(struct TMPRecord));
	target->t=DOUBLE;
	out = dereference(rec, my_ST, 1, operand1);
	if (out!=0) return out;
	out = dereference(rec, my_ST, 2, operand2);
	if (out!=0) return out;
	if ((operand1->t == STRING) && (operand2->t == STRING)){
		int comp = strcmp(operand1->value.s, operand2->value.s);
		switch(rec->inst){
			// operace definovane nad STRING
			case KIN_EQ:
				if (comp == 0) target->value.i = 1;
				else target->value.i = 0;
				break;
			case KIN_SMALLER:
				if (comp < 0) target->value.i = 1;
				else target->value.i = 0;
				break;
			case KIN_GREATER:
				if (comp > 0) target->value.i = 1;
				else target->value.i = 0;
				break;
			case KIN_SMALLER_EQ:
				if (comp <= 0) target->value.i = 1;
				else target->value.i = 0;
				break;
			case KIN_GREATER_EQ:
				if (comp >= 0) target->value.i = 1;
				else target->value.i = 0;
				break;
			case KIN_NOT_EQ:
				if (comp != 0) target->value.i = 1;
				else target->value.i = 0;
				break;
			default:
				out = 4;
			}
			target->t=INT;
	}
	else{
		// numericke operace se vzdy pocitaji jako double a nasledne se pretypuji
		out = to_double(operand1);
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
	}
	if ((operand1->t == INT) && (operand2->t == INT)){
		target->value.i = (int)target->value.d;
		target->t = INT;
	}
	if (rec->t_t==VARIABLE){
		BSTFind(my_ST, rec->t.variable);
		if (!BSTActive(my_ST)) return 3;
		out = LSTSet(my_ST, target);
		gc_free(target);
	} 
	else if (rec->t_t==TMP){
		store_tmp(target, rec->t.tmp);
	}
	else return 10;
	gc_free(operand1);
	gc_free(operand2);
	return out;
}


int assignmentOp(struct Operation *rec, tBSTPtr my_ST){
	struct TMPRecord * dereferenced = gc_malloc(sizeof(struct TMPRecord));
	int out = dereference(rec, my_ST, 1, dereferenced);
	if (out!=0) return out;
	if (rec->t_t==VARIABLE){
		BSTFind(my_ST, rec->t.variable);
		if (!BSTActive(my_ST)) return 3;
		out = LSTSet(my_ST, dereferenced);
		gc_free(dereferenced);
	}
	else return 10; //neni duvod volat tuto FCI na TMP promene
	return out;
}

int pushOp(struct Operation *rec, tBSTPtr my_ST){
	int out = 0;
	struct TMPRecord * dereferenced = gc_malloc(sizeof(struct TMPRecord));
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
			tmp = gc_malloc(sizeof(struct TMPRecord));
			BSTFind(my_ST, dereferenced->value.variable);
			if (!BSTActive(my_ST)) return 3;
			out = LSTGet(my_ST, tmp);
			dereferenced->value = tmp->value;
			dereferenced->t = tmp->t;
			gc_free(tmp);
			break;
		default:
			return 10;
	}
	return out;
}

void * extendTmp(void *ptr){
	//working_tmp = extendTmp(working_tmp, &tmp_size);
	working_size = working_size * 2;
	ptr = gc_realloc(ptr, sizeof(struct TMPRecord *) * working_size);
	return ptr;
}

void * extendPush(void *ptr, unsigned int size){
	// zmeni velikost zasobniku na push
	return gc_realloc(ptr, sizeof(struct TMPRecord *) * size);
}

int to_double(struct TMPRecord * tmp){
	if(tmp->t==STRING) return 4;
	else if(tmp->t==INT) {
		tmp->value.d=(double)tmp->value.i;
	}
	return 0;

}

void store_tmp(struct TMPRecord * tmp, int i){
	while (i>=((int)working_size)){
		working_tmp = extendTmp(working_tmp);
	}
	working_tmp[i] = tmp;
}

void store_push(struct TMPRecord * tmp){
	push_size++;
	working_push = extendPush(working_push, push_size);
	working_push[push_size-1] = tmp;
}

int coutOp(struct Operation *rec, tBSTPtr my_ST){
	int out = 0;
	struct TMPRecord * dereferenced = gc_malloc(sizeof(struct TMPRecord));
	out = dereference(rec, my_ST, 0, dereferenced);
	if (!out){
		if (dereferenced->t==STRING){
			printf("%s", dereferenced->value.s); //!!!
		}
		else if (dereferenced->t==INT){
			printf("%d", dereferenced->value.i);
		}
		else if (dereferenced->t==DOUBLE){
			printf("%g", dereferenced->value.d);
		} 
		else out = 10;
	}
	gc_free(dereferenced);
	return out;
}

int cinOp(struct Operation *rec, tBSTPtr my_ST){
	int out = 0;
	if (rec->t_t!=VARIABLE) return 2;
	struct TMPRecord * dereferenced = gc_malloc(sizeof(struct TMPRecord));
	out = dereference(rec, my_ST, 0, dereferenced);
	if (out!=3){
		char * str = gc_malloc(1);
		char * ptr = NULL;
		char c;
		int count = 1;
		while(isspace(c=fgetc(stdin)));

		do {
			str = gc_realloc(str, ++count);
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
	gc_free(dereferenced);
	return out;
}

int condition(struct Operation *rec, tBSTPtr my_ST, int * jump){
	int out = 0;
	struct TMPRecord * dereferenced = gc_malloc(sizeof(struct TMPRecord));
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
	gc_free(dereferenced);
	return out;
}

void jumpOp(struct Operation *rec, tDLList * my_tac){
	unsigned int label = rec->t.label;
	activate_first(my_tac);
	while(is_active(my_tac)){
		struct Operation *rec = copy_active(my_tac);
		if (rec->label == label) return;
		shift_active(my_tac);
	}
}

int returnOp(struct Operation *rec, tBSTPtr my_ST, struct TMPRecord * ret, char c){
	int out = 0;
	struct TMPRecord * dereferenced = gc_malloc(sizeof(struct TMPRecord));
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
	gc_free(dereferenced);
	return out;
}

Type convert(char c){
	if (c=='d') return DOUBLE;
	else if (c=='i') return INT;
	else if (c=='s') return STRING;
	else return EMPTY; 
}

int unaryminusOp(struct Operation *rec, tBSTPtr my_ST){
	int out = 0;
	struct TMPRecord * dereferenced = gc_malloc(sizeof(struct TMPRecord));
	out = dereference(rec, my_ST, 1, dereferenced);
	if (!out){
		if (dereferenced->t==DOUBLE) dereferenced->value.d = -dereferenced->value.d;
		else if (dereferenced->t==INT) dereferenced->value.i = -dereferenced->value.i;
		else return 4;
		if (rec->t_t==VARIABLE){
			BSTFind(my_ST, rec->t.variable);
			if (!BSTActive(my_ST)) return 3;
			out = LSTSet(my_ST, dereferenced);
			gc_free(dereferenced);
		} else if (rec->t_t==TMP){
			store_tmp(dereferenced, rec->t.tmp);
		} 
		else return 10;
	}
	return out;
}

int unaryOp(struct Operation *rec, tBSTPtr my_ST, int con){
	int out = 0;
	struct TMPRecord * dereferenced = gc_malloc(sizeof(struct TMPRecord));
	if (rec->t_op1==EMPTY){
		out = dereference(rec, my_ST, 0, dereferenced);
		if (dereferenced->t==DOUBLE) dereferenced->value.d = dereferenced->value.d + con;
		else if (dereferenced->t==INT) dereferenced->value.i = dereferenced->value.i + con;
		else return 4;
		if (rec->t_t==VARIABLE){
			BSTFind(my_ST, rec->t.variable);
			if (!BSTActive(my_ST)) return 3;
			out = LSTSet(my_ST, dereferenced);
			gc_free(dereferenced);
		} 
		else return 4; // nebo 6
	}
	else{
		out = dereference(rec, my_ST, 1, dereferenced);
		if (out) return 3;
		if (dereferenced->t==DOUBLE) dereferenced->value.d = dereferenced->value.d + con;
		else if (dereferenced->t==INT) dereferenced->value.i = dereferenced->value.i + con;
		else return 4;
		if (rec->t_op1==VARIABLE){
			BSTFind(my_ST, rec->op1.variable);
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

int funcOp(struct Operation *rec, tBSTPtr my_ST, tDLList * my_tac){
	int out = 0;
	char * fce_name = rec->op1.fce;
	BSTFind(&Func, fce_name);
	if (!BSTActive(&Func)) return 10;
	char * names = ((struct tFunc *)Func.Act->data)->names;
	char * params = ((struct tFunc *)Func.Act->data)->params;
	struct TMPRecord * ret = gc_malloc(sizeof(struct TMPRecord)); //navratova hodnota fce
	struct tBST ST; // vyvori Symbol Table pro volanou funkci
	BSTInit(&ST);
	for (int i = strlen(params)-1; i>=1; i--){ //naplni strom paramery
		Type parameter = convert(params[i]);
		char * name = get_name(names, i);
		struct TMPRecord * operand;
		operand = working_push[push_size-1];
		decrease_push();
		if ((operand->t!=parameter)&&((operand->t==STRING)||(parameter==STRING))){
			return 4;
		}
		BSTFind(&ST, name);
		if (BSTActive(&ST)) return 6;
		BSTAdd(&ST, name);
		LSTAdd(&ST, parameter, 0);
		LSTSet(&ST, operand);
	}

	struct TMPRecord** BU_working_tmp = working_tmp; //tmp promene aktualni funce
	unsigned int BU_working_size = working_size;
	tDLElemPtr my_active = my_tac->Active; //zapamovavat pozici v 3AC
	out = function(fce_name, &ST, ret);
	my_tac->Active = my_active; //nahrat pozici v 3AC
	LSTDispose(&ST);
	if (out) return out;
	working_tmp = BU_working_tmp;
	working_size = BU_working_size;
	if (rec->t_t==VARIABLE){
		BSTFind(my_ST, rec->t.variable);
		if (!BSTActive(my_ST)) return 3;
		out = LSTSet(my_ST, ret);
	} 
	else if (rec->t_t==TMP){
		store_tmp(ret, rec->t.tmp);
	}
	return out;
}


void decrease_push(){
	push_size--;
	working_push = extendPush(working_push, push_size);
}

char * get_name(char * names, int  i){
	int counter = 0;
	int length = strlen(names);
	int begin = 0;
	int end = 0;
	for (int j = 0; j<length; j++){
		if (names[j]==' ') {
			counter++;
			if (counter==i) end = j;
			if (counter==i-1) begin = j+1;
		}
	}
	if (counter<i) end = length;
	length = end - begin + 1;
	char * name = gc_malloc(sizeof(char)*length);
	memcpy(name, &names[begin], length-1);
	name[length-1]='\0';
	return name;
}