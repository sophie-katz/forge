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

#include <forge/assert.h>
#include <forge/ast/node_kind_info.h>
#include <forge/ast/visitor.h>
#include <forge/ast/visitor_acceptors.h>

// Defined in visitor.c
frg_ast_visitor_status_t _frg_ast_accept_recursive(frg_ast_node_t** mut_node,
                                                   const frg_ast_visitor_t* visitor,
                                                   const GList* parents);

frg_ast_visitor_status_t _frg_ast_visitor_acceptor_recursive_list(
  GList** mut_list, const frg_ast_visitor_t* visitor, const GList* parents) {
  frg_assert_pointer_non_null(mut_list);
  frg_assert_pointer_non_null(visitor);

  GList* iter = *mut_list;
  while (iter != NULL) {
    frg_ast_node_t* node = iter->data;
    frg_ast_visitor_status_t status
      = _frg_ast_accept_recursive(&node, visitor, parents);
    if (status != FRG_AST_VISITOR_STATUS_OK) {
      return status;
    } else if (node == NULL) {
      GList* next = iter->next;
      *mut_list   = g_list_delete_link(*mut_list, iter);
      iter        = next;
    } else {
      iter->data = node;
      iter       = iter->next;
    }
  }

  return FRG_AST_VISITOR_STATUS_OK;
}

frg_ast_visitor_status_t frg_ast_visitor_acceptor_type_pointer(
  frg_ast_node_t** mut_node, const frg_ast_visitor_t* visitor, const GList* parents) {
  frg_assert_pointer_non_null(visitor);
  frg_assert_pointer_non_null(mut_node);
  frg_assert_pointer_non_null(*mut_node);
  frg_assert_int_equal_to((*mut_node)->kind, FRG_AST_NODE_KIND_TYPE_POINTER);

  frg_ast_visitor_status_t status = _frg_ast_accept_recursive(
    &((frg_ast_node_type_pointer_t*)*mut_node)->value, visitor, parents);
  if (status != FRG_AST_VISITOR_STATUS_OK) {
    return status;
  }

  return FRG_AST_VISITOR_STATUS_OK;
}

frg_ast_visitor_status_t frg_ast_visitor_acceptor_type_function(
  frg_ast_node_t** mut_node, const frg_ast_visitor_t* visitor, const GList* parents) {
  frg_assert_pointer_non_null(visitor);
  frg_assert_pointer_non_null(mut_node);
  frg_assert_pointer_non_null(*mut_node);
  frg_assert_int_equal_to((*mut_node)->kind, FRG_AST_NODE_KIND_TYPE_FUNCTION);

  frg_ast_visitor_status_t status = status = _frg_ast_visitor_acceptor_recursive_list(
    &((frg_ast_node_type_function_t*)*mut_node)->arguments, visitor, parents);
  if (status != FRG_AST_VISITOR_STATUS_OK) {
    return status;
  }

  status = _frg_ast_accept_recursive(
    &((frg_ast_node_type_function_t*)*mut_node)->variadic_positional_arguments,
    visitor,
    parents);
  if (status != FRG_AST_VISITOR_STATUS_OK) {
    return status;
  }

  status = _frg_ast_accept_recursive(
    &((frg_ast_node_type_function_t*)*mut_node)->variadic_keyword_arguments,
    visitor,
    parents);
  if (status != FRG_AST_VISITOR_STATUS_OK) {
    return status;
  }

  status = _frg_ast_accept_recursive(
    &((frg_ast_node_type_function_t*)*mut_node)->return_type, visitor, parents);
  if (status != FRG_AST_VISITOR_STATUS_OK) {
    return status;
  }

  return FRG_AST_VISITOR_STATUS_OK;
}

