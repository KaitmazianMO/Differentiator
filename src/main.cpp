#include <iostream>
#include <string>
#include <fstream>
#include <Windows.h>

#include "File.h"
#include "Lexer/Token.h"
#include "Lexer/Lexer.h"
#include "Parser/Parser.h"
#include "AST/Graphviz.h"
#include "AST/Differentiator.h"
#include "AST/Latex.h"
#include "DifferentiatorController.h"

using namespace DC;

void printNChar (std::ostream &os, size_t n, char c);
void printLine (std::ostream &os, const char *line);

int main()
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY);
    try 
    {

        std::ifstream expr_in ("D:\\Projects C++\\Differentiator\\src\\Expression.txt");
        std::ofstream latex_expr_out ("D:\\Projects C++\\Differentiator\\src\\Latex.tex");

        DifferentiatorController dc (
            expr_in, latex_expr_out,                   
            Flags::InputFormat (Format::ordinary),
            Flags::OutputFormat (Format::latex),
            Flags::DoInitialTreeDump (true),  
            Flags::DoDifferentiatedTreeDump (true), 
            Flags::DoOptimizedTreeDump (true), 
            Flags::DoErrorConstructedTreeDump (true) 
        );

        dc.run ("x");
    } 
    catch (const LexerException &lex)
    {
        std::cerr << "error: " << lex.what() << std::endl;
    }                                                     
    catch (const ParserException &pex)
    {         
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
        std::cerr << "error(" << pex.errLoc().nline << "): " << pex.what() << std::endl;
        std::cerr << "\'";
        printLine (std::cerr, pex.errLoc().line_beg);
        std::cerr << "\'" << std::endl;
        printNChar (std::cerr, pex.errLoc().first_sym - pex.errLoc().line_beg, ' ');
        std::cerr << "~^~" << std::endl;                                                
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    }
    catch (const std::exception &ex)
    {
        std::cerr << "error: " << ex.what() << std::endl;
    }

    return 0;
}

void printLine (std::ostream &os, const char *line)
{
    while (line && *line && *line != '\n' && *line != '\r')
    {
        os.put (*line++);
    }
}

void printNChar (std::ostream &os, size_t n, char c)
{
    for (int i = 0; i < n; ++i)
    {
        os.put (c);
    }
}