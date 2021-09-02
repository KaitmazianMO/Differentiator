#ifndef LOCATIION_H_INCLUDED
#define LOCATIION_H_INCLUDED 

#include <string>

struct Location
{
    const char *line_beg  = nullptr;
    const char *first_sym = nullptr;
    const char *last_sym  = nullptr;
    size_t      nline     = 1;

    Location() = default;
    Location (
        const char *new_line_beg,
        const char *new_first_sym,
        const char *new_last_sym,
        size_t      new_nline
    );

    char peek() const;
    char advance (size_t offset);
    void newLine();
    void nextTok();
    std::string str() const;
};


#endif