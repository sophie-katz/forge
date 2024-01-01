// Copyright (c) 2023 Sophie Katz
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
#include <forge/common/check.h>
#include <forge/common/memory.h>

frg_status_t frg_ast_scope_frame_new(frg_ast_scope_frame_t** scope_frame) {
    if (scope_frame == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    } else if (*scope_frame != NULL) {
        return FRG_STATUS_ERROR_UNEXPECTED_ARGUMENT_VALUE;
    }

    frg_check(
        frg_safe_malloc((void**)scope_frame, sizeof(frg_ast_scope_frame_t))
    );

    (*scope_frame)->ast = g_hash_table_new(g_str_hash, g_str_equal);
    (*scope_frame)->ir = g_hash_table_new(g_str_hash, g_str_equal);

    return FRG_STATUS_OK;
}

frg_status_t frg_ast_scope_frame_destroy(frg_ast_scope_frame_t** scope_frame) {
    if (scope_frame == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    } else if (*scope_frame == NULL) {
        return FRG_STATUS_ERROR_UNEXPECTED_ARGUMENT_VALUE;
    }

    g_hash_table_destroy((*scope_frame)->ast);
    g_hash_table_destroy((*scope_frame)->ir);

    frg_check(
        frg_safe_free((void**)scope_frame)
    );

    return FRG_STATUS_OK;
}

frg_status_t frg_ast_scope_frame_add_ast(frg_ast_scope_frame_t* scope_frame, frg_ast_t* ast) {
    if (scope_frame == NULL || ast == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    }

    const char* name = NULL;
    frg_check(
        frg_ast_get_name(&name, ast)
    );

    g_hash_table_insert(scope_frame->ast, (void*)name, (void*)ast);

    return FRG_STATUS_OK;
}

frg_status_t frg_ast_scope_frame_get_ast(
    frg_ast_t** ast,
    frg_ast_scope_frame_t* scope_frame,
    const char* name
) {
    if (scope_frame == NULL || ast == NULL || name == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    } else if (*ast != NULL) {
        return FRG_STATUS_ERROR_UNEXPECTED_ARGUMENT_VALUE;
    } else if (*name == 0) {
        return FRG_STATUS_ERROR_EMPTY_STRING;
    }

    *ast = g_hash_table_lookup(
        scope_frame->ast,
        (void*)name
    );

    if (*ast == NULL) {
        return FRG_STATUS_ERROR_KEY_NOT_FOUND;
    }

    return FRG_STATUS_OK;
}

frg_status_t frg_ast_scope_frame_add_ir(frg_ast_scope_frame_t* scope_frame, const char* name, void* ir) {
    if (scope_frame == NULL || name == NULL || ir == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    } else if (*name == 0) {
        return FRG_STATUS_ERROR_EMPTY_STRING;
    }

    g_hash_table_insert(scope_frame->ir, (void*)name, (void*)ir);

    return FRG_STATUS_OK;
}

frg_status_t frg_ast_scope_frame_get_ir(
    void** ir,
    frg_ast_scope_frame_t* scope_frame,
    const char* name
) {
    if (scope_frame == NULL || ir == NULL || name == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    } else if (*ir != NULL) {
        return FRG_STATUS_ERROR_UNEXPECTED_ARGUMENT_VALUE;
    } else if (*name == 0) {
        return FRG_STATUS_ERROR_EMPTY_STRING;
    }

    *ir = g_hash_table_lookup(
        scope_frame->ir,
        (void*)name
    );

    if (*ir == NULL) {
        return FRG_STATUS_ERROR_KEY_NOT_FOUND;
    }

    return FRG_STATUS_OK;
}

frg_status_t frg_ast_scope_frame_load_decl_block(
    frg_ast_scope_frame_t* scope_frame,
    frg_ast_decl_block_t* block
) {
    if (scope_frame == NULL || block == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    }

    GList* it = NULL;
    for (
        it = g_list_first(block->decls);
        it != NULL;
        it = g_list_next(it)
    ) {
        frg_ast_t* ast = (frg_ast_t*)it->data;
        frg_check(
            frg_ast_scope_frame_add_ast(scope_frame, ast)
        );
    }

    return FRG_STATUS_OK;
}

frg_status_t frg_ast_scope_frame_load_fn_args(
    frg_ast_scope_frame_t* scope_frame,
    frg_ast_decl_fn_t* fn
) {
    if (scope_frame == NULL || fn == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    }

    GList* it = NULL;
    for (
        it = g_list_first(fn->args);
        it != NULL;
        it = g_list_next(it)
    ) {
        frg_ast_t* ast = (frg_ast_t*)it->data;
        frg_check(
            frg_ast_scope_frame_add_ast(scope_frame, ast)
        );
    }

    return FRG_STATUS_OK;
}

frg_status_t frg_ast_scope_new(frg_ast_scope_t** scope) {
    if (scope == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    } else if (*scope != NULL) {
        return FRG_STATUS_ERROR_UNEXPECTED_ARGUMENT_VALUE;
    }

    frg_check(
        frg_safe_malloc((void**)scope, sizeof(frg_ast_scope_t))
    );

    (*scope)->frames = NULL;

    frg_check(
        frg_ast_scope_push_frame(
            *scope
        )
    );

    return FRG_STATUS_OK;
}

