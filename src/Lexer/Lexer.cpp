#include "Lexer.h"

#include <assert.h>
#include <exception>
#include <iostream>

#define call  std::cout << "Called " << __func__ << std::endl;

Lexer::Lexer (const char *source_code_) :
    source_code (source_code_),
    curr_loc    ()
{
    if (source_code)
    {             
        curr_loc.line_beg  = source_code;
        curr_loc.first_sym = source_code;
        curr_loc.last_sym  = source_code;
    }
    else
    {
        throw LexerException ("Invadid sourse code (nullptr)");
    }
}

Token Lexer::getNextTok()
{
    call
    while (true)
    { 
        char front = curr_loc.peek();
        if (isalpha (front))
            return getIdentifierlToken();

        if (isdigit (front) && front != '+')
            return getNumberToken();

        switch (front)
        {               
        case '(': case ')':
            curr_loc.advance (1);
            return newToken (static_cast<TokType> (front));

        case '+': case '-':
        case '*': case '/':
            curr_loc.advance (1);
            return newToken (TokType::operation);

        case '\n': case '\r':
            skipLines();
            break;

        case ' ' : case '\t':
        case '\f': case '\v':
            skipWhitespaces();
            continue;

        case '\0': case EOF:
            return Token (TokType::eof, "eof", curr_loc);
        }
    }

    return {};
}

void Lexer::tokenize()
{
    do 
    { 
        tokens.push_back (getNextTok());
    } 
    while (tokens.back().type != TokType::eof);
}

Token Lexer::getNumberToken()
{
    call
    while (isdigit (curr_loc.advance (1)))
        ;

    Token tok = newToken (TokType::number);
    return tok;
}

Token Lexer::getIdentifierlToken()
{
    call
    do 
    {
        std::cout << curr_loc.advance (1) << " ";
    }
    while (isalpha (curr_loc.peek()) || isdigit (curr_loc.peek()));

    std::cout << "The last id char " << curr_loc.peek() << "(" << int (curr_loc.peek()) << ")" << std::endl;

    Token tok = newToken (TokType::identifier);
    return tok;
}

Token Lexer::newToken (TokType ttype)
{
    call
    Token tok (ttype, curr_loc.str(), curr_loc);
    std::cout << "**Tok " << curr_loc.str() << std::endl;
    curr_loc.nextTok();
    return tok;
}

void Lexer::skipLines()
{   /* UNIX - '\n' MACOS - '\r\n' WINDOWS - '\n\r' */
    call
    if (curr_loc.peek() == '\r') curr_loc.advance (1);
    while (curr_loc.peek() == '\n')
    {
        curr_loc.advance (1);
        if (curr_loc.peek() == '\r') curr_loc.advance (1);
        curr_loc.newLine();
    }
}

void  Lexer::skipWhitespaces()
{
    call
    while (curr_loc.peek() == ' ' ||
        curr_loc.peek() == '\t' || 
        curr_loc.peek() == '\f' ||
        curr_loc.peek() == '\v')
        curr_loc.advance (1);

    curr_loc.first_sym = curr_loc.last_sym;
}

void Lexer::dump()
{
    for (const auto &tok : tokens)
        printf ("type = %d, val = \'%s\' (%zu)\n", tok.type, tok.val.c_str(), tok.loc.nline);
}