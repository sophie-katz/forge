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

#pragma once

#include <forge/ast/ast.h>

typedef frg_ast_visitor_status_t (*frg_ast_visitor_callback_t)(
    GList* parents,
    frg_ast_t** ast,
    void* user_data
);

typedef struct {
    frg_ast_visitor_callback_t callback_pre;
    frg_ast_visitor_callback_t callback_post;
} frg_ast_visitor_entry_item_t;

frg_ast_visitor_entry_item_t* frg_ast_visitor_entry_item_new(
    frg_ast_visitor_callback_t callback_pre,
    frg_ast_visitor_callback_t callback_post
);

void frg_ast_visitor_entry_item_destroy(frg_ast_visitor_entry_item_t** item);

typedef struct {
    GList* items;
} frg_ast_visitor_entry_t;

typedef struct {
    frg_ast_visitor_entry_t entries[FRG_AST_KIND_COUNT];
    void* user_data;
} frg_ast_visitor_t;

frg_ast_visitor_t* frg_ast_visitor_new(void* user_data);
void frg_ast_visitor_destroy(frg_ast_visitor_t** visitor);

void frg_ast_visitor_add_entry(
    frg_ast_visitor_t* visitor,
    frg_ast_kind_t kind,
    frg_ast_visitor_callback_t callback_pre,
    frg_ast_visitor_callback_t callback_post
);

frg_ast_visitor_status_t frg_ast_visitor_call_entry(
    frg_ast_visitor_t* visitor,
    GList* parents,
    frg_ast_t** ast,
    bool pre
);
