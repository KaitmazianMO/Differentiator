#ifndef PARSER_H_INCLUDED
#define PARSER_H_INCLUDED

#include <memory>

#include "../AST/Expression.h"
#include "../Lexer/Token.h"
#include "../Lexer/Lexer.h"
#include "../NonCopyable.h"
#include "../BasicException.h"

/*
 *
 * @todo change precedence oarser format
 *
 * @warning if function does not have ( parser thinks tha it is var and can't catch error
 * 
 */

/*
 * expression := A
 * 
 * A := A + B | A - B  | B
 * 
 * B := B * C | B / C | C
 * 
 * C := [a-b] | [a-b]() | +-[0-9] | (expression)
 */

class ParserException : public BasicException
{
public:
    ParserException (const std::string &msg) :
        BasicException (msg)
    {
    }
                                                      
    ParserException (const Location& loc) :
        BasicException ("", loc)
    {
    }

    ParserException (const std::string &msg, const Location& loc) :
        BasicException (msg, loc)
    {
    }
};

class Parser : NonCopyable
{
public:
    Parser() = default;
    void parse (TokenIt begin, TokenIt end);
    ExprNodePtr getAST()  { return AST; };

private:
    ExprNodePtr parseA();
    ExprNodePtr parseB();
    ExprNodePtr parseC();


    ExprNodePtr parseNumber();
    ExprNodePtr parseIdentifier();
    ExprNodePtr parseFunction();
    ExprNodePtr parseVariable();

    void error (const std::string &msg) { throw ParserException (msg, current_token->loc); }
    void errorExpected (const std::string &expected, const std::string &had) { throw ParserException ("Expected \'" + expected + "\', but had " + had, current_token->loc); }

    void getNextToken() { ++current_token; }
    void getPrevToken() { --current_token; }
    const std::string& getTokenValue() { return current_token->val; }
    Operation getTokenOperation() { return machOperation (getTokenValue()); }
    Location getTokenLocation() { return current_token->loc; }
    const TokType getTokenType() { return current_token->type; }
    bool isLastToken() { return current_token == last_token; }

    TokenIt first_token;
    TokenIt last_token;
    TokenIt current_token;

    ExprNodePtr AST   = nullptr;
};


#endif // !PARSER_H_INCLUDED
