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
#include "llvm/TargetParser/Triple.h"

namespace sa {

// A global map to hold the result of visiting an expression.
static llvm::Value* V;

CodeGen::CodeGen() {
    TheContext = std::make_unique<llvm::LLVMContext>();
    TheModule = std::make_unique<llvm::Module>("sa_module", *TheContext);
    Builder = std::make_unique<llvm::IRBuilder<>>(*TheContext);

    // Set the target triple so LLVM emits macOS-compatible ARM64 objects
    llvm::Triple triple("arm64-apple-macos");
    TheModule->setTargetTriple(triple);
    
    // Set the data layout for macOS ARM64
    TheModule->setDataLayout("e-m:o-i64:64-i128:128-n32:64-S128");
}

void CodeGen::run(const std::vector<std::unique_ptr<Decl>>& ast) {
    // --- The External 'print' Function ---
    // We assume it's like a C `void print(char*)`. In modern LLVM, this is `void(ptr)`.

    // 1. Get the modern opaque pointer type `ptr`.
    llvm::Type* PtrType = Builder->getPtrTy();

    // 2. Define the function type: `void(ptr)`.
    llvm::FunctionType* PrintFuncType = llvm::FunctionType::get(
        Builder->getVoidTy(), {PtrType}, false);

    // 3. Declare the function in our LLVM Module.
    TheModule->getOrInsertFunction("print", PrintFuncType);

    // --- The Main Code Generation Loop ---
    for (const auto& decl : ast) {
        decl->accept(*this);
    }

    // After visiting all nodes, print the generated IR to stdout.
    TheModule->print(llvm::outs(), nullptr);
}

// --- Visitor Method Implementations ---
void CodeGen::visit(FunctionDecl& decl) {
    // Detect if this is the 'main' function
    bool isMain = (decl.getName() == "main");

    // Use int32 return type for main, void otherwise
    llvm::Type* returnType = isMain ? Builder->getInt32Ty() : Builder->getVoidTy();

    // Use the function name as-is; the Mach-O mangling will add underscore automatically
    llvm::Function* TheFunction = TheModule->getFunction(decl.getName());
    if (!TheFunction) {
        llvm::FunctionType* FT = llvm::FunctionType::get(returnType, false);
        TheFunction = llvm::Function::Create(FT, llvm::Function::ExternalLinkage,
                                             decl.getName(), TheModule.get());
    }

    llvm::BasicBlock* BB = llvm::BasicBlock::Create(*TheContext, "entry", TheFunction);
    Builder->SetInsertPoint(BB);
    NamedValues.clear();

    // Generate function body
    for (const auto& stmt : decl.getBody()) {
        stmt->accept(*this);
    }

    // Return type depends on whether it's main or not
    if (isMain) {
        Builder->CreateRet(llvm::ConstantInt::get(Builder->getInt32Ty(), 0));
    } else {
        Builder->CreateRetVoid();
    }

    llvm::verifyFunction(*TheFunction);
}

void CodeGen::visit(VarDecl& decl) {
    decl.getInitializer()->accept(*this);
    llvm::Value* InitializerValue = V;

    if (!InitializerValue) {
        std::cerr << "CodeGen Error: VarDecl initializer is null." << std::endl;
        return;
    }

    llvm::AllocaInst* Alloca = Builder->CreateAlloca(InitializerValue->getType(),
                                                    nullptr, decl.getName());
    Builder->CreateStore(InitializerValue, Alloca);
    NamedValues[decl.getName()] = Alloca;
}

void CodeGen::visit(DeclStmt& stmt) {
    stmt.getDecl()->accept(*this);
}

void CodeGen::visit(ExprStmt& stmt) {
    stmt.getExpr()->accept(*this);
}

void CodeGen::visit(StringLiteralExpr& expr) {
    V = Builder->CreateGlobalString(expr.getValue());
}

void CodeGen::visit(VariableExpr& expr) {
    llvm::Value* VarAlloca = NamedValues[expr.getName()];
    if (!VarAlloca) {
        std::cerr << "CodeGen Error: Unknown variable name '" << expr.getName() << "'." << std::endl;
        V = nullptr;
        return;
    }
    V = Builder->CreateLoad(static_cast<llvm::AllocaInst*>(VarAlloca)->getAllocatedType(),
                            VarAlloca, expr.getName());
}

void CodeGen::visit(CallExpr& expr) {
    llvm::Function* CalleeF = TheModule->getFunction(expr.getCalleeName());
    if (!CalleeF) {
        std::cerr << "CodeGen Error: Unknown function referenced: " << expr.getCalleeName() << std::endl;
        V = nullptr;
        return;
    }

    std::vector<llvm::Value*> ArgsV;
    for (const auto& arg : expr.getArgs()) {
        arg->accept(*this);
        ArgsV.push_back(V);
    }

    // Only give the call a name if it returns a non-void value
    if (CalleeF->getReturnType()->isVoidTy()) {
        V = Builder->CreateCall(CalleeF, ArgsV);
    } else {
        V = Builder->CreateCall(CalleeF, ArgsV, "calltmp");
    }
}

llvm::Value* CodeGen::visitExpression(Expr* expr) {
    return nullptr;
}

} // namespace sa