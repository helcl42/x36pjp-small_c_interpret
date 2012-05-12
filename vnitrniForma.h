#include <iostream>

//Abstract
class Node
{
public:
	Node* next;	
	
	virtual ~Node() {}
	virtual void Execute() = 0;
};



class Express 
{   
public:
	Express* right;

 	virtual ~Express() {}
  	virtual double eval() = 0;
	virtual double set(double) = 0;  
};




class CompoundStatm : public Node
{
private:	
	Node*  	first;
	
public:
	CompoundStatm(Node* a, Node* n)
	{
		this->first = a;
		this->next = n;
	}
	
	~CompoundStatm()
	{
		delete this->first;
		delete this->next;
	}	

	void Execute()
	{
		if(first)
			first->Execute();		
		if(next)
			next->Execute();
	}
};



class Printf : public Node
{
private:	
	Express* 	attr;

public:

	Printf(Express* attr, Node* nn)
	{
		this->attr = attr;
		this->next = nn;
	}

	~Printf()
	{
		delete this->next;
		delete this->attr;
	}

	void Execute()
	{
		double res = attr->eval();		
		std::cout << "Printf: "<< res << std::endl;
		if(next)
			next->Execute();
	}
};



class Condition : public Express
{
private:

  	Express* 	left;
  	int 		opId;
  
public:

	Condition(int op, Express* left, Express* right)  
	{
		this->left 	= left;
		this->right = right;    
		this->opId 	= op;		
	}
  
	~Condition()
	{
		delete left;
		delete right;    
	}
  
	double eval()
	{    	    
		switch (opId)  
		{	
			case LEX_GREATER_THAN:
				if(left->eval() > right->eval())	
					return 1;
				else 
					return 0;

			case LEX_LESS_THAN:
				if(left->eval() < right->eval())
					return 1;
				else
					return 0;

			case LEX_LESS_EQU_THAN:
				if(left->eval() <= right->eval())
					return 1;
				else
					return 0;

			case LEX_GREATER_EQU_THAN:
				if(left->eval() >= right->eval())
					return 1;
				else
					return 0;                       

			case LEX_ISEQUAL:
				if(left->eval() == right->eval())
					return 1;
				else
					return 0;

			case LEX_ISNEQUAL:            
				if(left->eval() != right->eval())
					return 1;
				else
					return 0; 
			default:
				return 0;
    		}
  	}
	
	double set(double)
	{
	}    
};



class IfStatm : public Node
{  
private:

	Express* 	cond;
	Node* 	ifbr;
	Node* 	elsebr;

public:
  
	IfStatm(Node* ifbr, Node* elsebr, Express* cond, Node* next)
	{
		this->cond 	 = cond;
		this->ifbr	 = ifbr;
		this->elsebr = elsebr;
		this->next 	 = next;
	}
  
	~IfStatm()
	{
		delete cond;
		delete ifbr;
		delete elsebr;
		delete next;  
	}
  
	void Execute()
	{
		if(cond->eval())
		{
      		ifbr->Execute();		
		}
		else
		{
      		if(elsebr)
			{	
				elsebr->Execute();				
			}
    	}

		if (next)
		{
      		next->Execute();
		}
  	}
};



class While : public Node
{  
private:
	Express* 	cond;
	Node* 	whBody;
  
public:

  	While(Node* wh, Express* cond, Node* next)
  	{
    		this->cond   = cond;
    		this->whBody = wh;
    		this->next   = next;
  	}
  
	~While()
	{
		delete whBody;
		delete cond;  
		delete next;
	}
  
	void Execute()
	{
		while(cond->eval())
      		whBody->Execute();
		if (next)
      		next->Execute();      
  	}
};


class For : public Node
{  
private:

	Express*	decl;
	Express* 	cond;
	Express*	exp;
	Node* 	forBody;	
  
public:

  	For(Express* decl, Express* cond, Express* exp, Node* fr, Node* next)
  	{
		this->decl = decl; 
		this->cond = cond;
		this->exp = exp;
    	this->forBody = fr;
    	this->next = next;
  	}
  
