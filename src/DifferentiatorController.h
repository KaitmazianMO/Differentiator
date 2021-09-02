#ifndef DIFFERENTIATOR_CONTROLLER_H_INCLUDED
#define DIFFERENTIATOR_CONTROLLER_H_INCLUDED

#include "NonCopyable.h"
#include "Lexer/Lexer.h"
#include "Parser/Parser.h"

#include <fstream>

namespace Diff
{ 
    enum class Format 
    {
        latex, ordinary,
    };
    
    struct Flags
    {    
        struct OutputFormat { explicit OutputFormat (Format f) : format (f) {} Format format; };
        struct InputFormat  { explicit InputFormat  (Format f) : format (f) {} Format format; };
        struct DoInitialTreeDump            { explicit DoInitialTreeDump          (bool dump) : active (dump) {} bool active; };
        struct DoDifferentiatedTreeDump     { explicit DoDifferentiatedTreeDump   (bool dump) : active (dump) {} bool active; };
        struct DoOptimizedDump              { explicit DoOptimizedDump            (bool dump) : active (dump) {} bool active; };
        struct DoErrorConstructedTreeDump   { explicit DoErrorConstructedTreeDump (bool dump) : active (dump) {} bool active; };
    
        InputFormat                input                       = InputFormat   (Format::ordinary);
        OutputFormat               output                      = OutputFormat (Format::ordinary);
        DoInitialTreeDump          initial_tree_dump           = DoInitialTreeDump (false);  
        DoDifferentiatedTreeDump   differentiated_tree_dump    = DoDifferentiatedTreeDump (false); 
        DoOptimizedDump            optimized_tree_dump         = DoOptimizedDump (false); 
        DoErrorConstructedTreeDump error_constructed_tree_dump = DoErrorConstructedTreeDump (false); 
    };
    
    class DifferentiatorController : NonCopyable
    {
    public:
        explicit DifferentiatorController (
            std::istream& expression_stream,
            std::ostream& output_stream,      
            Flags::InputFormat iformat = Flags::InputFormat (Format::ordinary),
            Flags::OutputFormat oformat = Flags::OutputFormat (Format::ordinary),
            Flags::DoInitialTreeDump initial_tree_dump = Flags::DoInitialTreeDump (false),  
            Flags::DoDifferentiatedTreeDump differentiated_tree_dump = Flags::DoDifferentiatedTreeDump (false), 
            Flags::DoOptimizedDump optimized_tree_dump = Flags::DoOptimizedDump (false), 
            Flags::DoErrorConstructedTreeDump error_constructed_tree_dump = Flags::DoErrorConstructedTreeDump (false) 
        );
    
        Flags bind (
            Flags::InputFormat iformat = Flags::InputFormat (Format::ordinary),
            Flags::OutputFormat oformat = Flags::OutputFormat (Format::ordinary),
            Flags::DoInitialTreeDump initial_tree_dump = Flags::DoInitialTreeDump (false),  
            Flags::DoDifferentiatedTreeDump differentiated_tree_dump = Flags::DoDifferentiatedTreeDump (false), 
            Flags::DoOptimizedDump optimized_tree_dump = Flags::DoOptimizedDump (false), 
            Flags::DoErrorConstructedTreeDump error_constructed_tree_dump = Flags::DoErrorConstructedTreeDump (false)
        );

        Flags bind (Flags);

        void run();
    
    private:
        Flags         flags;
        std::istream &expression_stream;
        std::ostream &output_stream;
    
    };
}

#endif // !DIFFERENTIATOR_CONTROLLER_H_INCLUDED
