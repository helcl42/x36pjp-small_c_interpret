#include "tokens.h"
#include "lexan.h"
#include "parser.h"
#include <cstring>


using namespace std;




Parser::Parser(char* filename)
{
	lexan = new Lexan(filename);
	previousToken = LEX_NULL;
	usePrevious = false;
	token = lexan->getNextToken();
}


int Parser::Show()
{
	
	if(usePrevious)
	{
		token = previousToken;
		usePrevious = false;
	}
	else 
	{
		this->previousToken = this->token;	
		this->token = lexan->getNextToken();		
	}

	return this->token;
}


int Parser::ShowPrevious()
{
	this->previousToken = token;
	return this->token;
}


bool Parser::Compare(int lexTok)
{
	if(lexTok == token)
	{
	//	cout << "srovnani: " << lexan->tokenToString(lexTok) << " s " << lexan->tokenToString(token) << endl;
		return true;
	}
	else	
	{
		cout << "ERROR: Expected... " << lexan->tokenToString(lexTok) << " instead of " << lexan->tokenToString(token) << endl;	
		return false;
	}
}


bool Parser::VariableExist(string idVar)
{				
	if (declTab.find(idVar) != declTab.end())
	{
		cout << "Variable with name "<< idVar << " already EXIST." << endl;
		return true;
	} 
	else
		return false;			
}


double* Parser::getPointerToVar(string varName)
{	
	
	if (declTab.find(varName) == declTab.end())
	{			
		cout << "ERROR: usage of not declared. value >> "<< declTab[varName]<< endl;
	} 
	return &declTab[varName];
}


void Parser::Error(string msg)
{
	cout << "ERROR: Syntax error on line: " << lexan->getRow() << " from " << msg << endl;
}



int Parser::relationalOperator()
{
    switch(Show())
    {
        case LEX_GREATER_THAN:
            return LEX_GREATER_THAN;

        case LEX_LESS_THAN:
            return LEX_LESS_THAN;

        case LEX_LESS_EQU_THAN:
            return LEX_LESS_EQU_THAN;

        case LEX_GREATER_EQU_THAN:
            return LEX_GREATER_EQU_THAN;

        case LEX_ISEQUAL:
            return LEX_ISEQUAL;

        case LEX_ISNEQUAL:           
            return LEX_ISNEQUAL;
    }
}



Express* Parser::Podminka()
{
	Express* 	exp1;
	Express* 	exp2;
	int 		op;

	exp1 = Vyraz();				
	op = relationalOperator();
	exp2 = Vyraz();
	return new Condition(op, exp1, exp2);
}




Express* Parser::Deklarace()
{
	if(!VariableExist(lexan->getIdentName()))
	{
		declTab.insert(make_pair(lexan->getIdentName(), 0.0));
//				declTab[lexan->getIdentName()] = 0;			
		return new Var(getPointerToVar(lexan->getIdentName()));
	}
	else
	{
		Error("DEKLARACE");
		return NULL;			
	}
}


Express* Parser::Faktor()
{
	Express* ret;
 
	switch(Show())
	{
    		case LEX_IDENT:
		      Compare(LEX_IDENT);			
			if(VariableExist(lexan->getIdentName()))
	      		return IncDec(new Var(getPointerToVar(lexan->getIdentName())));		

    		case LEX_INCREMENT:
		      Compare(LEX_INCREMENT);
			Show();
			Compare(LEX_IDENT);
			if(VariableExist(lexan->getIdentName()))
	      		return IncDec(new Pre(LEX_INCREMENT, new Var(getPointerToVar(lexan->getIdentName()))));

   		case LEX_DECREMENT:
		      Compare(LEX_DECREMENT);
			Show();
			Compare(LEX_IDENT);
			if(VariableExist(lexan->getIdentName()))
	      		return IncDec(new Pre(LEX_DECREMENT, new Var(getPointerToVar(lexan->getIdentName()))));

		case LEX_INT:
		      Compare(LEX_INT);
		      return new Const(lexan->getIntValue());

		case LEX_FLOAT:
		      Compare(LEX_FLOAT);
		      return new Const(lexan->getFloatValue());

		case LEX_LPA:
			Compare(LEX_LPA);
		      ret = Vyraz();
		      Show();
		      Compare(LEX_RPA);
		      return ret;

		default: 
			Error("FAKTOR");
			usePrevious = true;			
			ShowPrevious();
//			exit(EXIT_FAILURE);
			return NULL;
	}
}



