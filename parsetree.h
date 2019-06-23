/*
 * parsetree.h
 */

#ifndef PARSETREE_H_
#define PARSETREE_H_
#include <vector>
#include <map>
#include "parse.h"
#include "value.h"
#include "tokens.h"
using namespace std;

// NodeType represents all possible types
enum NodeType { ERRTYPE, INTTYPE, STRTYPE, BOOLTYPE };
extern void RunTimeError (string);

// a "forward declaration" for a class to hold values
class Value;

//--Added map called SYMBOLS
//static map<string, Value> SYMBOLS;
//static map<string, int> SYMBOLS;

//--
extern map<string, Value> evars;


class ParseTree {
//if it fails, change to public
protected:
	int			linenum;
	ParseTree	*left;
	ParseTree	*right;

public:
	ParseTree(int linenum, ParseTree *l = 0, ParseTree *r = 0)
		: linenum(linenum), left(l), right(r) {}

	virtual ~ParseTree() {
		delete left;
		delete right;
	}

	int GetLinenum() const { return linenum; }

	virtual NodeType GetType() const { return ERRTYPE; }
    virtual bool IsStringConst() const { return false; }
    virtual bool IsIdent() const { return false; }
    virtual bool IsBooler() const { return false; }
    virtual bool getBool() const {return false; }
	virtual string GetId() const { return ""; }

	int LeafCount() const {
		int lc = 0;
		if( left ) lc += left->LeafCount();
		if( right ) lc += right->LeafCount();
		if( left == 0 && right == 0 )
			lc++;
		return lc;
	}


	int IdentCount() const {
		int cnt = 0;
		if( left ) cnt += left->IdentCount();
		if( right ) cnt += right->IdentCount();
		if( IsIdent() )
			cnt++;
		return cnt;
	}

	int StringCount() const {
		int cnt = 0;
		if( left ) cnt += left->StringCount();
		if( right ) cnt += right->StringCount();
		if( IsStringConst() )
			cnt++;
		return cnt;
	}

	void GetVars(map<string,int>& var) {
		if( left ) left->GetVars(var);
		if( right ) right->GetVars(var);
		if( IsIdent() )
			var[ this->GetId() ]++;
	}
    
    //--
    //virtual Value Eval (map <string, Value> SYMBOLS);
    //virtual Value Eval (map <string, Value> &SYMBOLS);
    virtual Value Eval(map<string, Value> &evars) = 0;
};

/*
When declaring new Evars below, this is your template

virtual Value Eval(map <string, Value> &evars)
    {
         
    }
*/      
                       
/*
Evaluate the left, like this: left->Eval(vars)
If there is a right, evaluate like this: right->Eval(vars)
To detect if right exists:
if(right)
{

}
*/
                      
class StmtList : public ParseTree {

public:
	StmtList(ParseTree *l, ParseTree *r) : ParseTree(0, l, r) {}
    
    virtual Value Eval(map <string, Value> &evars)
    {
        left->Eval(evars);
        if(right)
        {
            right->Eval(evars);
        }
        return Value();
    }
};

class IfStatement : public ParseTree {
public:
	IfStatement(int line, ParseTree *ex, ParseTree *stmt) : ParseTree(line, ex, stmt) {}
    
   virtual Value Eval(map <string, Value> &evars)
    {
       Value results = left->Eval(evars);
       if(results.isBoolType() == true)
       {
         if(results.getBoolean())
         {
            return right->Eval(evars);  
         }
       }
        else
        {
            RunTimeError("if expression is not boolean typed");
        }
        return Value();
        //return 0;
    }
    
};

class Assignment : public ParseTree {
public:
	Assignment(int line, ParseTree *lhs, ParseTree *rhs) : ParseTree(line, lhs, rhs) {}
    /*
    Do not use:
    &evars.insert(left->Eval(&evars), right->Eval(&evars));
    
    Instead, use: 
    evars[aa] = bb;
    As explained in:
    http://www.cplusplus.com/reference/map/map/operator[]/
    
    LMAO IGNORE THAT
    */
    virtual Value Eval(map <string, Value> &evars)
    {
        if(left->IsIdent())
        {
           //if(evars.count(left->GetId()) && !evars.empty())
          //{
             Value right1 = right->Eval(evars);
             string left1 = left->GetId();
               
             evars[left1] = right1;
             return right1;
           //}

        }
        else
        {
          RunTimeError("Left hand side must be an identifier");
        }
        return Value();
        /*
        string aa = left->GetId();
        Value bb = right->Eval(evars);
        //http://www.cplusplus.com/reference/map/map/operator[]/
        evars[aa] = bb;
        return Value();   
        */
    }
};

class PrintStatement : public ParseTree {
public:
	PrintStatement(int line, ParseTree *e) : ParseTree(line, e) {}
    virtual Value Eval(map <string, Value> &evars)
    {
	    Value lVal = left->Eval(evars);
        if (lVal.isIntType())
            cout << lVal.getInteger() << endl;
        else if (lVal.isStringType())
            cout << lVal.getString() << endl;
        else if (lVal.isBoolType())
            cout << lVal << endl;
        return Value();

	    /*
      cout << left->Eval(evars) << '\n';
      return Value();
	     */
    }
};

class PlusExpr : public ParseTree {
public:
	PlusExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line,l,r) {}
    
    virtual Value Eval(map <string, Value> &evars)
    {
      return left->Eval(evars) + right->Eval(evars);
    }
};

