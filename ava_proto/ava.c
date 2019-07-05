/*
Program: ava.c
written by: Marcus Vinícius Martins Meneses
date: 20/09/2015
description:
AVA is a subset of the plain interpreted language;
it is reduced in complexity and features so it can be run in
microcontrollers for instance.
the built-in API of version 0.1 is for AVR atmega328.
the API 0.1 simulates a virtual microcontroller
*/


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>



#define DIGIT     	     1
#define PERIOD               2
#define ALPHA                3
#define UNDERSCORE           4


#define LINE_BREAK           4
#define BLANK_SPACE          3

#define TOKEN_EOF            -1   // END OF FILE    \0
#define TOKEN_DEF            -2   // DEFINITION     function, var
#define TOKEN_IDF            -3   // IDENTIFIER     variablename, functionname, mumbojumbo
#define TOKEN_BHD            -4   // BLOCK HEADERS  for, while, if, else, do
#define TOKEN_NBR            -5   // NUMBER {[0..9]}[.]{[0..9]}
#define TOKEN_SPR            -6   // SEPARATOR  {, }, whitespace, (, ), [, ], :, ;,  comma, ", '
#define TOKEN_MOP            -7   // MATHEMATICAL OPERATOR +, -, *, /, ^, % 
#define TOKEN_LOP            -8   // LOGICAL OPERATOR >, <, =, &, |, ! 
#define TOKEN_BIF            -9   // BUILT-IN FUNCTION 
#define TOKEN_STR            -10  // STRINGS (yeah!) 
#define TOKEN_PMT            -11  // primitives : true, false, null, nan, void
#define TOKEN_PSO            -12  // pseudo Objects (built in objects that offer specific functions)
#define TOKEN_DFF	     -13  // programmer-defined function
#define TOKEN_VAR	     -14  // programmer-defined variables (plus supervars);

// The variable data structure;
// The data is stored in char* because our
// variables will not be typed;
typedef char EVAL_CHAR;

typedef struct Tnode Tvariable;

struct Tnode {
char * value;
char * name;
Tvariable *entry; //for array-type variables && function arguments
Tvariable *next;
};
//-------------------------------------------------------------

// TOKEN ELEMENT STRUCTURES:
typedef struct Tokele tokenElement;
struct Tokele {
int  type;        	//type of the token returned
int internal;     	// the internal value of the token (for speedup purposes)
char * value;     	//value of the grabbed token
char * dereference;     //value of the grabbed VARIABLE/function token
long start;      	//where did we begin chewing? THIS IS A NICE APPROACH, DO NOT MESS THIS TIME!
};
//-------------------------------------------------------------

bool debug=false;
Tvariable * VarHead=NULL;    // head variable
Tvariable * FunHead=NULL;   // head function
Tvariable * StaHead=NULL;   // head STACK;
char * programText=NULL;     // our program resides here
long instructionPointer=0;   // our virtual instruction pointer

//all the function prototypes:
Tvariable * isVar(char * name);
int addVar(char * name, char  * value);
bool remVar (char * name);
Tvariable * isFun(char * name);
int addFun(char * name, char  * value);
bool remFun (char * name);
long intEval(char*name);
double floatEval(char*name);
char * strEval(char*name);
int tokentype(char * tex);
int varkind(char * name);
char * typescope(int val);
int isNBR(char ev);
int isALP(char ev);
int isALN(char ev);
int isSPR(char ev);
int isMOP(char ev);
int isLOP(char ev);
char chew();
char peek();
bool getToken(tokenElement * returns);
void load(char*loa);
bool getpopen( tokenElement * returns, char* command);
void lexer();
void throwErr(int error, char* mesg, char* mesg2);

bool define();
bool express(tokenElement * returns, char ender);





// FUNCTION HANDLING ----------------------------------------------------------------------
//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------




// This function verifies if a function named <name>
// exists in our pseudo-stack. if it is found,
// then the function returns a pointer to it, if not
// it returns NULL

