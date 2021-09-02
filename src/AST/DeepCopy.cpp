#include "DeepCopy.h"
#include "Expression.h"

ExpressionNode *DeepCopy::visit (NumberNode *pnode)   
{
    return new NumberNode (pnode->val);
}

ExpressionNode *DeepCopy::visit (VariableNode *pnode) 
{                                    
    return new VariableNode (pnode->name, pnode->value);
}

ExpressionNode *DeepCopy::visit (BinOpNode *pnode)    
{
    return new BinOpNode (pnode->left->getAction (this), pnode->op, pnode->right->getAction (this));
}

ExpressionNode *DeepCopy::visit (FunctionNode *pnode)
{                                                                                               
    return new FunctionNode (pnode->func, pnode->arg->getAction (this));
}