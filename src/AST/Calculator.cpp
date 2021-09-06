#include <stdexcept>

#include "Calculator.h"
#include "Expression.h"

void Calculator::visit (NumberNode *pnode)   
{
    if (pnode)
    {
        calc_values.push (pnode->val);
    } 
    else
    {
        calc_values.push (1);
    }
}

void Calculator::visit (VariableNode *pnode) 
{
    if (pnode)
    {
        if (context.inScope (pnode->name))
        { 
            calc_values.push (context.getValue (pnode->name));
        }
        else
        {
            throw std::runtime_error ("Unknown name: " + pnode->name);
        }
    }                                 
    else
    {
        calc_values.push (1);
    }
}

void Calculator::visit (BinOpNode *pnode)    
{
    if (pnode)
    {                                
        pnode->left ->doAction (this);
        pnode->right->doAction (this);   

        auto R = getTopAndPop();
        auto L = getTopAndPop();
        switch (pnode->op)
        {                                   
        case Operation::add:
        {
            calc_values.push (L + R);
            break;
        }
        case Operation::sub:   
        {
            calc_values.push (L - R);
            break;
        }
        
        case Operation::mul:
        {                                 
            calc_values.push (L * R);
            break;
        }
        case Operation::div:                                                                                                       
        {                         
            if (abs (R) <= DBL_EPSILON)
                throw std::invalid_argument ("Division by zero");
            calc_values.push (L / R);
            break;
        }
        default:
            throw std::runtime_error ("Unkown or uncalculatable operation");
        }
    }
}

void Calculator::visit (FunctionNode *pnode) 
{
 if (pnode)
    {
    pnode->arg->doAction (this);
    auto arg = getTopAndPop();
    switch (pnode->func)
    {                   
        case Function::sin:
        {         
            calc_values.push (sin (arg));
            break;
        }
        case Function::cos:
        {                                
            calc_values.push (cos (arg));
            break;
        }
        case Function::tg:
        {
            calc_values.push (tan (arg));
            break;
        }
        
        case Function::ln:
        {
            if (arg <= 0)
                throw std::invalid_argument ("ln argiment is less than zero");
            calc_values.push (log (arg));
            break;
        }
        default:                 
            throw std::runtime_error ("Unkown or uncalculatable operation");
        }
    }
   
}


double Calculator::calculate (ExpressionNode *AST, const DC::Context& context)
{
    if (AST)
    {
        Calculator calc (context);
        AST->doAction (&calc);
        return calc.getTopValue();
    }
    return 0;
}

double Calculator::getTopAndPop()
{
    auto top = calc_values.top();
    calc_values.pop();
    return top;
}