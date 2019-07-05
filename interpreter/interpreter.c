#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>



// The variable data structure;
// The data is stored in char* because our
// variables will not be typed;
typedef struct Tnode Tvariable;

struct Tnode {
char * value;
char * name;
Tvariable *entry; //for array-type variables
Tvariable *next;
};


// Our list head
Tvariable * VarHead=NULL;
//---------------------------------------


// This function verifies if a variable named <name>
// exists in our pseudo-stack. if it is found,
// then the function returns a pointer to it, if not
// it returns NULL

Tvariable * isVar(char * name){

Tvariable * cursor=NULL;

cursor = VarHead;

while (cursor != NULL) {

if (strcmp(cursor->name, name)==0){
return cursor;
}

cursor = cursor->next;
}

return NULL;

}
//-------------------------------------------------



//adds a variable to our pseudo-stack.
//Will be called in every assignment operation.
//If The variable already exists we will only update its value
//in the list. If it doesn't we will check for variable
//declaration rules and assign them accordingly
int addVar(char * name, char  * value){

//printf("Adding variable");

Tvariable * isThere = NULL;
isThere = isVar(name);

if (strcmp(value,"")==0){
// cannot assign or reassign empty values
printf("ERROR - cannot assign or reassign empty values\n");
return -3; // EMPTY_VALUE
}


if (isThere != NULL){
//change value of variable. it is already defined and allocating it
//again would be very dumb;
free(isThere->value);
isThere->value = malloc(strlen(value)+1);
strcpy(isThere->value, value);
return 0; // VARIABLE_UPDATED (future constants)
} else {
// now check for reserved words and illegal assignments

if (name[0]!='@'){
printf("ERROR - missing starter @ for var name. invalid assignment.\n");
return -1; // INVALID_ASSIGNMENT
} else {

Tvariable * newnode = malloc(sizeof(Tvariable));
free(newnode->name);
free(newnode->value);
newnode->name=malloc(strlen(name)+1);
newnode->value=malloc(strlen(value+1));
strcpy(newnode->name,name);
strcpy(newnode->value,value);

newnode->next=VarHead;
VarHead = newnode;

return  1; // VARIABLE_ASSIGNED
}

}

}
//--------------------------------------------------------

// Removes a variable from our pseudo-stack.
// Must be implemented as garbage collector, as we will need no
// dealocation currently. Will problably be needed anyways

bool remVar (char * name) {
// traverse the stack and remove the variable.
// do not forget to mend the list!!!


Tvariable * cursor=NULL;
Tvariable * latecursor=NULL;

cursor = VarHead;
latecursor = VarHead;

while (cursor != NULL) {

if (strcmp(cursor->name, name)==0){

//printf("late: %s \n",latecursor->name);
//printf("current: %s \n", cursor->name);
//printf("next: %s \n", cursor->next->name);
if (cursor==VarHead){
VarHead=cursor->next;
free(cursor);
return true;
} else {
latecursor->next=cursor->next;
free(cursor);
return true;
}

}
latecursor = cursor;
cursor = cursor->next;

}

return false;





}
//---------------------------------------------------------

// this function returns the VALUE as long integer

long intEval(char*name){

Tvariable * isThere = NULL;
isThere = isVar(name);
if (isThere==NULL) {
printf("WARNING - undeclared variable   %s  \n", name);
return 0;
}
return atoi(isThere->value);

}
//---------------------------------------------------------

// this function returns the VALUE as double

double floatEval(char*name){

Tvariable * isThere = NULL;
isThere = isVar(name);
if (isThere==NULL) {
printf("WARNING - undeclared variable   %s  \n", name);
return 0;
}
return atof(isThere->value);

}
//---------------------------------------------------------
// this function returns the VALUE as string

char * strEval(char*name){

Tvariable * isThere = NULL;
isThere = isVar(name);

if (isThere==NULL) {
printf("WARNING - undeclared variable   %s  \n", name);
return "";
}

return isThere->value;

}
//---------------------------------------------------------
//checks kind of given token 
int tokentype(char * tex){

bool hasalpha=false;
bool hasperiod=false;
bool hasnumber=false;

if ((tex[0]=='@')){
//printf("token  %s  is a variable type\n", name);
return 5;
}


if ((tex[0]=='"')&&(tex[strlen(tex)-1]=='"')){
//printf("token  %s  is a string type\n", name);
return 1;
}

if ((tex[0]=='{')&&(tex[strlen(tex)-1]=='}')){
//printf("token   %s  is a complex type\n", name);
return 4;
}

//printf("size   %i  \n", strlen(isThere->value));

int i=0;
while (tex[i]){
if (isalpha(tex[i])) hasalpha=true;
if (isdigit(tex[i])) hasnumber=true;
if (tex[i]=='.') hasperiod=true;
i++;
}

if (!hasalpha && hasnumber && hasperiod){
//printf("token   %s  is a float type\n", name);
return 3;
}
if (!hasalpha && hasnumber && !hasperiod){
//printf("token   %s  is an int type\n", name);
return 2;
}

if (isop(tex)) return 6; //you tried to check type of an operator

if (isctrl(tex)) return 7; //you tried to check type of an operator

return 8;


}
//-----------------------------------------------

