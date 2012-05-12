#include "tokens.h"
#include "lexan.h"
#include "parser.h"



Lexan::Lexan(char* filename) : eof(-1)
{
	cout << filename << endl;
	fileName = new char [strlen(filename) + 1];
	strcpy(fileName, filename);
	fin = new ifstream;
	fin->open(fileName);
	row = column 	= 1;
	lastCharValid 	= false;
	floatValue 		= 0.0;
	lastColumn = intValue = 0;

	initKeywords();	
	initTokens();
}	



void Lexan::initKeywords()
{
	keywords.insert(make_pair(LEX_K_INT,"int"));	
	keywords.insert(make_pair(LEX_K_FLOAT,"float"));
	keywords.insert(make_pair(LEX_K_VOID, "void"));
	keywords.insert(make_pair(LEX_IF, "if"));
	keywords.insert(make_pair(LEX_ELSE, "else"));
	keywords.insert(make_pair(LEX_WHILE, "while"));
	keywords.insert(make_pair(LEX_FOR, "for"));
	keywords.insert(make_pair(LEX_CONTINUE, "continue"));
	keywords.insert(make_pair(LEX_BREAK, "break"));
	keywords.insert(make_pair(LEX_K_MAIN, "main"));
	keywords.insert(make_pair(LEX_K_PRINTF, "printf"));
}



void Lexan::initTokens()
{
	tokens.insert(make_pair(LEX_EOF,"LEX_EOF"));
	tokens.insert(make_pair(LEX_NULL, "LEX_NULL"));
	tokens.insert(make_pair(LEX_ERROR, "LEX_ERROR"));
	tokens.insert(make_pair(LEX_DIV, "LEX_DIV"));
	tokens.insert(make_pair(LEX_DIVASSIGN, "LEX_DIVASSIGN"));
	tokens.insert(make_pair(LEX_IDENT, "LEX_IDENT"));
	tokens.insert(make_pair(LEX_INT, "LEX_INT"));
	tokens.insert(make_pair(LEX_FLOAT, "LEX_FLOAT"));
	tokens.insert(make_pair(LEX_IF, "LEX_IF"));
	tokens.insert(make_pair(LEX_ELSE,"LEX_ELSE"));
	tokens.insert(make_pair(LEX_WHILE, "LEX_WHILE"));
	tokens.insert(make_pair(LEX_FOR, "LEX_FOR"));
	tokens.insert(make_pair(LEX_BREAK, "LEX_BREAK"));
	tokens.insert(make_pair(LEX_CONTINUE, "LEX_CONTINUE"));			
	
	tokens.insert(make_pair(LEX_LPA, "LEX_LEVA ZAVORKA"));
	tokens.insert(make_pair(LEX_RPA, "LEX_PRAVA_ZAVORKA"));

	tokens.insert(make_pair(LEX_MUL, "LEX_MUL"));
	tokens.insert(make_pair(LEX_MULASSIGN, "LEX_MULASSIGN"));

	tokens.insert(make_pair(LEX_ADD, "LEX_ADD"));
	tokens.insert(make_pair(LEX_ADDASSIGN, "LEX_ADDASSIGN"));

	tokens.insert(make_pair(LEX_SUB, "LEX_SUB"));
	tokens.insert(make_pair(LEX_SUBASSIGN, "LEX_SUBASSIGN"));

	tokens.insert(make_pair(LEX_BEGIN, "LEX_BEGIN"));
	tokens.insert(make_pair(LEX_END, "LEX_END"));
	
	tokens.insert(make_pair(LEX_SEMICOLON, "LEX_SEMICOLON"));
	tokens.insert(make_pair(LEX_ASSIGN, "LEX_ASSIGN"));

	tokens.insert(make_pair(LEX_GREATER_THAN, "LEX_GREATER_THAN"));
	tokens.insert(make_pair(LEX_GREATER_EQU_THAN, "LEX_GREATER EQU_THAN"));
	tokens.insert(make_pair(LEX_LESS_THAN, "LEX_GREATER_THAN"));
	tokens.insert(make_pair(LEX_LESS_EQU_THAN, "LEX_LESS_EQU_THAN"));
	
	tokens.insert(make_pair(LEX_ISEQUAL, "LEX_ISEQUAL"));		
	tokens.insert(make_pair(LEX_ISNEQUAL, "LEX_ISNEQUAL"));
	tokens.insert(make_pair(LEX_EXCLAMATION, "LEX_EXCLAMATION"));

	tokens.insert(make_pair(LEX_MOD, "LEX_MOD"));
	tokens.insert(make_pair(LEX_MODASSIGN, "LEX_MODASSIGN"));

	tokens.insert(make_pair(LEX_INCREMENT, "LEX_INCREMENT"));
	tokens.insert(make_pair(LEX_DECREMENT, "LEX_DECREMENT"));
	tokens.insert(make_pair(LEX_K_INT,"LEX_K_INT"));	
	tokens.insert(make_pair(LEX_K_FLOAT,"LEX_K_FLOAT"));
	tokens.insert(make_pair(LEX_K_VOID, "LEX_K_VOID"));
	tokens.insert(make_pair(LEX_K_MAIN, "LEX_K_MAIN"));
	tokens.insert(make_pair(LEX_K_PRINTF, "LEX_K_PRINTF"));

	
}


