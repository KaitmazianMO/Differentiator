#include "Optimizer.h"
#include "Expression.h"

//#define BACK     (*operations_trace.back())
//#define TRACE(n) (*operations_trace[n]) 
//#define NODE     (*ppnode)
//
//void ConstantFolder::visit (NumberNode **ppnode)
//{
//    for (int i = operations_trace.size() - 2;
//        BACK->op < TRACE(i)->op; --i)
//    {
//        BACK = try2Fold();
//    }
//}
//
//void ConstantFolder::visit (VariableNode **pnode)
//{
//}
//
//void ConstantFolder::visit (BinOpNode **ppnode)
//{
//    operations_trace.push (pnode);
//    pnode->left ->doAction (this);
//    pnode->right->doAction (this);
//}
//
//void ConstantFolder::visit (FunctionNode **ppnode)
//{
//    if (ppnode)
//    { 
//        ConstantFolder cf;
//        NODE->arg->doAction (&cf);
//    }                   
//}