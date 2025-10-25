//===--- Visitor.h - AST Visitor Interface ----------------------*- C++ -*-===//
//
// This file defines the Visitor interface for traversing the AST.
//
//===----------------------------------------------------------------------===//

#pragma once

namespace sa {

// Forward declarations of all AST node types
class FunctionDecl;
class VarDecl;
class DeclStmt;
class ExprStmt;
class StringLiteralExpr;
class VariableExpr;
class CallExpr;

// The Visitor base class defines the interface for visiting AST nodes.
// Any class that wants to traverse the AST should inherit from this class
// and implement the visit methods for the node types it cares about.
class Visitor {
public:
    virtual ~Visitor() = default;

    // Declaration visitors
    virtual void visit(FunctionDecl& decl) = 0;
    virtual void visit(VarDecl& decl) = 0;

    // Statement visitors
    virtual void visit(DeclStmt& stmt) = 0;
    virtual void visit(ExprStmt& stmt) = 0;

    // Expression visitors
    virtual void visit(StringLiteralExpr& expr) = 0;
    virtual void visit(VariableExpr& expr) = 0;
    virtual void visit(CallExpr& expr) = 0;
};

} // namespace sa