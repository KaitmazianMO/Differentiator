#include "ASTDeleter.h"
#include "Expression.h"

void ASTDeleter::visit (NumberNode *pnode)  
{
    delete pnode;    
}
void ASTDeleter::visit (VariableNode *pnode)
{
    delete pnode;    
}

void ASTDeleter::visit (BinOpNode *pnode)   
{   
    if (pnode)
    {
        pnode->left-> doAction (this);
        pnode->right->doAction (this);
    }

    delete pnode;    
}

void ASTDeleter::visit (FunctionNode *pnode)
{                                     
    if (pnode) 
        pnode->arg->doAction (this);
    
    delete pnode;    
}

void ASTDeleter::free (ExpressionNode *pnode)
{
    static ASTDeleter deleter;
    if (pnode)
    {
        pnode->doAction (&deleter);
    }
}