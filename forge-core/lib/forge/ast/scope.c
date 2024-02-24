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
#include <forge/ast/utilities.h>
#include <forge/assert.h>
#include <forge/memory.h>

frg_ast_scope_frame_t* frg_ast_scope_frame_new() {
    frg_ast_scope_frame_t* scope_frame = frg_malloc(sizeof(frg_ast_scope_frame_t));

    scope_frame->_map_ast = g_hash_table_new((GHashFunc)g_str_hash, (GEqualFunc)g_str_equal);
    scope_frame->_map_ir = g_hash_table_new((GHashFunc)g_str_hash, (GEqualFunc)g_str_equal);

    return scope_frame;
}

void frg_ast_scope_frame_destroy(frg_ast_scope_frame_t* scope_frame) {
    frg_assert_pointer_non_null(scope_frame);

    g_hash_table_destroy(scope_frame->_map_ast);
    g_hash_table_destroy(scope_frame->_map_ir);

    frg_free(scope_frame);
}

void frg_ast_scope_frame_add_ast(
    frg_ast_scope_frame_t* mut_scope_frame,
    frg_ast_node_t* node
) {
    frg_assert_pointer_non_null(mut_scope_frame);
    frg_assert_pointer_non_null(node);

    const char* name;

    if (node->kind == FRG_AST_NODE_KIND_DECLARATION_FUNCTION_ARGUMENT) {
        name = frg_ast_declaration_function_argument_name_get(node);
    } else {
        name = frg_ast_declaration_name_get(node);
    }
    
    frg_assert_string_non_empty(name);

    g_hash_table_insert(mut_scope_frame->_map_ast, (void*)name, (void*)node);
}

frg_ast_node_t* frg_ast_scope_frame_get_ast(
    frg_ast_scope_frame_t* mut_scope_frame,
    const char* name
) {
    frg_assert_pointer_non_null(mut_scope_frame);
    frg_assert_string_non_empty(name);

    return g_hash_table_lookup(
        mut_scope_frame->_map_ast,
        (void*)name
    );
}

void frg_ast_scope_frame_add_ir(
    frg_ast_scope_frame_t* mut_scope_frame,
    const char* name,
    void* ir
) {
    frg_assert_pointer_non_null(mut_scope_frame);
    frg_assert_string_non_empty(name);
    frg_assert_pointer_non_null(ir);

    g_hash_table_insert(mut_scope_frame->_map_ir, (void*)name, (void*)ir);
}

void* frg_ast_scope_frame_get_ir(
    frg_ast_scope_frame_t* mut_scope_frame,
    const char* name
) {
    frg_assert_pointer_non_null(mut_scope_frame);
    frg_assert_string_non_empty(name);

    return g_hash_table_lookup(
        mut_scope_frame->_map_ir,
        (void*)name
    );
}

void frg_ast_scope_frame_load_declaration_block(
    frg_ast_scope_frame_t* mut_scope_frame,
    const frg_ast_node_declaration_block_t* block
) {
    frg_assert_pointer_non_null(mut_scope_frame);
    frg_assert_pointer_non_null(block);

    GList* it = NULL;
    for (
        it = g_list_first(block->declarations);
        it != NULL;
        it = g_list_next(it)
    ) {
        frg_ast_scope_frame_add_ast(mut_scope_frame, (frg_ast_node_t*)it->data);
    }
}

void frg_ast_scope_frame_load_declaration_function_args(
    frg_ast_scope_frame_t* mut_scope_frame,
    const frg_ast_node_declaration_function_t* function
) {
    frg_assert_pointer_non_null(mut_scope_frame);
    frg_assert_pointer_non_null(function);

    GList* it = NULL;
    for (
        it = g_list_first(function->type->arguments);
        it != NULL;
        it = g_list_next(it)
    ) {
        frg_ast_scope_frame_add_ast(mut_scope_frame, (frg_ast_node_t*)it->data);
    }
}

frg_ast_scope_t* frg_ast_scope_new() {
    frg_ast_scope_t* scope = frg_malloc(sizeof(frg_ast_scope_t));

    scope->_frames = NULL;

    frg_ast_scope_push_frame(
        scope
    );

    return scope;
}

void frg_ast_scope_destroy(frg_ast_scope_t* scope) {
    frg_assert_pointer_non_null(scope);

    GList* it = NULL;
    for (
        it = g_list_first(scope->_frames);
        it != NULL;
        it = g_list_next(it)
    ) {
        frg_ast_scope_frame_destroy((frg_ast_scope_frame_t*)it->data);
    }

    g_list_free(scope->_frames);

    frg_free(scope);
}

