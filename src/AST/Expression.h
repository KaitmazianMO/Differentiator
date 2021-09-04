#ifndef EXPRESSION_H_INCLUDED
#define EXPRESSION_H_INCLUDED


#include <unordered_map>
#include <assert.h>
#include <string>
#include <memory>

#include "../TextLocation.h"
#include "Visitor.h"

/*
 *@todo opStr funcStr are terrible methods, it's should be external functions
 */

struct ExpressionNode;
using ExprNodePtr = ExpressionNode *;

struct ExpressionNode
{
    ExpressionNode (const Location &loc_);
    virtual ~ExpressionNode() = default;
                                           
    virtual ExprNodePtr getAction (VisitorExpr *) = 0; // what about template visitor with return type as template? virtual doesn't support this, but...
    virtual void        doAction  (VisitorVoid *) = 0; // what about template visitor with return type as template? virtual doesn't support this, but...
    virtual ExprNodePtr lightCopy() = 0;
    virtual ExprNodePtr deepCopy() = 0;

    const Location loc;
};

struct NumberNode : ExpressionNode
{
    double val = 0;

    NumberNode (double num, const Location& loc_);
    NumberNode (double num);         
    ExprNodePtr lightCopy() override;
    ExprNodePtr deepCopy() override;
    
    ExprNodePtr getAction (VisitorExpr *) override;
    void        doAction  (VisitorVoid *) override;
};

struct VariableNode : ExpressionNode
{

    VariableNode (const std::string &var_name, double val, const Location& loc_);
    VariableNode (const std::string &var_name, double val);    
    ExprNodePtr lightCopy() override;
    ExprNodePtr deepCopy() override;

    ExprNodePtr getAction (VisitorExpr *) override;
    void        doAction  (VisitorVoid *) override;

    const std::string name;
    double value = 0;
};

enum class Operation { add, sub, mul, div, nop };
bool operator < (Operation, Operation);
std::string to_string (Operation op);
Operation   to_operation (const std::string& op_str);
Operation machOperation (const std::string& op_str);

struct BinOpNode : ExpressionNode
{
    BinOpNode (ExprNodePtr L, Operation operation, ExprNodePtr R, const Location& loc_);
    BinOpNode (ExprNodePtr L, Operation operation, ExprNodePtr R);      
    ExprNodePtr lightCopy() override;
    ExprNodePtr deepCopy() override;
    static std::unordered_map<Operation, std::string> to_string;

    ExprNodePtr getAction (VisitorExpr *) override;
    void        doAction  (VisitorVoid *) override;         

    Operation   op;
    ExprNodePtr left;
    ExprNodePtr right;
};

enum class Function { sin, cos, tg, ln, nof };  
std::string to_string (Function op);
Function    to_function (const std::string& op_str);
Function machFunction (const std::string& func_str);

struct FunctionNode : ExpressionNode
{
    FunctionNode (Function f, ExprNodePtr farg, const Location& loc_);
    FunctionNode (Function f, ExprNodePtr farg);      
    ExprNodePtr lightCopy() override;
    ExprNodePtr deepCopy() override;
    static std::unordered_map<Function, std::string> to_string;

    ExprNodePtr getAction (VisitorExpr *) override;
    void        doAction  (VisitorVoid *) override;

    Function        func;
    ExpressionNode *arg;
};


#endif // !EXPRESSION_H_INCLUDED
