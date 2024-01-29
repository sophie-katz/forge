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

#include <forge/ast/visitor.h>
#include <forge/common/memory.h>
#include <forge/common/error.h>

frg_ast_visitor_entry_item_t* frg_ast_visitor_entry_item_new(
    frg_ast_visitor_callback_t callback_pre,
    frg_ast_visitor_callback_t callback_post
) {
    frg_ast_visitor_entry_item_t* item = frg_safe_malloc(sizeof(frg_ast_visitor_entry_item_t));
    item->callback_pre = callback_pre;
    item->callback_post = callback_post;
    return item;
}

void frg_ast_visitor_entry_item_destroy(frg_ast_visitor_entry_item_t** item) {
    frg_safe_free((void**)item);
}

frg_ast_visitor_t* frg_ast_visitor_new(void* user_data) {
    frg_ast_visitor_t* visitor = frg_safe_malloc(sizeof(frg_ast_visitor_t));
    memset(visitor->entries, 0, sizeof(visitor->entries));
    visitor->user_data = user_data;
    return visitor;
}

void frg_ast_visitor_destroy(frg_ast_visitor_t** visitor) {
    frg_assert_pointer_non_null(visitor);
    frg_assert_pointer_non_null(*visitor);

    for (frg_ast_kind_t kind = FRG_AST_KIND_FIRST; kind <= FRG_AST_KIND_LAST; kind++) {
        if ((*visitor)->entries[kind].items != NULL) {
            for (GList* iter = (*visitor)->entries[kind].items; iter != NULL; iter = iter->next) {
                frg_ast_visitor_entry_item_destroy((frg_ast_visitor_entry_item_t**)&iter->data);
            }

            g_list_free((*visitor)->entries[kind].items);
        }
    }

    frg_safe_free((void**)visitor);
}

void frg_ast_visitor_add_entry(
    frg_ast_visitor_t* visitor,
    frg_ast_kind_t kind,
    frg_ast_visitor_callback_t callback_pre,
    frg_ast_visitor_callback_t callback_post
) {
    frg_assert_pointer_non_null(visitor);
    frg_assert_int_ge(kind, FRG_AST_KIND_FIRST);
    frg_assert_int_le(kind, FRG_AST_KIND_LAST);

    frg_ast_visitor_entry_item_t* item = frg_ast_visitor_entry_item_new(
        callback_pre,
        callback_post
    );

    visitor->entries[kind].items = g_list_append(
        visitor->entries[kind].items,
        item
    );
}

frg_ast_visitor_status_t frg_ast_visitor_call_entry(
    frg_ast_visitor_t* visitor,
    GList* parents,
    frg_ast_t** ast,
    bool pre
) {
    frg_assert_pointer_non_null(visitor);
    frg_assert_pointer_non_null(ast);
    frg_assert_pointer_non_null(*ast);

    for (GList* iter = visitor->entries[(*ast)->kind].items; iter != NULL; iter = iter->next) {
        frg_ast_visitor_entry_item_t* item = iter->data;
        frg_ast_visitor_callback_t callback = (pre ? item->callback_pre : item->callback_post);
        if (callback != NULL) {
            frg_ast_visitor_status_t status = callback(
                parents,
                ast,
                visitor->user_data
            );
            if (status != FRG_AST_VISITOR_STATUS_OK) {
                return status;
            }
        }
    }

    return FRG_AST_VISITOR_STATUS_OK;
}