frg_ast_visitor_status_t frg_ast_visitor_acceptor_declaration_union(
  frg_ast_node_t** mut_node, const frg_ast_visitor_t* visitor, const GList* parents) {
  frg_assert_pointer_non_null(visitor);
  frg_assert_pointer_non_null(mut_node);
  frg_assert_pointer_non_null(*mut_node);
  frg_assert_int_equal_to((*mut_node)->kind, FRG_AST_NODE_KIND_DECLARATION_UNION);

  frg_ast_visitor_status_t status = status = _frg_ast_visitor_acceptor_recursive_list(
    &((frg_ast_node_declaration_union_t*)*mut_node)->properties, visitor, parents);
  if (status != FRG_AST_VISITOR_STATUS_OK) {
    return status;
  }

  return FRG_AST_VISITOR_STATUS_OK;
}

frg_ast_visitor_status_t frg_ast_visitor_acceptor_declaration_structure(
  frg_ast_node_t** mut_node, const frg_ast_visitor_t* visitor, const GList* parents) {
  frg_assert_pointer_non_null(visitor);
  frg_assert_pointer_non_null(mut_node);
  frg_assert_pointer_non_null(*mut_node);
  frg_assert_int_equal_to((*mut_node)->kind, FRG_AST_NODE_KIND_DECLARATION_STRUCTURE);

  frg_ast_visitor_status_t status = status = _frg_ast_visitor_acceptor_recursive_list(
    &((frg_ast_node_declaration_structure_t*)*mut_node)->declarations,
    visitor,
    parents);
  if (status != FRG_AST_VISITOR_STATUS_OK) {
    return status;
  }

  return FRG_AST_VISITOR_STATUS_OK;
}

frg_ast_visitor_status_t frg_ast_visitor_acceptor_declaration_property(
  frg_ast_node_t** mut_node, const frg_ast_visitor_t* visitor, const GList* parents) {
  frg_assert_pointer_non_null(visitor);
  frg_assert_pointer_non_null(mut_node);
  frg_assert_pointer_non_null(*mut_node);
  frg_assert_int_equal_to((*mut_node)->kind, FRG_AST_NODE_KIND_DECLARATION_PROPERTY);

  frg_ast_visitor_status_t status = _frg_ast_accept_recursive(
    &((frg_ast_node_declaration_property_t*)*mut_node)->type, visitor, parents);
  if (status != FRG_AST_VISITOR_STATUS_OK) {
    return status;
  }

  return FRG_AST_VISITOR_STATUS_OK;
}

frg_ast_visitor_status_t frg_ast_visitor_acceptor_declaration_interface(
  frg_ast_node_t** mut_node, const frg_ast_visitor_t* visitor, const GList* parents) {
  frg_assert_pointer_non_null(visitor);
  frg_assert_pointer_non_null(mut_node);
  frg_assert_pointer_non_null(*mut_node);
  frg_assert_int_equal_to((*mut_node)->kind, FRG_AST_NODE_KIND_DECLARATION_INTERFACE);

  frg_ast_visitor_status_t status = _frg_ast_visitor_acceptor_recursive_list(
    &((frg_ast_node_declaration_interface_t*)*mut_node)->extends, visitor, parents);
  if (status != FRG_AST_VISITOR_STATUS_OK) {
    return status;
  }

  status = _frg_ast_visitor_acceptor_recursive_list(
    &((frg_ast_node_declaration_interface_t*)*mut_node)->declarations,
    visitor,
    parents);
  if (status != FRG_AST_VISITOR_STATUS_OK) {
    return status;
  }

  return FRG_AST_VISITOR_STATUS_OK;
}

