#ifndef PARSER_H
#define PARSER_H

#include <map>
#include <cstring>

class Parser
{
private:

	Lexan* 	lexan;
	int 		token;
	int 		previousToken;
	bool		usePrevious;

	bool		Compare(int);
	int 		Show();
	int		ShowPrevious();
	bool  	VariableExist(string);
	void 		Error(string);

public:

	map<string, double> 		declTab;
	map<string, double>::iterator it;
	
	Parser(char*);
	~Parser()		{ delete lexan; }

	Node*			Program();			//startovni neterminal
	Node*			BlokPrikazu();
	Node*  		Prikaz();	
	Express* 		Deklarace();

	Express*		Vyraz();
	Express* 		Vyraz1(Express*);
	Express*		ZbytekVyrazu(Express*);
	Express*		ZbytekTermu(Express*);
	Express* 		Faktor();
	Express*		IncDec(Express*);
	Express*		Term();	
	Express* 		Podminka();
	
	int 			relationalOperator();
	double*		getPointerToVar(string);
	

	void  	PodminkaIf1();
	void  	CyklusWhile();
	void		CyklusFor();
	void 		OpPrirazeni();
	void  	Typ();		
};


#endif