string Lexan::tokenToString(int toknum)
{
	map<int, string>::iterator ii;

	
	for(ii = tokens.begin(); ii != tokens.end(); ii++)
	{
		if(ii->first == toknum)
			return ii->second;
	}	
}



int Lexan::getNextToken()
{
	int State = 0;
	char Char;
	int Exp = 0, ExpSign = 1; /* Velikost a znamenko exponentu */
	double DmsPos = 0.1; /* prave zpracovavany desetinny rad */

	while (true) {	
		Char = getChar();
		switch (State) 
		{
			case 0:
				if (Char == ' ' || Char == '\n' || Char == '\t' || Char == '\r')
					break;
				if (Char == ';')
					return (LEX_SEMICOLON);
				if (Char == '/') {
					State = 2;
					break;
				}				
				if (Char == '=') {
					State = 29;
					break;
				}			
				if (Char == '*') {
					State = 25;
					break;
				}
				if (Char == '(')
					return (LEX_LPA);
				if (Char == ')')
					return (LEX_RPA);							
				if (Char == '}')
					return (LEX_END);
				if (Char == '{')
					return (LEX_BEGIN);								
				if (isalpha(Char)) {
					identifierName = ""; 		//na zacatku je string nazvu prazdny			
					identifierName += Char;		//prvni znak identifikatoru
					State = 38;
					break;
				}				
				if (Char == '0') {
					intValue = 0;
					floatValue = 0.0;
					State = 7;
					break;
				}
				if (isdigit(Char)) 	//nula je chycana drive
				{ 
					intValue = Char - '0';
					floatValue = Char - '0';
					State = 11;
					break;
				}
				if (Char == '+')
				{
					State = 17;
					break;
				}
				if (Char == '-')
				{
					State = 20;
					break;
				}
				if (Char == '%')
				{
					State = 27;
					break;
				}
				if (Char == '!') {
					State = 31;
					break;
				}
				if (Char == '<')
				{
					State = 33;
					break;
				}
				if (Char == '>')
				{
					State = 35;
					break;
				}
				if (Char == '.') {
					floatValue = 0.0;
					DmsPos = 0.1;
					State = 16;
					break;
				}				
				if (Char == EOF)
					return (LEX_NULL);

				return (LEX_ERROR);								


			case 2:
				if (Char == '/') {	//jednoradkovy komentar
					State = 4;
					break;
				}
				if (Char == '=') {	//operator /=				
					return (LEX_DIVASSIGN);
				}
				if (Char == '*') {
					State = 5;
					break;
				}				

				ungetChar(Char);		//operator /
				return (LEX_DIV);


			case 4:
				if (Char == '\n') 
				{
					State = 0;
					break;
				}
				if (Char == EOF)
					return (LEX_ERROR); 
				break; 			//pokud by byl eof uvnitr
								// komentare zustava ve stavu 4

			case 5:
				if (Char == '*')
				{
					State = 6;
					break;
				}
				if(Char == LEX_EOF)			
					return (LEX_ERROR);				

				
			case 6:
				if (Char == '/') {	//konec komentare
					State = 0;
					break;
				}
				if (Char != '*')
				{
					State = 5;
					break;
				}
				
				break;			//state porad = 6 pri *


			case 7:
				if (isodigit(Char)) {
					State = 8;
					intValue = Char - '0';
					break;
				}
				if(Char == 'x' || Char == 'X')
				{				
					State = 9;
					break;
				}
				if(Char == '.')
				{
					State = 12;
					break;
				}
				if (Char == EOF)
					return (LEX_ERROR); 
				if(isalpha(Char))
					return (LEX_ERROR);

				ungetChar(Char);
				cout << "intValue = " << intValue << endl;
				return (LEX_INT);		//pokud neni ani okta ani hexa


			case 8:
				if (isodigit(Char)) {
					intValue = (intValue << 3) + (Char - '0');
					State = 8; 		//state bude porad 8
					break;
				}
				if(isalpha(Char))
					return (LEX_ERROR);

				ungetChar(Char);
				cout << "intValue = " << intValue << endl;
				return(LEX_INT);


			case 9:
				if(isxdigit(Char))
				{			
					Char = (char)toupper(Char);		
					intValue = (Char >= 'A') ? Char - 'A' + 10 : Char - '0';
					State = 10;
					break;
				}
				return(LEX_ERROR);


			case 10:				
				if (isxdigit(Char)) {
					Char = (char)toupper(Char); 
					intValue = (intValue << 4) + ((Char >= 'A') ? Char - 'A' + 10 : Char - '0');					
					break;
				}
			

			case 11:
				if(isdigit(Char))
				{
					intValue = intValue * 10 + Char - '0';
					floatValue = floatValue * 10.0 + Char - '0';
					break;
				}
				if(Char == '.')
				{
					State = 12;
					break;
				}
				if(isalpha(Char))
					return (LEX_ERROR);					

				ungetChar(Char); 		// nalezen integer 
				cout << "intValue = " << intValue << endl;
				return (LEX_INT);				
			

			case 12:
				if(isdigit(Char))
				{
					floatValue += DmsPos * (Char - '0'); 
					DmsPos /= 10.0; 	// pocitat intValue
					break;
				}
				if(Char == 'E' || Char == 'e')
				{	
					Exp = 0;
					ExpSign = 1;
					State = 13;
					break;
				}
				if(isalpha(Char))
					return (LEX_ERROR);

				ungetChar(Char); 		//desetinne cislo s jednim destinnym mistem
				cout << "floatValue = " << floatValue << endl;
				return (LEX_FLOAT);

			
			case 13:
				if(isdigit(Char))
				{
					Exp = Char - 0x30;
					State = 15;
					break;
				}
				if (Char == '+' || Char == '-') {
					ExpSign = (Char == '-') ? -1 : 1;						
					State = 14;
					break;
				}
				return (LEX_ERROR);


			case 14:
				if(isdigit(Char))
				{
					Exp = Char - 0x30;
					State = 15;
					break;
				}

				return (LEX_ERROR);

			
			case 15:
				if(isdigit(Char))
				{
					Exp = 10 * Exp + Char - 0x30; //dalsi cifry exponentu
					break;
				}
				
				ungetChar(Char);
				floatValue *= pow(10.0, Exp * ExpSign);
				cout << "floatValue = " << floatValue << endl;
				return (LEX_FLOAT);


			case 16:
				if(isdigit(Char))	
				{		
					floatValue = 0.1 * (Char - '0');
					DmsPos = 0.01; 			//priste jsou zpracovavany setiny
					State = 12;
					break;
				}


			case 17:
				if (Char == '=' || Char == '+') // ==
				{
					if(Char == '=')
					{
						State = 18;
						ungetChar(Char);
						break;
					}
					if (Char == '+')				
					{
						State = 19;
						ungetChar(Char);
						break;
					}
				}
				else
				{
					ungetChar(Char);
					return (LEX_ADD);
				}
			

			case 18:
				if(Char == '=')
					return LEX_ADDASSIGN; 
				else {
					ungetChar(Char); // jenom =
					return LEX_ADD;
				}
		

			case 19:			
				if(Char == '+')
					return (LEX_INCREMENT);
				else
				{	
					ungetChar(Char);
					return (LEX_ADD);
				}
			

			case 20:
				if(Char == '=' || Char == '-')
				{
					if(Char == '=')
					{
						State = 22;
						ungetChar(Char);
						break;
					
					}
					if(Char == '-')
					{
						State = 21;
						ungetChar(Char);
						break;
					}
				}
				else
				{
					ungetChar(Char);
					return (LEX_SUB);
				}


			case 21:
				if(Char == '-')				
					return (LEX_DECREMENT);
				else
				{
					ungetChar(Char);
					return (LEX_SUB);
				}	
			
			case 22:
				if(Char == '=')
					return (LEX_SUBASSIGN);
				else
				{
					ungetChar(Char);
					return (LEX_SUB);
				}				

			
			case 25:
				if(Char == '=')
					return (LEX_MULASSIGN);
				else
				{
					ungetChar(Char);
					return (LEX_MUL);	
				}

			case 27:
				if(Char == '=')
					return (LEX_MODASSIGN);
				else
				{	
					ungetChar(Char);
					return (LEX_MOD);
				}

			
			case 29:
				if(Char == '=')
					return (LEX_ISEQUAL);
				else
				{
					ungetChar(Char);
					return (LEX_ASSIGN);
				}
		
			case 31:
				if(Char == '=')
					return (LEX_ISNEQUAL);
				else
				{
					ungetChar(Char);
					return (LEX_EXCLAMATION);
				}

			case 33:
				if(Char == '=')
					return (LEX_LESS_EQU_THAN);
				else
				{
					ungetChar(Char);
					return (LEX_LESS_THAN);
				}

			case 35:
				if(Char == '=')
					return (LEX_GREATER_EQU_THAN);
				else
				{
					ungetChar(Char);
					return (LEX_GREATER_THAN);
				}
			
			case 38:
				if(isalpha(Char) || isdigit(Char))
				{
					identifierName += Char;				
					break;
				} else {				

					ungetChar(Char);
					map<int, string>::iterator i;

					for (i = keywords.begin(); i != keywords.end(); i++)
						if (identifierName.compare(i->second) == 0)
						{
							cout << "keyword: " << i->second << endl;
							return (i->first);
						}

					//identifikator nebyl nalezen v klicovych slovech
					cout << "identifier: " << identifierName << endl;
					return (LEX_IDENT);
				}

			default:
				throw "Lexical ERROR: unknown state";
				break;
		}
	}
}
			

