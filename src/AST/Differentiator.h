#ifndef DIFFERENTIATOR_H_INCLUDED
#define DIFFERENTIATOR_H_INCLUDED


#include <unordered_map>

#include "Visitor.h"
#include "../Context.h"

class Differentiator : public VisitorExpr
{
public:
	Differentiator (const DC::Context &con)
		: context (con)
	{
	}

	ExpressionNode *visit (NumberNode *)   override;
	ExpressionNode *visit (VariableNode *) override;
	ExpressionNode *visit (BinOpNode *)    override;
	ExpressionNode *visit (FunctionNode *) override;

	static ExpressionNode *differentiate (ExpressionNode *pnode, const DC::Context &context);

private:
	const DC::Context& context;
};


#endif // !DIFFERENTIATOR_H_INCLUDED