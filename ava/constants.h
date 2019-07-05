#ifndef CONSTANTS
#define CONSTANTS

// single-character classification
#define DIGIT 		1	// 0..9
#define ALPHA 		2	// A..Z, a..z
#define ALPHANUM	3	// 0..9, A..Z, a..z
#define PERIOD 		4	// .
#define UNDERSCORE  	-10	// _
//#define SEMICOLON	6	// ;  - statement separator
#define SEPARATOR	7	// { } [ ] ( ) , : " ' ;


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
	#define LBREAK		82	// ;
	#define BLANK		83	//
	#define HASH		84	// #


#define MOPERATOR	8	// + - * / % ^
#define LOPERATOR	9	// & | !
#define COMPARATOR	10	// >   <   =   ! 


//multi-character classification
#define TEOF		-1	// END OF STREAM REACHED
#define INUM		-2	// [-] DIGIT [{DIGIT}]
#define FNUM		-3	// [-] DIGIT [{DIGIT}] . DIGIT [{DIGIT}]
#define SSTR		-4	// "{[DIGIT | ALPHA | PERIOD | ... ]}"
#define CSTR 		-5	// '{[DIGIT | ALPHA | PERIOD | ... ]}'

#define MOPE		-6	// MOPERATOR [MOPERATOR]

	#define PLUS	-61	// +
	#define MINUS	-62	// -
	#define TIMES	-63	// *
	#define DIV	-64	// /
	#define MOD	-65	// %
	#define POW	-66	// ^
	#define INC	-67	// ++
	#define DEC	-68	// --

#define LOPE		-7	// LOPERATOR [LOPERATOR]

	#define AND	-71	// &&
	#define OR	-72	// ||
	#define NAND	-73	// !&
	#define NOR	-74	// !|
	#define NOT	-75	// !
	#define ET	-76	// &
	#define PIPE	-77	// |

#define COMP		-8	// COMPARATOR|LOPERATOR [COMPARATOR]

	#define GT	-81	// >
	#define GTE	-82	// >=
	#define LT	-83	// <
	#define LTE	-84	// <=
	#define EQ	-85	// ==
	#define NEQ	-86	// !=
	#define ATRIB	-87	// =

#define IDEF		-9	// UNDERSCORE | ALPHA [{ALPHANUM}] default: unknown identifier

	#define VNAM	-91	// An identifier, but also a declared variable name
	#define FNAM	-92	// An identifier, but also a declared function name

	#define BIFN	-93	// An identifier, but also a built in function name

		#define	BREAK 	-931	// exits loop
		#define RETURN	-932	// returns from function
		#define IMPORT	-933	// imports program file
		#define SYSCALL	-934	// calls system program
		#define PRINT	-935	// prints to destination
		#define EXIT	-936	// exits program
		#define READ	-937	// reads from source
		#define THROW	-938	// throws event
		#define EVAL	-939	// evaluates string as code
 

	#define BIVN	-94	// An identifier, but also a built in variable name

		#define	_ARGS 	-941	// arguments
		#define _ENV	-942	// environment variables
		#define _GET	-943	// get from apache
		#define _POST	-944	// post from apache
		#define _SESS	-945	// session from apache
		#define _VARS	-946	// variables pool
		#define _OBJS	-947	// objects pool
		#define _EVEN	-948	// events pool
		#define _FUNC	-949	// functions pool

	#define BHDR	-95	// Block header

		#define	IF 	-951	// if
		#define ELSE	-952	// else
		#define FOR	-953	// for
		#define WHILE	-954	// while
		#define UNTIL	-955	// until
		#define FOREACH	-956	// foreach
		#define SWITCH	-957	// switch
		#define CASE	-958	// case
		#define DEFAULT	-959	// default
		

	#define DECL	-96	// declaration

		#define	FUNCTIONDCL	-961	// function declaration
		#define METHODDCL	-962	// method declaration
		#define VARIABLEDCL	-963	// variable declaration
		#define VARDCL		-964	// variable declaration
		#define EVENTDCL	-965	// event declaration
		#define FILEDCL		-966	// file declaration
		#define DATAMODELDCL	-967	// datamodel declaration (future)
		#define OBJECTDCL	-968	// object declaration (future)
		#define SOCKETDCL	-969	// socket declaration (future)
		
	#define PSOB	-97	// pseudoObjects

		#define	_MATH		-971	// Math. pseudoobject
		#define _STRING		-972	// String. pseudoobject
		#define _ARRAY		-973	// Array. pseudoobject
		#define _FILE		-974	// File. pseudoobject
		#define _HASH		-975	// Hash. pseudoobject
		#define _DATABASE	-976	// Database. pseudoobject
		#define _SOCKET		-977	// Socket. pseudoobject
		#define _CURL		-978	// Curl. pseudoobject
		#define _DATASET	-979	// Dataset. pseudoobject

	#define PRIM	-98	// primitives

		#define	_NULL		-981	// NULL
		#define _NAN		-982	// NAN
		#define _VOID		-983	// VOID
		#define _TRUE		-984	// true
		#define _FALSE		-985	// false
		#define _NOP		-976	// no operation

#define SEPR		-10	// UNDERSCORE | ALPHA [{ALPHANUM}]
#define STR 		-11	// "[{ALPHANUM}]"
#define EOL		-12 	// END OF STATEMENT ;



#endif
 

