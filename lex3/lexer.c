#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>



#define DIGIT     	     1
#define PERIOD               2
#define ALPHA                3


#define LINE_BREAK           4
#define BLANK_SPACE          3

#define TOKEN_EOF            -1   // END OF FILE    \0
#define TOKEN_DEF            -2   // DEFINITION     function, var
#define TOKEN_IDF            -3   // IDENTIFIER     variablename, functionname, mumbojumbo
#define TOKEN_BHD            -4   // BLOCK HEADERS  for, while, if, else, do, header, body, event
#define TOKEN_NBR            -5   // NUMBER {[0..9]}[.]{[0..9]}
#define TOKEN_SPR            -6   // SEPARATOR  {, }, whitespace, (, ), [, ], :, ;,  comma, ", '
#define TOKEN_MOP            -7   // MATHEMATICAL OPERATOR +, -, *, /, ^, % 
#define TOKEN_LOP            -8   // LOGICAL OPERATOR >, <, =, &, |, ! 







// TOKEN ELEMENT STRUCTURES:
typedef struct Tokele tokenElement;
struct Tokele {
int  type;        //type of the token returned
char * value;     //value of the grabbed token
long start;       //where did we begin chewing? THIS IS A NICE APPROACH, DO NOT MESS THIS TIME!
};
//-------------------------------------------------------------
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
	} else {
		return 0;
	}
}

int isALN(char ev){
	if (isalpha(ev)) {	
		return ALPHA;
	} else if (isNBR(ev)) {	
		return DIGIT;
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
case 13: return LINE_BREAK;break;
case '(': return 5;break;
case ')': return 6;break;
case '[': return 7;break;
case ']': return 8;break;
case ':': return 9;break;
case ';': return 10;break;
case ',': return 11;break;
case '"': return 12;break;
case '\'': return 13;break;

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
case '/': return 3;break;
case '^': return 4;break;
case '%': return 5;break;

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
case '%': return 5;break;

default:break;
}
return 0;
}


char * programText=NULL; // our program resides here
long instructionPointer=0; // our virtual instruction pointer

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
return ret;
}

bool getToken(tokenElement * returns){

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
locursor=0;
token[locursor] = lastchar;  
//printf("\nconsumed: %.1s",&lastchar);
while (isALN((lastchar = chew()))) {
//printf("|%.1s",&lastchar);
 locursor++;
 token[locursor] = lastchar; 
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

if (token[0]=='f') { //feeble optimization
  if ( strcmp(token, "function") ==0 ) {returns->type = TOKEN_DEF;return true;}  
  if ( strcmp(token, "for") ==0 )      {returns->type = TOKEN_BHD;return true;} 
  if ( strcmp(token, "foreach") ==0 )  {returns->type = TOKEN_BHD;return true;}
} else if (token[0]=='e') { //feeble optimization
  if ( strcmp(token, "else") ==0 )     {returns->type = TOKEN_BHD;return true;}
  if ( strcmp(token, "event") ==0 )    {returns->type = TOKEN_BHD;return true;}  
} else {
  if ( strcmp(token, "do") ==0 )       {returns->type = TOKEN_BHD;return true;}
  if ( strcmp(token, "if") ==0 )       {returns->type = TOKEN_BHD;return true;} 
  if ( strcmp(token, "var") ==0 )      {returns->type = TOKEN_DEF;return true;}  
  if ( strcmp(token, "body") ==0 )     {returns->type = TOKEN_BHD;return true;}
  if ( strcmp(token, "while") ==0 )    {returns->type = TOKEN_BHD;return true;}  
  if ( strcmp(token, "header") ==0 )   {returns->type = TOKEN_BHD;return true;}
  if ( strcmp(token, "document") ==0 )   {returns->type = TOKEN_BHD;return true;}
}

// the variable evaluator from the first lexer comes HERE

// ------------------------------------------------------
//printf("\n identifier : %s \n",&token);
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

returns->type = TOKEN_IDF;
if (countperiod<2) returns->type = TOKEN_NBR;
return true;

} else if(lastchar=='/'){ // handling comments
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
 //printf("\n multi ignoring:");
    while ( (lastchar!='*') && (peek()!='/') && lastchar!= 0 ) { //printf("%.1s",&lastchar);
	lastchar = chew();
    }
 lastchar = chew();lastchar = chew();
  if (lastchar!=0) { 
     //printf("\n reaweken in : %.1s",&lastchar);
  goto retry;
  }
 }
} else if(isSPR(lastchar)){ // handling separators, separators are 1-digit wide;
token[0] = lastchar; 
token[1] = 0;
//if (returns.value!=NULL) free(returns.value);
//printf("\n separator : %s \n",&token);
returns->value=malloc(strlen(token)+2);

int i=0;
for (i=0;i<=strlen(token);i++) {
returns->value[i]=token[i];
}
returns->value[i+1]=0;
//printf("\n separator : %s \n",&token);


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

returns->type = TOKEN_LOP;

return true;

} else if(isMOP(lastchar)){ // handling mathematical operators. All the opeartors in this class are 1 char wide
locursor=0;
token[locursor] = lastchar; 
token[locursor+1] = 0; 
//if (returns.value!=NULL) free(returns.value);
returns->value=malloc(strlen(token)+2);
int countperiod=0;
int i=0;
for (i=0;i<=strlen(token);i++) {
returns->value[i]=token[i];

}
returns->value[i+1]=0;


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
//printf("\nadding: %.1s",&token[i]);]
}
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
#define TOKEN_BHD            -4   // BLOCK HEADERS  for, while, if, else, do, header, body, event
#define TOKEN_NBR            -5   // NUMBER {[0..9]}[.]{[0..9]}
#define TOKEN_SPR            -6   // SEPARATOR  {, }, whitespace, (, ), [, ], :, ;,  comma, ", '
#define TOKEN_MOP            -7   // MATHEMATICAL OPERATOR +, -, *, /, ^, % 
#define TOKEN_LOP            -8   // LOGICAL OPERATOR >, <, =, &, |, ! 
*/