	~For()
	{
		delete forBody;
		delete next;
		delete decl;
		delete cond;
		delete exp;  
	}
  
	void Execute()
	{	
		for(decl->eval(); cond->eval(); exp->eval())
      		forBody->Execute();
		if (next)
      		next->Execute();      
  	}
};



class Expression : public Node
{
private:
	Express* exp;

public:
	Expression(Express* ex, Node* nd)
	{
		this->next = nd;
		this->exp  = ex;
	}

	~Expression()
	{
		delete this->exp;
		delete this->next;
	}

	void Execute()
	{
		exp->eval();
	
		if(next)
			next->Execute();		
	}
};



class Var : public Express
{
private:

	double* pVal;

public:

	Var(double* nic)
	{
		this->pVal = nic;
	}

	~Var(){}
	
	double eval()	
	{
		double post = 0;
		
		if(right)
			post = right->eval();
		return *pVal += post;
	}

	double set(double nic)
	{
		*pVal = nic;
	}
};



class Const : public Express
{
private:		
	double value;

public:

	Const(double nic)
	{
		this->value = nic;		
	}
	~Const(){}
	
	double eval()
	{
		return value;		
	}

	double set(double val)
	{
	}
};



class Assign : public  Express
{
private:
	Express* left;
	
public:	

	Assign(Express* left, Express* right)
	{
		this->left = left;
		this->right = right;
	}
	
	~Assign()
	{
		delete this->left;
		delete this->right;
	}

	double eval()
	{	
		return left->set(right->eval());
	}	

	double set(double d) 
	{
	}
};



class UnMinus : public Express
{
public:

	UnMinus(Express* right)
	{	
		this->right = right;
	}
	
	~UnMinus()
	{
		delete this->right;
	}

	double eval()
	{
		return (right->eval()) * -1;
	}

	double set(double val)
	{
	}
};



class BinOp : public Express
{
private:
	Express* left;
	int 	 opId;

public:
	
	BinOp(int tokenId, Express* left, Express* right)
	{		
		this->left 	= left;
		this->right = right;				
		this->opId 	= tokenId;
	}

	~BinOp()
	{
		delete this->left;
		delete this->right;
	}

	double eval()
	{
		switch(opId)
		{
			case LEX_ADD:
				return (left->eval() + right->eval());
			case LEX_SUB:
				return (left->eval() - right->eval());
			case LEX_MUL:
				return (left->eval() * right->eval());
			case LEX_DIV:
				return (left->eval() / right->eval());
			default:
				return(-1); ////tohle by se memelo stat
		}
	}	
	
	double set(double d) 
	{
	}
};



class Post : public Express
{
private:
	int tokId;

public:
	Post(int tokId, Express* right)
	{
		this->right = right;	
		this->tokId = tokId;
	}

	~Post()
	{
		delete this->right;		
	}
	

	double eval()
	{
		double old =  (right->eval());
		double newVal = 0;
		switch(tokId)
		{
			case LEX_INCREMENT:
				newVal = old;
				right->set(++newVal);
				return old;

			case LEX_DECREMENT:
				newVal = old;
				right->set(++newVal);
				return old;

			default:
				std::cout << "ERROR: Post in/de_crement." << std::endl;
				return (-1);//by nemelo nastat
		}
	}
	
	double set(double d) 
	{
	}
};



class Pre : public Express
{
private:

	int opId;

public:

	Pre(int op, Express* var)
	{
		this->right = var;
		this->opId 	= op; 
	}

	~Pre()
	{
		delete right;
	}

	double eval()
	{
	
		double old = (right->eval());
		switch(opId)
		{
			case LEX_INCREMENT:
				right->set(++old);
				return old;

			case LEX_DECREMENT:
				right->set(--old);
				return old;

			default:
				std::cout << "ERROR: Pre-in/de_crement" << std::endl;
				return (-1);				
		}
	}
		
	double set(double n)
	{
	}
};


