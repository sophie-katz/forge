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

#include <forge/verification/type_resolvers.h>
#include <forge/verification/type_operations.h>
#include <forge/ast/kind_info.h>
#include <forge/ast/operations.h>
#include <forge/common/error.h>
#include <forge/messages/codes.h>
#include <forge/formatting/format.h>

frg_ast_t* frg_ast_type_resolver_decl_union(
    frg_message_buffer_t* message_buffer,
    const frg_ast_scope_t* scope,
    const frg_ast_t* ast
) {
    frg_assert_pointer_non_null(message_buffer);
    frg_assert_pointer_non_null(scope);
    frg_assert_pointer_non_null(ast);
    frg_assert_int_eq(ast->kind, FRG_AST_KIND_DECL_UNION);

    frg_message_emit_eft_1_unsupported_requirement(
        message_buffer,
        &ast->source_range,
        9,
        "Union declarations"
    );

    return NULL;
}

frg_ast_t* frg_ast_type_resolver_decl_struct(
    frg_message_buffer_t* message_buffer,
    const frg_ast_scope_t* scope,
    const frg_ast_t* ast
) {
    frg_assert_pointer_non_null(message_buffer);
    frg_assert_pointer_non_null(scope);
    frg_assert_pointer_non_null(ast);
    frg_assert_int_eq(ast->kind, FRG_AST_KIND_DECL_STRUCT);

    frg_message_emit_eft_1_unsupported_requirement(
        message_buffer,
        &ast->source_range,
        10,
        "Struct declarations"
    );

    return NULL;
}

frg_ast_t* frg_ast_type_resolver_decl_prop(
    frg_message_buffer_t* message_buffer,
    const frg_ast_scope_t* scope,
    const frg_ast_t* ast
) {
    frg_assert_pointer_non_null(message_buffer);
    frg_assert_pointer_non_null(scope);
    frg_assert_pointer_non_null(ast);
    frg_assert_int_eq(ast->kind, FRG_AST_KIND_DECL_PROP);

    if (((frg_ast_decl_prop_t*)ast)->ty == NULL) {
        frg_message_emit_eft_1_unsupported_requirement(
            message_buffer,
            &ast->source_range,
            11,
            "Dynamic objects"
        );
    }

    return frg_ast_clone(((frg_ast_decl_prop_t*)ast)->ty);
}

frg_ast_t* frg_ast_type_resolver_decl_iface(
    frg_message_buffer_t* message_buffer,
    const frg_ast_scope_t* scope,
    const frg_ast_t* ast
) {
    frg_assert_pointer_non_null(message_buffer);
    frg_assert_pointer_non_null(scope);
    frg_assert_pointer_non_null(ast);
    frg_assert_int_eq(ast->kind, FRG_AST_KIND_DECL_IFACE);

    frg_message_emit_eft_1_unsupported_requirement(
        message_buffer,
        &ast->source_range,
        11,
        "Interface declarations"
    );

    return NULL;
}

frg_ast_t* frg_ast_type_resolver_decl_fn_arg(
    frg_message_buffer_t* message_buffer,
    const frg_ast_scope_t* scope,
    const frg_ast_t* ast
) {
    frg_assert_pointer_non_null(message_buffer);
    frg_assert_pointer_non_null(scope);
    frg_assert_pointer_non_null(ast);
    frg_assert_int_eq(ast->kind, FRG_AST_KIND_DECL_FN_ARG);

    frg_assert_pointer_non_null(((frg_ast_decl_fn_arg_t*)ast)->prop);

    return frg_ast_resolve_type(
        message_buffer,
        scope,
        (frg_ast_t*)((frg_ast_decl_fn_arg_t*)ast)->prop
    );
}