// check the kind of the variable
int varkind(char * name) {
// 1 - string, 2-integer, 3-float, 4-complex (complex may be anything);
// 5 - variable;

Tvariable * isThere = NULL;
isThere = isVar(name);
if (isThere==NULL) {
printf("WARNING - undeclared variable   %s  \n", name);
return 0; // why are you checking on an inexistent variable?
}

return tokentype(isThere->value);

}


//---------------------------------------------------------
// typescope (simple helper) 

char * typescope(int val){
switch (val) {

case 1: return "STRING";break;  // open block
case 2: return "INTEGER";break;  // close block
case 3: return "FLOAT";break;  // end sentence
case 4: return "COMPLEX";break;  // start varname
case 5: return "VARIABLE";break;  // start varname
}
return "OOPS"; //not a valid variable;

}


// This function traverses and prints our variable list

void printall(){

Tvariable * cursor=NULL;

cursor = VarHead;

while (cursor != NULL) {

printf("\x1b[0m %s ",typescope(varkind(cursor->name)));
printf( "\x1b[34m %s \x1b[0m", cursor->name);
printf(" @ ");
printf("\x1b[34m %i\x1b[0m", cursor);
printf(" & ");
printf("\x1b[34m %.14s\x1b[0m", cursor->value);
printf(" >> ");
printf("\x1b[34m %i\x1b[0m", cursor->next);
printf("\n");
cursor = cursor->next;
}


}
//-------------------------------------------------


//verify the nature of the operation
int isop(char op[]){
switch (op[0]) {
case '=':
  switch (op[1]) {
  case '=': return 2; break; // == (logical equal)
  default: return 1; break; // = (assignment equal)
  }
break;
case '<':
  switch (op[1]) {
  case '=': return 3; break; // <= (less or equal)
  default: return 4; break; // < (less than)
  }
break;
case '>':
  switch (op[1]) {
  case '=': return 5; break; // >= (above or equal)
  default: return 6; break; // < (above than)
  }
break;
case '!':
  switch (op[1]) {
  case '=': return 7; break; // != (not equal)
  default: return 8; break; // ! (unary not)
  }
break;
case '&':
  switch (op[1]) {
  case '&': return 9; break; // && (logical and)
  default: return 10; break; // & (binary and)
  }
break;
case '|':
  switch (op[1]) {
  case '|': return 11; break; // || (logical or)
  default: return 12; break; // | (binary or)
  }
break;
case '+':
  switch (op[1]) {
  case '+': return 13; break; // ++  (increment)
  default: return 14; break; // + (binary sum)
  }
break;
case '-':
  switch (op[1]) {
  case '-': return 15; break; // --  (decrement)
  default: return 16; break; // - (binary subtraction)
  }
break;
case '*': return 17;  // *  (multiply)
case '/': return 18;  // /  (divide)
case '^': return 19;  // ^  (power)
case '%': return 20;  // %  (modulo)
break;


}
return 0; //not an operator
}
//-------------------------------------------------------

