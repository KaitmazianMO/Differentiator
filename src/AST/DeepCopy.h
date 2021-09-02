#ifndef DEEP_COPY_H_INCLUDED
#define DEEP_COPY_H_INCLUDED


#include "Visitor.h"

class DeepCopy : public VisitorExpr
{
	ExpressionNode *visit (NumberNode *)   override;
	ExpressionNode *visit (VariableNode *) override;
	ExpressionNode *visit (BinOpNode *)    override;
	ExpressionNode *visit (FunctionNode *) override;
};


#endif // !DEEP_COPY_H_INCLUDED