void load(char*loa) {
long i=0;
//if(programText!=NULL) free(programText);
/*
programText = malloc(strlen(loa)+1);
for (i=0;i<strlen(loa);i++){
programText[i]=loa[i];
}
programText[i+1]=0;
*/

FILE * source;
source = fopen(loa, "rb");
if (source == NULL) {
printf("\nERROR - cannot open file: %s\n", loa);
} else {
long filesize;
fseek(source,0, SEEK_END);
filesize=ftell(source);
rewind(source);
printf("\nfile %s size: %i Bytes\n", loa, filesize);
programText = malloc((filesize*(sizeof(char)))+1);
printf("\nfloading at position %x\n", programText);
fread(programText, sizeof(char), filesize, source);
fclose(source);
programText[filesize]=0;
}


}


void lexer() {
//printf("\ncalling tokenizer");
printf("\nsetting up lexer\n");
tokenElement * grabs;
tokenElement grabs2;
grabs = &grabs2;
grabs->value=NULL;
//printf("at position %x\n", (*grabs).value);
//getToken(grabs);
printf("\nsurvived!\n");

while (getToken(grabs)) {


switch (grabs->type) {
case -2:printf("\x1b[31m ");break;
case -3:printf(" \x1b[32m");break;
case -4:printf("\x1b[33m");break;
case -5:printf("\x1b[34m");break;
case -6:printf(" \x1b[35m");break;
case -7:printf("\x1b[35m");break;
case -8:printf("\x1b[35m");break;
}
printf("%s\x1b[0m",grabs->value);

if (grabs->value!=NULL) free(grabs->value);


}
/*}
printf(" %s\x1b[0m",grabs.value);
if (grabs.value!=NULL) free(grabs.value);
grabs=getToken();

}

while (grabs.type!=-1) {
switch (grabs.type) {
case -2:printf("\ndefinition:\x1b[31m");break;
case -3:printf("\nidentifier:\x1b[32m");break;
case -4:printf("\nblock header:\x1b[33m");break;
case -5:printf("\nnumber:\x1b[34m");break;
case -6:printf("\nseparator:\x1b[35m");break;
case -7:printf("\nmath operator:\x1b[34m");break;
case -8:printf("\nlogic operator:\x1b[32m");break;
}
printf("  %s \x1b[0m",grabs.value);
printf(" seek started at: %i",grabs.start);
grabs=getToken();
}

*/




}

int main()
{
int excount = 0;
printf("\n============ START OF LEXER =============\n");

load("mosaic.plain");
printf("program loaded : %s\n", programText);
while (excount<1) {
lexer();
instructionPointer =0; 
excount++;
printf("\n\nexecutions: %i\n\n",excount);
}
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
