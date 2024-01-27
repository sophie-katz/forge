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

#include <forge/ast/scope.h>
#include <forge/common/error.h>
#include <forge/common/memory.h>

frg_ast_scope_frame_t* frg_ast_scope_frame_new() {
    frg_ast_scope_frame_t* scope_frame = frg_safe_malloc(sizeof(frg_ast_scope_frame_t));

    scope_frame->ast = g_hash_table_new(g_str_hash, g_str_equal);
    scope_frame->ir = g_hash_table_new(g_str_hash, g_str_equal);

    return scope_frame;
}

void frg_ast_scope_frame_destroy(frg_ast_scope_frame_t** scope_frame) {
    frg_assert_pointer_non_null(scope_frame);
    frg_assert_pointer_non_null(*scope_frame);

    g_hash_table_destroy((*scope_frame)->ast);
    g_hash_table_destroy((*scope_frame)->ir);

    frg_safe_free((void**)scope_frame);
}

void frg_ast_scope_frame_add_ast(frg_ast_scope_frame_t* scope_frame, frg_ast_t* ast) {
    frg_assert_pointer_non_null(scope_frame);
    frg_assert_pointer_non_null(ast);

    const char* name;

    if (ast->kind == FRG_AST_KIND_DECL_FN_ARG) {
        name = frg_ast_get_decl_fn_arg_name(ast);
    } else {
        name = frg_ast_get_decl_name(ast);
    }
    
    frg_assert_string_non_empty(name);

    g_hash_table_insert(scope_frame->ast, (void*)name, (void*)ast);
}

frg_ast_t* frg_ast_scope_frame_get_ast(
    frg_ast_scope_frame_t* scope_frame,
    const char* name
) {
    frg_assert_pointer_non_null(scope_frame);
    frg_assert_string_non_empty(name);

    return g_hash_table_lookup(
        scope_frame->ast,
        (void*)name
    );
}

void frg_ast_scope_frame_add_ir(frg_ast_scope_frame_t* scope_frame, const char* name, void* ir) {
    frg_assert_pointer_non_null(scope_frame);
    frg_assert_string_non_empty(name);
    frg_assert_pointer_non_null(ir);

    g_hash_table_insert(scope_frame->ir, (void*)name, (void*)ir);
}

void* frg_ast_scope_frame_get_ir(
    frg_ast_scope_frame_t* scope_frame,
    const char* name
) {
    frg_assert_pointer_non_null(scope_frame);
    frg_assert_string_non_empty(name);

    return g_hash_table_lookup(
        scope_frame->ir,
        (void*)name
    );
}

void frg_ast_scope_frame_load_decl_block(
    frg_ast_scope_frame_t* scope_frame,
    frg_ast_decl_block_t* block
) {
    frg_assert_pointer_non_null(scope_frame);
    frg_assert_pointer_non_null(block);

    GList* it = NULL;
    for (
        it = g_list_first(block->decls);
        it != NULL;
        it = g_list_next(it)
    ) {
        frg_ast_scope_frame_add_ast(scope_frame, (frg_ast_t*)it->data);
    }
}

void frg_ast_scope_frame_load_decl_fn_args(
    frg_ast_scope_frame_t* scope_frame,
    frg_ast_decl_fn_t* fn
) {
    frg_assert_pointer_non_null(scope_frame);
    frg_assert_pointer_non_null(fn);

    GList* it = NULL;
    for (
        it = g_list_first(fn->ty->args);
        it != NULL;
        it = g_list_next(it)
    ) {
        frg_ast_scope_frame_add_ast(scope_frame, (frg_ast_t*)it->data);
    }
}

frg_ast_scope_t* frg_ast_scope_new() {
    frg_ast_scope_t* scope = frg_safe_malloc(sizeof(frg_ast_scope_t));

    scope->frames = NULL;

    frg_ast_scope_push_frame(
        scope
    );

    return scope;
}

void frg_ast_scope_destroy(frg_ast_scope_t** scope) {
    frg_assert_pointer_non_null(scope);
    frg_assert_pointer_non_null(*scope);

    GList* it = NULL;
    for (
        it = g_list_first((*scope)->frames);
        it != NULL;
        it = g_list_next(it)
    ) {
        frg_ast_scope_frame_t* frame = (frg_ast_scope_frame_t*)it->data;
        frg_ast_scope_frame_destroy(&frame);
    }

    frg_safe_free((void**)scope);
}

