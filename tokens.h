#define	LEX_EOF			-1 //konec souboru
#define	LEX_NULL			0
#define	LEX_ERROR			1 // chyba, neznamy lex. element
#define	LEX_DIV			2 // operator '/'
#define	LEX_DIVASSIGN		3 // operator '/=' */
#define	LEX_IDENT			4 // identifikator, ne klicove slovo
#define	LEX_INT			5 // cele cislo
#define	LEX_FLOAT			6 //desetinne cislo
#define	LEX_IF			7 //klicove slovo if
#define	LEX_ELSE			8 //klicove slovo else
#define	LEX_WHILE			9 //klicove slovo while
#define	LEX_FOR			10//klicove slovo for
#define	LEX_BREAK			11
#define	LEX_CONTINUE		12
#define	LEX_LPA			13 //leva zavorka
#define	LEX_RPA 			14 //prava zavorka

#define	LEX_MUL			15 //operator nasobeni '*'
#define	LEX_MULASSIGN		16 //operator '*='

#define	LEX_ADD			17 //operator scitani '+'
#define	LEX_ADDASSIGN		18 //operator '+='

#define	LEX_SUB			19 //operator odecitani '-'
#define	LEX_SUBASSIGN		20 //operator '-='

#define	LEX_BEGIN			21 //operator zacatku bloku '{'
#define	LEX_END			22 //operator konce bloku '}'

#define	LEX_SEMICOLON		23 //strednik ukonceni prikazu
#define	LEX_ASSIGN			24 //operator prirazeni '='

#define	LEX_GREATER_THAN		25 //operator '>'
#define	LEX_GREATER_EQU_THAN	26 //operator '>='
#define	LEX_LESS_THAN		27 //operator '<'
#define	LEX_LESS_EQU_THAN		28 //operator '<'

#define	LEX_ISEQUAL			29 //operator rovnosti '=='
#define	LEX_ISNEQUAL		30 //operator nerovnosti '!='
#define	LEX_EXCLAMATION		31 // nepravda !

#define	LEX_MOD			32 //operator modulo '%'
#define	LEX_MODASSIGN		33 //operator '%='

#define	LEX_INCREMENT		34 //operator++
#define	LEX_DECREMENT		35 //operator--
#define	LEX_K_VOID			36
#define	LEX_K_INT			37
#define	LEX_K_FLOAT			38
#define 	LEX_K_MAIN			39
#define 	LEX_K_PRINTF		40
