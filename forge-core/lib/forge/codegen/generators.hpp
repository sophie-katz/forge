// Copyright (c) 2024 Sophie Katz
//
// This file is part of Forge.
//
// Forge is free software: you can redistribute it and/or modify it under the terms of
// the GNU General Public License as published by the Free Software Foundation, either
// version 3 of the License, or (at your option) any later version.
//
// Forge is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
// PURPOSE. See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with Forge.
// If not, see <https://www.gnu.org/licenses/>.

#pragma once

#include <llvm/IR/Function.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Type.h>

extern "C" {
#include <forge/ast/ast.h>
#include <forge/ast/scope.h>

llvm::Type* _frg_generate_type(
    llvm::LLVMContext& ctx,
    frg_ast_scope_t* scope,
    frg_ast_t* ast
);

void _frg_generate_decl_fn(
    llvm::IRBuilder<>& builder,
    llvm::LLVMContext& ctx,
    llvm::Module& module,
    frg_ast_scope_t* scope,
    frg_ast_t* ast
);

void _frg_generate_stmt(
    llvm::IRBuilder<>& builder,
    llvm::LLVMContext& ctx,
    frg_ast_scope_t* scope,
    frg_ast_t* ast
);

llvm::Value* _frg_generate_value(
    llvm::LLVMContext& ctx,
    frg_ast_scope_t* scope,
    frg_ast_t* ast
);
}