frg_ast_t* frg_ast_type_resolver_decl_fn(
    frg_message_buffer_t* message_buffer,
    const frg_ast_scope_t* scope,
    const frg_ast_t* ast
) {
    frg_assert_pointer_non_null(message_buffer);
    frg_assert_pointer_non_null(scope);
    frg_assert_pointer_non_null(ast);
    frg_assert_int_eq(ast->kind, FRG_AST_KIND_DECL_FN);

    frg_assert_pointer_non_null(((frg_ast_decl_fn_t*)ast)->ty);

    return frg_ast_clone((frg_ast_t*)((frg_ast_decl_fn_t*)ast)->ty);
}

frg_ast_t* frg_ast_type_resolver_decl_var(
    frg_message_buffer_t* message_buffer,
    const frg_ast_scope_t* scope,
    const frg_ast_t* ast
) {
    frg_assert_pointer_non_null(message_buffer);
    frg_assert_pointer_non_null(scope);
    frg_assert_pointer_non_null(ast);
    frg_assert_int_eq(ast->kind, FRG_AST_KIND_DECL_VAR);

    frg_assert_pointer_non_null(((frg_ast_decl_var_t*)ast)->prop);

    return frg_ast_resolve_type(
        message_buffer,
        scope,
        (frg_ast_t*)((frg_ast_decl_var_t*)ast)->prop
    );
}

frg_ast_t* frg_ast_type_resolver_as_ty_bool(
    frg_message_buffer_t* message_buffer,
    const frg_ast_scope_t* scope,
    const frg_ast_t* ast
) {
    return frg_ast_new_ty_primary(
        &frg_parsing_range_null,
        FRG_AST_KIND_TY_BOOL
    );
}

frg_ast_t* frg_ast_type_resolver_value_int(
    frg_message_buffer_t* message_buffer,
    const frg_ast_scope_t* scope,
    const frg_ast_t* ast
) {
    frg_assert_pointer_non_null(message_buffer);
    frg_assert_pointer_non_null(scope);
    frg_assert_pointer_non_null(ast);
    frg_assert_int_eq(ast->kind, FRG_AST_KIND_VALUE_INT);

    frg_assert_pointer_non_null(((frg_ast_value_int_t*)ast)->ty);

    return frg_ast_clone(((frg_ast_value_int_t*)ast)->ty);
}

frg_ast_t* frg_ast_type_resolver_value_float(
    frg_message_buffer_t* message_buffer,
    const frg_ast_scope_t* scope,
    const frg_ast_t* ast
) {
    frg_assert_pointer_non_null(message_buffer);
    frg_assert_pointer_non_null(scope);
    frg_assert_pointer_non_null(ast);
    frg_assert_int_eq(ast->kind, FRG_AST_KIND_VALUE_FLOAT);

    frg_assert_pointer_non_null(((frg_ast_value_float_t*)ast)->ty);

    return frg_ast_clone(((frg_ast_value_float_t*)ast)->ty);
}

frg_ast_t* frg_ast_type_resolver_value_char(
    frg_message_buffer_t* message_buffer,
    const frg_ast_scope_t* scope,
    const frg_ast_t* ast
) {
    frg_assert_pointer_non_null(message_buffer);
    frg_assert_pointer_non_null(scope);
    frg_assert_pointer_non_null(ast);
    frg_assert_int_eq(ast->kind, FRG_AST_KIND_VALUE_CHAR);

    frg_message_emit_eft_1_unsupported_requirement(
        message_buffer,
        &ast->source_range,
        5,
        "Character literals"
    );

    return NULL;
}

frg_ast_t* frg_ast_type_resolver_value_str(
    frg_message_buffer_t* message_buffer,
    const frg_ast_scope_t* scope,
    const frg_ast_t* ast
) {
    frg_assert_pointer_non_null(message_buffer);
    frg_assert_pointer_non_null(scope);
    frg_assert_pointer_non_null(ast);
    frg_assert_int_eq(ast->kind, FRG_AST_KIND_VALUE_STR);

    frg_message_emit_eft_1_unsupported_requirement(
        message_buffer,
        &ast->source_range,
        4,
        "String literals"
    );

    return NULL;
}

