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
#include <forge/assert.h>
#include <forge/ast/node_kind_info.h>
#include <forge/ast/utilities.h>

llvm::Type* _frg_codegen_generate_type(llvm::LLVMContext& ctx,
                                       frg_ast_scope_t* mut_scope,
                                       const frg_ast_node_t* node) {
  frg_assert_pointer_non_null(mut_scope);
  frg_assert_pointer_non_null(node);

  llvm::Type* value = NULL;

  switch (node->kind) {
  case FRG_AST_NODE_KIND_TYPE_BOOL:
    return llvm::Type::getInt1Ty(ctx);
  case FRG_AST_NODE_KIND_TYPE_INT:
    return llvm::Type::getIntNTy(ctx, ((frg_ast_node_type_int_t*)node)->bit_width);
  case FRG_AST_NODE_KIND_TYPE_FLOAT:
    if (((frg_ast_node_type_float_t*)node)->bit_width == 32) {
      return llvm::Type::getFloatTy(ctx);
    } else if (((frg_ast_node_type_float_t*)node)->bit_width == 64) {
      return llvm::Type::getDoubleTy(ctx);
    } else {
      frg_die("Unsupported floating point bit width %u",
              ((frg_ast_node_type_float_t*)node)->bit_width);
    }
  case FRG_AST_NODE_KIND_TYPE_SYMBOL:
    return (llvm::Type*)frg_ast_scope_get_ir(
      mut_scope, ((const frg_ast_node_type_symbol_t*)node)->name->str);
  case FRG_AST_NODE_KIND_TYPE_POINTER:
    value = _frg_codegen_generate_type(
      ctx, mut_scope, ((const frg_ast_node_type_pointer_t*)node)->value);

    return llvm::TypedPointerType::get(value, 0);
  default:
    frg_die_ast_kind_not_yet_supported(node->kind);
  }
}

void _frg_codegen_generate_declaration_function(llvm::IRBuilder<>& builder,
                                                llvm::LLVMContext& ctx,
                                                llvm::Module& module,
                                                frg_ast_scope_t* mut_scope,
                                                const frg_ast_node_t* node) {
  frg_assert_pointer_non_null(mut_scope);
  frg_assert_pointer_non_null(node);
  frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_DECLARATION_FUNCTION);

  // Generate return type
  llvm::Type* return_ty = _frg_codegen_generate_type(
    ctx,
    mut_scope,
    ((const frg_ast_node_declaration_function_t*)node)->type->return_type);

  // Generate argument types
  std::vector<llvm::Type*> arg_types;

  for (const GList* it
       = ((const frg_ast_node_declaration_function_t*)node)->type->arguments;
       it != NULL;
       it = it->next) {
    frg_assert_int_equal_to(((const frg_ast_node_t*)it->data)->kind,
                            FRG_AST_NODE_KIND_DECLARATION_FUNCTION_ARGUMENT);
    llvm::Type* arg_ty = _frg_codegen_generate_type(
      ctx,
      mut_scope,
      ((frg_ast_node_declaration_property_t*)((frg_ast_node_declaration_function_argument_t*)
                                                it->data)
         ->property)
        ->type);

    arg_types.push_back(arg_ty);
  }

  // Generate function type
  llvm::FunctionType* fn_type = llvm::FunctionType::get(return_ty, arg_types, false);

  // Generate function
  llvm::Function* fn          = llvm::Function::Create(
    fn_type,
    llvm::Function::ExternalLinkage,
    ((const frg_ast_node_declaration_function_t*)node)->name->str,
    module);

  GList* it_ast = ((const frg_ast_node_declaration_function_t*)node)->type->arguments;
  auto it_ir    = fn->arg_begin();
  while (it_ast != NULL && it_ir != fn->arg_end()) {
    frg_assert_int_equal_to(((const frg_ast_node_t*)it_ast->data)->kind,
                            FRG_AST_NODE_KIND_DECLARATION_FUNCTION_ARGUMENT);

    frg_ast_node_declaration_function_argument_t* arg
      = (frg_ast_node_declaration_function_argument_t*)it_ast->data;

    it_ir->setName(((frg_ast_node_declaration_property_t*)arg->property)->name->str);

    it_ast = it_ast->next;
    it_ir++;
  }

  // Generate function body
  llvm::BasicBlock* basic_block = llvm::BasicBlock::Create(ctx, "entry", fn);
  builder.SetInsertPoint(basic_block);

  _frg_codegen_generate_statement(
    builder, ctx, mut_scope, ((const frg_ast_node_declaration_function_t*)node)->body);
}

void _frg_codegen_generate_statement(llvm::IRBuilder<>& builder,
                                     llvm::LLVMContext& ctx,
                                     frg_ast_scope_t* mut_scope,
                                     const frg_ast_node_t* node) {
  frg_assert_pointer_non_null(mut_scope);
  frg_assert_pointer_non_null(node);

  llvm::Value* value = NULL;

  if (node->kind == FRG_AST_NODE_KIND_STATEMENT_BLOCK) {
    frg_ast_scope_push_frame(mut_scope);

    for (GList* it = ((const frg_ast_node_statement_block_t*)node)->statements;
         it != NULL;
         it = it->next) {
      _frg_codegen_generate_statement(
        builder, ctx, mut_scope, (frg_ast_node_t*)it->data);
    }

    frg_ast_scope_pop_frame(mut_scope);
  } else if ((frg_ast_node_kind_info_get(node->kind)->flags
              & FRG_AST_NODE_KIND_FLAG_STATEMENT)
             != 0) {
    switch (node->kind) {
    case FRG_AST_NODE_KIND_STATEMENT_RETURN:
      value = _frg_codegen_generate_value(
        builder, ctx, mut_scope, ((const frg_ast_node_statement_return_t*)node)->value);

      builder.CreateRet(value);

      break;
    default:
      frg_die_ast_kind_not_yet_supported(node->kind);
    }
  } else if ((frg_ast_node_kind_info_get(node->kind)->flags
              & FRG_AST_NODE_KIND_FLAG_VALUE)
             != 0) {
    value = _frg_codegen_generate_value(builder, ctx, mut_scope, node);

    builder.Insert(value);
  } else {
    frg_die_ast_kind_not_yet_supported(node->kind);
  }
}

