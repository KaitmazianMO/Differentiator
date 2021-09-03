#ifndef DIFFERENTIATOR_H_INCLUDED
#define DIFFERENTIATOR_H_INCLUDED


#include "Visitor.h"

class Differentiator : public VisitorExpr
{
public:
	ExpressionNode *visit (NumberNode *)   override;
	ExpressionNode *visit (VariableNode *) override;
	ExpressionNode *visit (BinOpNode *)    override;
	ExpressionNode *visit (FunctionNode *) override;

	static ExpressionNode *differentiate (ExpressionNode *);
};


#endif // !DIFFERENTIATOR_H_INCLUDED