frg_ast_visitor_status_t frg_ast_visitor_acceptor_declaration_function_argument(
  frg_ast_node_t** mut_node, const frg_ast_visitor_t* visitor, const GList* parents) {
  frg_assert_pointer_non_null(visitor);
  frg_assert_pointer_non_null(mut_node);
  frg_assert_pointer_non_null(*mut_node);
  frg_assert_int_equal_to((*mut_node)->kind,
                          FRG_AST_NODE_KIND_DECLARATION_FUNCTION_ARGUMENT);

  frg_ast_visitor_status_t status = _frg_ast_accept_recursive(
    (frg_ast_node_t**)&((frg_ast_node_declaration_function_argument_t*)*mut_node)
      ->property,
    visitor,
    parents);
  if (status != FRG_AST_VISITOR_STATUS_OK) {
    return status;
  }

  status = _frg_ast_accept_recursive(
    &((frg_ast_node_declaration_function_argument_t*)*mut_node)->default_value,
    visitor,
    parents);
  if (status != FRG_AST_VISITOR_STATUS_OK) {
    return status;
  }

  return FRG_AST_VISITOR_STATUS_OK;
}

frg_ast_visitor_status_t frg_ast_visitor_acceptor_declaration_function(
  frg_ast_node_t** mut_node, const frg_ast_visitor_t* visitor, const GList* parents) {
  frg_assert_pointer_non_null(visitor);
  frg_assert_pointer_non_null(mut_node);
  frg_assert_pointer_non_null(*mut_node);
  frg_assert_int_equal_to((*mut_node)->kind, FRG_AST_NODE_KIND_DECLARATION_FUNCTION);

  frg_ast_visitor_status_t status = _frg_ast_accept_recursive(
    (frg_ast_node_t**)&((frg_ast_node_declaration_function_t*)*mut_node)->type,
    visitor,
    parents);
  if (status != FRG_AST_VISITOR_STATUS_OK) {
    return status;
  }

  status = _frg_ast_accept_recursive(
    &((frg_ast_node_declaration_function_t*)*mut_node)->body, visitor, parents);
  if (status != FRG_AST_VISITOR_STATUS_OK) {
    return status;
  }

  return FRG_AST_VISITOR_STATUS_OK;
}

frg_ast_visitor_status_t frg_ast_visitor_acceptor_declaration_variable(
  frg_ast_node_t** mut_node, const frg_ast_visitor_t* visitor, const GList* parents) {
  frg_assert_pointer_non_null(visitor);
  frg_assert_pointer_non_null(mut_node);
  frg_assert_pointer_non_null(*mut_node);
  frg_assert_int_equal_to((*mut_node)->kind, FRG_AST_NODE_KIND_DECLARATION_VARIABLE);

  frg_ast_visitor_status_t status = _frg_ast_accept_recursive(
    (frg_ast_node_t**)&((frg_ast_node_declaration_variable_t*)*mut_node)->property,
    visitor,
    parents);
  if (status != FRG_AST_VISITOR_STATUS_OK) {
    return status;
  }

  status = _frg_ast_accept_recursive(
    &((frg_ast_node_declaration_variable_t*)*mut_node)->initial_value,
    visitor,
    parents);
  if (status != FRG_AST_VISITOR_STATUS_OK) {
    return status;
  }

  return FRG_AST_VISITOR_STATUS_OK;
}

frg_ast_visitor_status_t frg_ast_visitor_acceptor_declaration_block(
  frg_ast_node_t** mut_node, const frg_ast_visitor_t* visitor, const GList* parents) {
  frg_assert_pointer_non_null(visitor);
  frg_assert_pointer_non_null(mut_node);
  frg_assert_pointer_non_null(*mut_node);
  frg_assert_int_equal_to((*mut_node)->kind, FRG_AST_NODE_KIND_DECLARATION_BLOCK);

  frg_ast_visitor_status_t status = _frg_ast_visitor_acceptor_recursive_list(
    &((frg_ast_node_declaration_block_t*)*mut_node)->declarations, visitor, parents);
  if (status != FRG_AST_VISITOR_STATUS_OK) {
    return status;
  }

  return FRG_AST_VISITOR_STATUS_OK;
}

