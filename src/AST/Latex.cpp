#include "Latex.h"
#include "Expression.h"

void LatexExpressionFormater::visit (NumberNode *pnode)
{
    if (pnode) expression += std::to_string (pnode->val);
}

void LatexExpressionFormater::visit (VariableNode *pnode) 
{
    if (pnode) expression += (pnode->name);
}

void LatexExpressionFormater::visit (BinOpNode *pnode)    
{
    if (pnode)
    {
        auto prev_op = ops.top();
        ops.push (pnode->op);

        if (pnode->op < prev_op) expression += "\\left("; 
        switch (pnode->op)
        {                   
        case Operation::add:
            pnode->left-> doAction (this);
            expression += " + ";
            pnode->right->doAction (this);
            break;

        case Operation::sub:
            pnode->left-> doAction (this);
            expression += " - ";
            pnode->right->doAction (this);
            break;

        case Operation::mul:                      
            pnode->left-> doAction (this);
            expression += " \\times ";
            pnode->right->doAction (this);
            break;

        case Operation::div:                      
            expression += " \\cfrac { ";
            ops.push (Operation (0)); // numerator don't need any priority operation parens
            pnode->left-> doAction (this);         
            ops.pop();                // it's already have figure braces {}
            expression += " } ";
                                                              
            expression += " { ";
            ops.push (Operation (0));
            pnode->right->doAction (this);                      
            ops.pop();
            expression += " } ";
            break;
        
        default:
            pnode->left-> doAction (this);
            expression += " NOP ";
            pnode->right->doAction (this);
            break;
        }
        if (pnode->op < prev_op) expression += "\\right)"; 
        
        ops.pop();
    }
}

void LatexExpressionFormater::visit (FunctionNode *pnode)
{
    if (pnode) 
    {        
        expression += ::to_string (pnode->func) + "(";
        pnode->arg->doAction (this);
        expression += ")";
    }
}

std::string LatexExpressionFormater::to_string (ExpressionNode *AST)
{
    static LatexExpressionFormater converter;
    if (AST)
    {
        AST->doAction (&converter);
        return converter.getExpr();
    }
    
    return "";
}