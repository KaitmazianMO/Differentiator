#include "Lexer.h"

#include <assert.h>
#include <exception>
#include <iostream>
#include <string>

Lexer::Lexer (const std::string &source_code_) :
    source_code (source_code_),
    curr_loc    ()
{
    curr_loc.line_beg  = &source_code[0];
    curr_loc.first_sym = &source_code[0];
    curr_loc.last_sym  = &source_code[0];
}

Token Lexer::getNextTok()
{
    while (true)
    { 
        char front = curr_loc.peek();
        if (isalpha (front))
            return getIdentifierlToken();

        if (isdigit (front) && front != '+') // don't support unary plus
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

        default:
            throw LexerException ("Unknown symbol(" + std::to_string (front) + ")");
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
    while (isdigit (curr_loc.peek()) || curr_loc.peek() == '.')
        curr_loc.advance (1);

    Token tok = newToken (TokType::number);
    return tok;
}

Token Lexer::getIdentifierlToken()
{
    while (isalpha (curr_loc.peek()) || isdigit (curr_loc.peek()))
    {
        curr_loc.advance (1);
    }

    return newToken (TokType::identifier);
}

Token Lexer::newToken (TokType ttype)
{
    Token tok (ttype, curr_loc.str(), curr_loc);
    curr_loc.nextTok();
    return tok;
}

void Lexer::skipLines()
{
    if (curr_loc.peek() == '\r') 
        curr_loc.advance (1);
    
    while (curr_loc.peek() == '\n')
    {
        curr_loc.advance (1);
        if (curr_loc.peek() == '\r')
            curr_loc.advance (1);
        curr_loc.newLine();
    }
}

void  Lexer::skipWhitespaces()
{
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