frg_ast_visitor_status_t frg_ast_visitor_acceptor_statement_return(
  frg_ast_node_t** mut_node, const frg_ast_visitor_t* visitor, const GList* parents) {
  frg_assert_pointer_non_null(visitor);
  frg_assert_pointer_non_null(mut_node);
  frg_assert_pointer_non_null(*mut_node);
  frg_assert_int_equal_to((*mut_node)->kind, FRG_AST_NODE_KIND_STATEMENT_RETURN);

  frg_ast_visitor_status_t status = _frg_ast_accept_recursive(
    &((frg_ast_node_statement_return_t*)*mut_node)->value, visitor, parents);
  if (status != FRG_AST_VISITOR_STATUS_OK) {
    return status;
  }

  return FRG_AST_VISITOR_STATUS_OK;
}

frg_ast_visitor_status_t frg_ast_visitor_acceptor_statement_if(
  frg_ast_node_t** mut_node, const frg_ast_visitor_t* visitor, const GList* parents) {
  frg_assert_pointer_non_null(visitor);
  frg_assert_pointer_non_null(mut_node);
  frg_assert_pointer_non_null(*mut_node);
  frg_assert_int_equal_to((*mut_node)->kind, FRG_AST_NODE_KIND_STATEMENT_IF);

  frg_ast_visitor_status_t status = _frg_ast_accept_recursive(
    &((frg_ast_node_statement_if_t*)*mut_node)->condition, visitor, parents);
  if (status != FRG_AST_VISITOR_STATUS_OK) {
    return status;
  }

  status = _frg_ast_accept_recursive(
    &((frg_ast_node_statement_if_t*)*mut_node)->then_clause, visitor, parents);
  if (status != FRG_AST_VISITOR_STATUS_OK) {
    return status;
  }

  status = _frg_ast_accept_recursive(
    &((frg_ast_node_statement_if_t*)*mut_node)->else_clause, visitor, parents);
  if (status != FRG_AST_VISITOR_STATUS_OK) {
    return status;
  }

  return FRG_AST_VISITOR_STATUS_OK;
}

frg_ast_visitor_status_t frg_ast_visitor_acceptor_statement_while(
  frg_ast_node_t** mut_node, const frg_ast_visitor_t* visitor, const GList* parents) {
  frg_assert_pointer_non_null(visitor);
  frg_assert_pointer_non_null(mut_node);
  frg_assert_pointer_non_null(*mut_node);
  frg_assert_int_equal_to((*mut_node)->kind, FRG_AST_NODE_KIND_STATEMENT_WHILE);

  frg_ast_visitor_status_t status = _frg_ast_accept_recursive(
    &((frg_ast_node_statement_while_t*)*mut_node)->condition, visitor, parents);
  if (status != FRG_AST_VISITOR_STATUS_OK) {
    return status;
  }

  status = _frg_ast_accept_recursive(
    &((frg_ast_node_statement_while_t*)*mut_node)->body, visitor, parents);
  if (status != FRG_AST_VISITOR_STATUS_OK) {
    return status;
  }

  return FRG_AST_VISITOR_STATUS_OK;
}

frg_ast_visitor_status_t frg_ast_visitor_acceptor_statement_block(
  frg_ast_node_t** mut_node, const frg_ast_visitor_t* visitor, const GList* parents) {
  frg_assert_pointer_non_null(visitor);
  frg_assert_pointer_non_null(mut_node);
  frg_assert_pointer_non_null(*mut_node);
  frg_assert_int_equal_to((*mut_node)->kind, FRG_AST_NODE_KIND_STATEMENT_BLOCK);

  frg_ast_visitor_status_t status = _frg_ast_visitor_acceptor_recursive_list(
    &((frg_ast_node_statement_block_t*)*mut_node)->statements, visitor, parents);
  if (status != FRG_AST_VISITOR_STATUS_OK) {
    return status;
  }

  return FRG_AST_VISITOR_STATUS_OK;
}

