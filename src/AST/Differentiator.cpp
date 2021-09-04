#include "Differentiator.h"
#include "Expression.h"

ExpressionNode *Differentiator::visit (NumberNode *pnode)  
{
    return new NumberNode (0);
}

ExpressionNode *Differentiator::visit (VariableNode *pnode)
{
    if (pnode && context.isDiffVar (pnode->name))
        return new NumberNode (1);

    return new NumberNode (0);
}
                                
#define DIFF_LNC  (pnode->left->lightCopy()->getAction (this))
#define DIFF_RNC  (pnode->right->lightCopy()->getAction (this))
#define LNC       (pnode->left->deepCopy())
#define RNC       (pnode->right->deepCopy())

ExpressionNode *Differentiator::visit (BinOpNode *pnode)   
{
    if (pnode)
    {
        switch (pnode->op)
        {                   
        case Operation::add:
        case Operation::sub:
            return new BinOpNode (DIFF_LNC, pnode->op, DIFF_RNC);
        
        case Operation::mul:
        {
            BinOpNode *L = new BinOpNode (DIFF_LNC, Operation::mul, RNC);  
            BinOpNode *R = new BinOpNode (LNC, Operation::mul, DIFF_RNC);   
            return new BinOpNode (L, Operation::add, R);
        }
        case Operation::div: // (L'R - LR') / R^2 = D / N                                                                                                       
        {
            BinOpNode *L = new BinOpNode (DIFF_LNC, Operation::mul, RNC);  
            BinOpNode *R = new BinOpNode (LNC, Operation::mul, DIFF_RNC);   
            BinOpNode *D = new BinOpNode (L, Operation::sub, R);               
            BinOpNode *N = new BinOpNode (RNC, Operation::mul, RNC);               
            return new BinOpNode (D, Operation::div, N);
        }
        }
    }
    return nullptr;
}

#undef DIFF_LNC  
#undef DIFF_RNC  
#undef LNC       
#undef RNC       

ExpressionNode *Differentiator::visit (FunctionNode *pnode)
{
    if (pnode)
    {
    switch (pnode->func)
    {                   
        case Function::sin:
        {
            FunctionNode *cos = new FunctionNode (Function::cos, pnode->arg->deepCopy());
            ExpressionNode *diff_arg = cos->arg->getAction (this);
            return new BinOpNode (diff_arg, Operation::mul, cos);
        }
        case Function::cos:
        case Function::tg:
        case Function::ln:
        default:
            return nullptr; 
        }
    }
    return nullptr;
}

ExpressionNode *Differentiator::differentiate (ExpressionNode *pnode, const Context &context)
{
    Differentiator differentiator (context);
    if (pnode)
    {
        return pnode->getAction (&differentiator);
    }

    return nullptr;
}