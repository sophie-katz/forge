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

#include <forge/ast/node.h>

typedef frg_ast_visitor_status_t (*frg_ast_visitor_callback_t)(
  frg_ast_node_t** mut_node, void* mut_user_data, const GList* parents);

typedef struct {
  frg_ast_visitor_callback_t event_callbacks[FRG_AST_VISITOR_EVENT_COUNT];
} frg_ast_visitor_handler_t;

frg_ast_visitor_handler_t* frg_ast_visitor_handler_new();

void frg_ast_visitor_handler_destroy(frg_ast_visitor_handler_t* handler);

typedef struct {
  GList* _handlers[FRG_AST_NODE_KIND_COUNT];
  void* _mut_user_data;
} frg_ast_visitor_t;

frg_ast_visitor_t* frg_ast_visitor_new(void* mut_user_data);

void frg_ast_visitor_destroy(frg_ast_visitor_t* visitor);

frg_ast_visitor_handler_t* frg_ast_visitor_add_handler(frg_ast_visitor_t* mut_visitor,
                                                       frg_ast_node_kind_t node_kind);

bool frg_ast_visitor_has_handler_for_node_kind(const frg_ast_visitor_t* visitor,
                                               frg_ast_node_kind_t node_kind);

frg_ast_visitor_status_t frg_ast_visitor_handle_event(frg_ast_node_t** mut_node,
                                                      const frg_ast_visitor_t* visitor,
                                                      const GList* parents,
                                                      frg_ast_visitor_event_t event);

frg_ast_visitor_status_t frg_ast_accept(frg_ast_node_t** mut_node,
                                        const frg_ast_visitor_t* visitor);
