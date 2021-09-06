#ifndef CALCULATOR_H_INCLUDED
#define CALCULATOR_H_INCLUDED


#include "Visitor.h"
#include "../Context.h"

#include <stack>

class Calculator : public VisitorVoid
{
public:
	Calculator (const DC::Context &con) :
		context (con)
	{
	}

	void visit (NumberNode *)   override;
	void visit (VariableNode *) override;
	void visit (BinOpNode *)    override;
	void visit (FunctionNode *) override;
	
	double getTopValue() { return calc_values.top(); }
	static double calculate (ExpressionNode *AST, const DC::Context& context);
	

private:   
	double getTopAndPop();

	const DC::Context& context;
	std::stack<double> calc_values;	
};


#endif // !CALCULATOR_H_INCLUDED
