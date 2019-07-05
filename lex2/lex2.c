#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define EXPECT_ALPHA         1
#define EXPECT_NUMERIC       2
#define EXPECT_ALPHANUMERIC  3
#define EXPECT_CTRL          4
#define EXPECT_EOF           100


// VIRTUAL HEAP ---------------
typedef struct Tele Telement;

struct Tele {
long value;
Telement *next;
};
//------------------------------
// expression parser element
typedef struct Texe Texel;

struct Texe {
char * contains;
Telement *left;
Telement *right;
Telement *parent;
};
//-----------------------------
// token parser element
typedef struct Tpar Tparser;

struct Tpar {
char type;
char * token;
char * index;
bool success;
};
//-----------------------------
// lookahead parser element
//-----------------------------


// virtual heap entry
Telement * HEAP=NULL;
//virtual stack entry
Telement * STACK=NULL;
// our input string (must be loaded)
char * input;
// our cursor
long cursor=0;


//function declarations:
bool push(Telement ** targetheap, long value);
long pop(Telement ** targetheap);
void trasversal(Telement * targetheap);
int isctrl(char op);
long lexit(char  input []);
long body(char  input []);
long header(char  input []);
void throwErr(int error, char * mesg);
bool isblanka(char op);
char * tokenizer(int EXPECT);
int probe();
char consume();
bool comp(char*compare);


void load(char*loa) {
long i=0;
input = malloc(strlen(loa)+1);
for (i=0;i<strlen(loa);i++){
input[i]=loa[i];
}
input[i+1]=0;

}




//----probes kind of next character in the stream
int probe(){

char probeme = input[cursor+1];
//printf("\nevaling: %.1s",&input[cursor+1]);

if (probeme==0) {
return 100; //return code for end of file 
}
if (probeme=='.'){
//printf("\nwhitey");
return 5;
} 
if (isalpha(probeme)){
//printf("\nalpha");
return 1;
} else if (isdigit(probeme)){
//printf("\nnumby");
return 2;
} else if (isctrl(probeme)){
//printf("\nctrly");
return 3;
} else if (isblanka(probeme)){
//printf("\nwhitey");
return 4;
}else {
return 0;
}

return 0;
}

int flyeval(char probeme){

//printf("\nevaling: %.1s",&input[cursor+1]);

if (probeme==0) {
return 100; //return code for end of file 
}

if (probeme=='.'){
//printf("\nwhitey");
return 5;
} 

if (isalpha(probeme)){
//printf("\nalpha");
return 1;
} else if (isdigit(probeme)){
//printf("\nnumby");
return 2;
} else if (isctrl(probeme)){
//printf("\nctrly");
return 3;
} else if (isblanka(probeme)){
//printf("\nwhitey");
return 4;
}else  {
return 0;
}

return 0;
}


//consumes next char
char consume(){
cursor++;
//printf("\nfetching: %.1s",&input[cursor]);
return input[cursor];
}

/*
#define EXPECT_ALPHA         1
#define EXPECT_NUMERIC       2
#define EXPECT_ALPHANUMERIC  3
#define EXPECT_CTRL          4
char * separator
" ={A1
*/

//This function advances on the input,
//returning 
char * tokenizer(int EXPECT){
char token[255];
int locursor=0;
token[locursor]=0;
bool error=false;
bool EoF=false;
bool warning=false;
char * response;
long startpos = cursor;


switch (EXPECT) {

case 1: //ALPHA
while (probe()==4) consume(); //skip whitespaces
while ( (probe()!=4)&&(probe()!=3) ) {
if (probe()==100) { EoF = true; break; } 
if (probe()==2) error=true;
if (probe()==0) error=true;
token[locursor]=consume();
locursor++;
token[locursor]=0;
}
//printf("\ntoken: %s", &token);

break;
case 2: //NUMERIC
while (probe()==4) consume(); //skip whitespaces
while ( ((probe()!=4)&&(probe()!=3)) ) {
if (probe()==100) { EoF = true; break; } 
if (probe()==1) error=true;
if (probe()==0) error=true;
token[locursor]=consume();
locursor++;
token[locursor]=0;
}
//printf("\ntoken: %s", &token);
break;
case 3: //ALPHANUMERIC
while (probe()==4) consume(); //skip whitespaces
while ((probe()!=4)&&(probe()!=3)) {
if (probe()==100) { EoF = true; break; } 
if (probe()==0) error=true;
token[locursor]=consume();
locursor++;
token[locursor]=0;
}
//printf("\ntoken: %s", &token);
break;
case 4: //SYMBOLIC
while (probe()==4) consume(); //skip whitespaces
while ((probe()!=4)&&(probe()!=2)&&(probe()!=1)) {
if (probe()==100) { EoF = true; break; } 
if (probe()!=3) {error=true; printf("wrong: %i",probe()); break;}
token[locursor]=consume();
locursor++;
token[locursor]=0;
}
//printf("\ntoken: %s", &token);
break;
}

int ccp=0;

printf("\ntoken: %s\n", &token);
if (error==true) {
throwErr(4,token);
response = malloc(strlen("null")+1);


response[0]='n';
response[1]='u';
response[2]='l';
response[3]='l';
response[4]=0;

return response;

} else {
response = malloc(strlen(token)+1);

for (ccp=0; ccp<strlen(token); ccp++){
response[ccp]=token[ccp];
}
return response;

}


}