void frg_ast_scope_push_frame(frg_ast_scope_t* scope) {
    frg_assert_pointer_non_null(scope);

    frg_ast_scope_frame_t* frame = frg_ast_scope_frame_new();

    scope->frames = g_list_prepend(scope->frames, (void*)frame);
}

void frg_ast_scope_pop_frame(frg_ast_scope_t* scope) {
    frg_assert_pointer_non_null(scope);
    frg_assert_pointer_non_null(scope->frames);

    frg_ast_scope_frame_t* frame = (frg_ast_scope_frame_t*)scope->frames->data;
    frg_ast_scope_frame_destroy(&frame);

    scope->frames = g_list_delete_link(scope->frames, scope->frames);

    if (scope->frames == NULL) {
        frg_die("must not pop first scope frame");
    }
}

frg_ast_scope_frame_t* frg_ast_scope_get_current_frame(
    frg_ast_scope_t* scope
) {
    frg_assert_pointer_non_null(scope);
    frg_assert_pointer_non_null(scope->frames);

    return (frg_ast_scope_frame_t*)scope->frames->data;
}

void frg_ast_scope_add_ast(
    frg_ast_scope_t* scope,
    frg_ast_t* ast
) {
    frg_assert_pointer_non_null(scope);
    frg_assert_pointer_non_null(ast);

    frg_ast_scope_frame_t* scope_frame = frg_ast_scope_get_current_frame(
        scope
    );

    frg_ast_scope_frame_add_ast(
        scope_frame,
        ast
    );
}

frg_ast_t* frg_ast_scope_get_ast(
    frg_ast_scope_t* scope,
    const char* name
) {
    frg_assert_pointer_non_null(scope);
    frg_assert_string_non_empty(name);

    for (
        GList* it = g_list_first(scope->frames);
        it != NULL;
        it = g_list_next(it)
    ) {
        frg_ast_scope_frame_t* scope_frame = (frg_ast_scope_frame_t*)it->data;

        frg_ast_t* ast = frg_ast_scope_frame_get_ast(
            scope_frame,
            name
        );

        if (ast != NULL) {
            return ast;
        }
    }

    return NULL;
}

void frg_ast_scope_add_ir(
    frg_ast_scope_t* scope,
    const char* name,
    void* ir
) {
    frg_assert_pointer_non_null(scope);
    frg_assert_string_non_empty(name);
    frg_assert_pointer_non_null(ir);

    frg_ast_scope_frame_t* scope_frame = frg_ast_scope_get_current_frame(
        scope
    );

    frg_ast_scope_frame_add_ir(
        scope_frame,
        name,
        ir
    );
}

void* frg_ast_scope_get_ir(
    frg_ast_scope_t* scope,
    const char* name
) {
    frg_assert_pointer_non_null(scope);
    frg_assert_string_non_empty(name);

    for (
        GList* it = g_list_first(scope->frames);
        it != NULL;
        it = g_list_next(it)
    ) {
        frg_ast_scope_frame_t* scope_frame = (frg_ast_scope_frame_t*)it->data;

        void* ir = frg_ast_scope_frame_get_ir(
            scope_frame,
            name
        );

        if (ir != NULL) {
            return ir;
        }
    }

    return NULL;
}

void frg_ast_scope_load_decl_block(
    frg_ast_scope_t* scope,
    frg_ast_decl_block_t* block
) {
    frg_assert_pointer_non_null(scope);
    frg_assert_pointer_non_null(block);

    frg_ast_scope_frame_t* scope_frame = frg_ast_scope_get_current_frame(
        scope
    );

    frg_ast_scope_frame_load_decl_block(
        scope_frame,
        block
    );
}

void frg_ast_scope_load_decl_fn_args(
    frg_ast_scope_t* scope,
    frg_ast_decl_fn_t* fn
) {
    frg_assert_pointer_non_null(scope);
    frg_assert_pointer_non_null(fn);

    frg_ast_scope_frame_t* scope_frame = frg_ast_scope_get_current_frame(
        scope
    );

    frg_ast_scope_frame_load_decl_fn_args(
        scope_frame,
        fn
    );
}
