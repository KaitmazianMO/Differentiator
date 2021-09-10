#ifndef DIFFERENTIATOR_CONTROLLER_H_INCLUDED
#define DIFFERENTIATOR_CONTROLLER_H_INCLUDED


#include "NonCopyable.h"
#include "Lexer/Lexer.h"
#include "Parser/Parser.h"

#include <fstream>

namespace DC
{ 
    enum class Format 
    {
        latex, ordinary,
    };
    
    struct Flags
    {    
        struct DoInitialTreeDump            { explicit DoInitialTreeDump          (bool dump) : active (dump) {} bool active; };
        struct DoDifferentiatedTreeDump     { explicit DoDifferentiatedTreeDump   (bool dump) : active (dump) {} bool active; };
        struct DoOptimizedTreeDump          { explicit DoOptimizedTreeDump        (bool dump) : active (dump) {} bool active; };
        struct DoErrorConstructedTreeDump   { explicit DoErrorConstructedTreeDump (bool dump) : active (dump) {} bool active; };
    
        DoInitialTreeDump          initial_tree_dump           = DoInitialTreeDump (false);  
        DoDifferentiatedTreeDump   differentiated_tree_dump    = DoDifferentiatedTreeDump (false); 
        DoOptimizedTreeDump        optimized_tree_dump         = DoOptimizedTreeDump (false); 
        DoErrorConstructedTreeDump error_constructed_tree_dump = DoErrorConstructedTreeDump (false); 
    };
    
    class DifferentiatorController : NonCopyable
    {
    public:
        explicit DifferentiatorController ( 
            Flags::DoInitialTreeDump initial_tree_dump = Flags::DoInitialTreeDump (false),  
            Flags::DoDifferentiatedTreeDump differentiated_tree_dump = Flags::DoDifferentiatedTreeDump (false), 
            Flags::DoOptimizedTreeDump optimized_tree_dump = Flags::DoOptimizedTreeDump (false), 
            Flags::DoErrorConstructedTreeDump error_constructed_tree_dump = Flags::DoErrorConstructedTreeDump (false) 
        );
    
        Flags bind (
            Flags::DoInitialTreeDump initial_tree_dump = Flags::DoInitialTreeDump (false),  
            Flags::DoDifferentiatedTreeDump differentiated_tree_dump = Flags::DoDifferentiatedTreeDump (false), 
            Flags::DoOptimizedTreeDump optimized_tree_dump = Flags::DoOptimizedTreeDump (false), 
            Flags::DoErrorConstructedTreeDump error_constructed_tree_dump = Flags::DoErrorConstructedTreeDump (false)
        );

        Flags bind (Flags);
                       
        void setDifferentiationVariable (const std::string &name, double val);
        void differentiate (const std::string &expr);
        void differentiate (std::ifstream &expr_is);
        void write (std::ostream &os, Format format);
        double compute();
    private:
        Flags         flags;
                                              
        ExpressionNode *diffAST = nullptr;
        DC::Context     context = DC::Context ("x");
    };
}


#endif // !DIFFERENTIATOR_CONTROLLER_H_INCLUDED
