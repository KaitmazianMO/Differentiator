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
    std::string expression = readFromStream (expression_stream);
    if (expression.empty())
        return;
    log ("Finished reading.");

    log ("Start tokenizing..." );
    std::cerr << "\'" << expression << "\'" << std::endl;
    Lexer lexer (expression.c_str());
    lexer.tokenize();                                  
    log ("Finished tokenizing.");

    log ("Start parsing...");
    Parser parser;
    try 
    { 
        parser.parse (lexer.getFirstTokenIt(), lexer.getLastTokenIt());
    }
    catch (const BasicException &bex)
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
    auto diff_AST = Differentiator::differentiate (parser.getAST(), Context (diff_var));
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
    os << "\\documentclass[a4paper,12pt]{article} %% добавить leqno в [] для нумерации слева\n"                  
          "%%%%%% Работа с русским языком\n"              
          "\\usepackage{cmap}					    %% поиск в PDF                     \n"              
          "\\usepackage{mathtext} 				    %% русские буквы в фомулах         \n"              
          "\\usepackage[T2A]{fontenc}			    %% кодировка                       \n"              
          "\\usepackage[utf8]{inputenc}			%% кодировка исходного текста      \n"              
          "\\usepackage[english,russian]{babel}	%% локализация и переносы          \n"              
          "%%%%%% Дополнительная работа с математикой                                               \n"              
          "\\usepackage{amsmath,amsfonts,amssymb,amsthm,mathtools} %% AMS                         \n"              
          "\\usepackage{icomma} %% \"Умная\" запятая: $0,2$ --- число, $0, 2$ --- перечисление    \n"              
          "%%%% Шрифты                                                          \n"              
          "\\usepackage{euscript}	 %% Шрифт Евклид                          \n"              
          "\\usepackage{mathrsfs} %% Красивый матшрифт                         \n"              
          "                                                                   \n"              
          "%%%%%% Заголовок                                                      \n"              
          "\\title{1.2 Awesome differentiating}                               \n"              
          "\\date{\\today}                                                    \n"              
          "                                                                   \n"              
          "\\begin{document} %% конец преамбулы, начало документа              \n"              
          "                                                                   \n"              
          "\\maketitle                                                        \n"              
          "                                                                   \n"              
          "Here we go again \n"
          "\\[ ";

    os << LatexExpressionFormater::to_string (exprAST);

    os << " \\]\n"
          "\\end{document}\n";

}