Tvariable * isFun(char * name){

Tvariable * cursor=NULL;

cursor = FunHead;

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
int addFun(char * name, char  * value){

//printf("Adding variable");

Tvariable * isThere = NULL;
isThere = isFun(name);


if (name[strlen(name)-1]=='.') {
throwErr(100, "invalid function name", name);
return -1;
}

if (strcmp(value,"")==0){
// cannot assign or reassign empty values
//printf("ERROR - cannot assign or reassign empty values\n");
//return -3; // EMPTY_VALUE
value=malloc(5*sizeof(char));
value[0]='v';
value[1]='o';
value[2]='i';
value[3]='d';
value[4]=0;
}


if (isThere != NULL){
//change value of variable. it is already defined and allocating it
//again would be very dumb;
//printf("freeing %i\n",isThere->value);
free(isThere->value);
//printf("freed %i\n",isThere->value);
isThere->value = malloc(strlen(value)+2);
//printf("allocated size: %i\n",strlen(value)+2);
strcpy(isThere->value, value);
return 0; // VARIABLE_UPDATED (future constants)
} else {
// now check for reserved words and illegal assignments

if (!isALP(name[0])){
//printf("ERROR - invalid var name.\n");
throwErr(101, "invalid function name", name);
return -1; // INVALID_ASSIGNMENT
}  

for (int tras=0; tras<strlen(name); tras++){

if (!isALN(name[tras])){
//printf("ERROR - invalid var name.\n");
throwErr(101, "invalid function name", name);
return -1; // INVALID_ASSIGNMENT
}  

}



Tvariable * newnode = malloc(sizeof(Tvariable));
//free(newnode->name);
//free(newnode->value);
//printf("allocated size: %i\n",strlen(name)+2);
newnode->name=malloc(strlen(name)+2);
newnode->value=malloc(strlen(value)+2);
strcpy(newnode->name,name);
strcpy(newnode->value,value);
newnode->next=FunHead;
FunHead = newnode;

return  1; // FUnCTION_ASSIGNED
 

}

}
//--------------------------------------------------------

// Removes a variable from our pseudo-stack.
// Must be implemented as garbage collector, as we will need no
// dealocation currently. Will problably be needed anyways
 
bool remFun (char * name) {
// traverse the stack and remove the variable.
// do not forget to mend the list!!!


Tvariable * cursor=NULL;
Tvariable * latecursor=NULL;

cursor = FunHead;
latecursor = FunHead;

while (cursor != NULL) {

if (strcmp(cursor->name, name)==0){

//printf("late: %s \n",latecursor->name);
//printf("current: %s \n", cursor->name);
//printf("next: %s \n", cursor->next->name);
if (cursor==FunHead){
FunHead=cursor->next;
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


// VARIABLE HANDLING ----------------------------------------------------------------------
//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------




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

Tvariable * isThere2 = NULL;
isThere2 = isFun(name);

if (isThere2 != NULL){
//cannot create variable homonym to function
//should raise error here!
// but i don't know how to arbitrary exit program
throwErr(102, "cannot create variable homonym to function", name);
return -1;
}

if (name[strlen(name)-1]=='.') {
throwErr(102, "invalid variable name", name);
return -1;
}

if (strcmp(value,"")==0){
// cannot assign or reassign empty values
//printf("ERROR - cannot assign or reassign empty values\n");
//return -3; // EMPTY_VALUE
value=malloc(5*sizeof(char));
value[0]='v';
value[1]='o';
value[2]='i';
value[3]='d';
value[4]=0;
}


if (isThere != NULL){
//change value of variable. it is already defined and allocating it
//again would be very dumb;
//printf("freeing %i\n",isThere->value);
free(isThere->value);
//printf("freed %i\n",isThere->value);
isThere->value = malloc(strlen(value)+2);
//printf("allocated size: %i\n",strlen(value)+2);
strcpy(isThere->value, value);
return 0; // VARIABLE_UPDATED (future constants)
} else {
// now check for reserved words and illegal assignments

if (!isALP(name[0])){
//printf("ERROR - invalid var name.\n");
throwErr(101, "invalid variable name", name);
return -1; // INVALID_ASSIGNMENT
}  

for (int tras=0; tras<strlen(name); tras++){

if (!isALN(name[tras])){
//printf("ERROR - invalid var name.\n");
throwErr(101, "invalid variable name", name);
return -1; // INVALID_ASSIGNMENT
}  

}



Tvariable * newnode = malloc(sizeof(Tvariable));
//free(newnode->name);
//free(newnode->value);
//printf("allocated size: %i\n",strlen(name)+2);
newnode->name=malloc(strlen(name)+2);
newnode->value=malloc(strlen(value)+2);
strcpy(newnode->name,name);
strcpy(newnode->value,value);
newnode->next=VarHead;
VarHead = newnode;

return  1; // VARIABLE_ASSIGNED
 

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

//if ((tex[0]=='@')){
//printf("token  %s  is a variable type\n", name);
//return 5;
//}


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

//if (isop(tex)) return 6; //you tried to check type of an operator

//if (isctrl(tex)) return 7; //you tried to check type of an operator

return 5; //WILD VARIABLE ON THE LOOSE!


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

case 1: return "STRING";break;   // this variable begins and ends with "
case 2: return "INTEGER";break;  // this is a numeric variable
case 3: return "FLOAT";break;    // this is a float-point type variable
case 4: return "COMPLEX";break;  // this is a complex-type variable (an """object""")
case 5: return "VARIABLE";break; // a primitive variable type. untyped (but boolean by nature)
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
printf("\x1b[34m %s\x1b[0m", cursor->value);
printf(" >> ");
printf("\x1b[34m %i\x1b[0m", cursor->next);
printf("\n");
cursor = cursor->next;
}


cursor = FunHead;

while (cursor != NULL) {

printf("\x1b[0m FUNCTION ");
printf( "\x1b[34m %s \x1b[0m", cursor->name);
printf(" @ ");
printf("\x1b[34m %i\x1b[0m", cursor);
printf(" & ");
printf("\x1b[34m %s\x1b[0m", cursor->value);
printf(" >> ");
printf("\x1b[34m %i\x1b[0m", cursor->next);
printf("\n");
cursor = cursor->next;
}



}
//-------------------------------------------------
//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------


//LANGUAGE HANDLING -----------------------------------------------------------------------
//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------


//single char definitions:

 
int isNBR(char ev) {
	if (isdigit(ev)) {
		return DIGIT;		
	} else if (ev=='.') {
		return PERIOD;
	} else {
		return 0;
	}
}
 
int isALP(char ev){
	if (isalpha(ev)) {	
		return ALPHA;
	} else if (ev=='_') {	
		return ALPHA;
	} else {
		return 0;
	}
}
 
int isALN(char ev){
	if (isalpha(ev)) {	
		return ALPHA;
	} else if (isNBR(ev)) {	
		return DIGIT;
	} else if (ev=='_') {	
		return UNDERSCORE;
	} else {
		return 0;
	}
}


//SEPARATOR DEFINITIONS



 
int isSPR(char ev) {
//SEPARATOR  {, }, whitespace, linebreak, (, ), [, ], :, ;,  comma
switch (ev){

case '{': return 1;break;
case '}': return 2;break;
case 32: return BLANK_SPACE;break;
case 9: return BLANK_SPACE;break;
case 11: return LINE_BREAK;break;
case 10: return LINE_BREAK;break;
case 13: return LINE_BREAK;break;
case 15: return LINE_BREAK;break;
case '(': return 5;break;
case ')': return 6;break;
case '[': return 7;break;
case ']': return 8;break;
case ':': return 9;break;
case ';': return 10;break;
case ',': return 11;break;
case '"': return 12;break;
case '\\': return 13;break;
case '\'': return 14;break;
case '#': return 15;break; //asshole
case '.': return 16;break;  
case '@': return 17;break;  

default:break;
}
return 0;
}
 
int isMOP(char ev) {
//MATHEMATICAL OPERATOR +, -, *, /, ^, % 
switch (ev){

case '+': return 1;break;
case '-': return 2;break;
case '*': return 3;break;
case '/': return 4;break;
case '^': return 5;break;
case '%': return 6;break;

default:break;
}
return 0;
}
 
int isLOP(char ev) {
//LOGICAL OPERATOR >, <, =, &, |, ! 
switch (ev){

case '>': return 1;break;
case '<': return 2;break;
case '=': return 3;break;
case '&': return 3;break;
case '|': return 4;break;
case '!': return 5;break;
case '~': return 6;break;

default:break;
}
return 0;
}


//CHEWS NEXT CHARACTER FROM THE STREAM -------------------------
//well, not a stream, in fact it is only a memory place with all
//the code put there (but it works, so NO streams)--------------
//-------------------------------------------------------------- 
char chew(){
//printf("\nchewing...");
char ret;
ret = programText[instructionPointer];
//printf("\nchewing: %.1s",&ret);
instructionPointer++;
return ret;
}
 
char peek() {
char ret;
ret = programText[instructionPointer];
//printf("\nchewing: %.1s",&ret);
//instructionPointer++;
return ret;
}
 
bool getToken(tokenElement * returns){
if(debug) printf("\ntokenizing");
char token[512]; // we will limit the size of the token to 512, resize it as needed (or not).
//printf("setting begin\n");
returns->start = instructionPointer; // this is to inform the lexer where we got this token from.
//printf("set begin\n");
returns->value=NULL; // SAFE(?) pointer
char lastchar;
int locursor=0;

retry: 
locursor=0;
lastchar=chew();
while ( (isSPR(lastchar)==BLANK_SPACE ) || (isSPR(lastchar)== LINE_BREAK ) ) { 


//printf("\nignored: %.1s",&lastchar);
lastchar = chew();
}


// evaluate IDENTIFIERS --------------------------------------------------------------------------
if (isALP(lastchar)) { // identifier: [a-zA-Z][a-zA-Z0-9]*  
if(debug) printf(" identifier ");
locursor=0;
token[locursor] = lastchar;  
//printf("\nconsumed: %.1s",&lastchar);
while (isALN((lastchar = chew()))) {
//printf("|%.1s",&lastchar);
 locursor++;
 token[locursor] = lastchar; 
	if (lastchar=='.') {instructionPointer++;break;}
}
 token[locursor+1] = 0;
//printf("\n");
instructionPointer--;
//if (returns.value!=NULL) free(returns.value);
returns->value=malloc(strlen(token)+2);
int i=0;
for (i=0;i<=strlen(token);i++) {
returns->value[i]=token[i];
//printf("\nadding: %.1s",&token[i]);
}
//i++;
//returns.value[i]=0;


//feeble optimization. would be better to check
// strlen(token) and classify the test based on the token size
switch (token[0]){
case 'f':
   if ( !strcmp(token,       "function")  )  {returns->type = TOKEN_DEF;returns->internal = 1;return true;}  
   else if (!strcmp(token,   "for")   )      {returns->type = TOKEN_BHD;returns->internal = 15;return true;} 
   else if ( !strcmp(token,  "foreach") )    {returns->type = TOKEN_BHD;returns->internal = 17;return true;}
   else if ( !strcmp(token,  "false")  )     {returns->type = TOKEN_PMT;returns->internal = 151;return true;}
   else if ( !strcmp(token,  "file")  )      {returns->type = TOKEN_DEF;returns->internal = 6;return true;}
   else if ( !strcmp(token,  "file.")  )    {returns->type = TOKEN_PSO;returns->internal = 134;return true;}  
break;
case 'e':
   if ( !strcmp(token,       "else")  )      {returns->type = TOKEN_BHD;returns->internal = 11;return true;}
   else if ( !strcmp(token,  "event")  )     {returns->type = TOKEN_DEF;returns->internal = 5;return true;} 
   else if ( !strcmp(token,  "exit")  )      {returns->type = TOKEN_BIF;returns->internal = 118;return true;}   
   else if ( !strcmp(token,  "eval")  )      {returns->type = TOKEN_BIF;returns->internal = 122;return true;}
break;
case 'd': 
   if ( !strcmp(token,       "do")  )        {returns->type = TOKEN_BHD;returns->internal = 12;return true;}
   else if ( !strcmp(token,  "date")  )      {returns->type = TOKEN_BIF;returns->internal = 124;return true;}  
   else if ( !strcmp(token,  "database")  ) {returns->type = TOKEN_BIF;returns->internal = 129;return true;}
   else if ( !strcmp(token,  "database.")  ) {returns->type = TOKEN_PSO;returns->internal = 135;return true;}  
   else if ( !strcmp(token,  "default")  )  {returns->type = TOKEN_BHD;returns->internal = 20;return true;}
   else if ( !strcmp(token,  "datamodel")  )  {returns->type = TOKEN_DEF;returns->internal = 7;return true;}

break;
case 'i':
    if ( !strcmp(token,      "if")  )        {returns->type = TOKEN_BHD;returns->internal = 10;return true;}
    else if ( !strcmp(token, "import") )     {returns->type = TOKEN_BIF;returns->internal = 102;return true;}  
   // else if ( !strcmp(token, "include") )    {returns->type = TOKEN_BIF;return true;}
   // else if ( !strcmp(token, "int")  )       {returns->type = TOKEN_DEF;return true;}
break;
case 'v':
   if ( !strcmp(token,       "var") )        {returns->type = TOKEN_DEF;returns->internal = 4;return true;}
   else if ( !strcmp(token,  "void")  )      {returns->type = TOKEN_PMT;returns->internal = 155;return true;} 
   else if ( !strcmp(token,  "variable")  )  {returns->type = TOKEN_DEF;returns->internal = 4;return true;} 
break;
case 'm':
 //   if ( !strcmp(token,      "main")  )   {returns->type = TOKEN_BIF;returns->internal = 100;return true;}
  // else
    if ( !strcmp(token,  "method")  ) {returns->type = TOKEN_DEF;returns->internal = 2;return true;}  
   else if ( !strcmp(token,  "math.")  ) {returns->type = TOKEN_PSO;returns->internal = 130;return true;}
 break;
case 'r':
   if ( !strcmp(token,       "read")  )      {returns->type = TOKEN_BIF;returns->internal = 127;return true;} 
   else if ( !strcmp(token,  "return")  )    {returns->type = TOKEN_BIF;returns->internal = 101;return true;} 
break;
case 'n':
   if ( !strcmp(token,       "null")  )      {returns->type = TOKEN_PMT;returns->internal = 152;return true;} 
  else if ( !strcmp(token,   "nan")  )       {returns->type = TOKEN_PMT;returns->internal = 153;return true;} 
  else if ( !strcmp(token,   "nop")  )       {returns->type = TOKEN_PMT;returns->internal = 154;return true;} 
break;
case 't':
       if ( !strcmp(token,   "throw")  )     {returns->type = TOKEN_BIF;returns->internal = 121;return true;} 
  else if ( !strcmp(token,   "true")  )      {returns->type = TOKEN_PMT;returns->internal = 150;return true;}
break;
case 'h':
        if ( !strcmp(token,   "halt")  )      {returns->type = TOKEN_BIF;returns->internal = 119;return true;} 
   else if ( !strcmp(token,   "hash.")  ) {returns->type = TOKEN_PSO;returns->internal = 133;return true;}  
break;
//case 'a':
 
//break;
case 'p':
       if ( !strcmp(token,   "print")  )     {returns->type = TOKEN_BIF;returns->internal = 117;return true;} 
  else if ( !strcmp(token,   "push")  )      {returns->type = TOKEN_BIF;returns->internal = 104;return true;} 
  else if ( !strcmp(token,   "pop")  )       {returns->type = TOKEN_BIF;returns->internal = 105;return true;} 
break;
case 's':
       if ( !strcmp(token,   "switch")  )     {returns->type = TOKEN_BHD;returns->internal = 18;return true;}
  else if ( !strcmp(token,   "syscall")  )    {returns->type = TOKEN_BIF;returns->internal = 103;return true;}
   else if ( !strcmp(token,  "string.")  ) {returns->type = TOKEN_PSO;returns->internal = 131;return true;} 
  //else if ( !strcmp(token,   "struct") )      {returns->type = TOKEN_DEF;return true;} 
break;
case 'c':
      
   if ( !strcmp(token,   "case")  )      {returns->type = TOKEN_BHD;returns->internal = 19;return true;}
  //else if ( !strcmp(token,   "char")  )      {returns->type = TOKEN_DEF;return true;}  
break;
default:



       if ( !strcmp(token,   "while")  )     {returns->type = TOKEN_BHD;returns->internal = 13;return true;}  
  else if ( !strcmp(token,   "until")  )     {returns->type = TOKEN_BHD;returns->internal = 14;return true;} 
  else if ( !strcmp(token,   "jump")  )      {returns->type = TOKEN_BIF;returns->internal = 106;return true;} 
  else if ( !strcmp(token,   "break")  )     {returns->type = TOKEN_BHD;returns->internal = 21;return true;} 
  else if ( !strcmp(token,   "_eval")  )      {returns->type = TOKEN_BIF;returns->internal = 123;return true;}
  else if ( !strcmp(token,   "array.")  )   {returns->type = TOKEN_PSO;returns->internal = 132;return true;}
  
break;
}

if (token[strlen(token)-1]=='.') {
  returns->internal = 264;//unknown identifier?
  returns->type = TOKEN_IDF; return true;
}

// the variable evaluator from the first lexer comes HERE
Tvariable * isThere = NULL;
isThere = isVar(token);

if (isThere!=NULL) {
if(debug) printf("\n	is variable!");
returns->type = TOKEN_VAR;returns->internal = isThere;
returns->dereference = malloc(strlen(isThere->value)+2);
int gcur=0;
for (gcur=0;gcur<strlen(isThere->value);gcur++){
returns->dereference[gcur]=isThere->value[gcur];
}
returns->dereference[gcur]=0;
return true;
}


// ------------------------------------------------------
// the function evaluator comes HERE

// ------------------------------------------------------

//printf("\n identifier : %s \n",&token);
  returns->internal = 264;//unknown identifier?
  returns->type = TOKEN_IDF; return true;


} else if(isNBR(lastchar)==DIGIT){ // we will not accept floats like  .23 
//int countperiod=0;
token[locursor] = lastchar; 
//printf("\n");
//printf("\nconsumed: %.1s",&lastchar);
while (isNBR((lastchar = chew()))) {
//printf("|%.1s",&lastchar);
 locursor++;
 token[locursor] = lastchar; 

//if (peek()=='.') countperiod++;
//if (countperiod==2) {instructionPointer--;goto retry;}
}
 token[locursor+1] = 0;

instructionPointer--;
//if (returns.value!=NULL) free(returns.value);
returns->value=malloc(strlen(token)+2);
int countperiod=0;
int i=0;
for (i=0;i<=strlen(token);i++) {
returns->value[i]=token[i];
if (token[i]=='.') countperiod++;
//printf("\nadding: %.1s",&token[i]);
}
//printf("\n identifier : %s \n",&token);

  returns->internal = 264;//unknown identifier ?
  returns->type = TOKEN_IDF;
if (countperiod<2) {
returns->type = TOKEN_NBR;
  returns->internal = 262;//number
}

return true;

} else if(isSPR(lastchar)){ // handling separators, separators are 1-digit wide;
// charstring special case : ---------------------
if (isSPR(lastchar)==14) { 	//printf("trying recovery from"); '
long tempoint = instructionPointer-1;
lastchar=chew();

if ((isSPR(lastchar)==13)){
//printf("dipshit");
//lastchar=chew();
while ((isSPR(peek())==14)){
//printf("dipshit");
lastchar=chew();
}
}
instructionPointer--;
lastchar=chew();
    while ( (isSPR(lastchar)!=14) && lastchar!=0 ) { //printf("%.1s",&lastchar);
	lastchar = chew();//printf("%.1s",&lastchar);
    }
if (lastchar == 0) { //read to the end, no glory
 goto retry; //it will fail badly
} else {
returns->value=malloc(instructionPointer-tempoint+2);
//instructionPointer=tempoint;
int i=0;
int j=0;
for (i=tempoint;i<instructionPointer;i++){
returns->value[j]=programText[i];
j++;
}
//instructionPointer++;instructionPointer++;
returns->value[j]=0;
returns->internal = 265;//charstring constant
returns->type = TOKEN_STR; return true;

}


}




// ------------------------------------------------
//string special case: ----------------------------
if (isSPR(lastchar)==12) {
long tempoint = instructionPointer-1;
lastchar=chew();

    while ( (lastchar!='"') && lastchar!= 0 ) { //printf("%.1s",&lastchar);
	lastchar = chew();
	
    }
if (lastchar == 0) { //read to the end, no glory
 goto retry; //it will fail badly
} else {
returns->value=malloc(instructionPointer-tempoint+2);
//instructionPointer=tempoint;
int i=0;
int j=0;
for (i=tempoint;i<instructionPointer;i++){
returns->value[j]=programText[i];
j++;
}
//instructionPointer++;instructionPointer++;
returns->value[j]=0;
returns->internal = 263;//string constant
returns->type = TOKEN_STR; return true;

}


}


//--------------------------------
token[0] = lastchar; 
token[1] = 0;
//if (returns.value!=NULL) free(returns.value);
//printf("\n separator : %s \n",&token);
returns->value=malloc(strlen(token)+2);

int i=0;
for (i=0;i<strlen(token);i++) {
returns->value[i]=token[i];
}
returns->value[i]=0;
//printf("\n separator : %s \n",&token);

//evaluate separator here and set returns->internal accordingly.
switch (token[0]){

case '{': returns->internal = 240; break;
case '}': returns->internal = 241; break;
case '(': returns->internal = 242; break;
case ')': returns->internal = 243; break;
case '[': returns->internal = 244; break;
case ']': returns->internal = 245; break;
case ';': returns->internal = 246; break;
case ':': returns->internal = 247; break;
case '#': returns->internal = 248; break;
case ',': returns->internal = 249; break;
case '.': returns->internal = 250; break;
// the following seps are most probably eaten by lexer-----
// and must not display even in debug.
// they are here for development purposes only.
case 32: returns->internal = 251; break;
case 9: returns->internal = 252; break;
case 11: returns->internal = 253; break;
case 10: returns->internal = 254; break;
case 13: returns->internal = 255; break;
case 15: returns->internal = 256; break;
// ----------------------------------------end of bestiary
case '\"': returns->internal = 257; break;
case '\'': returns->internal = 258; break;
case '\\': returns->internal = 259; break;
case '@': returns->internal = 260; break;
default :  returns->internal = 0; break;

}


//returns->internal = 0;//null return
returns->type = TOKEN_SPR;
return true;

} else if(isLOP(lastchar)){ // handling logic operators. As we didn't define them all, we will leava an unknown size of its combinations
locursor=0;
token[locursor] = lastchar; 
while (isLOP((lastchar = chew()))) {
 locursor++;
 token[locursor] = lastchar; 
 
} 
token[locursor+1] = 0;
instructionPointer--;


//if (returns.value!=NULL) free(returns.value);
returns->value=malloc(strlen(token)+2);
int countperiod=0;
int i=0;
for (i=0;i<=strlen(token);i++) {
returns->value[i]=token[i];
}
returns->value[i+1]=0;
// evaluate operator type here: -------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------
//--------------------------------------------------------------------
//--------------------------------------------------------------------
switch (token[0]) {
case '=': returns->internal = 215; if(token[1]=='=') returns->internal = 200; break;
case '!': returns->internal = 208; if(token[1]=='=') returns->internal = 201; break;
case '>': returns->internal = 202; 
	if(token[1]=='='){ returns->internal = 203; 
	} else if(token[1]=='>'){ returns->internal = 217; }
break;
case '<': returns->internal = 204;
	if(token[1]=='='){ returns->internal = 205; 
	} else if(token[1]=='<'){ returns->internal = 216; }
	if (i>=2){
	if ((token[1]=='~')&&(token[2]=='>')) returns->internal = 211;
	}
break;
case '&': returns->internal = 212; 
	if(token[1]=='&') { returns->internal = 206; 
	} else if(token[1]=='|') { returns->internal = 209; }
break;
case '|': returns->internal = 213; if(token[1]=='|') returns->internal = 207; break;
case '~': returns->internal = 214; if(token[1]=='>') returns->internal = 210; break;

default :  returns->internal = 0; break;
}

//------------------------------------------------------------------

//evaluate operator here and set returns->internal accordingly.
//returns->internal = 0;//null return
returns->type = TOKEN_LOP;

return true;

} else if(isMOP(lastchar)){ // handling mathematical operators. All the opeartors in this class are 1 char wide

// handling comments:
if(lastchar=='/'){ // handling comments
 if (peek() == '/') { // single line comments;
  //printf("\n ignoring:");
  while (lastchar!= 0 && lastchar!='\n' && lastchar!='\r') { 
	//printf("%.1s",&lastchar); 
   lastchar = chew();
   }
  if (lastchar!=0) {
  goto retry;
  }
  //if (lastchar != 0)
 } else if (peek() == '*') { // multi line comments
 lastchar = chew();lastchar = chew();
 //printf("\n multi ignoring:");printf("%.1s - ",&lastchar); printf("%i",lastchar);
    //rew:
   while ( !(lastchar=='*' && peek()=='/') && lastchar!= 0) { //printf("%.1s",&lastchar); 
   lastchar = chew();
   } //printf("\nfound:");printf("%.1s - ",&lastchar);

 lastchar = chew();lastchar = chew();
  if (lastchar!=0) { 
     //printf("\n reaweken in : %.1s",&lastchar);
  goto retry;
  }
 }
}


//-------------------


locursor=0;
token[locursor] = lastchar; 
token[locursor+1] = 0; 
//printf("\n separator : %s \n",token);

//if (returns.value!=NULL) free(returns.value);
returns->value=malloc(strlen(token)+2);
int countperiod=0;
int i=0;
for (i=0;i<=strlen(token);i++) {
returns->value[i]=token[i];

}
returns->value[i+1]=0;




switch (token[0]){

case '+': returns->internal = 220; break;
case '-': returns->internal = 221; break; //how to handle -20, -30, etc?
case '/': returns->internal = 222; break;
case '*': returns->internal = 223; break;
case '^': returns->internal = 224; break;
case '%': returns->internal = 225; break;
default :  returns->internal = 0; break;

}

//evaluate operator here and set returns->internal accordingly.
returns->type = TOKEN_MOP;

return true;

} else if (lastchar != 0) {// handling every other occurence
token[0] = lastchar;
token[1] = 0;
//if (returns.value!=NULL) free(returns.value);
returns->value=malloc(strlen(token)+2);
int i=0;
for (i=0;i<=strlen(token);i++) {
returns->value[i]=token[i]; 
}
//printf("\nadding: %i", token[0]);
//instructionPointer++;
returns->value[i+1]=0;
returns->type = TOKEN_IDF;
return true;

}


if (lastchar == 0) { // handling END OF INPUT

returns->type = TOKEN_EOF;
return false;

}

/*
if (LastChar == '#') {  // Comment until end of line.  do LastChar = getchar();  while (LastChar != EOF && LastChar != '\n' && LastChar != '\r');
  if (LastChar != EOF)    return gettok(); }
*/
// --------------------------------------------------------------------------------------------


}

/*
#define TOKEN_EOF            -1   // END OF FILE    \0
#define TOKEN_DEF            -2   // DEFINITION     function, var
#define TOKEN_IDF            -3   // IDENTIFIER     variablename, functionname, mumbojumbo
#define TOKEN_BHD            -4   // BLOCK HEADERS  for, while, if, else, do
#define TOKEN_NBR            -5   // NUMBER {[0..9]}[.]{[0..9]}
#define TOKEN_SPR            -6   // SEPARATOR  {, }, whitespace, (, ), [, ], :, ;,  comma, ", '
#define TOKEN_MOP            -7   // MATHEMATICAL OPERATOR +, -, *, /, ^, % 
#define TOKEN_LOP            -8   // LOGICAL OPERATOR >, <, =, &, |, ! 
#define TOKEN_BIF            -9   // BUILT-IN FUNCTION 
#define TOKEN_STR            -10  // STRINGS (yeah!)  
*/

 
void load(char*loa) {
long i=0;
FILE * source;
source = fopen(loa, "rb");
if (source == NULL) {
throwErr(200, "cannot open file", loa);
//printf("\nERROR - cannot open file: %s\n", loa);
} else {
long filesize;
fseek(source,0, SEEK_END);
filesize=ftell(source);
rewind(source);
//printf("\nfile %s size: %i Bytes\n", loa, filesize);
programText = malloc((filesize*(sizeof(char)))+1);
//printf("\nfloading at position %x\n", programText);
fread(programText, sizeof(char), filesize, source);
fclose(source);
programText[filesize]=0;
}
}

 
bool getpopen( tokenElement * returns, char* command){
int j=0;
FILE * pipe;
char * buffer;
long filesize;

pipe=popen(command,"r");
buffer=malloc(1024*1024*sizeof(char)); // 1 MB of buffer for the stream :(
fread( buffer, sizeof(char), filesize, pipe);
filesize=strlen(buffer);

returns->value = malloc(filesize+2);

for (j=0; j<=filesize; j++){
returns->value[j]=buffer[j];
}
free(buffer); //bye big chunk
returns->value[j]=0;
returns->start = 0;
returns->internal = 263;//string constant
returns->type = TOKEN_STR; return true;
if (pclose(pipe)!=0){
throwErr(201, "cannot close pipe o.O", "GOD!");
return false;
}
return false;

}

 
void lexer() {
 
tokenElement * grabs;
tokenElement grabs2;
grabs = &grabs2;
grabs->value=NULL;
 

while (getToken(grabs)) {

switch (grabs->type) {
case TOKEN_DEF: instructionPointer=grabs->start; define();
break;
case TOKEN_IDF:  
break;
case TOKEN_BHD:  
break;
case TOKEN_NBR:  
break;
case TOKEN_SPR: 
break;
case TOKEN_MOP: 
break;
case TOKEN_LOP:  
break;
case TOKEN_BIF: 
break;
case TOKEN_STR:  
break;
case TOKEN_PMT: 
break;
case TOKEN_DFF: 
break;
case TOKEN_VAR:  
break;
}  
 
 
 
if (grabs->value!=NULL) free(grabs->value);


}
 
}
 
bool express(tokenElement * returns, char ender){
if(debug) printf("\nexp eval");
//printf("eval express\n");
char expression[512];
tokenElement * grabs;
tokenElement grabs2;
grabs = &grabs2;
grabs->value=NULL; 

int locursor=0;
int locursor2=0;
getToken(grabs);
 printf("\n");
while (grabs->value[0]!=ender){
 //printf("  %i",  grabs->type);

	if (grabs->type == TOKEN_VAR){
		for (locursor2=0;locursor2<strlen(grabs->dereference);locursor2++){
		//printf("%.1s", &grabs->value[locursor2] );
		expression[locursor]=grabs->dereference[locursor2];
		locursor++;
		}

	} else if (grabs->type == TOKEN_BIF) {

	//buit in functions

	} else if (grabs->type == TOKEN_DFF) {
		printf("DFF");
		//user defined functions

	} else if (grabs->type == TOKEN_IDF) {
		throwErr(244,"unknown identifier", grabs->value);
		//unknown identifier fail
		return false;

	} else if (grabs->type == TOKEN_DEF) {
		throwErr(245,"unexpected identifier", grabs->value);
		//unknown identifier fail
		return false;

	} else if (grabs->type == TOKEN_PSO) {

		//pseudoObjects (they require another identifier after them)
		printf("PSO");

	} else {

	for (locursor2=0;locursor2<strlen(grabs->value);locursor2++){
	//printf("%.1s", &grabs->value[locursor2] );
	expression[locursor]=grabs->value[locursor2];
	locursor++;

	}

	}


	if (grabs->value!=NULL) free(grabs->value);
		getToken(grabs);
 		//expression[locursor]='p';
 		//locursor++;
	}
expression[locursor]=0;
//printf("\n %i",  grabs->internal);



returns->value=malloc((strlen(expression)+2)*sizeof(char));

int i=0;

for (i=0; i< strlen(expression);i++){

returns->value[i]=expression[i];

}

returns->value[i]=0;

//printf("\nresult: %f", Eval(expression));

return true;
}

void nop(){

}

bool define(){
if(debug) printf("\ndefinition");
tokenElement * grabs;
tokenElement grabs2;
grabs = &grabs2;
grabs->value=NULL;
getToken(grabs);


switch (grabs->internal) {
case 4: nop;
tokenElement * grabs1;
tokenElement grabs12;
grabs1 = &grabs12;
grabs1->value=NULL;
getToken(grabs1);

	if ((grabs1->type==TOKEN_VAR)||(grabs1->type==TOKEN_IDF)) {
	//printf("%s\x1b[0m: %i" ,grabs1->value, grabs1->internal);

	tokenElement * grabs12;
	tokenElement grabs13;
	grabs12 = &grabs13;
	grabs12->value=NULL;
	getToken(grabs12);


	if (grabs12->internal==215) {
	tokenElement * grabs12;
	tokenElement grabs13;
	grabs12 = &grabs13;
	grabs12->value=NULL;
	if (express(grabs12,';')) {
	//printf("valid expression after =");
	addVar(grabs1->value,grabs12->value);
	if (grabs1->value!=NULL) free(grabs1->value);
	if (grabs12->value!=NULL) free(grabs12->value);
	return true; // declaration with assignment
	} else { // this is not a valid expression, boy.
		//another something went wrong :(
	return false;
	}

	} else if (grabs12->internal==246){
	addVar(grabs1->value,"null");
	if (grabs1->value!=NULL) free(grabs1->value);
	return true; //declaration without assignment;
	} else {
		//something wrong, bro
		return false;
	}


	if (grabs12->value!=NULL) free(grabs12->value);
	} else {
	// throw invalid definition error
		throwErr(203,"unexpected identifier - ",grabs1->value);
	return false;
	}

if (grabs1->value!=NULL) free(grabs1->value);
break;
case 2: printf("\nmethod : \x1b[32m");
break;
case 1: printf("\nvariable : \x1b[36m ");
break;
case 6: printf("\nfile : \x1b[35m");
break;
case 7: printf("\ndatamodel : \x1b[34m");
break;
default: printf("\nnot a definition : \x1b[31m");
break;
}  

//printf("%s\x1b[0m: %i" ,grabs->value, grabs->internal);

if (grabs->value!=NULL) free(grabs->value);

}




int main(int argc, char *argv[])
{
// estabilish supervars

 if( argc == 2 )
   {
      //printf("The argument supplied is %s\n", argv[1]);
   }
   else if( argc > 2 )
   {
      printf("Too many arguments.\n");
      return 1;
   }
   else
   {
      printf("Filename expected.\n");
      return 1;
   }


//addVar (varname, varvalue)

//these are not variables, in fact, these are entry points for
//variable lists.
//have to adapt the variable dereference stuff in order to accept
// constructs like _POST["varname"];
// don't want to distance very much from PHP


addVar("_ARGS","null");	 	// get calling arguments and put them here
addVar("_GET","null"); 		// get GET data and put them here (from apache)
addVar("_POST","null"); 	// get POST data and put them here (from apache)
addVar("_SESSION","null");	// get SESSION data and put them here (from apache)
addVar("_STACK","null"); 	// wrapper to stack access
addVar("_ENVIRONMENT","null"); 	// wrapper to environment variables access
addVar("_EVENTS","null"); 	// should be able to call events from here
addVar("_FUNCTIONS","null"); 	// should be able to call functions from here
addVar("_METHODS","null"); 	// should be able to call methods from here
addVar("_VARIABLES","null"); 	// should be able to access variables from here (not this variable, though)
//addFunc (functionName, arguments)
addFun("funcie","a, b, c"); 	// should be able to access variables from here (not this variable, though)

//addVar("serial","22"); 	// should be able to access variables from here (not this variable, though)







int excount = 0;
//printf("\n============ START OF LEXER =============\n");

load(argv[1]);
//printf("program loaded : %s\n", programText);

while (excount<1  ) {
lexer();
instructionPointer =0; 
excount++;
}

printall();
printf("\n\n<--END OF PROGRAM-->\n\n");
//printf("\n============= END OF LEXER ==============\n");
//tokenElement * grabs;
//tokenElement grabs2;
//grabs = &grabs2;
//grabs->value=NULL;

//getpopen(grabs, "ls /");
//printf("in main: %s\n", grabs->value);
//if (grabs->value!=NULL) free(grabs->value);

//addVar("END",grabs->value);

return 0;
}




 
void throwErr(int error, char* mesg, char* mesg2){

switch (error){
case 1:
printf("\nERROR %i - empty stack \n",error);
break;
default:
printf("\nERROR %i - %s %s \n",error,mesg,mesg2);
break;

}

//errors break execution forever
exit(1);

}