frg_ast_t* frg_ast_type_resolver_value_symbol(
    frg_message_buffer_t* message_buffer,
    const frg_ast_scope_t* scope,
    const frg_ast_t* ast
) {
    frg_assert_pointer_non_null(message_buffer);
    frg_assert_pointer_non_null(scope);
    frg_assert_pointer_non_null(ast);
    frg_assert_int_eq(ast->kind, FRG_AST_KIND_VALUE_STR);

    frg_assert_gstring_non_empty(((frg_ast_value_symbol_t*)ast)->name);

    ast = frg_ast_scope_get_ast(
        scope,
        ((frg_ast_value_symbol_t*)ast)->name->str
    );

    if (ast == NULL) {
        frg_emit_message_et_1_undeclared_symbol(
            message_buffer,
            &ast->source_range,
            ((frg_ast_value_symbol_t*)ast)->name
        );

        return NULL;
    }

    return frg_ast_resolve_type(
        message_buffer,
        scope,
        ast
    );
}

frg_ast_t* frg_ast_type_resolver_value_deref(
    frg_message_buffer_t* message_buffer,
    const frg_ast_scope_t* scope,
    const frg_ast_t* ast
) {
    frg_assert_pointer_non_null(message_buffer);
    frg_assert_pointer_non_null(scope);
    frg_assert_pointer_non_null(ast);
    frg_assert_int_eq(ast->kind, FRG_AST_KIND_VALUE_DEREF);

    ast = frg_ast_resolve_type(
        message_buffer,
        scope,
        ((frg_ast_value_unary_t*)ast)->operand
    );

    if (ast == NULL) {
        return NULL;
    }

    if (ast->kind != FRG_AST_KIND_TY_POINTER) {
        frg_emit_message_et_2_cannot_deref_non_pointer(
            message_buffer,
            &ast->source_range,
            ast
        );

        return NULL;
    }

    frg_assert_pointer_non_null(((frg_ast_ty_pointer_t*)ast)->value);

    return frg_ast_clone(((frg_ast_ty_pointer_t*)ast)->value);
}

frg_ast_t* frg_ast_type_resolver_value_getaddr(
    frg_message_buffer_t* message_buffer,
    const frg_ast_scope_t* scope,
    const frg_ast_t* ast
) {
    frg_assert_pointer_non_null(message_buffer);
    frg_assert_pointer_non_null(scope);
    frg_assert_pointer_non_null(ast);
    frg_assert_int_eq(ast->kind, FRG_AST_KIND_VALUE_GETADDR);

    frg_ast_t* result = frg_ast_resolve_type(
        message_buffer,
        scope,
        ((frg_ast_value_unary_t*)ast)->operand
    );

    if (result == NULL) {
        return NULL;
    }

    return (frg_ast_t*)frg_ast_new_ty_pointer(
        &frg_parsing_range_null,
        result
    );
}

frg_ast_t* frg_ast_type_resolver_value_call(
    frg_message_buffer_t* message_buffer,
    const frg_ast_scope_t* scope,
    const frg_ast_t* ast
) {
    frg_assert_pointer_non_null(message_buffer);
    frg_assert_pointer_non_null(scope);
    frg_assert_pointer_non_null(ast);
    frg_assert_int_eq(ast->kind, FRG_AST_KIND_VALUE_GETADDR);

    frg_assert_pointer_non_null(((frg_ast_value_call_t*)ast)->callee);

    ast = frg_ast_resolve_type(
        message_buffer,
        scope,
        ((frg_ast_value_call_t*)ast)->callee
    );

    if (ast == NULL) {
        return NULL;
    }

    if (ast->kind != FRG_AST_KIND_TY_FN) {
        frg_emit_message_et_3_cannot_call_non_fn(
            message_buffer,
            &ast->source_range,
            ast
        );

        return NULL;
    }

    frg_assert_pointer_non_null(((frg_ast_ty_fn_t*)ast)->return_ty);

    return frg_ast_clone(((frg_ast_ty_fn_t*)ast)->return_ty);
}

