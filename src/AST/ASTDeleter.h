#ifndef AST_DELETER_H_INCLUDED
#define AST_DELETER_H_INCLUDED


#include "Visitor.h"

class ASTDeleter : public VisitorVoid
{
public:
	ASTDeleter() = default;
   ~ASTDeleter() = default;

	void visit (NumberNode *)   override;
	void visit (VariableNode *) override;
	void visit (BinOpNode *)    override;
	void visit (FunctionNode *) override;

	static void free (ExpressionNode *);
};


#endif