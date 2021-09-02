#include "Expression.h"
#include "Ordinary.h"

void OrdinaryExpressionFormater::visit (NumberNode *pnode)
{
    if (pnode) expression += std::to_string (pnode->val);
}

void OrdinaryExpressionFormater::visit (VariableNode *pnode) 
{
    if (pnode) expression += (pnode->name);
}

void OrdinaryExpressionFormater::visit (BinOpNode *pnode)    
{
    if (pnode)
    { 
        auto prev_op = ops.top();
        ops.push (pnode->op);

        if (pnode->op < prev_op) expression += "("; 
        pnode->left->doAction (this);
        expression += ::to_string (pnode->op);
        pnode->right->doAction (this);
        if (pnode->op < prev_op) expression += ")"; 
        
        ops.pop();
    }
}

void OrdinaryExpressionFormater::visit (FunctionNode *pnode)
{
    if (pnode) 
    {        
        expression += ::to_string (pnode->func) + "(";
        pnode->arg->doAction (this);
        expression += ")";
    }
}

std::string OrdinaryExpressionFormater::to_string (ExpressionNode *AST)
{
    static OrdinaryExpressionFormater converter;
    if (AST)
    {
        AST->doAction (&converter);
        return converter.getExpr();
    }
    
    return "";
}