llvm::Value* _frg_codegen_generate_value(llvm::IRBuilder<>& builder,
                                         llvm::LLVMContext& ctx,
                                         frg_ast_scope_t* mut_scope,
                                         const frg_ast_node_t* node) {
  frg_assert_pointer_non_null(mut_scope);
  frg_assert_pointer_non_null(node);

  switch (node->kind) {
  case FRG_AST_NODE_KIND_VALUE_BOOL:
    if (((const frg_ast_node_value_bool_t*)node)->value) {
      return llvm::ConstantInt::getTrue(ctx);
    } else {
      return llvm::ConstantInt::getFalse(ctx);
    }
  case FRG_AST_NODE_KIND_VALUE_INT:
    switch (frg_get_case_for_type_int(&((const frg_ast_node_value_int_t*)node)->type)) {
    case frg_get_case_for_int_attributes(true, 8):
      return llvm::ConstantInt::get(
        llvm::Type::getInt8Ty(ctx),
        ((const frg_ast_node_value_int_t*)node)->value.as_i8,
        false);
    case frg_get_case_for_int_attributes(true, 16):
      return llvm::ConstantInt::get(
        llvm::Type::getInt16Ty(ctx),
        ((const frg_ast_node_value_int_t*)node)->value.as_i16,
        false);
    case frg_get_case_for_int_attributes(true, 32):
      return llvm::ConstantInt::get(
        llvm::Type::getInt32Ty(ctx),
        ((const frg_ast_node_value_int_t*)node)->value.as_i32,
        false);
    case frg_get_case_for_int_attributes(true, 64):
      return llvm::ConstantInt::get(
        llvm::Type::getInt64Ty(ctx),
        ((const frg_ast_node_value_int_t*)node)->value.as_i64,
        false);
    case frg_get_case_for_int_attributes(false, 8):
      return llvm::ConstantInt::get(
        llvm::Type::getInt8Ty(ctx),
        ((const frg_ast_node_value_int_t*)node)->value.as_u8,
        false);
    case frg_get_case_for_int_attributes(false, 16):
      return llvm::ConstantInt::get(
        llvm::Type::getInt16Ty(ctx),
        ((const frg_ast_node_value_int_t*)node)->value.as_u16,
        false);
    case frg_get_case_for_int_attributes(false, 32):
      return llvm::ConstantInt::get(
        llvm::Type::getInt32Ty(ctx),
        ((const frg_ast_node_value_int_t*)node)->value.as_u32,
        false);
    case frg_get_case_for_int_attributes(false, 64):
      return llvm::ConstantInt::get(
        llvm::Type::getInt64Ty(ctx),
        ((const frg_ast_node_value_int_t*)node)->value.as_u64,
        false);
    default:
      frg_die("Unexpected bit width");
    }
  case FRG_AST_NODE_KIND_VALUE_BIT_NOT:
    return builder.CreateNot(_frg_codegen_generate_value(
      builder, ctx, mut_scope, ((const frg_ast_node_value_unary_t*)node)->operand));
  case FRG_AST_NODE_KIND_VALUE_BIT_AND:
    return builder.CreateAnd(
      _frg_codegen_generate_value(
        builder, ctx, mut_scope, ((const frg_ast_node_value_binary_t*)node)->left),
      _frg_codegen_generate_value(
        builder, ctx, mut_scope, ((const frg_ast_node_value_binary_t*)node)->right));
  case FRG_AST_NODE_KIND_VALUE_BIT_OR:
    return builder.CreateOr(
      _frg_codegen_generate_value(
        builder, ctx, mut_scope, ((const frg_ast_node_value_binary_t*)node)->left),
      _frg_codegen_generate_value(
        builder, ctx, mut_scope, ((const frg_ast_node_value_binary_t*)node)->right));
  case FRG_AST_NODE_KIND_VALUE_BIT_XOR:
    return builder.CreateXor(
      _frg_codegen_generate_value(
        builder, ctx, mut_scope, ((const frg_ast_node_value_binary_t*)node)->left),
      _frg_codegen_generate_value(
        builder, ctx, mut_scope, ((const frg_ast_node_value_binary_t*)node)->right));
  case FRG_AST_NODE_KIND_VALUE_BIT_SHIFT_LEFT:
    return builder.CreateShl(
      _frg_codegen_generate_value(
        builder, ctx, mut_scope, ((const frg_ast_node_value_binary_t*)node)->left),
      _frg_codegen_generate_value(
        builder, ctx, mut_scope, ((const frg_ast_node_value_binary_t*)node)->right));
  case FRG_AST_NODE_KIND_VALUE_BIT_SHIFT_RIGHT:
    return builder.CreateLShr(
      _frg_codegen_generate_value(
        builder, ctx, mut_scope, ((const frg_ast_node_value_binary_t*)node)->left),
      _frg_codegen_generate_value(
        builder, ctx, mut_scope, ((const frg_ast_node_value_binary_t*)node)->right));
  default:
    frg_die_ast_kind_not_yet_supported(node->kind);
  }
}
}
