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
#include "constants.h"
#include "types.h"



char * programText=NULL;     // our program resides here
long instructionPointer=0;   // our virtual instruction pointer
char * lexdest=NULL;
int  internal=-9999;
long start; // last token entry point in the stream


//we will keep track of the interpreting in the future. so leave these variables here.
long line=0;
long character=0;


bool lex(){

char token[512]; // why in hell would you need more?
long locursor=0;
int i;
start=instructionPointer;

if (cleanup()) {


// IDENTIFIERS ----------------------------------------------------------
if (isALP(peek())>0) {
token[locursor]=chew();
locursor++;

	while (isALN(peek())) {
	token[locursor] = chew();
	//printf("%.1s",&token[locursor]);
	locursor++;
	}

token[locursor] = 0;

	if (lexdest!=NULL) free(lexdest);

lexdest = malloc (strlen(token)+2);

	for ( i =0; i<=strlen(token); i++) {
	lexdest[i] =token[i];
	}

internal = IDEF;

lexdest[i] =0;

// ------------- NOW CHECK FOR VARIABLE & DEREFERENCE IF POSSIBLE -------




// ------------ NOW FOR FUNCTION CALL, PLEASE --------------------------

// first PREDEF


// now USERDEF


// NOW CHECK FOR BUILT IN PSEUDO OBJECTS -------------------------------



// ----------------------------------------------------- THANKS --------

internal = identIDF(lexdest);
// ---------------------------------------------------------------------
return true;

} else if (isNBR(peek())) { // NUMBERS ---------------------------------


	while (isNBR(peek())) {
	token[locursor] = chew();
	//printf("%.1s",&token[locursor]);
	locursor++;
	}

token[locursor] = 0;

	if (lexdest!=NULL) free(lexdest);

	lexdest = malloc (strlen(token)+2);
	int countperiod=0;

	for ( i =0; i<=strlen(token); i++) {
		if (token[i]=='.') countperiod++;
		lexdest[i] =token[i];
	}

lexdest[i] =0;

//printf("%s\n",&token);

//now you count the periods and decide if you fail or not!

	if (countperiod==0) {
		internal = INUM;
	} else if (countperiod==1) {
		internal = FNUM;
	} else {
		internal = IDEF;
	}

// -------------------------------------------------------


return true;

}  else if (isLOP(peek())){
	while (isLOP(peek())) {
		token[locursor] = chew();
		//printf("%.1s",&token[locursor]);
		locursor++;
	}
token[locursor] = 0;
	if (lexdest!=NULL) free(lexdest);
lexdest = malloc (strlen(token)+2);

	for ( i =0; i<=strlen(token); i++) {
		lexdest[i] =token[i];
	}

internal = LOPE;
// now verify if it is a logic operator or a comparator ----------------------

switch (token[0]){

case '>':
internal=COMP;
break;
case '<':
internal=COMP;
break;
case '!':
 if (token[1]=='=') internal=COMP;
break;
case '=':
internal=COMP;
break;
case ';':
internal=EOL;
break;

default: break;

}



//----------------------------------------------------------------------------


lexdest[i] =0;

return true;

} else if (isMOP(peek())){


// comments, please ----------------------
	if (peek()=='/'){
		chew();
		if (peek()=='/') { //single line comment
			while (isSPR(peek())!=LBREAK) { chew(); }
		if (peek()==0) return false;
		return lex();//malandrão
		} else if (peek()=='*') { // multi line comment
			while ( !((chew()=='*')&&(peek()=='/'))) {
				if (peek()==0) { printf("oops");
					return false;	
				}
			} chew();return lex();
		} else {

		instructionPointer--;

		}
	} 


// now ye olde mathematical operators ---------------

locursor=0;
	while (isMOP(peek())){
		token[locursor]=chew();
		locursor++;
	}
	token[locursor]=0;

	if (lexdest!=NULL) free(lexdest);
lexdest = malloc (strlen(token)+2);

	for ( i =0; i<=strlen(token); i++) {
	lexdest[i] =token[i];
	}

internal = MOPE;
lexdest[i] =0;


return true;
} else if ( isSPR(peek()) ){

// String special case! --------------------------------------------
// CORRIJA ISSO!


if (peek()=='\"') {
long reset=instructionPointer;
locursor=0;
chew();
locursor++;
	while (peek()!='\"') {
		chew();
		locursor++;
			if (peek()==0) {
				printf("broken");
 				return false;
			}
	}

chew();
locursor++;

	if (lexdest!=NULL) free(lexdest);
	lexdest = malloc (locursor+2);
	locursor=0;
	for ( i =reset; i<instructionPointer; i++) {
	lexdest[locursor] = programText[i];
	locursor++;
	}

internal = STR;
lexdest[locursor]=0;

return true;

} else if (peek()=='\'') { // charstring special case
long reset=instructionPointer;
locursor=0;
chew();
locursor++;
	while (peek()!='\'') {
		chew();
		locursor++;
			if (peek()==0) {
				printf("broken");
 				return false;

			}
	}


chew();
locursor++;

	if (lexdest!=NULL) free(lexdest);
	lexdest = malloc (locursor+2);
	locursor=0;
	for ( i =reset; i<instructionPointer; i++) {
	lexdest[locursor] = programText[i];
	locursor++;

	}

internal = STR;
lexdest[locursor]=0;
return true;

} else {

token[0]=chew();
token[1]=0;

	if (lexdest!=NULL) free(lexdest);
lexdest = malloc (strlen(token)+2);

	for ( i =0; i<=strlen(token); i++) {
		lexdest[i] =token[i];
	}
internal = isSPR(lexdest[0]);

/*
switch (token[0]){

case ';':
internal=EOL;
break;

default: break;

}
*/




lexdest[i] =0;


return true;

}


} else {
//chew();

 return false;

}



} 
//printf("no cleanup");

}







