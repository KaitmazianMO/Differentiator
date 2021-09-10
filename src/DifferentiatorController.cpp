#include <assert.h>
#include <iostream>
#include <fstream>
#include <string>
#include <Windows.h>

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
inline ExpressionNode *buildAST (const std::string &expr);

DC::DifferentiatorController::DifferentiatorController (               
    Flags::DoInitialTreeDump initial_tree_dump,  
    Flags::DoDifferentiatedTreeDump differentiated_tree_dump, 
    Flags::DoOptimizedTreeDump optimized_tree_dump, 
    Flags::DoErrorConstructedTreeDump error_constructed_tree_dump 
)
{
    flags.initial_tree_dump           = initial_tree_dump;
    flags.differentiated_tree_dump    = differentiated_tree_dump;
    flags.optimized_tree_dump         = optimized_tree_dump;
    flags.error_constructed_tree_dump = error_constructed_tree_dump;
}

//void DC::DifferentiatorController::run (const DC::Context &context)  
//{   
//    SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY);
//    log ("Start reading expression...");
//    static const std::string expression = readFromStream (expression_stream); // Token locations have pointers to this string all the time, so after we got exception the string is distructed on the higher levels and we can't use it. So avoid the string distuctor.
//    std::cout << "Expr = " << expression << std::endl;
//    if (expression.empty())
//        return;
//    log ("Finished reading.");
//
//    run (expression, context);
//}
//
//void DC::DifferentiatorController::run (const std::string &expression, const DC::Context &context)
//{
//    ExpressionNode *AST = nullptr;
//    try 
//    { 
//        AST = buildAST (expression);
//    }
//    catch (const BasicException &bex)
//    {
//        if (flags.error_constructed_tree_dump.active) 
//            GraphvizPrinter::dump ("InitialExpression(error)", AST);
//        ASTDeleter::free (AST);
//        throw;
//    }                                                    
//    log ("Finished parsing.");
//
//    if (flags.initial_tree_dump.active) 
//        GraphvizPrinter::dump ("InitialExpression", AST);
//
//    log ("Differentiating...");
//    auto diff_AST = Differentiator::differentiate (AST, context);
//    log ("Differentiating's finished.");
//
//    if (flags.differentiated_tree_dump.active) 
//        GraphvizPrinter::dump ("DifferentiatedExpression", diff_AST);
//
//    log ("Deleting source AST...");
//    ASTDeleter::free (AST); 
//    
//    log ("Writing...");
//    if (flags.output.format == Format::latex)
//        massiveLatexEquationWritingPattern (output_stream, diff_AST);
//    else if (flags.output.format == Format::ordinary)
//        output_stream << OrdinaryExpressionFormater::to_string (diff_AST) << std::endl;
//    log ("Finished writing.");
//    
//    //optimize (diff_AST);                                                                                         
//    SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
//    //std::cout << "Enter " << diff_var << ": ";
//    //double dval = 0;
//    //std::cin >> dval;
//    //context.setValue (diff_var, dval);
//    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_GREEN);
//    std::cout << "Derivative value: " << Calculator::calculate (diff_AST, context) << std::endl;
//    SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
//
//    ASTDeleter::free (diff_AST);    
//}

void DC::DifferentiatorController::differentiate (std::ifstream &expr_is)
{
    SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY);
    log ("Start reading expression...");
    static const std::string expression = readFromStream (expr_is); // Token locations have pointers to this string all the time, so after we got exception the string is distructed on the higher levels and we can't use it. So avoid the string distuctor.
    std::cout << "Expr = " << expression << std::endl;
    if (expression.empty())
        return;
    log ("Finished reading.");

    differentiate (expression);
}

void DC::DifferentiatorController::differentiate (const std::string &expression)
{   
    if (diffAST)
    {
        ASTDeleter::free (diffAST);
    }

    ExpressionNode *AST = nullptr;
    try 
    { 
        AST = buildAST (expression);
    }
    catch (const BasicException &bex)
    {
        if (flags.error_constructed_tree_dump.active) 
            GraphvizPrinter::dump ("InitialExpression(error)", AST);
        ASTDeleter::free (AST);
        throw;
    }                                                    
    log ("Finished parsing.");

    if (flags.initial_tree_dump.active) 
        GraphvizPrinter::dump ("InitialExpression", AST);

    log ("Differentiating...");
    diffAST = Differentiator::differentiate (AST, context);
    log ("Differentiating's finished.");

    if (flags.differentiated_tree_dump.active) 
        GraphvizPrinter::dump ("DifferentiatedExpression", diffAST);

    log ("Deleting source AST...");
    ASTDeleter::free (AST);     
}

void DC::DifferentiatorController::write (std::ostream &os, Format format)
{
    log ("Writing...");
    if (diffAST)
    {
        if (format == Format::latex)
            massiveLatexEquationWritingPattern (os, diffAST);
        else if (format == Format::ordinary)
            os << OrdinaryExpressionFormater::to_string (diffAST) << std::endl;
    }
    log ("Finished writing.");
}

void DC::DifferentiatorController::setDifferentiationVariable (const std::string &name, double val)
{
    context.setDiffVar (name, val);
}

double DC::DifferentiatorController::compute()
{
    return Calculator::calculate (diffAST, context);
}

Flags DC::DifferentiatorController::bind (
    Flags::DoInitialTreeDump initial_tree_dump,  
    Flags::DoDifferentiatedTreeDump differentiated_tree_dump, 
    Flags::DoOptimizedTreeDump optimized_tree_dump, 
    Flags::DoErrorConstructedTreeDump error_constructed_tree_dump 
)
{
    Flags old_flags                   = flags;
    
    flags.initial_tree_dump           = initial_tree_dump;
    flags.differentiated_tree_dump    = differentiated_tree_dump;
    flags.optimized_tree_dump         = optimized_tree_dump;
    flags.error_constructed_tree_dump = error_constructed_tree_dump;

    return old_flags;
}

Flags DC::DifferentiatorController::bind (Flags flags_)
{
    return bind (
        flags_.initial_tree_dump,
        flags_.differentiated_tree_dump,
        flags_.optimized_tree_dump,
        flags_.error_constructed_tree_dump
    );
}

std::string readFromStream (std::istream& stream)
{
    std::string read;
    std::string s;
    while (getline (stream, s, '\0'))
    {
        read += s;
    }
    return read;
}

ExpressionNode *buildAST (const std::string &expr)
{
    log ("Start tokenizing..." );
    Lexer lexer (expr.c_str());
    lexer.tokenize();                                                                               
    log ("Finished tokenizing.");

    log ("Start parsing...");
    Parser parser;
    parser.parse (lexer.getFirstTokenIt(), lexer.getLastTokenIt());                                                 
    log ("Finished parsing.");

    return parser.getAST();
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