#ifndef DIFFERENTIATOR_H_INCLUDED
#define DIFFERENTIATOR_H_INCLUDED

/*
 * 
 * @bug When copy in mull differantiating we copy ekft and right and don't call differantiating function
 * 
 */

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