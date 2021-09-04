#ifndef LATEX_H_INCLUDED
#define LATEX_H_INCLUDED


#include "../VisualStudioWarnings.h"
#include "Visitor.h"
#include "Expression.h"

#include <string>
#include <fstream>
#include <stack>

class LatexExpressionFormater : public VisitorVoid
{
public:
	LatexExpressionFormater() { ops.push (static_cast<Operation> (0)); }; // pushing the lowest priority op for skipping first parens
   ~LatexExpressionFormater() = default;

	void visit (NumberNode *)   override;
	void visit (VariableNode *) override;
	void visit (BinOpNode *)    override;
	void visit (FunctionNode *) override;
																	  
	static std::string to_string (ExpressionNode *AST);
	const std::string &getExpr() const { return expression; };
	std::string       &getExpr()	   { return expression; };

private:
	std::string expression;

	std::stack<Operation> ops;
};


#endif