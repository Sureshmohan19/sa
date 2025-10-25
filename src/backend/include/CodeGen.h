//===--- CodeGen.h - The 'sa' Language Code Generator -----------*- C++ -*-===//
//
// This file defines the CodeGen class, which is responsible for generating
// LLVM IR from the 'sa' Abstract Syntax Tree.
//
//===----------------------------------------------------------------------===//

#pragma once

#include "ast/include/Visitor.h"
#include "ast/include/Decl.h"
#include "ast/include/Stmt.h"
#include "ast/include/Expr.h"

// --- LLVM Headers ---
// We will need these for the LLVM C++ API.
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Module.h"

#include <map>
#include <string_view>

namespace sa {

class CodeGen : public Visitor {
public:
    // Constructor.
    CodeGen();

    // The main entry point to generate code for the entire AST.
    void run(const std::vector<std::unique_ptr<Decl>>& ast);

private:
    // --- LLVM Core Objects ---
    // The LLVMContext is a core LLVM data structure that owns and manages
    // various core LLVM data structures.
    std::unique_ptr<llvm::LLVMContext> TheContext;

    // The IRBuilder is a helper object that makes it easy to generate
    // LLVM instructions.
    std::unique_ptr<llvm::IRBuilder<>> Builder;

    // A Module is the top-level container for all other LLVM IR objects.
    std::unique_ptr<llvm::Module> TheModule;

    // --- Symbol Table ---
    // This map keeps track of which named values are in the current scope.
    // In our simple case, it will map variable names to their memory location.
    std::map<std::string_view, llvm::Value*> NamedValues;

    // --- Visitor Methods ---
    // We will override the 'visit' method for each of our AST node types
    // to generate the corresponding LLVM IR.
    void visit(FunctionDecl& decl) override;
    void visit(VarDecl& decl) override;
    void visit(DeclStmt& stmt) override;
    void visit(ExprStmt& stmt) override;
    void visit(StringLiteralExpr& expr) override;
    void visit(VariableExpr& expr) override;
    void visit(CallExpr& expr) override;

    // We will need a way to get the result of visiting an expression.
    // This will be crucial.
    llvm::Value* visitExpression(Expr* expr);
};

} // namespace sa