// Execute operations
char * evalop(char op[], char * name1, char * name2){
// 1 - string, 2-integer, 3-float, 4-complex (complex may be anything);
// 5 - variable;
char * result;
char * val1;
char * val2;
int vkd1,vkd2;
int tk1 =0;
int tk2 =0;
Tvariable * isvar;



vkd1 = tokentype(name1);

if (vkd1==6) {
		 printf("ERROR - Cannot operate on UNDEFINED type \n");
		 return "NULL";
}

if (vkd1==5) {
isvar = isVar(name1);
	if (isvar!=NULL) {

		vkd2 = tokentype(isvar->value);
		tk1=vkd2;
		if (vkd2!=4) { val1 = isvar->value;
		} else {
		 printf("ERROR - Cannot operate on complex type \n");
		 return "NULL";
		}
	} else {
		 printf("ERROR - Undeclared Variable %s \n", name1);
		 return "NULL";
	}
} else if (vkd1!=4) {
        tk1=vkd1;
	val1 = name1;
} else {
	printf("ERROR - Cannot operate on complex type \n");
	return "NULL";
}

vkd1 = tokentype(name2);

if (vkd1==6) {
		 printf("ERROR - Cannot operate on UNDEFINED type \n");
		 return "NULL";
}

if (vkd1==5) {
isvar = isVar(name2);
	if (isvar!=NULL) {
		vkd2 = tokentype(isvar->value);
		tk2=vkd2;
		if (vkd2!=4) { val2 = isvar->value;
		} else {
		 printf("ERROR - Cannot operate on complex type \n");
		 return "NULL";
		}
	} else {
		 printf("ERROR - Undeclared Variable %s \n", name2);
		 return "NULL";
	}
} else if (vkd1!=4) {
	val2 = name2;
	tk2 = vkd1;
} else {
	printf("ERROR - Cannot operate on complex type \n");
	return "NULL";
}


if ((tk1!=tk2)&&(tk1!=1)) {
        printf("ERROR - Illegal operation %s %s %s \n", typescope(tk1), op, typescope(tk2));
  printf("%d | %d \n", tk1, tk2);
	return "NULL";
}

// now we finally can evaluate the operations
// 1 - string, 2-integer, 3-float, 4-complex (complex may be anything);
// 5 - variable;

printf("\n%s",val1);
printf(" %s ",op);
printf("%s \n",val2);
 printf("%d | %d \n", tk1, tk2);



switch (tk1){

case 1:

	switch (isop(op)) {

		case 14: // +

			result = malloc(strlen(val1)+strlen(val2)+1);
			strcpy(result, val1);
	 		  if (tk2!=1){
				for (int i=0; i<strlen(val2);i++){
				result[strlen(val1)-1+i] = val2[i];
				}
	    		  result[strlen(val1)+strlen(val2)-1]='"';
	    		  } else {
				for (int i=0; i<strlen(val2);i++){
				result[strlen(val1)-1+i] = val2[i+1];
			  	}
			  }
		break;
		case 2: // ==
			if (strcmp(val1, val2)==0){
			return "true";
			} else {
			return "false";
			}
		break;
		case 3: // <=
			if (strlen(val1)>strlen(val2)){
			return "false";
			} else {
			return "true";
			}
		break;
		case 4: // <
			if (strlen(val1)<strlen(val2)){
			return "true";
			} else {
			return "false";
			}
		break;
		case 5: // >=
			if (strlen(val1)<strlen(val2)){
			return "false";
			} else {
			return "true";
			}
		break;
		case 6: // >
			if (strlen(val1)>strlen(val2)){
			return "true";
			} else {
			return "false";
			}
		break;
		case 7: // !=
			if (strcmp(val1, val2)==0){
			return "false";
			} else {
			return "true";
			}
		break;
		default:
			printf("ERROR - invalid operator %s\n",op);
			return NULL;
		break;
	}


break;


}

/*
switch (isop(op)) {

case 14: 
	if (tk1==1) {
	result = malloc(strlen(val1)+strlen(val2)+1);
	strcpy(result, val1);
	   if (tk2!=1){
		for (int i=0; i<strlen(val2);i++){
		result[strlen(val1)-1+i] = val2[i];
		}
	    result[strlen(val1)+strlen(val2)-1]='"';
	    } else {
		for (int i=0; i<strlen(val2);i++){
		result[strlen(val1)-1+i] = val2[i+1];
		}

	    }
	}
	if (tk1==2) {
	int a = atoi(val1);
	int b = atoi(val2);
	int c = a+b;
	char res[20];
	sprintf(res,"%i",c);
        result = malloc(strlen(res)+1);
	strcpy(result, res);
	}

break;  // open block


}
*/



// ------------------------------------------
printf("size of return : %i ->", strlen(result));
return result;
}



//evaluate the significance of a specific char
int isctrl(char op[]){
switch (op[0]) {

case '{': return 1;break;  // open block
case '}': return 2;break;  // close block
case ';': return 3;break;  // end sentence
case '@': return 4;break;  // start varname
case '(': return 5;break;  // open parenthesis
case ')': return 6;break;  // close parenthesis
case '[': return 7;break;  // open bracket
case ']': return 8;break;  // close bracket
case ',': return 9;break;  // comma separator
case '.': return 10;break; // period separator


}
return 0; //not a control character
}
//-------------------------------------------------------
// evaluate binary expressions
char * bineval(char op[], char varname1[], char varname2[]){
//types: 1=integer, 2=float, 3=string;




}
//-------------------------------------------------------

int main()
{

VarHead = NULL;

//VarHead->next = NULL;
/*
addVar("@INTEGER", "1");
addVar("@STRING", "\"MARCUS : \"");
addVar("@FLOAT", "-111.2");
addVar("@COMPLEX.g", "{function(@B) {@NEWVAR=513; stdout.print(\"@B\");} }");
addVar("@COMPLEX.a", "22");
addVar("@COMPLEX.b", "345");
addVar("@COMPLEX.c", "993");
addVar("@COMPLEX.j", "\"HELP\"");
printall();

varkind("@INTEGER");
varkind("@STRING");
varkind("@FLOAT");
varkind("@COMPLEX");

printf("%s :)\n",evalop("&&", "@STRING", "\"MARCUS : \""));

*/


addVar("@VAR1", "1");
addVar("@COMPLEX", "{ a=1, b=2, 3}");
printall();
addVar("@VAR2", "\"CHICAO\"");
printall();
printf("%s \n",evalop("+", "@VAR2", "@VAR1"));

// "@VAR1 = 22; @VAR2 = @VAR1 + 12; print(@VAR2);
// container {
//     div {
//      
//     event(onhover) { parent.html=server.event("PVAR=22");
//      }
//     }
// }






return 0;
}
