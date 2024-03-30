// This program is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software Foundation,
// either version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
// PARTICULAR PURPOSE. See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with this
// program. If not, see <https://www.gnu.org/licenses/>.
//
// Copyright 2023-2024 Sophie Katz

#include <forge/assert.h>
#include <forge/ast/node_kind_info.h>
#include <forge/ast/visitor.h>
#include <forge/log.h>
#include <forge/memory.h>

frg_ast_visitor_handler_t* frg_ast_visitor_handler_new() {
  frg_ast_visitor_handler_t* handler = frg_malloc(sizeof(frg_ast_visitor_handler_t));

  memset(handler->event_callbacks, 0, sizeof(handler->event_callbacks));

  return handler;
}

void frg_ast_visitor_handler_destroy(frg_ast_visitor_handler_t* handler) {
  frg_free(handler);
}

frg_ast_visitor_t* frg_ast_visitor_new(void* mut_user_data) {
  frg_ast_visitor_t* visitor = frg_malloc(sizeof(frg_ast_visitor_t));
  memset(visitor->_handlers, 0, sizeof(visitor->_handlers));
  visitor->_mut_user_data = mut_user_data;
  return visitor;
}

void frg_ast_visitor_destroy(frg_ast_visitor_t* visitor) {
  frg_assert_pointer_non_null(visitor);

  for (frg_ast_node_kind_t kind = 0; kind <= FRG_AST_NODE_KIND_LAST; kind++) {
    if (visitor->_handlers[kind] != NULL) {
      for (GList* iter = visitor->_handlers[kind]; iter != NULL; iter = iter->next) {
        frg_ast_visitor_handler_destroy((frg_ast_visitor_handler_t*)iter->data);
      }

      g_list_free(visitor->_handlers[kind]);
    }
  }

  frg_free(visitor);
}

frg_ast_visitor_handler_t* frg_ast_visitor_add_handler(frg_ast_visitor_t* mut_visitor,
                                                       frg_ast_node_kind_t node_kind) {
  frg_assert_pointer_non_null(mut_visitor);
  frg_assert_int_greater_than_or_equal_to(node_kind, 0);
  frg_assert_int_less_than_or_equal_to(node_kind, FRG_AST_NODE_KIND_LAST);

  frg_ast_visitor_handler_t* handler = frg_ast_visitor_handler_new();

  mut_visitor->_handlers[node_kind]
    = g_list_append(mut_visitor->_handlers[node_kind], handler);

  return handler;
}

bool frg_ast_visitor_has_handler_for_node_kind(const frg_ast_visitor_t* visitor,
                                               frg_ast_node_kind_t node_kind) {
  frg_assert_pointer_non_null(visitor);
  frg_assert_int_greater_than_or_equal_to(node_kind, 0);
  frg_assert_int_less_than_or_equal_to(node_kind, FRG_AST_NODE_KIND_LAST);

  return (visitor->_handlers[node_kind] != NULL);
}

frg_ast_visitor_status_t frg_ast_visitor_handle_event(frg_ast_node_t** mut_node,
                                                      const frg_ast_visitor_t* visitor,
                                                      const GList* parents,
                                                      frg_ast_visitor_event_t event) {
  frg_assert_pointer_non_null(mut_node);
  frg_assert_pointer_non_null(*mut_node);
  frg_assert_pointer_non_null(visitor);

  frg_ast_node_kind_t original_kind = (*mut_node)->kind;

  for (const GList* iter = visitor->_handlers[(*mut_node)->kind]; iter != NULL;
       iter              = iter->next) {
    if (*mut_node == NULL) {
      break;
    } else if (original_kind != (*mut_node)->kind) {
      // TODO: Put a warning here
      break;
    }

    const frg_ast_visitor_handler_t* handler
      = (const frg_ast_visitor_handler_t*)iter->data;
    frg_ast_visitor_callback_t callback = handler->event_callbacks[event];
    if (callback != NULL) {
      frg_ast_visitor_status_t status
        = callback(mut_node, visitor->_mut_user_data, parents);
      if (status != FRG_AST_VISITOR_STATUS_OK) {
        return status;
      }
    }
  }

  return FRG_AST_VISITOR_STATUS_OK;
}

frg_ast_visitor_status_t _frg_ast_accept_recursive(frg_ast_node_t** mut_node,
                                                   const frg_ast_visitor_t* visitor,
                                                   const GList* parents) {
  frg_assert_pointer_non_null(visitor);
  frg_assert_pointer_non_null(mut_node);

  if (*mut_node == NULL) {
    return FRG_AST_VISITOR_STATUS_OK;
  }

  frg_ast_print_debug_options_t options
    = { .indentation = 4, .max_depth = 1, .max_list_length = 0 };

  if (frg_log_event("Entering '%s' node",
                    frg_ast_node_kind_info_get((*mut_node)->kind)->name)) {
    frg_log_property("Node", NULL);

    frg_ast_print_debug(frg_log_stream_get(), *mut_node, &options);

    frg_stream_output_write_character(frg_log_stream_get(), '\n');
  }

  frg_ast_visitor_status_t status = frg_ast_visitor_handle_event(
    mut_node, visitor, parents, FRG_AST_VISITOR_EVENT_ENTER);
  if (status == FRG_AST_VISITOR_STATUS_SKIP) {
    return FRG_AST_VISITOR_STATUS_OK;
  } else if (status != FRG_AST_VISITOR_STATUS_OK) {
    return status;
  }

  if (*mut_node == NULL) {
    return FRG_AST_VISITOR_STATUS_OK;
  }

  bool is_top         = (parents == NULL);

  GList* parents_next = (GList*)parents;
  parents_next        = g_list_prepend(parents_next, *mut_node);
  status              = FRG_AST_VISITOR_STATUS_OK;

  frg_ast_visitor_acceptor_t visitor_acceptor
    = frg_ast_node_kind_info_get((*mut_node)->kind)->_visitor_acceptor;

  if (visitor_acceptor != NULL) {
    status = visitor_acceptor(mut_node, visitor, parents_next);
  }

  parents_next = g_list_delete_link( // NOLINT(clang-analyzer-deadcode.DeadStores)
    parents_next,
    parents_next);

  if (status == FRG_AST_VISITOR_STATUS_SKIP) {
    return FRG_AST_VISITOR_STATUS_OK;
  } else if (status != FRG_AST_VISITOR_STATUS_OK) {
    return status;
  }

  if (is_top) {
    frg_assert_pointer_null(parents);
  }

  if (frg_log_event("Leaving '%s' node",
                    frg_ast_node_kind_info_get((*mut_node)->kind)->name)) {
    frg_log_property("Node", NULL);

    frg_ast_print_debug(frg_log_stream_get(), *mut_node, &options);

    frg_stream_output_write_character(frg_log_stream_get(), '\n');
  }

  status = frg_ast_visitor_handle_event(
    mut_node, visitor, parents, FRG_AST_VISITOR_EVENT_LEAVE);
  if (status == FRG_AST_VISITOR_STATUS_STOP) {
    return status;
  }

  return FRG_AST_VISITOR_STATUS_OK;
}

frg_ast_visitor_status_t frg_ast_accept(frg_ast_node_t** mut_node,
                                        const frg_ast_visitor_t* visitor) {
  return _frg_ast_accept_recursive(mut_node, visitor, NULL);
}