void frg_ast_scope_push_frame(frg_ast_scope_t* mut_scope) {
    frg_assert_pointer_non_null(mut_scope);

    frg_ast_scope_frame_t* frame = frg_ast_scope_frame_new();

    mut_scope->_frames = g_list_prepend(mut_scope->_frames, (void*)frame);
}

void frg_ast_scope_pop_frame(frg_ast_scope_t* mut_scope) {
    frg_assert_pointer_non_null(mut_scope);
    frg_assert_pointer_non_null(mut_scope->_frames);

    frg_ast_scope_frame_destroy((frg_ast_scope_frame_t*)mut_scope->_frames->data);

    mut_scope->_frames = g_list_delete_link(mut_scope->_frames, mut_scope->_frames);

    if (mut_scope->_frames == NULL) {
        frg_die("must not pop first scope frame");
    }
}

frg_ast_scope_frame_t* frg_ast_scope_get_current_frame(
    frg_ast_scope_t* mut_scope
) {
    frg_assert_pointer_non_null(mut_scope);
    frg_assert_pointer_non_null(mut_scope->_frames);

    return (frg_ast_scope_frame_t*)mut_scope->_frames->data;
}

void frg_ast_scope_add_ast(
    frg_ast_scope_t* mut_scope,
    frg_ast_node_t* node
) {
    frg_assert_pointer_non_null(mut_scope);
    frg_assert_pointer_non_null(node);

    frg_ast_scope_frame_t* scope_frame = frg_ast_scope_get_current_frame(
        mut_scope
    );

    frg_ast_scope_frame_add_ast(
        scope_frame,
        node
    );
}

frg_ast_node_t* frg_ast_scope_get_ast(
    frg_ast_scope_t* mut_scope,
    const char* name
) {
    frg_assert_pointer_non_null(mut_scope);
    frg_assert_string_non_empty(name);

    for (
        GList* it = g_list_first(mut_scope->_frames);
        it != NULL;
        it = g_list_next(it)
    ) {
        frg_ast_node_t* node = frg_ast_scope_frame_get_ast(
            (frg_ast_scope_frame_t*)it->data,
            name
        );

        if (node != NULL) {
            return node;
        }
    }

    return NULL;
}

void frg_ast_scope_add_ir(
    frg_ast_scope_t* mut_scope,
    const char* name,
    void* ir
) {
    frg_assert_pointer_non_null(mut_scope);
    frg_assert_string_non_empty(name);
    frg_assert_pointer_non_null(ir);

    frg_ast_scope_frame_t* scope_frame = frg_ast_scope_get_current_frame(
        mut_scope
    );

    frg_ast_scope_frame_add_ir(
        scope_frame,
        name,
        ir
    );
}

void* frg_ast_scope_get_ir(
    frg_ast_scope_t* mut_scope,
    const char* name
) {
    frg_assert_pointer_non_null(mut_scope);
    frg_assert_string_non_empty(name);

    for (
        GList* it = g_list_first(mut_scope->_frames);
        it != NULL;
        it = g_list_next(it)
    ) {
        void* ir = frg_ast_scope_frame_get_ir(
            (frg_ast_scope_frame_t*)it->data,
            name
        );

        if (ir != NULL) {
            return ir;
        }
    }

    return NULL;
}

void frg_ast_scope_load_declaration_block(
    frg_ast_scope_t* mut_scope,
    const frg_ast_node_declaration_block_t* block
) {
    frg_assert_pointer_non_null(mut_scope);
    frg_assert_pointer_non_null(block);

    frg_ast_scope_frame_t* scope_frame = frg_ast_scope_get_current_frame(
        mut_scope
    );

    frg_ast_scope_frame_load_declaration_block(
        scope_frame,
        block
    );
}

void frg_ast_scope_load_declaration_function_args(
    frg_ast_scope_t* mut_scope,
    const frg_ast_node_declaration_function_t* function
) {
    frg_assert_pointer_non_null(mut_scope);
    frg_assert_pointer_non_null(function);

    frg_ast_scope_frame_t* scope_frame = frg_ast_scope_get_current_frame(
        mut_scope
    );

    frg_ast_scope_frame_load_declaration_function_args(
        scope_frame,
        function
    );
}