frg_status_t frg_ast_scope_destroy(frg_ast_scope_t** scope) {
    if (scope == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    } else if (*scope == NULL) {
        return FRG_STATUS_ERROR_UNEXPECTED_ARGUMENT_VALUE;
    }

    GList* it = NULL;
    for (
        it = g_list_first((*scope)->frames);
        it != NULL;
        it = g_list_next(it)
    ) {
        frg_ast_scope_frame_t* frame = (frg_ast_scope_frame_t*)it->data;
        frg_check(
            frg_ast_scope_frame_destroy(&frame)
        );
    }

    frg_check(
        frg_safe_free((void**)scope)
    );

    return FRG_STATUS_OK;
}

frg_status_t frg_ast_scope_push_frame(frg_ast_scope_t* scope) {
    if (scope == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    }

    frg_ast_scope_frame_t* frame = NULL;
    frg_check(
        frg_ast_scope_frame_new(&frame)
    );

    scope->frames = g_list_prepend(scope->frames, (void*)frame);

    return FRG_STATUS_OK;
}

frg_status_t frg_ast_scope_pop_frame(frg_ast_scope_t* scope) {
    if (scope == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    }

    if (scope->frames == NULL) {
        return FRG_STATUS_ERROR_UNEXPECTED_ARGUMENT_VALUE;
    }

    frg_ast_scope_frame_t* frame = (frg_ast_scope_frame_t*)scope->frames->data;
    frg_check(
        frg_ast_scope_frame_destroy(&frame)
    );

    scope->frames = g_list_delete_link(scope->frames, scope->frames);

    return FRG_STATUS_OK;
}

frg_status_t frg_ast_scope_get_current_frame(
    frg_ast_scope_frame_t** frame,
    frg_ast_scope_t* scope
) {
    if (frame == NULL || scope == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    } else if (*frame != NULL) {
        return FRG_STATUS_ERROR_UNEXPECTED_ARGUMENT_VALUE;
    }

    if (scope->frames == NULL) {
        return FRG_STATUS_ERROR_UNEXPECTED_ARGUMENT_VALUE;
    }

    *frame = (frg_ast_scope_frame_t*)scope->frames->data;

    return FRG_STATUS_OK;
}








frg_status_t frg_ast_scope_add_ast(
    frg_ast_scope_t* scope,
    frg_ast_t* ast
) {
    frg_ast_scope_frame_t* scope_frame = NULL;
    frg_check(
        frg_ast_scope_get_current_frame(
            &scope_frame,
            scope
        )
    );

    frg_check(
        frg_ast_scope_frame_add_ast(
            scope_frame,
            ast
        )
    );

    return FRG_STATUS_OK;
}

frg_status_t frg_ast_scope_get_ast(
    frg_ast_t** ast,
    frg_ast_scope_t* scope,
    const char* name
) {
    frg_ast_scope_frame_t* scope_frame = NULL;
    frg_check(
        frg_ast_scope_get_current_frame(
            &scope_frame,
            scope
        )
    );

    frg_check(
        frg_ast_scope_frame_get_ast(
            ast,
            scope_frame,
            name
        )
    );

    return FRG_STATUS_OK;
}

frg_status_t frg_ast_scope_add_ir(
    frg_ast_scope_t* scope,
    const char* name,
    void* ir
) {
    frg_ast_scope_frame_t* scope_frame = NULL;
    frg_check(
        frg_ast_scope_get_current_frame(
            &scope_frame,
            scope
        )
    );

    frg_check(
        frg_ast_scope_frame_add_ir(
            scope_frame,
            name,
            ir
        )
    );

    return FRG_STATUS_OK;
}

frg_status_t frg_ast_scope_get_ir(
    void** ir,
    frg_ast_scope_t* scope,
    const char* name
) {
    frg_ast_scope_frame_t* scope_frame = NULL;
    frg_check(
        frg_ast_scope_get_current_frame(
            &scope_frame,
            scope
        )
    );

    frg_check(
        frg_ast_scope_frame_get_ir(
            ir,
            scope_frame,
            name
        )
    );

    return FRG_STATUS_OK;
}

frg_status_t frg_ast_scope_load_decl_block(
    frg_ast_scope_t* scope,
    frg_ast_decl_block_t* block
) {
    frg_ast_scope_frame_t* scope_frame = NULL;
    frg_check(
        frg_ast_scope_get_current_frame(
            &scope_frame,
            scope
        )
    );

    frg_check(
        frg_ast_scope_frame_load_decl_block(
            scope_frame,
            block
        )
    );

    return FRG_STATUS_OK;
}

frg_status_t frg_ast_scope_load_fn_args(
    frg_ast_scope_t* scope,
    frg_ast_decl_fn_t* fn
) {
    frg_ast_scope_frame_t* scope_frame = NULL;
    frg_check(
        frg_ast_scope_get_current_frame(
            &scope_frame,
            scope
        )
    );

    frg_check(
        frg_ast_scope_frame_load_fn_args(
            scope_frame,
            fn
        )
    );

    return FRG_STATUS_OK;
}
