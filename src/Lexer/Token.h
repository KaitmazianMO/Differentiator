#ifndef TOKEN_H_INCLUDED
#define TOKEN_H_INCLUDED

#include <string>
#include <array>
#include <unordered_map>

#include "../VisualStudioWarnings.h"
#include "../TextLocation.h"


enum class TokType
{
    lparen = '(', rparen = ')',

    number,
    identifier,
    operation,

    eof,

    count
};

struct Token
{
    TokType      type;
    std::string  val;

    Location     loc;

    Token ();
    Token (
        TokType            type_,
        const std::string &val_,
        const char *const  line_beg_,
        const char *const  first_sym_,
        const char *const  last_sym_,
        const size_t       nline_
    );
    Token (
        TokType            type_,
        const std::string &val_,
        Location           loc_
    );                                      
};

#endif // !TOKEN_H_INCLUDED
