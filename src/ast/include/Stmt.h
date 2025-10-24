//===--- Stmt.h - The 'sa' Language Statement ASTs --------------*- C++ -*-===//
//
// This file defines the base class for all statements in the 'sa' language.
//
//===----------------------------------------------------------------------===//

#pragma once

#include "ast/include/Decl.h" // For ASTNode
#include <memory>

namespace sa {

// Forward-declarations to avoid circular include dependencies.
class Decl;
class Expr;

// The base class for all statement nodes in the AST.
// A statement is an action that can be executed.
class Stmt : public ASTNode {};

// Represents a statement that is just a declaration.
// This is an "adaptor" class that allows a Decl (like a variable declaration)
// to be used in a list of statements (like a function body).
// Example: The line 'let message = "Hello, sa!";' will be a VarDecl
// wrapped inside a DeclStmt.
class DeclStmt : public Stmt {
    std::unique_ptr<Decl> D;

public:
    DeclStmt(std::unique_ptr<Decl> d) : D(std::move(d)) {}
};

// Represents a statement that is just an expression.
// Any expression can be used as a statement (its value is just discarded).
// Example: The function call 'print(message);' is a CallExpr wrapped
// inside an ExprStmt.
class ExprStmt : public Stmt {
    std::unique_ptr<Expr> E;

public:
    ExprStmt(std::unique_ptr<Expr> e) : E(std::move(e)) {}
};

} // namespace sa