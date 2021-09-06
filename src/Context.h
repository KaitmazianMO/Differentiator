#ifndef CONTEXT_H_INCLUDED
#define CONTEXT_H_INCLUDED

#include <map>
#include <string>

namespace DC
{ 
    class Context
    {
    public:
        using SymbolTable = std::map<const std::string, double>;
    
        Context ();
        Context (const std::string &diff_var);
       ~Context () = default;
    
        bool inScope (const std::string &name) const;
        double getValue (const std::string &name) const;
        bool isDiffVar (const std::string &name) const;
                                               
        void newName (const std::string &name);
        bool setValue (const std::string &name, double val);
        void setDiffVar (const std::string &name);
    
    private:
        std::string differentiating_varriable;
        SymbolTable symbol_table;
    };
}


#endif // !CONTEXT_H_INCLUDED
