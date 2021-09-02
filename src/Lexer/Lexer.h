#ifndef LEXER_H_INCLUDED
#define LEXER_H_INCLUDED

#include <string>
#include <vector>
#include <stdexcept>

#include "Token.h"
#include "../File.h"
#include "../NonCopyable.h"
#include "../BasicException.h"

class LexerException : public BasicException
{
public:
    LexerException (const std::string &msg) :
        BasicException (msg)
    {
    }
                                                      
    LexerException (const Location& loc) :
        BasicException (""),
        err_loc (loc)
    {
    }

    LexerException (const std::string &msg, const Location& loc) :
        BasicException (msg),
        err_loc (loc)
    {
    }


    const Location &errLoc()
    {
        return err_loc;
    }

private:
    Location err_loc;
};

using TokenIt = std::vector<Token>::const_iterator;

class Lexer
{
public:
    Lexer () = default;
    Lexer (const char *source_code);
   ~Lexer () = default;

    void tokenize();
    TokenIt getFirstTokenIt() { return tokens.begin(); }
    TokenIt getLastTokenIt()  { return tokens.end(); }
    void dump();

private:
    Token getNextTok();
    Token getNumberToken();
    Token getIdentifierlToken();

    Token newToken (TokType ttype);
    Token createToken (const std::string &val) const;
    void  skipLines(); 
    void  skipWhitespaces();


    const char        *source_code;               
    Location           curr_loc;
    std::vector<Token> tokens;     
};

#endif // !LEXER_H_INCLUDED