class MinusExpr : public ParseTree {
public:
	MinusExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line,l,r) {}
    
    virtual Value Eval(map <string, Value> &evars)
    {
      return left->Eval(evars) - right->Eval(evars);
    }
};

class TimesExpr : public ParseTree {
public:
	TimesExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line,l,r) {}
    
    virtual Value Eval(map <string, Value> &evars)
    {
      return left->Eval(evars) * right->Eval(evars);
    }
};

class DivideExpr : public ParseTree {
public:
	DivideExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line,l,r) {}
    
    virtual Value Eval(map <string, Value> &evars)
    {
      return left->Eval(evars) / right->Eval(evars);
    }
};

class LogicAndExpr : public ParseTree {
public:
	LogicAndExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line,l,r) {}

	//NodeType GetType() const { return BOOLTYPE; }
    
     virtual Value Eval(map <string, Value> &evars)
    {
      Value left1 = left->Eval(evars);
      Value right1 = right->Eval(evars);
      if (left1.isBoolType() && right1.isBoolType())
      {
          return left1.isTrue() && right1.isTrue();
      }
      else
      {
        RunTimeError("first operand of && is not boolean typed");
        //return Value();  
      }
        return Value();
    }

    /*
    if(left && right)
      {
         return true;
      }
      else
      {
          return false;    
      }
       return Value();
    */
};

class LogicOrExpr : public ParseTree {
public:
	LogicOrExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line,l,r) {}

	//NodeType GetType() const { return BOOLTYPE; }
    
    virtual Value Eval(map <string, Value> &evars)
    {
      Value left1 = left->Eval(evars);
      Value right1 = right->Eval(evars);
      if (left1.isBoolType() || right1.isBoolType())
      {
          return left1.isTrue() || right1.isTrue();
      }
      else
      {
        RunTimeError("first operand of || is not boolean typed");
        //return Value();  
      }
        return Value();
    }
};

class EqExpr : public ParseTree {
public:
	EqExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line,l,r) {}

	//NodeType GetType() const { return BOOLTYPE; }
    virtual Value Eval(map <string, Value> &evars)
    {
      return left->Eval(evars) == right->Eval(evars);
    }
};

class NEqExpr : public ParseTree {
public:
	NEqExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line,l,r) {}

   //NodeType GetType() const { return BOOLTYPE; }
    
   virtual Value Eval(map <string, Value> &evars)
    {
      return left->Eval(evars) != right->Eval(evars);
    }
};

class LtExpr : public ParseTree {
public:
	LtExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line,l,r) {}

	//NodeType GetType() const { return BOOLTYPE; }
   virtual Value Eval(map <string, Value> &evars)
    {
      return left->Eval(evars) < right->Eval(evars);
    }
};

class LEqExpr : public ParseTree {
public:
	LEqExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line,l,r) {}

	//NodeType GetType() const { return BOOLTYPE; }
   virtual Value Eval(map <string, Value> &evars)
    {
      return left->Eval(evars) <= right->Eval(evars);
    }
};

class GtExpr : public ParseTree {
public:
	GtExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line,l,r) {}

	//NodeType GetType() const { return BOOLTYPE; }
    virtual Value Eval(map <string, Value> &evars)
    {
      return left->Eval(evars) > right->Eval(evars);
    }
};

class GEqExpr : public ParseTree {
public:
	GEqExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line,l,r) {}

	//NodeType GetType() const { return BOOLTYPE; }
    virtual Value Eval(map <string, Value> &evars)
    {
      return left->Eval(evars) >= right->Eval(evars);
    }
};

class IConst : public ParseTree {
	int val;

public:
	IConst(int l, int i) : ParseTree(l), val(i) {}
	IConst(Token& t) : ParseTree(t.GetLinenum()) {
		val = stoi(t.GetLexeme());
	}

	NodeType GetType() const { return INTTYPE; }

    virtual Value Eval(map <string, Value> &evars)
    {
      return Value(val);
    }
};

class BoolConst : public ParseTree {
	bool val;

public:
	BoolConst(Token& t, bool val) : ParseTree(t.GetLinenum()), val(val) {}

	NodeType GetType() const { return BOOLTYPE; }
    
   bool IsBooler() const { return true; }
   bool getBool() const {return val; }

   virtual Value Eval(map <string, Value> &evars)
    {
      return Value(val);
    }
};

class SConst : public ParseTree {
	string val;

public:
	SConst(Token& t) : ParseTree(t.GetLinenum()) {
		val = t.GetLexeme();
	}

	NodeType GetType() const { return STRTYPE; }
	bool IsStringConst() const { return true; }
    
   virtual Value Eval(map <string, Value> &evars)
    {
      return Value(val);
    }
};

class Ident : public ParseTree {
	string id;

public:
	Ident(Token& t) : ParseTree(t.GetLinenum()), id(t.GetLexeme()) {}

	bool IsIdent() const { return true; }
	string GetId() const { return id; }
    
    virtual Value Eval(map <string, Value> &evars)
    {
      if(!evars.empty() && evars.count(id))
      {
        return evars[id];
      }
      else
      {
       RunTimeError("Variable is used before being set");
          return Value();
      }
        return Value();
    }
    /*
    //FINISH THIS
    Value Eval(map <string, Value> &evars)
    {
      return Value();
    }
    */
};

#endif /* PARSETREE_H_ */