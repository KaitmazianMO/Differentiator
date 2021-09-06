#include <assert.h>
#include <iostream>
#include <fstream>
#include <string>

#include "DifferentiatorController.h"
#include "Lexer/Token.h"
#include "Lexer/Lexer.h"
#include "Parser/Parser.h"
#include "AST/Graphviz.h"
#include "AST/ASTDeleter.h"
#include "AST/Differentiator.h"
#include "AST/Latex.h"
#include "AST/Ordinary.h"
#include "AST/Optimizer.h"
#include "AST/Calculator.h"

using namespace DC;

std::string readFromStream (std::istream& stream);
inline void log (const std::string &msg) { std::cerr << msg << std::endl; }
inline void massiveLatexEquationWritingPattern (std::ostream &os, ExpressionNode *exprAST);

DC::DifferentiatorController::DifferentiatorController (             
    std::istream& expression_stream_,
    std::ostream& output_stream_,   
    Flags::InputFormat iformat,
    Flags::OutputFormat oformat,
    Flags::DoInitialTreeDump initial_tree_dump,  
    Flags::DoDifferentiatedTreeDump differentiated_tree_dump, 
    Flags::DoOptimizedTreeDump optimized_tree_dump, 
    Flags::DoErrorConstructedTreeDump error_constructed_tree_dump 
) : expression_stream (expression_stream_),
    output_stream     (output_stream_)
{
    flags.input                       = iformat;
    flags.output                      = oformat;
    flags.initial_tree_dump           = initial_tree_dump;
    flags.differentiated_tree_dump    = differentiated_tree_dump;
    flags.optimized_tree_dump         = optimized_tree_dump;
    flags.error_constructed_tree_dump = error_constructed_tree_dump;
}

void DC::DifferentiatorController::run (const std::string &diff_var)  
{   
    log ("Start reading expression...");
    static std::string expression = readFromStream (expression_stream); // Token locations have pointers to this string all the time, so after we got exception the string is distructed on the higher levels and we can't use it. So avoid the string distuctor.
    if (expression.empty())
        return;
    log ("Finished reading.");

    log ("Start tokenizing..." );
    Lexer lexer (expression.c_str());
    lexer.tokenize();                                                                               
    log ("Finished tokenizing.");

    log ("Start parsing...");
    Parser parser;
    try 
    { 
        parser.parse (lexer.getFirstTokenIt(), lexer.getLastTokenIt());
    }
    catch (const ParserException &pex)
    {
        if (flags.error_constructed_tree_dump.active) 
            GraphvizPrinter::dump ("InitialExpression(error)", parser.getAST());
        ASTDeleter::free (parser.getAST());
        throw;
    }                                                    
    log ("Finished parsing.");

    if (flags.initial_tree_dump.active) 
        GraphvizPrinter::dump ("InitialExpression", parser.getAST());

    log ("Differentiating...");
    Context context (diff_var);
    auto diff_AST = Differentiator::differentiate (parser.getAST(), context);
    log ("Differentiating's finished.");

    if (flags.differentiated_tree_dump.active) 
        GraphvizPrinter::dump ("DifferentiatedExpression", diff_AST);

    log ("Deleting source AST...");
    ASTDeleter::free (parser.getAST()); 
    
    log ("Writing...");
    if (flags.output.format == Format::latex)
        massiveLatexEquationWritingPattern (output_stream, diff_AST);
    else if (flags.output.format == Format::ordinary)
        output_stream << OrdinaryExpressionFormater::to_string (diff_AST) << std::endl;
    log ("Finished writing.");
    
    //optimize (diff_AST);
    std::cout << "Enter " << diff_var << ": ";
    double dval = 0;
    std::cin >> dval;
    std::cout << "dval = " << dval << std::endl;
    context.setValue (diff_var, dval);
    std::cout << "Derivative value: " << Calculator::calculate (diff_AST, context) << std::endl;

    ASTDeleter::free (diff_AST);
}

Flags DC::DifferentiatorController::bind (
    Flags::InputFormat iformat,
    Flags::OutputFormat oformat,
    Flags::DoInitialTreeDump initial_tree_dump,  
    Flags::DoDifferentiatedTreeDump differentiated_tree_dump, 
    Flags::DoOptimizedTreeDump optimized_tree_dump, 
    Flags::DoErrorConstructedTreeDump error_constructed_tree_dump 
)
{
    Flags old_flags                   = flags;
    
    flags.input                       = iformat;
    flags.output                      = oformat;
    flags.initial_tree_dump           = initial_tree_dump;
    flags.differentiated_tree_dump    = differentiated_tree_dump;
    flags.optimized_tree_dump         = optimized_tree_dump;
    flags.error_constructed_tree_dump = error_constructed_tree_dump;

    return old_flags;
}

Flags DC::DifferentiatorController::bind (Flags flags_)
{
    return bind (
        flags_.input, flags_.output,
        flags_.initial_tree_dump,
        flags_.differentiated_tree_dump,
        flags_.optimized_tree_dump,
        flags_.error_constructed_tree_dump
    );
}

std::string readFromStream (std::istream& stream)
{
    std::string read;
    if (stream.good())
        while (getline (stream, read, '\0'))
            ;
    return read;
}

void massiveLatexEquationWritingPattern (std::ostream &os, ExpressionNode *exprAST)
{
    os << "\\documentclass[a4paper,12pt]{article} %% �������� leqno � [] ��� ��������� �����\n"                  
          "%%%%%% ������ � ������� ������\n"              
          "\\usepackage{cmap}					    %% ����� � PDF                     \n"              
          "\\usepackage{mathtext} 				    %% ������� ����� � �������         \n"              
          "\\usepackage[T2A]{fontenc}			    %% ���������                       \n"              
          "\\usepackage[utf8]{inputenc}			%% ��������� ��������� ������      \n"              
          "\\usepackage[english,russian]{babel}	%% ����������� � ��������          \n"              
          "%%%%%% �������������� ������ � �����������                                               \n"              
          "\\usepackage{amsmath,amsfonts,amssymb,amsthm,mathtools} %% AMS                         \n"              
          "\\usepackage{icomma} %% \"�����\" �������: $0,2$ --- �����, $0, 2$ --- ������������    \n"              
          "%%%% ������                                                          \n"              
          "\\usepackage{euscript}	 %% ����� ������                          \n"              
          "\\usepackage{mathrsfs} %% �������� ��������                         \n"              
          "                                                                   \n"              
          "%%%%%% ���������                                                      \n"              
          "\\title{1.2 Awesome differentiating}                               \n"              
          "\\date{\\today}                                                    \n"              
          "                                                                   \n"              
          "\\begin{document} %% ����� ���������, ������ ���������              \n"              
          "                                                                   \n"              
          "\\maketitle                                                        \n"              
          "                                                                   \n"              
          "Here we go again \n"
          "\\[ ";

    os << LatexExpressionFormater::to_string (exprAST);

    os << " \\]\n"
          "\\end{document}\n";

}