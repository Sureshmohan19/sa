//===--- Parser.cpp - The 'sa' Language Parser --------------------*- C++ -*-===//
//
// This file implements the Parser class.
//
//===----------------------------------------------------------------------===//

#include "frontend/include/Parser.h"
#include <iostream>

namespace sa {

Parser::Parser(Lexer& lexer) : lexer(lexer) {
    // Prime the parser with the first token.
    advance();
}

// The main entry point.
std::vector<std::unique_ptr<Decl>> Parser::parse() {
    std::vector<std::unique_ptr<Decl>> declarations;

    while (!isAtEnd()) {
        declarations.push_back(parseTopLevelDecl());
    }

    return declarations;
}

// --- Core Primitives ---

void Parser::advance() {
    previousToken = currentToken;
    currentToken = lexer.scanNextToken();
}

void Parser::consume(tok::TokenKind kind, const char* message) {
    if (currentToken.kind == kind) {
        advance();
        return;
    }
    std::cerr << "Parse Error on line " << previousToken.line << ": " << message << std::endl;
    // For now, we'll exit on first error. A real compiler would try to recover.
    exit(1);
}

bool Parser::match(tok::TokenKind kind) {
    if (currentToken.kind == kind) {
        advance();
        return true;
    }
    return false;
}

bool Parser::isAtEnd() const {
    return currentToken.kind == tok::eof;
}


// --- Grammar Rule Implementations ---

std::unique_ptr<Decl> Parser::parseTopLevelDecl() {
    if (match(tok::kw_fn)) {
        return parseFunctionDefinition();
    }
    // In the future, we could parse 'struct', 'import', etc. here.
    std::cerr << "Parse Error: Expected a top-level declaration (like 'fn')." << std::endl;
    exit(1);
}

std::unique_ptr<FunctionDecl> Parser::parseFunctionDefinition() {
    Token name = currentToken;
    consume(tok::identifier, "Expected function name.");
    consume(tok::l_paren, "Expected '(' after function name.");
    consume(tok::r_paren, "Expected ')' after parameters.");
    consume(tok::arrow, "Expected '->' for return type.");
    consume(tok::kw_void, "Expected 'void' as return type for now.");
    consume(tok::l_brace, "Expected '{' before function body.");

    std::vector<std::unique_ptr<Stmt>> body;
    while (currentToken.kind != tok::r_brace && !isAtEnd()) {
        body.push_back(parseStatement());
    }

    consume(tok::r_brace, "Expected '}' after function body.");

    return std::make_unique<FunctionDecl>(name, std::move(body));
}

std::unique_ptr<Stmt> Parser::parseStatement() {
    if (match(tok::kw_let)) {
        return parseVarDeclStatement();
    }
    return parseExprStatement();
}

std::unique_ptr<DeclStmt> Parser::parseVarDeclStatement() {
    Token name = currentToken;
    consume(tok::identifier, "Expected variable name.");
    consume(tok::equal, "Expected '=' after variable name.");

    std::unique_ptr<Expr> initializer = parseExpression();

    consume(tok::semicolon, "Expected ';' after variable declaration.");

    auto varDecl = std::make_unique<VarDecl>(name, std::move(initializer));
    return std::make_unique<DeclStmt>(std::move(varDecl));
}

std::unique_ptr<ExprStmt> Parser::parseExprStatement() {
    std::unique_ptr<Expr> expr = parseExpression();
    consume(tok::semicolon, "Expected ';' after expression.");
    return std::make_unique<ExprStmt>(std::move(expr));
}

std::unique_ptr<Expr> Parser::parseExpression() {
    // For now, our only expressions are primary expressions (literals, identifiers, calls)
    return parsePrimaryExpression();
}

std::unique_ptr<Expr> Parser::parsePrimaryExpression() {
    if (match(tok::string_literal)) {
        return std::make_unique<StringLiteralExpr>(previousToken);
    }

    if (match(tok::identifier)) {
        Token callee = previousToken;
        if (match(tok::l_paren)) {
            // It's a function call
            std::vector<std::unique_ptr<Expr>> args;
            if (currentToken.kind != tok::r_paren) {
                // Parse arguments (just one for now for hello.sa)
                args.push_back(parseExpression());
            }
            consume(tok::r_paren, "Expected ')' after arguments.");
            return std::make_unique<CallExpr>(callee, std::move(args));
        } else {
            // It's a variable usage
            return std::make_unique<VariableExpr>(callee);
        }
    }

    std::cerr << "Parse Error: Expected an expression." << std::endl;
    exit(1);
}

} // namespace sa