frg_ast_t* frg_ast_type_resolver_value_access(
    frg_message_buffer_t* message_buffer,
    const frg_ast_scope_t* scope,
    const frg_ast_t* ast
) {
    frg_assert_pointer_non_null(message_buffer);
    frg_assert_pointer_non_null(scope);
    frg_assert_pointer_non_null(ast);
    frg_assert_int_eq(ast->kind, FRG_AST_KIND_VALUE_ACCESS);

    frg_message_emit_eft_1_unsupported_requirement(
        message_buffer,
        &ast->source_range,
        10,
        "Struct declarations"
    );

    return NULL;
}

frg_ast_t* frg_ast_type_resolver_value_unary_ident(
    frg_message_buffer_t* message_buffer,
    const frg_ast_scope_t* scope,
    const frg_ast_t* ast
) {
    frg_assert_pointer_non_null(message_buffer);
    frg_assert_pointer_non_null(scope);
    frg_assert_pointer_non_null(ast);
    frg_assert(frg_ast_kind_info_get(ast->kind)->flags & FRG_AST_KIND_FLAG_VALUE_UNARY);

    frg_assert_pointer_non_null(((frg_ast_value_unary_t*)ast)->operand);

    return frg_ast_resolve_type(
        message_buffer,
        scope,
        ((frg_ast_value_unary_t*)ast)->operand
    );
}

frg_ast_t* frg_ast_type_resolver_value_binary_containing(
    frg_message_buffer_t* message_buffer,
    const frg_ast_scope_t* scope,
    const frg_ast_t* ast
) {
    frg_assert_pointer_non_null(message_buffer);
    frg_assert_pointer_non_null(scope);
    frg_assert_pointer_non_null(ast);
    frg_assert(frg_ast_kind_info_get(ast->kind)->flags & FRG_AST_KIND_FLAG_VALUE_BINARY);

    frg_assert_pointer_non_null(((frg_ast_value_binary_t*)ast)->left);
    frg_assert_pointer_non_null(((frg_ast_value_binary_t*)ast)->right);

    frg_ast_t* type_left = frg_ast_resolve_type(
        message_buffer,
        scope,
        ((frg_ast_value_binary_t*)ast)->left
    );

    if (type_left == NULL) {
        return NULL;
    }

    frg_ast_t* type_right = frg_ast_resolve_type(
        message_buffer,
        scope,
        ((frg_ast_value_binary_t*)ast)->right
    );

    if (type_right == NULL) {
        return NULL;
    }

    frg_ast_t* type_containing = frg_ast_get_numeric_containing_type(
        type_left,
        type_right
    );

    frg_ast_destroy(&type_left);
    frg_ast_destroy(&type_right);

    if (type_containing == NULL) {
        frg_emit_message_it_1_no_containing_type(
            message_buffer,
            &ast->source_range,
            type_left,
            type_right
        );
    }

    return type_containing;
}

frg_ast_t* frg_ast_type_resolver_value_binary_left(
    frg_message_buffer_t* message_buffer,
    const frg_ast_scope_t* scope,
    const frg_ast_t* ast
) {
    frg_assert_pointer_non_null(message_buffer);
    frg_assert_pointer_non_null(scope);
    frg_assert_pointer_non_null(ast);
    frg_assert(frg_ast_kind_info_get(ast->kind)->flags & FRG_AST_KIND_FLAG_VALUE_BINARY);

    frg_assert_pointer_non_null(((frg_ast_value_binary_t*)ast)->left);

    return frg_ast_resolve_type(
        message_buffer,
        scope,
        ((frg_ast_value_binary_t*)ast)->left
    );
}
