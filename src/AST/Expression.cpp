#include "Expression.h"
#include "DeepCopy.h"

#include <algorithm>
#include <utility>

static DeepCopy g_deep_copy; // class for AST deep copying by visitor 

std::unordered_map<Operation, std::string> BinOpNode::to_string = {
    {Operation::add, "+"},
    {Operation::sub, "-"},
    {Operation::mul, "*"},  
    {Operation::div, "/"},
    {Operation::nop, "NOP"},
};
std::unordered_map<Function, std::string> FunctionNode::to_string = {
    {Function::cos, "cos"},
    {Function::sin, "sin"},    
    {Function::ln,  "ln"},
    {Function::tg,  "tg"},
    {Function::nof, "NOF"},
};

std::string to_string (Operation op) 
{
    auto found = BinOpNode::to_string.find (op);
    if (found != BinOpNode::to_string.end())
        return found->second;

    return "";
};

Operation to_operation (const std::string &op_str)
{
    Operation found = Operation::nop;
    for (auto it = BinOpNode::to_string.begin(); it != BinOpNode::to_string.end(); ++it)
        if (it->second == op_str)
            found = it->first;
    
    return found;
}

std::string to_string (Function op)
{
    auto found = FunctionNode::to_string.find (op);
    if (found != FunctionNode::to_string.end())
        return found->second;

    return "";
}
Function    to_function (const std::string &op_str)
{
    Function found = Function::nof;
    for (auto it = FunctionNode::to_string.begin(); it != FunctionNode::to_string.end(); ++it)
        if (it->second == op_str)
            found = it->first;
    
    return found;
}

ExpressionNode::ExpressionNode (const Location &loc_) :
    loc (loc_)
{
}


NumberNode::NumberNode (double num, const Location& loc_) :
    ExpressionNode (loc_),
    val (num)
{
}

NumberNode::NumberNode (double num) :
    ExpressionNode({}),
    val (num)
{
}

ExprNodePtr NumberNode::lightCopy()
{
    return new NumberNode (val);
}

ExprNodePtr NumberNode::deepCopy()
{
    return getAction (&g_deep_copy);
}

VariableNode::VariableNode (const std::string &var_name, double val, const Location& loc_) :
    ExpressionNode (loc_),
    name  (var_name),
    value (val)
{
}

VariableNode::VariableNode (const std::string &var_name, double val) :
    ExpressionNode ({}),
    name  (var_name),
    value (val)
{
}

ExprNodePtr VariableNode::lightCopy()
{
    return new VariableNode (name, value);
}

ExprNodePtr VariableNode::deepCopy()
{
    return getAction (&g_deep_copy);
}


Operation machOperation (const std::string &op_str)
{                                            
    if (op_str == "+") return Operation::add;
    if (op_str == "-") return Operation::sub;
    if (op_str == "*") return Operation::mul;
    if (op_str == "/") return Operation::div;
    
    return Operation::nop;
}

BinOpNode::BinOpNode (ExprNodePtr L, Operation operation, ExprNodePtr R, const Location& loc_) :
    ExpressionNode (loc_),
    left  (L),
    op    (operation),
    right (R)
{
}

BinOpNode::BinOpNode (ExprNodePtr L, Operation operation, ExprNodePtr R) :
    ExpressionNode ({}),
    left  (L),
    op    (operation),
    right (R)
{
}

ExprNodePtr BinOpNode::lightCopy()
{
    return new BinOpNode (left, op, right);
}

ExprNodePtr BinOpNode::deepCopy()
{
    return getAction (&g_deep_copy);
}

Function machFunction (const std::string &func_str)
{
    //sin, cos, tg, ln                          
    if (func_str == "sin") return Function::sin; 
    if (func_str == "cos") return Function::cos; 
    if (func_str == "tg")  return Function::tg;
    if (func_str == "ln")  return Function::ln;

    return Function::nof;
}

FunctionNode::FunctionNode (Function f, ExprNodePtr farg, const Location& loc_) :
    ExpressionNode(loc_),
    func (f),
    arg  (farg)
{
}

FunctionNode::FunctionNode (Function f, ExprNodePtr farg) :
    ExpressionNode({}),
    func (f),
    arg  (farg)
{
}

ExprNodePtr FunctionNode::lightCopy()
{
    return new FunctionNode (func, arg);
}

ExprNodePtr FunctionNode::deepCopy()
{
    return getAction (&g_deep_copy);
}

#define DECLARE_VISITOR_FUNCTIONS_FOR_NODE_TYPE( type ) \
ExprNodePtr type::getAction (VisitorExpr *visitor)      \
{                                                       \
    return visitor->visit (this);                       \
}                                                       \
                                                        \
void type::doAction (VisitorVoid *visitor)              \
{                                                       \
    visitor->visit (this);                              \
}                                                       \
                                                    
DECLARE_VISITOR_FUNCTIONS_FOR_NODE_TYPE(NumberNode)
DECLARE_VISITOR_FUNCTIONS_FOR_NODE_TYPE(VariableNode)
DECLARE_VISITOR_FUNCTIONS_FOR_NODE_TYPE(BinOpNode)
DECLARE_VISITOR_FUNCTIONS_FOR_NODE_TYPE(FunctionNode)

#undef DECLARE_VISITOR_FUNCTIONS_FOR_NODE_TYPE;