Express* Parser::Term()
{
	return ZbytekTermu(Faktor());
}


Express* Parser::ZbytekTermu(Express* exp)
{
	switch(Show())
  	{
    		case LEX_DIV:
      		Compare(LEX_DIV);
      		return ZbytekTermu(new BinOp(LEX_DIV, exp, Faktor()));

    		case LEX_MUL:
      		Compare(LEX_MUL);
      		return ZbytekTermu(new BinOp(LEX_MUL, exp, Faktor()));

    		default:
			usePrevious = true;
			ShowPrevious();		
      		return exp;
	}  
}


Express* Parser::ZbytekVyrazu(Express* exp)
{	
	switch(Show())
	{
		case LEX_ADD:
			Compare(LEX_ADD);
			return ZbytekVyrazu(new BinOp(LEX_ADD, exp, Term()));				

		case LEX_SUB:
			Compare(LEX_SUB);
			return ZbytekVyrazu(new BinOp(LEX_SUB, exp, Term()));

		default:
			usePrevious = true;
			ShowPrevious();
			return exp;
	}
}



Express* Parser::IncDec(Express* exp)
{
	switch(Show())
	{
		case LEX_INCREMENT:
			Compare(LEX_INCREMENT);
			return new Post(LEX_INCREMENT, exp);

		case LEX_DECREMENT:
			Compare(LEX_DECREMENT);
			return new Post(LEX_DECREMENT, exp);

		default:			
			usePrevious = true;
			ShowPrevious();
			return exp;
	}	
}



Express* Parser::Vyraz1(Express* exp)
{
	switch (Show()) 
  	{
    		case LEX_ASSIGN:
      		Compare(LEX_ASSIGN);
	      	return new Assign(exp, Vyraz());

		default:	
			usePrevious = true;		
			ShowPrevious();
		      return ZbytekVyrazu(ZbytekTermu(IncDec(exp)));			
  	}
}



Express* Parser::Vyraz()
{
	Express* tmp;

	switch(Show())
	{		
		case LEX_IDENT:
			Compare(LEX_IDENT);	
			return Vyraz1(new Var(getPointerToVar(lexan->getIdentName())));
			//return new Var(getPointerToVar(lexan->getIdentName()));
		
		case LEX_INCREMENT:
			Compare(LEX_INCREMENT);		
			Show();
			Compare(LEX_IDENT);			
			return ZbytekVyrazu(ZbytekTermu(new Pre(LEX_INCREMENT, new Var(getPointerToVar(lexan->getIdentName())))));
	
		case LEX_DECREMENT:
			Compare(LEX_DECREMENT);					
			Show();
			Compare(LEX_IDENT);	
			return ZbytekVyrazu(ZbytekTermu(new Pre(LEX_DECREMENT, new Var(getPointerToVar(lexan->getIdentName())))));

		case LEX_INT:	
			Compare(LEX_INT);			
			return ZbytekVyrazu(ZbytekTermu(new Const(lexan->getIntValue())));;

		case LEX_FLOAT:
			Compare(LEX_FLOAT);
			return ZbytekVyrazu(ZbytekTermu(new Const(lexan->getFloatValue())));

		case LEX_SUB:
			Compare(LEX_SUB); 
			return ZbytekVyrazu( new UnMinus(Term()));	

		case LEX_LPA:
			Compare(LEX_LPA);
			tmp = Vyraz();
			Show();
			Compare(LEX_RPA);
			return ZbytekVyrazu(ZbytekTermu(tmp));						
			
		default:
			Error("Prikaz");
			usePrevious = true;
			ShowPrevious();
			return NULL;							
	}
}



