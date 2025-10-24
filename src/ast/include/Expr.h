//===--- Expr.h - The 'sa' Language Expression ASTs -------------*- C++ -*-===//
//
// This file defines the base class for all expressions in the 'sa' language.
// An expression is any piece of code that computes a value.
//
//===----------------------------------------------------------------------===//

#pragma once

#include "ast/include/Stmt.h" // An Expr is a kind of Stmt.
#include "core/include/Token.h"
#include <memory>
#include <string_view>
#include <vector>

namespace sa {

// The base class for all expression nodes in the AST.
class Expr : public Stmt {};

// Represents a string literal expression, e.g., "Hello, sa!"
class StringLiteralExpr : public Expr {
    Token StrToken;

public:
    StringLiteralExpr(const Token& token) : StrToken(token) {}

    // Returns the content of the string, without the surrounding quotes.
    std::string_view getValue() const {
        // The lexeme includes the quotes, so we return a substring without them.
        return StrToken.lexeme.substr(1, StrToken.lexeme.length() - 2);
    }
};

// Represents the use of a variable in an expression.
// Example: The 'message' in 'print(message)'.
class VariableExpr : public Expr {
    Token Name;

public:
    VariableExpr(const Token& name) : Name(name) {}

    std::string_view getName() const { return Name.lexeme; }
};

// Represents a function call expression, e.g., print(message).
class CallExpr : public Expr {
    Token Callee;
    std::vector<std::unique_ptr<Expr>> Args;

public:
    CallExpr(const Token& callee, std::vector<std::unique_ptr<Expr>> args)
        : Callee(callee), Args(std::move(args)) {}
};

} // namespace sa