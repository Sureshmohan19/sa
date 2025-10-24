//===--- CodeGen.cpp - The 'sa' Language Code Generator ---------*- C++ -*-===//
//
// This file implements the CodeGen class.
//
//===----------------------------------------------------------------------===//

#include "backend/include/CodeGen.h"
#include <iostream>
#include <vector>

// --- LLVM Headers ---
#include "llvm/IR/Function.h"
#include "llvm/IR/Verifier.h"
#include "llvm/IR/Type.h"

namespace sa {

// A global map to hold the result of visiting an expression.
// This is a simple mechanism to pass the llvm::Value* from expression
// visitors back to their callers. A more advanced compiler might use a stack.
static llvm::Value* V;

CodeGen::CodeGen() {
    TheContext = std::make_unique<llvm::LLVMContext>();
    TheModule = std::make_unique<llvm::Module>("sa_module", *TheContext);
    Builder = std::make_unique<llvm::IRBuilder<>>(*TheContext);
}

void CodeGen::run(const std::vector<std::unique_ptr<Decl>>& ast) {
    // --- The External 'print' Function ---
    // First, we declare the external 'print' function so that our generated
    // code can call it. We assume it's like a C `void print(char*)`.
    
    // 1. Get the type for `char*`, which is a pointer to an 8-bit integer.
    llvm::Type* CharPtrType = Builder->getInt8PtrTy();

    // 2. Define the function type: `void(char*)`.
    llvm::FunctionType* PrintFuncType = llvm::FunctionType::get(
        Builder->getVoidTy(), {CharPtrType}, false);

    // 3. Declare the function in our LLVM Module.
    TheModule->getOrInsertFunction("print", PrintFuncType);

    // --- The Main Code Generation Loop ---
    // Now, walk the AST and generate code for each top-level declaration.
    for (const auto& decl : ast) {
        decl->accept(*this);
    }
    
    // After visiting all nodes, print the generated IR.
    TheModule->print(llvm::errs(), nullptr);
}

// --- Visitor Method Implementations ---

void CodeGen::visit(FunctionDecl& decl) {
    // Look up the function in the module's symbol table.
    llvm::Function* TheFunction = TheModule->getFunction(decl.getName());
    if (!TheFunction) {
        // If it's not found, we need to create it.
        // For now, we hard-code the type of 'main' as 'void()'.
        llvm::FunctionType* FT = llvm::FunctionType::get(Builder->getVoidTy(), false);
        TheFunction = llvm::Function::Create(FT, llvm::Function::ExternalLinkage,
                                             decl.getName(), TheModule.get());
    }

    // Create a new basic block to start insertion into.
    llvm::BasicBlock* BB = llvm::BasicBlock::Create(*TheContext, "entry", TheFunction);
    Builder->SetInsertPoint(BB);

    // Clear any old values from previous functions.
    NamedValues.clear();

    // Visit all the statements in the function's body.
    for (const auto& stmt : decl.getBody()) {
        stmt->accept(*this);
    }

    // Finish off the function.
    Builder->CreateRetVoid();

    // Validate the generated code, checking for consistency.
    llvm::verifyFunction(*TheFunction);
}

void CodeGen::visit(VarDecl& decl) {
    // Get the initializer expression by visiting it.
    // The result will be stored in the global 'V'.
    decl.getInitializer()->accept(*this);
    llvm::Value* InitializerValue = V;

    if (!InitializerValue) {
        std::cerr << "CodeGen Error: VarDecl initializer is null." << std::endl;
        return;
    }

    // Create a mutable variable on the stack (an alloca).
    llvm::AllocaInst* Alloca = Builder->CreateAlloca(InitializerValue->getType(),
                                                    nullptr, decl.getName());
    
    // Store the initializer's value into the allocated stack memory.
    Builder->CreateStore(InitializerValue, Alloca);

    // Add the new variable to our symbol table so other expressions can find it.
    NamedValues[decl.getName()] = Alloca;
}

void CodeGen::visit(DeclStmt& stmt) {
    // A DeclStmt just wraps a Decl, so we visit the inner declaration.
    stmt.getDecl()->accept(*this);
}

void CodeGen::visit(ExprStmt& stmt) {
    // An ExprStmt just wraps an Expr, so we visit the inner expression.
    // The resulting value is discarded.
    stmt.getExpr()->accept(*this);
}

void CodeGen::visit(StringLiteralExpr& expr) {
    // Create a global string constant in the LLVM Module.
    V = Builder->CreateGlobalStringPtr(expr.getValue());
}

void CodeGen::visit(VariableExpr& expr) {
    // Look up the variable in our symbol table.
    llvm::Value* VarAlloca = NamedValues[expr.getName()];
    if (!VarAlloca) {
        std::cerr << "CodeGen Error: Unknown variable name '" << expr.getName() << "'." << std::endl;
        V = nullptr;
        return;
    }
    
    // A variable expression *loads* the value from its memory location.
    V = Builder->CreateLoad(VarAlloca->getType()->getPointerElementType(),
                            VarAlloca, expr.getName());
}

void CodeGen::visit(CallExpr& expr) {
    // 1. Look up the name of the function in the LLVM module's symbol table.
    llvm::Function* CalleeF = TheModule->getFunction(expr.getCalleeName());
    if (!CalleeF) {
        std::cerr << "CodeGen Error: Unknown function referenced: " << expr.getCalleeName() << std::endl;
        V = nullptr;
        return;
    }

    // 2. Generate code for each argument.
    std::vector<llvm::Value*> ArgsV;
    for (const auto& arg : expr.getArgs()) {
        arg->accept(*this); // Visit the argument expression.
        ArgsV.push_back(V);   // The result is in the global 'V'.
    }

    // 3. Generate the 'call' instruction.
    V = Builder->CreateCall(CalleeF, ArgsV, "calltmp");
}

// visitExpression is no longer needed with our global 'V' approach for now.
// It can be removed or left as a placeholder.
llvm::Value* CodeGen::visitExpression(Expr* expr) {
    return nullptr;
}

} // namespace sa