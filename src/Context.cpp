#include "Context.h"

using namespace DC;

bool Context::inScope (const std::string &name) const
{
    return symbol_table.find (name) != symbol_table.end();
}

bool Context::isDiffVar (const std::string &name) const
{
    return differentiating_varriable == name;
}

double Context::getValue (const std::string &name) const
{
    auto it = symbol_table.find (name);
    if (it == symbol_table.end())
        return NAN;
    return it->second;
}

void Context::newName (const std::string &name)
{
    symbol_table[name] = 0;
}

bool Context::setValue (const std::string &name, double val)
{
    if (!inScope (name)) 
        return false;

    symbol_table[name] = val;           
    return true;
}                                      

void Context::setDiffVar (const std::string &name)
{
    differentiating_varriable = name;
    newName (name);
}

Context::Context (const std::string &diff_var)
    : differentiating_varriable (diff_var)
{
    newName (diff_var);
}

Context::Context()
    : differentiating_varriable ("x")
{
    newName (differentiating_varriable);
}