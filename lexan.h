#include <fstream>
#include <iostream>
#include <cstring>
#include <vector>
#include <map>
#include <cmath>
#include "vnitrniForma.h"


#ifndef LEXAN_H
#define LEXAN_H

using namespace std;




class Lexan
{
private:

	ifstream* 		fin;
	char* 		fileName;
	int 			row;
	int 			column;
	const char		eof;
	int 			intValue;
	double		floatValue;
	string		identifierName;
	
	char			lastChar;
	bool			lastCharValid;
	int			lastColumn;
	map<int, string>	keywords;	
	map<int, string>  tokens;

public:

	Lexan(char* filename);
	~Lexan()		{ delete fin; delete fileName; }

	char			getChar();
	void			ungetChar(char);
	int 			getNextToken();

	bool 			isodigit(char X);
	bool 			isxdigit(char X);
	bool		 	isdigit(char X);
	bool		 	isalpha(char X);

	void			initKeywords();
	void			initTokens();
	int 			getIntValue()	{ return this->intValue; }
	double		getFloatValue()	{ return this->floatValue; }
	string		getIdentName()	{ return this->identifierName; }

	int 			getColumn()		{ return this->column; }
	int 			getRow()		{ return this->row; }

	string		tokenToString(int tokNum);
	

};



#endif