Node* Parser::BlokPrikazu()
{
	Node* prik = Prikaz();
	Show();
	Compare(LEX_SEMICOLON);
	return prik;	
}


Node* Parser::Prikaz()
{
	Node* 	tmp;
	Node* 	tmp1;
	Express* 	exp;
	Express*	exp1;
	Express*	exp2;
	

	switch(Show())
	{
		case LEX_BEGIN:
			Compare(LEX_BEGIN);
			tmp = Prikaz();
			Show();		
			Compare(LEX_END);
			return new CompoundStatm(tmp, Prikaz());
	
		case LEX_K_PRINTF:
			Compare(LEX_K_PRINTF);
			Show();
			Compare(LEX_LPA);
			exp = Vyraz();	
			Show();			
			Compare(LEX_RPA);
			Show();
			Compare(LEX_SEMICOLON);
			return new Printf(exp, Prikaz());

		case LEX_IF:
	            Compare(LEX_IF);
	            Show();
	            Compare(LEX_LPA);
	            exp = Podminka();
            	Show();        
            	Compare(LEX_RPA);

           	 	tmp = Prikaz();
				Show();				//last
            	if(token == LEX_ELSE)
				{
					Show();
                	tmp1 = Prikaz();
				}
            	else
				{
					cout << "bez else" << endl;
					usePrevious = true;
					ShowPrevious();
					Show();
                	tmp1 = NULL;
				}			
            	return new IfStatm(tmp, tmp1, exp, Prikaz());

		case LEX_WHILE:
			Compare(LEX_WHILE);
			Show();
			Compare(LEX_LPA);
			exp = Podminka();
			Show();
			Show();
			Compare(LEX_RPA);
			tmp = Prikaz();
			Show();
			return new While(tmp, exp, Prikaz());

		case LEX_FOR:
			Compare(LEX_FOR);
			Show();				
			Compare(LEX_LPA);
			exp = Vyraz();
			Show();			
			Compare(LEX_SEMICOLON);
			exp1 = Podminka();
			Show();
			Compare(LEX_SEMICOLON);			
			exp2 = Vyraz();
			Show();
			Compare(LEX_RPA);
			tmp = Prikaz();
			Show();
	//		usePrevious = true;
	//		ShowPrevious();		
			return new For(exp, exp1, exp2, tmp, Prikaz());					

		//deklarace
		case LEX_K_INT:
		case LEX_K_FLOAT:
			if(Compare(LEX_K_INT) || Compare(LEX_K_FLOAT))
			{
				Show();
				Compare(LEX_IDENT);
				exp = Deklarace();
				declTab[lexan->getIdentName()] = 0;							
				Show();		
				Compare(LEX_SEMICOLON);
		      return Prikaz();
			}
			else
			Error("Deklarace");

		case LEX_SEMICOLON:
			Compare(LEX_SEMICOLON);
			return Prikaz();		

		case LEX_IDENT:			
		case LEX_INCREMENT:
		case LEX_DECREMENT:
		case LEX_INT:
		case LEX_FLOAT:
		case LEX_SUB:
		case LEX_LPA:
			usePrevious = true;
			ShowPrevious();
			exp = Vyraz();
			Show();
			Compare(LEX_SEMICOLON);
			return new Expression(exp, Prikaz());

		case LEX_NULL:					
			Compare(LEX_NULL);
			return NULL;

		default:
			usePrevious = true;
			ShowPrevious();			
			return NULL;
	
	}
}


Node* Parser::Program()
{
	Node* program;

	switch(token)	
	{
		case LEX_K_VOID:
			Compare(LEX_K_VOID);
			Show();
			Compare(LEX_K_MAIN);
			Show();
			Compare(LEX_LPA);
			Show();
			Compare(LEX_RPA);		
			program = Prikaz();									
			return program;

		default:
			Error("PROGRAM");		
			return NULL;		
	}
}







