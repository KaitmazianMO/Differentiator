#include <iostream>
#include <string>
#include <fstream>
#include <Windows.h>
#include <sstream>

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
    try 
    {
        std::ifstream expr_in ("D:\\Projects C++\\Differentiator\\src\\Expression.txt");
        std::ofstream latex_expr_out ("D:\\Projects C++\\Differentiator\\src\\Latex.tex");

        DifferentiatorController dc (                 
            Flags::DoInitialTreeDump (true),  
            Flags::DoDifferentiatedTreeDump (true), 
            Flags::DoOptimizedTreeDump (true), 
            Flags::DoErrorConstructedTreeDump (true) 
        );

        dc.setDifferentiationVariable ("x", 42);
        if (dc.differentiate (expr_in))
        { 
            dc.write (latex_expr_out, Format::latex);
            std::cout << "Result = " << dc.compute() << std::endl;
        }
        else
        {
            std::cerr << "Differentiation failed\n";
        }
    }                                            
    catch (const BasicException &bex)
    {         
        SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), FOREGROUND_RED);
        std::cerr << "error(" << bex.errLoc().nline << "): " << bex.what() << std::endl;
        std::cerr << "\'";
        printLine (std::cerr, bex.errLoc().line_beg);
        std::cerr << "\'" << std::endl;
        printNChar (std::cerr, bex.errLoc().first_sym - bex.errLoc().line_beg, ' ');
        std::cerr << "~^~" << std::endl;                                                
        SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    }
    catch (const std::exception &ex)
    {
        SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), FOREGROUND_RED);
        std::cerr << "error: " << ex.what() << std::endl;
        SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
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