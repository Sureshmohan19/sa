// main.cpp
//
// This file is part of the 'sac' compiler for the 'sa' programming language.
//
// The code block below, written in the target 'sa' language, represents the
// absolute first program we aim to make functional. This is our "Milestone 1".
// Achieving this will validate the entire compiler pipeline, from the lexer
// through the parser, AST generation, semantic analysis, and finally, LLVM
// code generation.
//
// ============================================================================
//                 TARGET PROGRAM FOR MILESTONE 1 (sa v0.1)
// ============================================================================
/*

 fn main() -> void {
     let message = "Hello, sa!";
     print(message);
 }

*/
// ============================================================================
//
// To achieve this, the compiler must be able to handle:
// - Keywords: 'fn', 'void', 'let'
// - Identifiers: 'main', 'message', 'print'
// - Literals: String literals ("...")
// - Punctuation: '(', ')', '{', '}', '->', ';', '='
// - Core Concepts: Function definitions, variable declarations, and function calls.
//
// ============================================================================

#include "core/include/Token.h"
#include "frontend/include/Lexer.h"
#include "frontend/include/Parser.h" 
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

// We will write a simple AST printer later to test this properly.
// For now, we just want it to compile and run without crashing.

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Usage: sac <filename.sa>" << std::endl;
        return 1;
    }

    std::ifstream file(argv[1]);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file '" << argv[1] << "'" << std::endl;
        return 1;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string sourceCode = buffer.str();

    sa::Lexer lexer(sourceCode);
    sa::Parser parser(lexer);

    std::cout << "--- Parsing file: " << argv[1] << " ---" << std::endl;
    
    // The magic happens here!
    auto ast = parser.parse();

    if (!ast.empty()) {
        std::cout << "Successfully parsed the source file!" << std::endl;
        std::cout << "Found " << ast.size() << " top-level declaration(s)." << std::endl;
    } else {
        std::cerr << "Parsing failed." << std::endl;
        return 1;
    }

    return 0;
}