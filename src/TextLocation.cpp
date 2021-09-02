#include "TextLocation.h"


void Location::nextTok()
{
    first_sym = last_sym;
}

Location::Location  (
    const char *new_line_beg,
    const char *new_first_sym,
    const char *new_last_sym,
    size_t      new_nline
) : line_beg  (new_line_beg),
    first_sym (new_first_sym),
    last_sym  (new_last_sym),
    nline     (new_nline)
{
}

char Location::peek() const
{
     return *last_sym;
}

char Location::advance (size_t offset)
{
    last_sym += offset;
    return *last_sym;
}

void Location::newLine()
{
    line_beg  = last_sym;
    first_sym = last_sym;
    ++nline;
}

std::string Location::str() const
{
    return std::string (first_sym, last_sym);
}
