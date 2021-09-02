#include "Optimizer.h"
#include "Expression.h"


void ConstantFolder::visit (NumberNode *pnode)
{
    //for (auto )
}

void ConstantFolder::visit (VariableNode *pnode)
{
}

void ConstantFolder::visit (BinOpNode *pnode)
{
    op_const_trace.push_back (pnode);
    pnode->left ->doAction (this);
    pnode->right->doAction (this);
}

void ConstantFolder::visit (FunctionNode *pnode)
{
    op_const_trace.clear();
    pnode->arg->doAction (this);
}