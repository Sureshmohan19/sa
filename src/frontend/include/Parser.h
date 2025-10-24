//===--- Parser.h - The 'sa' Language Parser --------------------*- C++ -*-===//
//
// This file defines the Parser interface.
//
//===----------------------------------------------------------------------===//
#pragma once

#include "core/include/Token.h"
#include "frontend/include/Lexer.h"
#include "ast/include/Decl.h"
#include "ast/include/Stmt.h"
#include "ast/include/Expr.h"
#include <vector>
#include <memory>

namespace sa {

class Parser {
public:
    // Constructor: Initializes the parser with a lexer.
    Parser(Lexer& lexer);

    // The main entry point. Parses the entire source file and returns the
    // root of the AST (a list of all top-level declarations).
    std::vector<std::unique_ptr<Decl>> parse();

private:
    Lexer& lexer;
    Token currentToken;
    Token previousToken;

    // --- Core Parsing Primitives ---
    // Advances the token stream.
    void advance();
    // Checks the current token type and consumes it, errors if it doesn't match.
    void consume(tok::TokenKind kind, const char* message);
    // Checks if the current token matches a given kind.
    bool match(tok::TokenKind kind);
    // Checks if we are at the end of the token stream.
    bool isAtEnd() const;

    // --- Grammar Rule Parsing Methods ---
    std::unique_ptr<Decl> parseTopLevelDecl();
    std::unique_ptr<FunctionDecl> parseFunctionDefinition();
    std::unique_ptr<Stmt> parseStatement();
    std::unique_ptr<DeclStmt> parseVarDeclStatement();
    std::unique_ptr<ExprStmt> parseExprStatement();

    std::unique_ptr<Expr> parseExpression();
    std::unique_ptr<Expr> parsePrimaryExpression();
};

} // namespace sa