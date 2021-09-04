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
        {
            FunctionNode *sin = new FunctionNode (Function::sin, pnode->arg->deepCopy());
            ExpressionNode *diff_arg_with_minus = 
                new BinOpNode (new NumberNode (-1),
                Operation::mul,
                sin->arg->getAction (this)
            );
            return new BinOpNode (diff_arg_with_minus, Operation::mul, sin);
        }
        case Function::tg:
        {
            auto cos = new FunctionNode (Function::cos, pnode->arg->deepCopy());
            auto denomerator = new BinOpNode (cos, Operation::mul, cos);
            auto numerator = pnode->arg->getAction (this);
            return new BinOpNode (numerator, Operation::div, denomerator);
            break;
        }
        
        case Function::ln:
        {
            auto denomerator = pnode->arg->deepCopy();
            auto numerator   = pnode->arg->lightCopy()->getAction (this);
            return new BinOpNode (numerator, Operation::div, denomerator);
            break;
        }
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