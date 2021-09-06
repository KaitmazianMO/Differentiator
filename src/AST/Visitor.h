#ifndef VISITOR_H_INCLUDED
#define VISITOR_H_INCLUDED

struct ExpressionNode;
struct NumberNode;
struct VariableNode;
struct BinOpNode;
struct FunctionNode;

/*
 @todo: Only one visitor (void)	with virtual method for return value if it's necessary
*/

class VisitorVoid
{
public:
	virtual ~VisitorVoid() = default;
	
	virtual void visit (NumberNode *)   = 0;
	virtual void visit (VariableNode *) = 0;
	virtual void visit (BinOpNode *)    = 0;
	virtual void visit (FunctionNode *) = 0;
};

class VisitorExpr
{															  
public:														  
	virtual ~VisitorExpr() = default;						  
															  
	virtual ExpressionNode *visit (NumberNode *)   = 0;		  
	virtual ExpressionNode *visit (VariableNode *) = 0;		  
	virtual ExpressionNode *visit (BinOpNode *)    = 0;		  
	virtual ExpressionNode *visit (FunctionNode *) = 0;		  												  
};		

class VisitorOptimizer
{
	virtual ~VisitorOptimizer() = default;						  
															  
	virtual void visit (NumberNode **)   = 0;		  
	virtual void visit (VariableNode **) = 0;		  
	virtual void visit (BinOpNode **)    = 0;		  
	virtual void visit (FunctionNode **) = 0;		  												  
};

#endif