frg_ast_visitor_status_t frg_ast_visitor_acceptor_value_call_keyword_argument(
  frg_ast_node_t** mut_node, const frg_ast_visitor_t* visitor, const GList* parents) {
  frg_assert_pointer_non_null(visitor);
  frg_assert_pointer_non_null(mut_node);
  frg_assert_pointer_non_null(*mut_node);
  frg_assert_int_equal_to((*mut_node)->kind,
                          FRG_AST_NODE_KIND_VALUE_CALL_KEYWORD_ARGUMENT);

  frg_ast_visitor_status_t status = _frg_ast_accept_recursive(
    &((frg_ast_node_value_call_keyword_argument_t*)*mut_node)->value, visitor, parents);
  if (status != FRG_AST_VISITOR_STATUS_OK) {
    return status;
  }

  return FRG_AST_VISITOR_STATUS_OK;
}

frg_ast_visitor_status_t frg_ast_visitor_acceptor_value_call(
  frg_ast_node_t** mut_node, const frg_ast_visitor_t* visitor, const GList* parents) {
  frg_assert_pointer_non_null(visitor);
  frg_assert_pointer_non_null(mut_node);
  frg_assert_pointer_non_null(*mut_node);
  frg_assert_int_equal_to((*mut_node)->kind, FRG_AST_NODE_KIND_VALUE_CALL);

  frg_ast_visitor_status_t status = _frg_ast_accept_recursive(
    &((frg_ast_node_value_call_t*)*mut_node)->callee, visitor, parents);
  if (status != FRG_AST_VISITOR_STATUS_OK) {
    return status;
  }

  status = _frg_ast_visitor_acceptor_recursive_list(
    &((frg_ast_node_value_call_t*)*mut_node)->arguments, visitor, parents);
  if (status != FRG_AST_VISITOR_STATUS_OK) {
    return status;
  }

  status = _frg_ast_visitor_acceptor_recursive_list(
    &((frg_ast_node_value_call_t*)*mut_node)->keyword_arguments, visitor, parents);
  if (status != FRG_AST_VISITOR_STATUS_OK) {
    return status;
  }

  return FRG_AST_VISITOR_STATUS_OK;
}

frg_ast_visitor_status_t frg_ast_visitor_acceptor_value_unary(
  frg_ast_node_t** mut_node, const frg_ast_visitor_t* visitor, const GList* parents) {
  frg_assert_pointer_non_null(visitor);
  frg_assert_pointer_non_null(mut_node);
  frg_assert_pointer_non_null(*mut_node);
  frg_assert(frg_ast_node_kind_info_get((*mut_node)->kind)->flags
             & FRG_AST_NODE_KIND_FLAG_VALUE_UNARY);

  frg_ast_visitor_status_t status = _frg_ast_accept_recursive(
    &((frg_ast_node_value_unary_t*)*mut_node)->operand, visitor, parents);
  if (status != FRG_AST_VISITOR_STATUS_OK) {
    return status;
  }

  return FRG_AST_VISITOR_STATUS_OK;
}

frg_ast_visitor_status_t frg_ast_visitor_acceptor_value_binary(
  frg_ast_node_t** mut_node, const frg_ast_visitor_t* visitor, const GList* parents) {
  frg_assert_pointer_non_null(visitor);
  frg_assert_pointer_non_null(mut_node);
  frg_assert_pointer_non_null(*mut_node);
  frg_assert(frg_ast_node_kind_info_get((*mut_node)->kind)->flags
             & FRG_AST_NODE_KIND_FLAG_VALUE_BINARY);

  frg_ast_visitor_status_t status = _frg_ast_accept_recursive(
    &((frg_ast_node_value_binary_t*)*mut_node)->left, visitor, parents);
  if (status != FRG_AST_VISITOR_STATUS_OK) {
    return status;
  }

  status = _frg_ast_accept_recursive(
    &((frg_ast_node_value_binary_t*)*mut_node)->right, visitor, parents);
  if (status != FRG_AST_VISITOR_STATUS_OK) {
    return status;
  }

  return FRG_AST_VISITOR_STATUS_OK;
}
