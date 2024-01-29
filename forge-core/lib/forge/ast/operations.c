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

#include <forge/ast/operations.h>
#include <forge/ast/kind_info.h>
#include <forge/common/error.h>
#include <forge/common/debug.h>

const char* frg_ast_get_decl_name(const frg_ast_t* ast) {
    frg_assert_pointer_non_null(ast);
    frg_assert(frg_ast_kind_info_get(ast->kind)->flags & FRG_AST_KIND_FLAG_DECL);

    frg_ast_decl_name_getter_t decl_name_getter = frg_ast_kind_info_get(ast->kind)->decl_name_getter;

    frg_assert_pointer_non_null(decl_name_getter);

    return decl_name_getter(ast);
}

const char* frg_ast_get_decl_fn_arg_name(const frg_ast_t* ast) {
    frg_assert_pointer_non_null(ast);
    frg_assert_int_eq(ast->kind, FRG_AST_KIND_DECL_FN_ARG);

    frg_ast_decl_name_getter_t decl_name_getter = frg_ast_kind_info_get(ast->kind)->decl_name_getter;

    frg_assert_pointer_non_null(decl_name_getter);

    return decl_name_getter(ast);
}

void frg_ast_print_debug(
    frg_stream_output_t* stream,
    const frg_ast_t* ast,
    frg_indent_t indent
) {
    if (ast == NULL) {
        frg_debug_print_node(stream, "null");
        return;
    }

    frg_debug_print_node(stream, frg_ast_kind_info_get(ast->kind)->name);

    frg_ast_debug_printer_t debug_printer = frg_ast_kind_info_get(ast->kind)->debug_printer;

    if (debug_printer != NULL) {
        debug_printer(stream, ast, indent);
    }
}

frg_ast_visitor_status_t _frg_ast_accept_recursive(
    frg_ast_visitor_t* visitor,
    GList* parents,
    frg_ast_t** ast
) {
    frg_assert_pointer_non_null(visitor);
    frg_assert_pointer_non_null(ast);
    
    if (*ast == NULL) {
        return FRG_AST_VISITOR_STATUS_OK;
    }

    frg_ast_visitor_status_t status = frg_ast_visitor_call_entry(
        visitor,
        parents,
        ast,
        true
    );
    if (status == FRG_AST_VISITOR_STATUS_SKIP) {
        return FRG_AST_VISITOR_STATUS_OK;
    } else if (status != FRG_AST_VISITOR_STATUS_OK) {
        return status;
    }

    if (*ast == NULL) {
        return FRG_AST_VISITOR_STATUS_OK;
    }

    bool is_top = (parents == NULL);

    parents = g_list_prepend(parents, *ast);
    status = FRG_AST_VISITOR_STATUS_OK;

    frg_ast_visitor_acceptor_t visitor_acceptor = frg_ast_kind_info_get((*ast)->kind)->visitor_acceptor;

    if (visitor_acceptor != NULL) {
        status = visitor_acceptor(
            visitor,
            parents,
            ast
        );
    }

    parents = g_list_delete_link(parents, parents);

    if (status == FRG_AST_VISITOR_STATUS_SKIP) {
        return FRG_AST_VISITOR_STATUS_OK;
    } else if (status != FRG_AST_VISITOR_STATUS_OK) {
        return status;
    }

    if (is_top) {
        frg_assert_pointer_null(parents);
    }

    status = frg_ast_visitor_call_entry(
        visitor,
        parents,
        ast,
        false
    );
    if (status == FRG_AST_VISITOR_STATUS_STOP) {
        return status;
    }

    return FRG_AST_VISITOR_STATUS_OK;
}

frg_ast_visitor_status_t frg_ast_accept(
    frg_ast_visitor_t* visitor,
    frg_ast_t** ast
) {
    return _frg_ast_accept_recursive(
        visitor,
        NULL,
        ast
    );
}