char Lexan::getChar()
{
	char c;

	if(lastCharValid) 
	{
		lastCharValid = false;
		c = lastChar;
	} 
	else if (!fin->get(c))
	{
		c = eof;
	}

	if(c != '\n')	
		column++;
	else
	{
		lastColumn = column;
		column = 1;
		row++;
	}

	return c;
}



void Lexan::ungetChar(char c)
{
	if(c != '\n')	
		column--;
	else
	{
		column = lastColumn;
		row--;
	}
	
	lastChar = c;
	lastCharValid = true;
}



bool Lexan::isodigit(char X) {
	return (X >= '0' && X <= '7');
}


bool Lexan::isxdigit(char X) {
	return (X >= '0' && X <= '9') 
		|| (X >= 'A' && X <= 'F')
		|| (X >= 'a' && X <= 'f');
}


bool Lexan::isdigit(char X) {
	return (X >= '0' && X <= '9');
}


bool Lexan::isalpha(char X) {
	return (X >= 'A' && X <= 'Z') || (X >= 'a' && X <= 'z');
}




int main(int argc, char** argv)
{

	if (argc != 2 )
	{
		cout << "Usage: " <<  argv[0] << " <SourceFile.c>" << endl;
		return (1);
	}

	Parser* par = new Parser(argv[1]);

	Node* root = par->Program();	
	std::cout << "\n------- RUN -------\n";
	root->Execute();

	delete par;
	delete root;	
	return 0;
}







