#include "Parser.h"

#include <iostream>
#include <assert.h>


void Parser::parse (TokenIt begin, TokenIt end)
{  
    assert (begin != end && "Got same iterators");
    
    first_token = begin;
    current_token = begin;
    last_token  = end;
    AST = parseA();                                                
    if (current_token != last_token - 1)
    { 
        throw ParserException ("Have not processed all the input, got " + current_token->loc.str() , current_token->loc);
    }
}

ExprNodePtr Parser::parseA()
{
    auto left = parseB();

    for (auto op = getTokenOperation();
        (op == Operation::add || op == Operation::sub) && current_token != last_token;
        op = getTokenOperation())
    {
        getNextToken();
        left = new BinOpNode (left, op, parseB());
    }

    return left;
}

ExprNodePtr Parser::parseB()
{       
    auto left = parseC();

    for (auto op = getTokenOperation();
        (op == Operation::mul || op == Operation::div) && current_token != last_token;
        op = getTokenOperation())
    {
        getNextToken();
        left = new BinOpNode (left, op, parseC());
    }

    return left;
}


ExprNodePtr Parser::parseC()
{
    ExprNodePtr res = nullptr;
    switch (getTokenType())
    {                        
    case TokType::identifier:
        res = parseIdentifier();
        break;

    case TokType::number:
        res = parseNumber();
        break;

    case TokType::lparen:
        getNextToken();
        res = parseA();
        if (getTokenType() != TokType::rparen)                                
            errorExpected (")", current_token->loc.str());
        getNextToken();
        break;

    default: 
        error ("Unexpected token");
    }

    return res;
}

ExprNodePtr Parser::parseNumber()
{
    assert (current_token->type == TokType::number && "Tried to parse not a nubmer by number parsing function");
    
    const double num_val = strtod (getTokenValue().c_str(), nullptr);
    auto ret = new NumberNode (num_val, getTokenLocation());
    getNextToken();
    return ret;
}

ExprNodePtr  Parser::parseIdentifier()
{
    assert (current_token->type == TokType::identifier && "Tried to parse not a identifier by identifier parsing function");

    getNextToken();
    auto next_type = getTokenType();
    getPrevToken();
    
    if (next_type != TokType::lparen)
        return parseVariable();
    
    return parseFunction();
}

ExprNodePtr Parser::parseVariable()
{                                                                                                                              
    assert (current_token->type == TokType::identifier && "Tried to parse not a identifier by varriable parsing function");

    auto var_node = new VariableNode (getTokenValue(), 0, getTokenLocation());
    getNextToken();
    return var_node;
}

ExprNodePtr  Parser::parseFunction()
{                                                                                                                              
    assert (current_token->type == TokType::identifier && "Tried to parse not a identifier by function parsing function");

    Function func = machFunction (getTokenValue());
    if (func == Function::nof)
        error ("Unknown function detected");
    
    getNextToken();                                               
    if (getTokenType() != TokType::lparen)                                  
        errorExpected (")", current_token->loc.str());

    getNextToken();
    auto arg = parseA();

    if (getTokenType() != TokType::rparen)
        errorExpected (")", current_token->loc.str());
    getNextToken();

    return new FunctionNode (func, arg);
}

