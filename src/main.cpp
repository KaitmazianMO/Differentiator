#include <iostream>
#include <string>
#include <fstream>

#include "File.h"
#include "Lexer/Token.h"
#include "Lexer/Lexer.h"
#include "Parser/Parser.h"
#include "AST/Graphviz.h"
#include "AST/Differentiator.h"
#include "AST/Latex.h"
#include "DifferentiatorController.h"

using namespace DC;

int main()
{
    std::ifstream expr ("D:\\Projects C++\\Differentiator\\src\\Expression.txt");

    DifferentiatorController dc (
        expr, std::cout,                   
        Flags::InputFormat (Format::ordinary),
        Flags::OutputFormat (Format::ordinary),
        Flags::DoInitialTreeDump (true),  
        Flags::DoDifferentiatedTreeDump (true), 
        Flags::DoOptimizedTreeDump (true), 
        Flags::DoErrorConstructedTreeDump (true) 
    );

    dc.run();

    //std::ifstream source_stream ("D:\\Projects C++\\Differentiator\\src\\Expression.txt");
    //std::string source_code = readFromStream (source_stream);
    //Lexer lexer (source_code.c_str());
    
    
    //try 
    //{
    //    std::cout << "Creating lexer" << std::endl;
    //                                              
    //    std::cout << "Tokenizing" << std::endl;;
    //    lexer.tokenize();
    //                                          
    //    std::cout << "Dump" << std::endl;
    //    lexer.dump();
    //
    //    std::cout << "Start parsing" << std::endl;
    //    Parser parser;
    //    parser.parse (lexer.getFirstTokenIt(), lexer.getLastTokenIt());
    //
    //
    //    auto AST = parser.getAST();
    //    GraphvizPrint gv_printer ("Expression graph");
    //    AST->doAction (&gv_printer);
    //
    //    std::cout << "Start differentiating" << std::endl;
    //    Differentiator differentiate;
    //    auto diff_AST = AST->getAction (&differentiate);
    //                                                            
    //    GraphvizPrint gv_diff_printer ("Differetiated expression graph");
    //    diff_AST->doAction (&gv_diff_printer);
    //
    //    LatexPrint latex_printer ("Formated euqation");
    //    diff_AST->doAction (&latex_printer);
    //
    //    //Token tok;
    //    //
    //    //std::cout << "Init table test\n" <<
    //    //    "( = " << Token::toString (TOKlparen) <<  "\n" <<
    //    //    "+ = " << Token::toString (TOKadd) <<     "\n" <<
    //    //    ") = " << Token::toString (TOKrparen);
    //}                    
    //catch (const LexerException& ex)
    //{
    //    std::cout << "Exception: " << ex.what() << std::endl;
    //}
    //catch (const ParserException& ex)
    //{
    //    std::cout << "Exception: " << ex.what() << "(" << ex.errLoc().str() << ")" << std::endl;
    //}
    //catch (std::exception& ex)
    //{
    //    std::cout << "Catched some ex: " << ex.what() << std::endl;
    //}
    return 0;
}
