#ifndef BASIC_EXCEPTION_H_INCLUDED
#define BASIC_EXCEPTION_H_INCLUDED

#include <stdexcept>
#include "TextLocation.h"

class BasicException : public std::runtime_error
{
public:
    BasicException (const std::string &msg) :
        std::runtime_error (msg)
    {
    }
                                                      
    BasicException (const Location& loc) :
        std::runtime_error (""),
        err_loc (loc)
    {
    }

    BasicException (const std::string &msg, const Location& loc) :
        std::runtime_error (msg),
        err_loc (loc)
    {
    }


    const Location &errLoc() const
    {
        return err_loc;
    }

private:
    Location err_loc;
};


#endif // !BASIC_EXCEPTION_H_INCLUDED