#ifndef OPTIMIZER_H_INCLUDED
#define OPTIMIZER_H_INCLUDED


#include "Visitor.h"
#include <vector>

class ConstantFolder : public VisitorVoid
{
public:
	void visit (NumberNode *)   override;
	void visit (VariableNode *) override;
	void visit (BinOpNode *)    override;
	void visit (FunctionNode *) override;

private:
	std::vector<BinOpNode *> op_const_trace;	// or stack
};


#endif // !OPTIMIZER_H_INCLUDED
