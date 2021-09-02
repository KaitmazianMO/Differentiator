#ifndef ORDINARY_H_INCLUDED
#define ORDINARY_H_INCLUDED


#include <stack>

#include "Visitor.h"

class OrdinaryExpressionFormater: public VisitorVoid
{
public:
    OrdinaryExpressionFormater() { ops.push (static_cast<Operation> (0)); };
   ~OrdinaryExpressionFormater() = default;

    void visit (NumberNode *)   override;
    void visit (VariableNode *) override;
    void visit (BinOpNode *)    override;
    void visit (FunctionNode *) override;

    static std::string to_string (ExpressionNode *);           
    const std::string &getExpr() const { return expression; };
          std::string  getExpr()       { return expression; };

private:
    std::string expression;

	std::stack<Operation> ops;
};


#endif // !ORDINARY_H_INCLUDED
