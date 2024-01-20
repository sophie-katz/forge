// Copyright (c) 2023-2024 Sophie Katz
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

#include "generators.hpp"
#include <llvm/IR/Constants.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/TypedPointerType.h>

extern "C" {
#include <forge/common/error.h>

llvm::Type* _frg_generate_type(
    llvm::LLVMContext& ctx,
    frg_ast_scope_t* scope,
    frg_ast_t* ast
) {
    frg_assert_pointer_non_null(scope);
    frg_assert_pointer_non_null(ast);

    llvm::Type* value = NULL;
    
    switch (ast->id) {
        case FRG_AST_ID_TY_BOOL:
            return llvm::Type::getInt1Ty(ctx);
        case FRG_AST_ID_TY_U8:
        case FRG_AST_ID_TY_I8:
            return llvm::Type::getInt8Ty(ctx);
        case FRG_AST_ID_TY_U16:
        case FRG_AST_ID_TY_I16:
            return llvm::Type::getInt16Ty(ctx);
        case FRG_AST_ID_TY_U32:
        case FRG_AST_ID_TY_I32:
            return llvm::Type::getInt32Ty(ctx);
        case FRG_AST_ID_TY_U64:
        case FRG_AST_ID_TY_I64:
            return llvm::Type::getInt64Ty(ctx);
        case FRG_AST_ID_TY_F32:
            return llvm::Type::getFloatTy(ctx);
        case FRG_AST_ID_TY_F64:
            return llvm::Type::getDoubleTy(ctx);
        case FRG_AST_ID_TY_SYMBOL:
            return (llvm::Type*)frg_ast_scope_get_ir(
                scope,
                ((frg_ast_ty_symbol_t*)ast)->name->str
            );
        case FRG_AST_ID_TY_POINTER:
            value = _frg_generate_type(
                ctx,
                scope,
                ((frg_ast_ty_pointer_t*)ast)->value
            );

            return llvm::TypedPointerType::get(
                value,
                0
            );
        default:
            frg_die_unexpected_enum_value(ast->id);
    }
}

void _frg_generate_decl_fn(
    llvm::IRBuilder<>& builder,
    llvm::LLVMContext& ctx,
    llvm::Module& module,
    frg_ast_scope_t* scope,
    frg_ast_t* ast
) {
    frg_assert_pointer_non_null(scope);
    frg_assert_pointer_non_null(ast);

    frg_ast_decl_fn_t* decl_fn = frg_ast_try_cast_decl_fn(ast);
    frg_assert_pointer_non_null(decl_fn);

    // Generate return type
    llvm::Type* return_ty = _frg_generate_type(
        ctx,
        scope,
        decl_fn->ty->return_ty
    );

    // Generate argument types
    std::vector<llvm::Type*> arg_types;

    for (GList* it = decl_fn->ty->args; it != NULL; it = it->next) {
        llvm::Type* arg_ty = _frg_generate_type(
            ctx,
            scope,
            ((frg_ast_decl_prop_t*)((frg_ast_decl_fn_arg_t*)it->data)->prop)->ty
        );

        arg_types.push_back(arg_ty);
    }

    // Generate function type
    llvm::FunctionType* fn_type = llvm::FunctionType::get(
        return_ty,
        arg_types,
        false
    );

    // Generate function
    llvm::Function* fn = llvm::Function::Create(
        fn_type,
        llvm::Function::ExternalLinkage,
        decl_fn->name->str,
        module
    );

    GList* it_ast = decl_fn->ty->args;
    auto it_ir = fn->arg_begin();
    while (it_ast != NULL && it_ir != fn->arg_end()) {
        frg_ast_decl_fn_arg_t* arg = (frg_ast_decl_fn_arg_t*)it_ast->data;

        it_ir->setName(((frg_ast_decl_prop_t*)arg->prop)->name->str);

        it_ast = it_ast->next;
        it_ir++;
    }

    // Generate function body
    llvm::BasicBlock* basic_block = llvm::BasicBlock::Create(ctx, "entry", fn);
    builder.SetInsertPoint(basic_block);

    _frg_generate_stmt(
        builder,
        ctx,
        scope,
        decl_fn->body
    );
}

void _frg_generate_stmt(
    llvm::IRBuilder<>& builder,
    llvm::LLVMContext& ctx,
    frg_ast_scope_t* scope,
    frg_ast_t* ast
) {
    frg_assert_pointer_non_null(scope);
    frg_assert_pointer_non_null(ast);

    llvm::Value* value = NULL;

    switch (ast->id) {
        case FRG_AST_ID_STMT_RETURN:
            value = _frg_generate_value(
                ctx,
                scope,
                ((frg_ast_stmt_return_t*)ast)->value
            );

            builder.CreateRet(value);

            break;
        case FRG_AST_ID_STMT_BLOCK:
            frg_ast_scope_push_frame(
                scope
            );

            for (GList* it = ((frg_ast_stmt_block_t*)ast)->stmts; it != NULL; it = it->next) {
                _frg_generate_stmt(
                    builder,
                    ctx,
                    scope,
                    (frg_ast_t*)it->data
                );
            }

            frg_ast_scope_pop_frame(
                scope
            );

            break;
        case FRG_AST_ID_VALUE_TRUE:
        case FRG_AST_ID_VALUE_FALSE:
        case FRG_AST_ID_VALUE_INT:
        case FRG_AST_ID_VALUE_FLOAT:
        case FRG_AST_ID_VALUE_CHAR:
        case FRG_AST_ID_VALUE_STR:
        case FRG_AST_ID_VALUE_SYMBOL:
        case FRG_AST_ID_VALUE_DEREF:
        case FRG_AST_ID_VALUE_GETADDR:
        case FRG_AST_ID_VALUE_CALL_KW_ARG:
        case FRG_AST_ID_VALUE_CALL:
        case FRG_AST_ID_VALUE_ACCESS:
        case FRG_AST_ID_VALUE_BIT_NOT:
        case FRG_AST_ID_VALUE_BIT_AND:
        case FRG_AST_ID_VALUE_BIT_OR:
        case FRG_AST_ID_VALUE_BIT_XOR:
        case FRG_AST_ID_VALUE_BIT_SHL:
        case FRG_AST_ID_VALUE_BIT_SHR:
        case FRG_AST_ID_VALUE_NEG:
        case FRG_AST_ID_VALUE_ADD:
        case FRG_AST_ID_VALUE_SUB:
        case FRG_AST_ID_VALUE_MUL:
        case FRG_AST_ID_VALUE_DIV:
        case FRG_AST_ID_VALUE_DIV_INT:
        case FRG_AST_ID_VALUE_MOD:
        case FRG_AST_ID_VALUE_EXP:
        case FRG_AST_ID_VALUE_EQ:
        case FRG_AST_ID_VALUE_NE:
        case FRG_AST_ID_VALUE_LT:
        case FRG_AST_ID_VALUE_LE:
        case FRG_AST_ID_VALUE_GT:
        case FRG_AST_ID_VALUE_GE:
        case FRG_AST_ID_VALUE_LOG_NOT:
        case FRG_AST_ID_VALUE_LOG_AND:
        case FRG_AST_ID_VALUE_LOG_OR:
        case FRG_AST_ID_VALUE_ASSIGN:
        case FRG_AST_ID_VALUE_BIT_AND_ASSIGN:
        case FRG_AST_ID_VALUE_BIT_OR_ASSIGN:
        case FRG_AST_ID_VALUE_BIT_XOR_ASSIGN:
        case FRG_AST_ID_VALUE_BIT_SHL_ASSIGN:
        case FRG_AST_ID_VALUE_BIT_SHR_ASSIGN:
        case FRG_AST_ID_VALUE_ADD_ASSIGN:
        case FRG_AST_ID_VALUE_INC:
        case FRG_AST_ID_VALUE_SUB_ASSIGN:
        case FRG_AST_ID_VALUE_DEC:
        case FRG_AST_ID_VALUE_MUL_ASSIGN:
        case FRG_AST_ID_VALUE_DIV_ASSIGN:
        case FRG_AST_ID_VALUE_DIV_INT_ASSIGN:
        case FRG_AST_ID_VALUE_MOD_ASSIGN:
        case FRG_AST_ID_VALUE_EXP_ASSIGN:
        case FRG_AST_ID_VALUE_LOG_AND_ASSIGN:
        case FRG_AST_ID_VALUE_LOG_OR_ASSIGN:
            value = _frg_generate_value(
                ctx,
                scope,
                ast
            );

            builder.Insert(value);

            break;
        default:
            frg_die_unexpected_enum_value(ast->id);
    }
}

llvm::Value* _frg_generate_value(
    llvm::LLVMContext& ctx,
    frg_ast_scope_t* scope,
    frg_ast_t* ast
) {
    frg_assert_pointer_non_null(scope);
    frg_assert_pointer_non_null(ast);

    frg_ast_value_int_t* value_int = NULL;

    switch (ast->id) {
        case FRG_AST_ID_VALUE_TRUE:
            return llvm::ConstantInt::getTrue(ctx);
        case FRG_AST_ID_VALUE_FALSE:
            return llvm::ConstantInt::getFalse(ctx);
        case FRG_AST_ID_VALUE_INT:
            value_int = (frg_ast_value_int_t*)ast;
            switch (value_int->ty->id) {
                case FRG_AST_ID_TY_U8:
                    return llvm::ConstantInt::get(
                        llvm::Type::getInt8Ty(ctx),
                        value_int->value.u8,
                        false
                    );
                    break;
                case FRG_AST_ID_TY_U16:
                    return llvm::ConstantInt::get(
                        llvm::Type::getInt16Ty(ctx),
                        value_int->value.u16,
                        false
                    );
                    break;
                case FRG_AST_ID_TY_U32:
                    return llvm::ConstantInt::get(
                        llvm::Type::getInt32Ty(ctx),
                        value_int->value.u32,
                        false
                    );
                    break;
                case FRG_AST_ID_TY_U64:
                    return llvm::ConstantInt::get(
                        llvm::Type::getInt64Ty(ctx),
                        value_int->value.u64,
                        false
                    );
                    break;
                case FRG_AST_ID_TY_I8:
                    return llvm::ConstantInt::get(
                        llvm::Type::getInt8Ty(ctx),
                        value_int->value.i8,
                        true
                    );
                    break;
                case FRG_AST_ID_TY_I16:
                    return llvm::ConstantInt::get(
                        llvm::Type::getInt16Ty(ctx),
                        value_int->value.i16,
                        true
                    );
                    break;
                case FRG_AST_ID_TY_I32:
                    return llvm::ConstantInt::get(
                        llvm::Type::getInt32Ty(ctx),
                        value_int->value.i32,
                        true
                    );
                    break;
                case FRG_AST_ID_TY_I64:
                    return llvm::ConstantInt::get(
                        llvm::Type::getInt64Ty(ctx),
                        value_int->value.i64,
                        true
                    );
                    break;
                default:
                    frg_die_unexpected_enum_value(value_int->ty->id);
            }
        default:
            frg_die_unexpected_enum_value(ast->id);
    }
}
}
