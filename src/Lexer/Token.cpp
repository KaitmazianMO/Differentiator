#include "Token.h"


Token::Token() :
    type     (),
    val      (""),
    loc      (nullptr, nullptr, nullptr, 1)
{
    //static TokenStringTablesInitializer __INTIT_TABLES__;
}

Token::Token (
    TokType            type_,
    const std::string &val_,
    const char *const  line_beg_,
    const char *const  first_sym_,
    const char *const  last_sym_,
    const size_t       nline_
) :                               
    type     (type_),
    val      (val_),
    loc      (line_beg_, first_sym_, last_sym_, nline_)
{
}

Token::Token (
    TokType            type_,
    const std::string &val_,
    Location           loc_    
) :                               
    type     (type_),
    val      (val_),
    loc      (loc_)
{
}

