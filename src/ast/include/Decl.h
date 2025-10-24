//===--- Decl.h - The 'sa' Language Declaration ASTs ------------*- C++ -*-===//
//
// This file defines the base class for all declarations in the 'sa' language.
//
//===----------------------------------------------------------------------===//

#pragma once

#include "core/include/Token.h"
#include <memory>
#include <string_view>
#include <vector>

namespace sa {

// Forward-declarations for other AST node types.
class Stmt;
class Expr;

// The base class for all AST nodes.
class ASTNode {
public:
    virtual ~ASTNode() = default;
};

// The base class for all declaration nodes (e.g., functions, variables).
class Decl : public ASTNode {
protected:
    Token Name;

public:
    Decl(const Token& name) : Name(name) {}

    std::string_view getName() const { return Name.lexeme; }
};

// Represents a variable declaration: 'let message = "Hello, sa!";'
class VarDecl : public Decl {
    // We use unique_ptr for automatic memory management of the AST.
    // The VarDecl 'owns' its initializer expression.
    std::unique_ptr<Expr> Initializer;

public:
    VarDecl(const Token& name, std::unique_ptr<Expr> initializer)
        : Decl(name), Initializer(std::move(initializer)) {}
};

// Represents a function declaration: 'fn main() -> void { ... }'
class FunctionDecl : public Decl {
    // The function 'owns' all the statements in its body.
    std::vector<std::unique_ptr<Stmt>> Body;

public:
    FunctionDecl(const Token& name, std::vector<std::unique_ptr<Stmt>> body)
        : Decl(name), Body(std::move(body)) {}
};

} // namespace sa