//push an item to any stack (heap included)
bool push(Telement ** targetheap, long value){
Telement * newitem = malloc(sizeof(Telement));
newitem->value=value;
newitem->next=*targetheap;
*targetheap=newitem;
//printf("\n%i",*targetheap);
//printf("\nPushing %i in position: %i, next: %i",newitem->value,newitem,newitem->next);

}
//pops an item from any stack
long pop(Telement ** targetheap){
long returnee=0;
Telement * tempsave;

if (*targetheap==NULL) {

} else {
returnee = (*targetheap)->value;
//printf("\nreturns %i",returnee);
tempsave = (*targetheap)->next;
free(*targetheap);
*targetheap = tempsave;
return returnee;
}

return 0;
}
void trasversal(Telement * targetheap) {
Telement * cursor;
cursor = targetheap;
long pos=0;

while (cursor!=NULL) {
//printf("\nPosition %i : %i",pos,cursor->value);
pos++;
cursor=cursor->next;
}


}
//-----------------------

// is the single character input a control char?
int isctrl(char op){
switch (op) {

// //check the following section with
// if ( isctrl(char)<100 ) {} 
case '{': return 1;break;   
case '}': return 2;break;   
case ';': return 3;break;  
case '@': return 4;break;  
case '(': return 5;break;  
case ')': return 6;break;  
case '[': return 7;break;  
case ']': return 8;break;  
case ',': return 9;break;  
//case '.': return 10;break;

//check the following section with
// if ( ((isctrl(char)/10) >=10) && ((isctrl(char)/10) < 100) ) {}
case '=': return 100;break;
case '>': return 200;break;
case '<': return 300;break;
case '!': return 400;break;
case '&': return 500;break;
case '|': return 600;break;

//check the following section with
// if ( ((isctrl(char)/10) >=100) && ((isctrl(char)/10) < 1200) ) {}
case '+': return 1000;break;
case '-': return 2000;break;
case '*': return 3000;break;
case '/': return 4000;break;
case '^': return 5000;break;
case '%': return 6000;break;
case '#': return 7000;break;
case ':': return 8000;break;


//this is not a control character, isalpha maybe?
default: return 0;

}

}
//--------------------------------------------

//is it a blank character?
bool isblanka(char op){
switch (op) {

case 32: return true;break; 
case 9: return true;break; 
case 10: return true;break; 
case 13: return true;break; 


}

return false;

}


// lex the fuck out of it ------------------------------

long lexit(char  input []) {



tokenizer(EXPECT_ALPHA);

}


// lex the fuck out BODY ------------------------------

long body(char  input[]) {

}

// lex the fuck out of header ------------------------------

long header(char  input[]) {

}



//-------------------------------------------------------

int main()
{

/*
for (int i=0; i<5; i++){
push (&HEAP,i*5000);
//printf("\nheap: %i",HEAP);
}

trasversal(HEAP);
pop(&HEAP);pop(&HEAP);
trasversal(HEAP);
*/
printf("\n============ START OF LEXER =============\n");
load(" serrrrrrviceous  {   } \n \n body{     }");
lexit("baboseira");
printf("\n============= END OF LEXER ==============\n");

return 0;
}






void throwErr(int error, char* mesg){

switch (error){
case 1:
printf("\nERROR %i - empty stack ",error);
break;
case 2:
printf("\nERROR %i - expected character %s",error, mesg);
break;
case 3:
printf("\nERROR %i - unexpected character %s",error, mesg);
break;
case 4:
printf("\nERROR %i - unexpected token %s",error, mesg);
break;
case 8:
printf("\nERROR %i - unexpected end of file",error);
break;
case 100:
printf("\ninterpretation ended %s",error, mesg);
break;



default:
printf("\nERROR - undefined error value %i",error);
break;

}

//errors break execution forever
return ;

}
