#ifndef STRUCTURES
#define STRUCTURES


// variable typedef

typedef struct Tnode Tvariable;

struct Tnode {
char * value;
char * name;
Tvariable *entry; //for array-type variables && function arguments
Tvariable *next;
};

// TOKEN ELEMENT STRUCTURES:
typedef struct Tokele tokenElement;
struct Tokele {
int  type;        	//type of the token returned
int internal;     	// the internal value of the token (for speedup purposes)
char * value;     	//value of the grabbed token
char * dereference;     //value of the grabbed VARIABLE/function token
long start;      	//where did we begin chewing? THIS IS A NICE APPROACH, DO NOT MESS THIS TIME!
};



// loads file to program text area
extern void load(char*loa);
// throws custom error
extern void throwErr(int error, char* mesg, char* mesg2);
// peeks next char without fucking with instructionPointer
extern char peek();
// chews next char and updates instructionPointer
extern char chew();
// is ev an Alphanumeric value? (we accept underscores too)
extern int isALN(char ev);
// is ev an Alphabetic value? (underscores return true, use with check)
extern int isALP(char ev);
// is ev a number?
extern int isNBR(char ev);
// is ev a separator?
extern int isSPR(char ev);
// is ev a logic operator?
extern int isLOP(char ev);
// is ev a math operator?
extern int isMOP(char ev);
//cleanup blanks
extern bool cleanup();
//verify if the next char is the expected
bool match (char ev);
//lexer
bool lex();


#endif
