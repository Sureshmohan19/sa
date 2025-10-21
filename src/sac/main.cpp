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