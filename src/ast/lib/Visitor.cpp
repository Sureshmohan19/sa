//===--- Visitor.cpp - AST Visitor Implementations --------------*- C++ -*-===//
//
// This file implements the accept methods for all AST nodes.
//
//===----------------------------------------------------------------------===//

#include "ast/include/Visitor.h"
#include "ast/include/Decl.h"
#include "ast/include/Stmt.h"
#include "ast/include/Expr.h"

namespace sa {

// Declaration accept methods
void FunctionDecl::accept(Visitor& visitor) {
    visitor.visit(*this);
}

void VarDecl::accept(Visitor& visitor) {
    visitor.visit(*this);
}

// Statement accept methods
void DeclStmt::accept(Visitor& visitor) {
    visitor.visit(*this);
}

void ExprStmt::accept(Visitor& visitor) {
    visitor.visit(*this);
}

// Expression accept methods
void StringLiteralExpr::accept(Visitor& visitor) {
    visitor.visit(*this);
}

void VariableExpr::accept(Visitor& visitor) {
    visitor.visit(*this);
}

void CallExpr::accept(Visitor& visitor) {
    visitor.visit(*this);
}

} // namespace sa