int main(int argc, char *argv[])
{

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


load(argv[1]);
printf("\n\n\n");
while (lex()){ printf("%s ", lexdest); printf("%i @", internal);printf("%i \n", start);}
printf("\n\n\n");
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


char peek(){

char returnee;
returnee=programText[instructionPointer];
return returnee;

}

char chew(){
char returnee;
returnee=programText[instructionPointer];
instructionPointer++;
return returnee;
}

 
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
		return UNDERSCORE;
	} else {
		return 0;
	}
}
 
int isALN(char ev){
	if (isalpha(ev)) {	
		return ALPHANUM;
	} else if (isNBR(ev)) {	
		return ALPHANUM;
	} else if (ev=='_') {	
		return UNDERSCORE;
	} else {
		return 0;
	}
}






int isSPR(char ev) {
 
switch (ev){
/*
	#define KEYOP		71	// {
	#define KEYCL		72	// }
	#define BRACKOP		73	// [
	#define BRACKCL		74	// ]
	#define PAROP		75	// (
	#define PARCL		76	// )
	#define COMMA		77	// ,
	#define COLON		78	// :
	#define QUOTE		79	// "
	#define APOS		80	// '
	#define SEMICOLON	81	// ;
*/
case '{': return KEYOP;break;
case '}': return KEYCL;break;
case 32: return  BLANK;break;
case 9:  return  BLANK;break;
case 11: return  LBREAK;break;
case 10: return  LBREAK;break;
case 13: return  LBREAK;break;
case 15: return  LBREAK;break;
case '(': return PAROP;break;
case ')': return PARCL;break;
case '[': return BRACKOP;break;
case ']': return BRACKCL;break;
case ':': return COLON;break;
case ';': return SEMICOLON;break;
case ',': return COMMA;break;
case '"': return QUOTE;break;
case '\'': return APOS;break;  
case '#': return HASH;break;  

default:break;
}
return 0;
}

 
int isMOP(char ev) {
//MATHEMATICAL OPERATOR +, -, *, /, ^, % 
switch (ev){

case '+': return PLUS;break;
case '-': return MINUS;break;
case '*': return TIMES;break;
case '/': return DIV;break;
case '^': return POW;break;
case '%': return MOD;break;

default:break;
}
return 0;
}
 
int isLOP(char ev) {
//LOGICAL OPERATOR >, <, =, &, |, ! 
switch (ev){

case '>': return GT;break;
case '<': return LT;break;
case '=': return ATRIB;break;
case '&': return ET;break;
case '|': return PIPE;break;
case '!': return NOT;break;

default:break;
}
return 0;
}


bool cleanup() {
char res=255;
while ((isSPR(peek())==BLANK) || (isSPR(peek())==LBREAK)) {
res = chew();
//printf("cleaning up %i\n", res);
}

if (res==0) return false;
return true;


} 


bool match (char ev){
if (peek()==ev) {
chew();
return true;
} else {
return false;
}

}
 

int identIDF(char * source) {
char token[512]; //no need for more, i guess

for (int i=0; i<strlen(source); i++){
token[i]=source[i];
}
token[strlen(source)]=0;

switch(token[0]) {
case 'a':
break;
case 'b':
break;
case 'c':
break;
case 'd':
break;
case 'e':
break;
case 'f':

if (strcmp(token, "function")==0) return FUNCTIONDCL;

break;
case 'g':
break;
case 'h':
break;
case 'i':
break;
case 'j':
break;
case 'k':
break;
case 'l':
break;
case 'm':
break;
case 'n':
break;
case 'o':
break;
case 'p':

if (strcmp(token, "print")==0) return PRINT;

break;
case 'q':
break;
case 'r':

if (strcmp(token, "return")==0) return RETURN;

break;
case 's':
break;
case 't':
break;
case 'u':
break;
case 'v':
break;
case 'w':
break;
case 'x':
break;
case 'y':
break;
case 'z':
break;






default: return IDEF; break;
}

return IDEF;


}

