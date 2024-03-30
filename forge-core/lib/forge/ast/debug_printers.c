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
#include <forge/ast/debug_printers.h>
#include <forge/ast/node_kind_info.h>
#include <forge/ast/utilities.h>
#include <forge/debug.h>
#include <forge/enum_printing.h>
#include <forge/lexical_casts/character.h>
#include <forge/lexical_casts/string.h>

// How to correctly indent
// =====================================================================================
//
// Whenever a node is debug printed with a given indentation level N, it should line up
// with text indented by N spaces. For example, if N is 0 then the node should be
// printed like this:
//
// [A]
//   x = [B]
//     y = 1
//   z = 2
//
// And if N is 2 then the node should be printed like this:
//
// [A]
//     x = [B]
//       y = 1
//     z = 2
//
// This looks off because the first line is never indented. It is expected that calling
// code will be responsible for this.
//
// Increasing indentation
// -------------------------------------------------------------------------------------
//
// Let M be the number of spaces to increase indentation by when printing properties of
// a node. So if N is 8 and M is 4, then the node should be printed like this:
//
// [A]
//         x = [B]
//             y = 1
//         z = 2
//
// All debug printers can trust that the value of N passed in via `options` is correct.
// It can be found in the `options.indentation` property. M will always be fixed to
// `FRG_DEBUG_INDENTATION_WIDTH`.
//
// Properties
// -------------------------------------------------------------------------------------
//
// All properties must be indented at a level of N + M to look correct. If we let N be 0
// and M be 2, then an example node would look like this:
//
// [A]
//   x = 1
//   y = 2
//
// If we let N be 2, then it would look like this:
//
// [A]
//     x = 1
//     y = 2
//
// `x` and `y` are indented by 4 spaces because N + M is 4.
//
// Before printing any property, a newline should be emitted followed by
// `options.indentation + FRG_DEBUG_INDENTATION_WIDTH` spaces. This can be done like so:
//
//   frg_ast_print_debug_options_t options_next
//     = frg_ast_print_debug_options_get_next(options, FRG_DEBUG_INDENTATION_WIDTH);
//
//   frg_debug_print_newline(mut_stream, options_next.indentation);
//
// Child nodes
// -------------------------------------------------------------------------------------
//
// When a property is not a node, it should be safe to assume its value won't contain
// any newlines. However, when a property has a node value, there will almost definitely
// be newlines which will need their own indentation.
//
// Here is an example:
//
// [A]
//   x = [B]
//     y = 1
//   z = 2
//
// When printing `A` N is 0 and M is 2. This is why `x` and `y` are both indented by 2
// spaces. When printing `B` we must set N to 2. This is why `y` is indented by 4
// spaces.
//
// This can be done like so:
//
//   frg_ast_print_debug_options_t options_next
//     = frg_ast_print_debug_options_get_next(options, FRG_DEBUG_INDENTATION_WIDTH);
//
//   frg_debug_print_newline(mut_stream, options_next.indentation);
//   frg_debug_print_property(mut_stream, "value", NULL);
//   frg_ast_print_debug(
//     mut_stream,
//     ((const frg_ast_node_type_pointer_t*)node)->value,
//     &options_next
//   );
//
// This way, the call to `frg_ast_print_debug` gets passed an `options.indentation`
// value of N + M.

void _frg_ast_print_debug_list(frg_stream_output_t* mut_stream,
                               const GList* list,
                               const char* label,
                               const frg_ast_print_debug_options_t* options) {
  frg_ast_node_count_t index = 0;

  for (const GList* iter = list; iter != NULL; iter = iter->next) {
    if (index >= options->max_list_length) {
      frg_debug_print_newline(mut_stream, options->indentation);
      frg_debug_print_property_with_index(mut_stream, label, index, "...");
      break;
    }

    frg_debug_print_newline(mut_stream, options->indentation);
    frg_debug_print_property_with_index(mut_stream, label, index, NULL);

    frg_ast_print_debug(mut_stream, (const frg_ast_node_t*)iter->data, options);

    index++;
  }
}

void frg_ast_debug_printer_type_int(frg_stream_output_t* mut_stream,
                                    const frg_ast_node_t* node,
                                    const frg_ast_print_debug_options_t* options) {
  frg_assert_pointer_non_null(mut_stream);
  frg_assert_pointer_non_null(node);
  frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_TYPE_INT);
  frg_assert_int_non_negative(options->indentation);
  frg_assert_int_non_negative(options->max_depth);

  frg_ast_print_debug_options_t options_next
    = frg_ast_print_debug_options_get_next(options, FRG_DEBUG_INDENTATION_WIDTH);

  frg_debug_print_newline(mut_stream, options_next.indentation);
  frg_debug_print_property(mut_stream, "flags", NULL);
  frg_ast_node_type_int_flags_print(mut_stream,
                                    ((const frg_ast_node_type_int_t*)node)->flags);

  frg_debug_print_newline(mut_stream, options_next.indentation);
  frg_debug_print_property(
    mut_stream, "bit-width", "%u", ((const frg_ast_node_type_int_t*)node)->bit_width);
}

void frg_ast_debug_printer_type_float(frg_stream_output_t* mut_stream,
                                      const frg_ast_node_t* node,
                                      const frg_ast_print_debug_options_t* options) {
  frg_assert_pointer_non_null(mut_stream);
  frg_assert_pointer_non_null(node);
  frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_TYPE_FLOAT);
  frg_assert_int_non_negative(options->indentation);

  frg_ast_print_debug_options_t options_next
    = frg_ast_print_debug_options_get_next(options, FRG_DEBUG_INDENTATION_WIDTH);

  frg_debug_print_newline(mut_stream, options_next.indentation);
  frg_debug_print_property(
    mut_stream, "bit-width", "%u", ((const frg_ast_node_type_float_t*)node)->bit_width);
}

void frg_ast_debug_printer_type_symbol(frg_stream_output_t* mut_stream,
                                       const frg_ast_node_t* node,
                                       const frg_ast_print_debug_options_t* options) {
  frg_assert_pointer_non_null(mut_stream);
  frg_assert_pointer_non_null(node);
  frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_TYPE_SYMBOL);
  frg_assert_int_non_negative(options->indentation);

  frg_ast_print_debug_options_t options_next
    = frg_ast_print_debug_options_get_next(options, FRG_DEBUG_INDENTATION_WIDTH);

  frg_debug_print_newline(mut_stream, options_next.indentation);
  frg_debug_print_property(mut_stream, "name", NULL);
  frg_print_string(mut_stream, ((const frg_ast_node_type_symbol_t*)node)->name->str);
}

void frg_ast_debug_printer_type_pointer(frg_stream_output_t* mut_stream,
                                        const frg_ast_node_t* node,
                                        const frg_ast_print_debug_options_t* options) {
  frg_assert_pointer_non_null(mut_stream);
  frg_assert_pointer_non_null(node);
  frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_TYPE_POINTER);
  frg_assert_int_non_negative(options->indentation);

  frg_ast_print_debug_options_t options_next
    = frg_ast_print_debug_options_get_next(options, FRG_DEBUG_INDENTATION_WIDTH);

  frg_debug_print_newline(mut_stream, options_next.indentation);
  frg_debug_print_property(mut_stream, "flags", NULL);
  frg_ast_node_type_pointer_flags_print(
    mut_stream, ((const frg_ast_node_type_pointer_t*)node)->flags);

  frg_debug_print_newline(mut_stream, options_next.indentation);
  frg_debug_print_property(mut_stream, "value", NULL);
  frg_ast_print_debug(
    mut_stream, ((const frg_ast_node_type_pointer_t*)node)->value, &options_next);
}

void frg_ast_debug_printer_type_array(frg_stream_output_t* mut_stream,
                                      const frg_ast_node_t* node,
                                      const frg_ast_print_debug_options_t* options) {
  frg_assert_pointer_non_null(mut_stream);
  frg_assert_pointer_non_null(node);
  frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_TYPE_ARRAY);
  frg_assert_int_non_negative(options->indentation);

  frg_ast_print_debug_options_t options_next
    = frg_ast_print_debug_options_get_next(options, FRG_DEBUG_INDENTATION_WIDTH);

  frg_debug_print_newline(mut_stream, options_next.indentation);
  frg_debug_print_property(
    mut_stream, "length", "%lu", ((const frg_ast_node_type_array_t*)node)->length);

  frg_debug_print_newline(mut_stream, options_next.indentation);
  frg_debug_print_property(mut_stream, "value", NULL);
  frg_ast_print_debug(
    mut_stream, ((const frg_ast_node_type_array_t*)node)->value, &options_next);
}

void frg_ast_debug_printer_type_function(frg_stream_output_t* mut_stream,
                                         const frg_ast_node_t* node,
                                         const frg_ast_print_debug_options_t* options) {
  frg_assert_pointer_non_null(mut_stream);
  frg_assert_pointer_non_null(node);
  frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_TYPE_FUNCTION);
  frg_assert_int_non_negative(options->indentation);

  frg_ast_print_debug_options_t options_next
    = frg_ast_print_debug_options_get_next(options, FRG_DEBUG_INDENTATION_WIDTH);

  _frg_ast_print_debug_list(mut_stream,
                            ((const frg_ast_node_type_function_t*)node)->arguments,
                            "arguments",
                            &options_next);

  frg_debug_print_newline(mut_stream, options_next.indentation);
  frg_debug_print_property(mut_stream, "variadic-positional-arguments", NULL);
  frg_ast_print_debug(
    mut_stream,
    ((const frg_ast_node_type_function_t*)node)->variadic_positional_arguments,
    &options_next);

  frg_debug_print_newline(mut_stream, options_next.indentation);
  frg_debug_print_property(mut_stream, "variadic-keyword-arguments", NULL);
  frg_ast_print_debug(
    mut_stream,
    ((const frg_ast_node_type_function_t*)node)->variadic_keyword_arguments,
    &options_next);

  frg_debug_print_newline(mut_stream, options_next.indentation);
  frg_debug_print_property(mut_stream, "return-type", NULL);
  frg_ast_print_debug(mut_stream,
                      ((const frg_ast_node_type_function_t*)node)->return_type,
                      &options_next);
}

void frg_ast_debug_printer_declaration_union(
  frg_stream_output_t* mut_stream,
  const frg_ast_node_t* node,
  const frg_ast_print_debug_options_t* options) {
  frg_assert_pointer_non_null(mut_stream);
  frg_assert_pointer_non_null(node);
  frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_DECLARATION_UNION);
  frg_assert_int_non_negative(options->indentation);

  frg_ast_print_debug_options_t options_next
    = frg_ast_print_debug_options_get_next(options, FRG_DEBUG_INDENTATION_WIDTH);

  frg_debug_print_newline(mut_stream, options_next.indentation);
  frg_debug_print_property(mut_stream, "name", NULL);
  frg_print_string(mut_stream,
                   ((const frg_ast_node_declaration_union_t*)node)->name->str);

  _frg_ast_print_debug_list(mut_stream,
                            ((const frg_ast_node_declaration_union_t*)node)->properties,
                            "properties",
                            &options_next);
}

void frg_ast_debug_printer_declaration_structure(
  frg_stream_output_t* mut_stream,
  const frg_ast_node_t* node,
  const frg_ast_print_debug_options_t* options) {
  frg_assert_pointer_non_null(mut_stream);
  frg_assert_pointer_non_null(node);
  frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_DECLARATION_STRUCTURE);
  frg_assert_int_non_negative(options->indentation);

  frg_ast_print_debug_options_t options_next
    = frg_ast_print_debug_options_get_next(options, FRG_DEBUG_INDENTATION_WIDTH);

  frg_debug_print_newline(mut_stream, options_next.indentation);
  frg_debug_print_property(mut_stream, "name", NULL);
  frg_print_string(mut_stream,
                   ((const frg_ast_node_declaration_structure_t*)node)->name->str);

  _frg_ast_print_debug_list(
    mut_stream,
    ((const frg_ast_node_declaration_structure_t*)node)->declarations,
    "declarations",
    &options_next);
}

void frg_ast_debug_printer_declaration_property(
  frg_stream_output_t* mut_stream,
  const frg_ast_node_t* node,
  const frg_ast_print_debug_options_t* options) {
  frg_assert_pointer_non_null(mut_stream);
  frg_assert_pointer_non_null(node);
  frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_DECLARATION_PROPERTY);
  frg_assert_int_non_negative(options->indentation);

  frg_ast_print_debug_options_t options_next
    = frg_ast_print_debug_options_get_next(options, FRG_DEBUG_INDENTATION_WIDTH);

  frg_debug_print_newline(mut_stream, options_next.indentation);
  frg_debug_print_property(mut_stream, "flags", NULL);
  frg_ast_node_declaration_property_flags_print(
    mut_stream, ((const frg_ast_node_declaration_property_t*)node)->flags);

  frg_debug_print_newline(mut_stream, options_next.indentation);
  frg_debug_print_property(mut_stream, "name", NULL);
  frg_print_string(mut_stream,
                   ((const frg_ast_node_declaration_property_t*)node)->name->str);

  frg_debug_print_newline(mut_stream, options_next.indentation);
  frg_debug_print_property(mut_stream, "type", NULL);
  frg_ast_print_debug(mut_stream,
                      ((const frg_ast_node_declaration_property_t*)node)->type,
                      &options_next);
}

void frg_ast_debug_printer_declaration_interface(
  frg_stream_output_t* mut_stream,
  const frg_ast_node_t* node,
  const frg_ast_print_debug_options_t* options) {
  frg_assert_pointer_non_null(mut_stream);
  frg_assert_pointer_non_null(node);
  frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_DECLARATION_INTERFACE);
  frg_assert_int_non_negative(options->indentation);

  frg_ast_print_debug_options_t options_next
    = frg_ast_print_debug_options_get_next(options, FRG_DEBUG_INDENTATION_WIDTH);

  frg_debug_print_newline(mut_stream, options_next.indentation);
  frg_debug_print_property(mut_stream, "flags", NULL);
  frg_ast_node_declaration_interface_flags_print(
    mut_stream, ((const frg_ast_node_declaration_interface_t*)node)->flags);

  frg_debug_print_newline(mut_stream, options_next.indentation);
  frg_debug_print_property(mut_stream, "name", NULL);
  frg_print_string(mut_stream,
                   ((const frg_ast_node_declaration_interface_t*)node)->name->str);

  _frg_ast_print_debug_list(
    mut_stream,
    ((const frg_ast_node_declaration_interface_t*)node)->extends,
    "extends",
    &options_next);

  _frg_ast_print_debug_list(
    mut_stream,
    ((const frg_ast_node_declaration_interface_t*)node)->declarations,
    "declarations",
    &options_next);
}

void frg_ast_debug_printer_declaration_function_argument(
  frg_stream_output_t* mut_stream,
  const frg_ast_node_t* node,
  const frg_ast_print_debug_options_t* options) {
  frg_assert_pointer_non_null(mut_stream);
  frg_assert_pointer_non_null(node);
  frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_DECLARATION_FUNCTION_ARGUMENT);
  frg_assert_int_non_negative(options->indentation);

  frg_ast_print_debug_options_t options_next
    = frg_ast_print_debug_options_get_next(options, FRG_DEBUG_INDENTATION_WIDTH);

  frg_debug_print_newline(mut_stream, options_next.indentation);

  frg_debug_print_property(mut_stream, "flags", NULL);
  frg_ast_node_declaration_function_argument_flags_print(
    mut_stream, ((const frg_ast_node_declaration_function_argument_t*)node)->flags);

  frg_debug_print_newline(mut_stream, options_next.indentation);
  frg_debug_print_property(mut_stream, "property", NULL);
  frg_ast_print_debug(
    mut_stream,
    (const frg_ast_node_t*)((const frg_ast_node_declaration_function_argument_t*)node)
      ->property,
    &options_next);

  frg_debug_print_newline(mut_stream, options_next.indentation);
  frg_debug_print_property(mut_stream, "default-value", NULL);
  frg_ast_print_debug(
    mut_stream,
    ((const frg_ast_node_declaration_function_argument_t*)node)->default_value,
    &options_next);
}

void frg_ast_debug_printer_declaration_function(
  frg_stream_output_t* mut_stream,
  const frg_ast_node_t* node,
  const frg_ast_print_debug_options_t* options) {
  frg_assert_pointer_non_null(mut_stream);
  frg_assert_pointer_non_null(node);
  frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_DECLARATION_FUNCTION);
  frg_assert_int_non_negative(options->indentation);

  frg_ast_print_debug_options_t options_next
    = frg_ast_print_debug_options_get_next(options, FRG_DEBUG_INDENTATION_WIDTH);

  frg_debug_print_newline(mut_stream, options_next.indentation);
  frg_debug_print_property(mut_stream, "flags", NULL);
  frg_ast_node_declaration_function_flags_print(
    mut_stream, ((const frg_ast_node_declaration_function_t*)node)->flags);

  frg_debug_print_newline(mut_stream, options_next.indentation);
  frg_debug_print_property(mut_stream, "name", NULL);
  frg_print_string(mut_stream,
                   ((const frg_ast_node_declaration_function_t*)node)->name->str);

  frg_debug_print_newline(mut_stream, options_next.indentation);
  frg_debug_print_property(mut_stream, "type", NULL);
  frg_ast_print_debug(
    mut_stream,
    (const frg_ast_node_t*)((const frg_ast_node_declaration_function_t*)node)->type,
    &options_next);

  frg_debug_print_newline(mut_stream, options_next.indentation);
  frg_debug_print_property(mut_stream, "body", NULL);
  frg_ast_print_debug(mut_stream,
                      ((const frg_ast_node_declaration_function_t*)node)->body,
                      &options_next);
}

void frg_ast_debug_printer_declaration_assignment(
  frg_stream_output_t* mut_stream,
  const frg_ast_node_t* node,
  const frg_ast_print_debug_options_t* options) {
  frg_assert_pointer_non_null(mut_stream);
  frg_assert_pointer_non_null(node);
  frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_DECLARATION_ASSIGNMENT);
  frg_assert_int_non_negative(options->indentation);

  frg_ast_print_debug_options_t options_next
    = frg_ast_print_debug_options_get_next(options, FRG_DEBUG_INDENTATION_WIDTH);

  frg_debug_print_newline(mut_stream, options_next.indentation);
  frg_debug_print_property(mut_stream, "property", NULL);
  frg_ast_print_debug(
    mut_stream,
    (const frg_ast_node_t*)((const frg_ast_node_declaration_assignment_t*)node)
      ->property,
    &options_next);

  frg_debug_print_newline(mut_stream, options_next.indentation);
  frg_debug_print_property(mut_stream, "value", NULL);
  frg_ast_print_debug(mut_stream,
                      ((const frg_ast_node_declaration_assignment_t*)node)->value,
                      &options_next);
}

void frg_ast_debug_printer_declaration_block(
  frg_stream_output_t* mut_stream,
  const frg_ast_node_t* node,
  const frg_ast_print_debug_options_t* options) {
  frg_assert_pointer_non_null(mut_stream);
  frg_assert_pointer_non_null(node);
  frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_DECLARATION_BLOCK);
  frg_assert_int_non_negative(options->indentation);

  frg_ast_print_debug_options_t options_next
    = frg_ast_print_debug_options_get_next(options, FRG_DEBUG_INDENTATION_WIDTH);

  _frg_ast_print_debug_list(
    mut_stream,
    ((const frg_ast_node_declaration_block_t*)node)->declarations,
    "declarations",
    &options_next);
}

void frg_ast_debug_printer_statement_return(
  frg_stream_output_t* mut_stream,
  const frg_ast_node_t* node,
  const frg_ast_print_debug_options_t* options) {
  frg_assert_pointer_non_null(mut_stream);
  frg_assert_pointer_non_null(node);
  frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_STATEMENT_RETURN);
  frg_assert_int_non_negative(options->indentation);

  frg_ast_print_debug_options_t options_next
    = frg_ast_print_debug_options_get_next(options, FRG_DEBUG_INDENTATION_WIDTH);

  frg_debug_print_newline(mut_stream, options_next.indentation);
  frg_debug_print_property(mut_stream, "value", NULL);
  frg_ast_print_debug(
    mut_stream, ((const frg_ast_node_statement_return_t*)node)->value, &options_next);
}

void frg_ast_debug_printer_statement_if_conditional_clause(
  frg_stream_output_t* mut_stream,
  const frg_ast_node_t* node,
  const frg_ast_print_debug_options_t* options) {
  frg_assert_pointer_non_null(mut_stream);
  frg_assert_pointer_non_null(node);
  frg_assert_int_equal_to(node->kind,
                          FRG_AST_NODE_KIND_STATEMENT_IF_CONDITIONAL_CLAUSE);
  frg_assert_int_non_negative(options->indentation);

  frg_ast_print_debug_options_t options_next
    = frg_ast_print_debug_options_get_next(options, FRG_DEBUG_INDENTATION_WIDTH);

  frg_debug_print_newline(mut_stream, options_next.indentation);
  frg_debug_print_property(mut_stream, "condition", NULL);
  frg_ast_print_debug(
    mut_stream,
    ((const frg_ast_node_statement_if_conditional_clause_t*)node)->condition,
    &options_next);

  frg_debug_print_newline(mut_stream, options_next.indentation);
  frg_debug_print_property(mut_stream, "body", NULL);
  frg_ast_print_debug(
    mut_stream,
    ((const frg_ast_node_statement_if_conditional_clause_t*)node)->body,
    &options_next);
}

void frg_ast_debug_printer_statement_if(frg_stream_output_t* mut_stream,
                                        const frg_ast_node_t* node,
                                        const frg_ast_print_debug_options_t* options) {
  frg_assert_pointer_non_null(mut_stream);
  frg_assert_pointer_non_null(node);
  frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_STATEMENT_IF);
  frg_assert_int_non_negative(options->indentation);

  frg_ast_print_debug_options_t options_next
    = frg_ast_print_debug_options_get_next(options, FRG_DEBUG_INDENTATION_WIDTH);

  _frg_ast_print_debug_list(
    mut_stream,
    ((const frg_ast_node_statement_if_t*)node)->conditional_clauses,
    "conditional-clauses",
    &options_next);

  frg_debug_print_newline(mut_stream, options_next.indentation);
  frg_debug_print_property(mut_stream, "else-clause", NULL);
  frg_ast_print_debug(
    mut_stream, ((const frg_ast_node_statement_if_t*)node)->else_clause, &options_next);
}

void frg_ast_debug_printer_statement_while(
  frg_stream_output_t* mut_stream,
  const frg_ast_node_t* node,
  const frg_ast_print_debug_options_t* options) {
  frg_assert_pointer_non_null(mut_stream);
  frg_assert_pointer_non_null(node);
  frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_STATEMENT_WHILE);
  frg_assert_int_non_negative(options->indentation);

  frg_ast_print_debug_options_t options_next
    = frg_ast_print_debug_options_get_next(options, FRG_DEBUG_INDENTATION_WIDTH);

  frg_debug_print_newline(mut_stream, options_next.indentation);
  frg_debug_print_property(mut_stream, "condition", NULL);
  frg_ast_print_debug(mut_stream,
                      ((const frg_ast_node_statement_while_t*)node)->condition,
                      &options_next);

  frg_debug_print_newline(mut_stream, options_next.indentation);
  frg_debug_print_property(mut_stream, "body", NULL);
  frg_ast_print_debug(
    mut_stream, ((const frg_ast_node_statement_while_t*)node)->body, &options_next);
}

void frg_ast_debug_printer_statement_block(
  frg_stream_output_t* mut_stream,
  const frg_ast_node_t* node,
  const frg_ast_print_debug_options_t* options) {
  frg_assert_pointer_non_null(mut_stream);
  frg_assert_pointer_non_null(node);
  frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_STATEMENT_BLOCK);
  frg_assert_int_non_negative(options->indentation);

  frg_ast_print_debug_options_t options_next
    = frg_ast_print_debug_options_get_next(options, FRG_DEBUG_INDENTATION_WIDTH);

  _frg_ast_print_debug_list(mut_stream,
                            ((const frg_ast_node_statement_block_t*)node)->statements,
                            "statements",
                            &options_next);
}

void frg_ast_debug_printer_value_bool(frg_stream_output_t* mut_stream,
                                      const frg_ast_node_t* node,
                                      const frg_ast_print_debug_options_t* options) {
  frg_assert_pointer_non_null(mut_stream);
  frg_assert_pointer_non_null(node);
  frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_VALUE_BOOL);
  frg_assert_int_non_negative(options->indentation);

  frg_ast_print_debug_options_t options_next
    = frg_ast_print_debug_options_get_next(options, FRG_DEBUG_INDENTATION_WIDTH);

  frg_debug_print_newline(mut_stream, options_next.indentation);
  frg_debug_print_property(mut_stream,
                           "value",
                           "%s",
                           ((const frg_ast_node_value_bool_t*)node)->value ? "true"
                                                                           : "false");
}

void frg_ast_debug_printer_value_int(frg_stream_output_t* mut_stream,
                                     const frg_ast_node_t* node,
                                     const frg_ast_print_debug_options_t* options) {
  frg_assert_pointer_non_null(mut_stream);
  frg_assert_pointer_non_null(node);
  frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_VALUE_INT);
  frg_assert_int_non_negative(options->indentation);

  frg_ast_print_debug_options_t options_next
    = frg_ast_print_debug_options_get_next(options, FRG_DEBUG_INDENTATION_WIDTH);

  frg_debug_print_newline(mut_stream, options_next.indentation);
  frg_debug_print_property(mut_stream, "type", NULL);
  frg_ast_print_debug(
    mut_stream,
    (const frg_ast_node_t*)&((const frg_ast_node_value_int_t*)node)->type,
    &options_next);

  frg_debug_print_newline(mut_stream, options_next.indentation);
  switch (frg_get_case_for_type_int(&((const frg_ast_node_value_int_t*)node)->type)) {
  case frg_get_case_for_int_attributes(true, 8):
    frg_debug_print_property(mut_stream,
                             "value",
                             "%ii8",
                             ((const frg_ast_node_value_int_t*)node)->value.as_i8);
    break;
  case frg_get_case_for_int_attributes(true, 16):
    frg_debug_print_property(mut_stream,
                             "value",
                             "%ii16",
                             ((const frg_ast_node_value_int_t*)node)->value.as_i16);
    break;
  case frg_get_case_for_int_attributes(true, 32):
    frg_debug_print_property(mut_stream,
                             "value",
                             "%ii32",
                             ((const frg_ast_node_value_int_t*)node)->value.as_i32);
    break;
  case frg_get_case_for_int_attributes(true, 64):
    frg_debug_print_property(mut_stream,
                             "value",
                             "%lii64",
                             ((const frg_ast_node_value_int_t*)node)->value.as_i64);
    break;
  case frg_get_case_for_int_attributes(false, 8):
    frg_debug_print_property(mut_stream,
                             "value",
                             "%uu8",
                             ((const frg_ast_node_value_int_t*)node)->value.as_u8);
    break;
  case frg_get_case_for_int_attributes(false, 16):
    frg_debug_print_property(mut_stream,
                             "value",
                             "%uu16",
                             ((const frg_ast_node_value_int_t*)node)->value.as_u16);
    break;
  case frg_get_case_for_int_attributes(false, 32):
    frg_debug_print_property(mut_stream,
                             "value",
                             "%uu32",
                             ((const frg_ast_node_value_int_t*)node)->value.as_u32);
    break;
  case frg_get_case_for_int_attributes(false, 64):
    frg_debug_print_property(mut_stream,
                             "value",
                             "%luu64",
                             ((const frg_ast_node_value_int_t*)node)->value.as_u64);
    break;
  default:
    frg_die("Unexpected bit width");
  }
}

void frg_ast_debug_printer_value_float(frg_stream_output_t* mut_stream,
                                       const frg_ast_node_t* node,
                                       const frg_ast_print_debug_options_t* options) {
  frg_assert_pointer_non_null(mut_stream);
  frg_assert_pointer_non_null(node);
  frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_VALUE_FLOAT);
  frg_assert_int_non_negative(options->indentation);

  frg_ast_print_debug_options_t options_next
    = frg_ast_print_debug_options_get_next(options, FRG_DEBUG_INDENTATION_WIDTH);

  frg_debug_print_newline(mut_stream, options_next.indentation);
  frg_debug_print_property(mut_stream, "type", NULL);
  frg_ast_print_debug(
    mut_stream,
    (const frg_ast_node_t*)&((const frg_ast_node_value_float_t*)node)->type,
    &options_next);

  frg_debug_print_newline(mut_stream, options_next.indentation);
  switch (((const frg_ast_node_value_float_t*)node)->type.bit_width) {
  case 32:
    frg_debug_print_property(mut_stream,
                             "value",
                             "%ff32",
                             ((const frg_ast_node_value_float_t*)node)->value.as_f32);
    break;
  case 64:
    frg_debug_print_property(mut_stream,
                             "value",
                             "%lff64",
                             ((const frg_ast_node_value_float_t*)node)->value.as_f64);
    break;
  default:
    frg_debug_print_property(
      mut_stream,
      "value",
      "%lf (defaulting to f64 since bit width %u is not supported)",
      ((const frg_ast_node_value_float_t*)node)->value.as_f64,
      ((const frg_ast_node_value_float_t*)node)->type.bit_width);
    break;
  }
}

void frg_ast_debug_printer_value_character(
  frg_stream_output_t* mut_stream,
  const frg_ast_node_t* node,
  const frg_ast_print_debug_options_t* options) {
  frg_assert_pointer_non_null(mut_stream);
  frg_assert_pointer_non_null(node);
  frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_VALUE_CHARACTER);
  frg_assert_int_non_negative(options->indentation);

  frg_ast_print_debug_options_t options_next
    = frg_ast_print_debug_options_get_next(options, FRG_DEBUG_INDENTATION_WIDTH);

  frg_debug_print_newline(mut_stream, options_next.indentation);
  frg_debug_print_property(mut_stream, "value", NULL);
  frg_print_character(mut_stream, ((frg_ast_node_value_character_t*)node)->value);
}

void frg_ast_debug_printer_value_string(frg_stream_output_t* mut_stream,
                                        const frg_ast_node_t* node,
                                        const frg_ast_print_debug_options_t* options) {
  frg_assert_pointer_non_null(mut_stream);
  frg_assert_pointer_non_null(node);
  frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_VALUE_STRING);
  frg_assert_int_non_negative(options->indentation);

  frg_ast_print_debug_options_t options_next
    = frg_ast_print_debug_options_get_next(options, FRG_DEBUG_INDENTATION_WIDTH);

  frg_debug_print_newline(mut_stream, options_next.indentation);
  frg_debug_print_property(mut_stream, "value", NULL);
  frg_print_string(mut_stream, ((frg_ast_node_value_string_t*)node)->value->str);
}

void frg_ast_debug_printer_value_array(frg_stream_output_t* mut_stream,
                                       const frg_ast_node_t* node,
                                       const frg_ast_print_debug_options_t* options) {
  frg_assert_pointer_non_null(mut_stream);
  frg_assert_pointer_non_null(node);
  frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_VALUE_ARRAY);
  frg_assert_int_non_negative(options->indentation);

  frg_ast_print_debug_options_t options_next
    = frg_ast_print_debug_options_get_next(options, FRG_DEBUG_INDENTATION_WIDTH);

  _frg_ast_print_debug_list(mut_stream,
                            ((frg_ast_node_value_array_t*)node)->elements,
                            "elements",
                            &options_next);
}

void frg_ast_debug_printer_value_array_repeated(
  frg_stream_output_t* mut_stream,
  const frg_ast_node_t* node,
  const frg_ast_print_debug_options_t* options) {
  frg_assert_pointer_non_null(mut_stream);
  frg_assert_pointer_non_null(node);
  frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_VALUE_ARRAY_REPEATED);
  frg_assert_int_non_negative(options->indentation);

  frg_ast_print_debug_options_t options_next
    = frg_ast_print_debug_options_get_next(options, FRG_DEBUG_INDENTATION_WIDTH);

  frg_debug_print_newline(mut_stream, options_next.indentation);
  frg_debug_print_property(
    mut_stream, "length", "%lu", ((frg_ast_node_value_array_repeated_t*)node)->length);

  frg_debug_print_newline(mut_stream, options_next.indentation);
  frg_debug_print_property(mut_stream, "element", NULL);
  frg_ast_print_debug(
    mut_stream, ((frg_ast_node_value_array_repeated_t*)node)->element, &options_next);
}

void frg_ast_debug_printer_value_structure(
  frg_stream_output_t* mut_stream,
  const frg_ast_node_t* node,
  const frg_ast_print_debug_options_t* options) {
  frg_assert_pointer_non_null(mut_stream);
  frg_assert_pointer_non_null(node);
  frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_VALUE_STRUCTURE);
  frg_assert_int_non_negative(options->indentation);

  frg_ast_print_debug_options_t options_next
    = frg_ast_print_debug_options_get_next(options, FRG_DEBUG_INDENTATION_WIDTH);

  frg_debug_print_newline(mut_stream, options_next.indentation);
  frg_debug_print_property(mut_stream, "assignments", NULL);
  _frg_ast_print_debug_list(mut_stream,
                            ((frg_ast_node_value_structure_t*)node)->assignments,
                            "assignments",
                            &options_next);
}

void frg_ast_debug_printer_value_symbol(frg_stream_output_t* mut_stream,
                                        const frg_ast_node_t* node,
                                        const frg_ast_print_debug_options_t* options) {
  frg_assert_pointer_non_null(mut_stream);
  frg_assert_pointer_non_null(node);
  frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_VALUE_SYMBOL);
  frg_assert_int_non_negative(options->indentation);

  frg_ast_print_debug_options_t options_next
    = frg_ast_print_debug_options_get_next(options, FRG_DEBUG_INDENTATION_WIDTH);

  frg_debug_print_newline(mut_stream, options_next.indentation);
  frg_debug_print_property(mut_stream, "name", NULL);
  frg_print_string(mut_stream, ((frg_ast_node_value_symbol_t*)node)->name->str);
}

void frg_ast_debug_printer_value_call_keyword_argument(
  frg_stream_output_t* mut_stream,
  const frg_ast_node_t* node,
  const frg_ast_print_debug_options_t* options) {
  frg_assert_pointer_non_null(mut_stream);
  frg_assert_pointer_non_null(node);
  frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_VALUE_CALL_KEYWORD_ARGUMENT);
  frg_assert_int_non_negative(options->indentation);

  frg_ast_print_debug_options_t options_next
    = frg_ast_print_debug_options_get_next(options, FRG_DEBUG_INDENTATION_WIDTH);

  frg_debug_print_newline(mut_stream, options_next.indentation);
  frg_debug_print_property(mut_stream, "name", NULL);
  frg_print_string(mut_stream,
                   ((frg_ast_node_value_call_keyword_argument_t*)node)->name->str);

  frg_debug_print_newline(mut_stream, options_next.indentation);
  frg_debug_print_property(mut_stream, "value", NULL);
  frg_ast_print_debug(mut_stream,
                      ((frg_ast_node_value_call_keyword_argument_t*)node)->value,
                      &options_next);
}

void frg_ast_debug_printer_value_call(frg_stream_output_t* mut_stream,
                                      const frg_ast_node_t* node,
                                      const frg_ast_print_debug_options_t* options) {
  frg_assert_pointer_non_null(mut_stream);
  frg_assert_pointer_non_null(node);
  frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_VALUE_CALL);
  frg_assert_int_non_negative(options->indentation);

  frg_ast_print_debug_options_t options_next
    = frg_ast_print_debug_options_get_next(options, FRG_DEBUG_INDENTATION_WIDTH);

  frg_debug_print_newline(mut_stream, options_next.indentation);
  frg_debug_print_property(mut_stream, "callee", NULL);
  frg_ast_print_debug(
    mut_stream, ((frg_ast_node_value_call_t*)node)->callee, &options_next);

  _frg_ast_print_debug_list(mut_stream,
                            ((frg_ast_node_value_call_t*)node)->arguments,
                            "arguments",
                            &options_next);

  _frg_ast_print_debug_list(mut_stream,
                            ((frg_ast_node_value_call_t*)node)->keyword_arguments,
                            "keyword-arguments",
                            &options_next);
}

void frg_ast_debug_printer_value_cast(frg_stream_output_t* mut_stream,
                                      const frg_ast_node_t* node,
                                      const frg_ast_print_debug_options_t* options) {
  frg_assert_pointer_non_null(mut_stream);
  frg_assert_pointer_non_null(node);
  frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_VALUE_CAST);
  frg_assert_int_non_negative(options->indentation);

  frg_ast_print_debug_options_t options_next
    = frg_ast_print_debug_options_get_next(options, FRG_DEBUG_INDENTATION_WIDTH);

  frg_debug_print_newline(mut_stream, options_next.indentation);
  frg_debug_print_property(mut_stream, "value", NULL);
  frg_ast_print_debug(
    mut_stream, ((frg_ast_node_value_cast_t*)node)->value, &options_next);

  frg_debug_print_newline(mut_stream, options_next.indentation);
  frg_debug_print_property(mut_stream, "type", NULL);
  frg_ast_print_debug(
    mut_stream, ((frg_ast_node_value_cast_t*)node)->type, &options_next);
}

void frg_ast_debug_printer_value_unary(frg_stream_output_t* mut_stream,
                                       const frg_ast_node_t* node,
                                       const frg_ast_print_debug_options_t* options) {
  frg_assert_pointer_non_null(mut_stream);
  frg_assert_pointer_non_null(node);
  frg_assert(frg_ast_node_kind_info_get(node->kind)->flags
             & FRG_AST_NODE_KIND_FLAG_VALUE_UNARY);
  frg_assert_int_non_negative(options->indentation);

  frg_ast_print_debug_options_t options_next
    = frg_ast_print_debug_options_get_next(options, FRG_DEBUG_INDENTATION_WIDTH);

  frg_debug_print_newline(mut_stream, options_next.indentation);
  frg_debug_print_property(mut_stream, "operand", NULL);
  frg_ast_print_debug(
    mut_stream, ((frg_ast_node_value_unary_t*)node)->operand, &options_next);
}

void frg_ast_debug_printer_value_binary(frg_stream_output_t* mut_stream,
                                        const frg_ast_node_t* node,
                                        const frg_ast_print_debug_options_t* options) {
  frg_assert_pointer_non_null(mut_stream);
  frg_assert_pointer_non_null(node);
  frg_assert(frg_ast_node_kind_info_get(node->kind)->flags
             & FRG_AST_NODE_KIND_FLAG_VALUE_BINARY);
  frg_assert_int_non_negative(options->indentation);

  frg_ast_print_debug_options_t options_next
    = frg_ast_print_debug_options_get_next(options, FRG_DEBUG_INDENTATION_WIDTH);

  frg_debug_print_newline(mut_stream, options_next.indentation);
  frg_debug_print_property(mut_stream, "left", NULL);
  frg_ast_print_debug(
    mut_stream, ((frg_ast_node_value_binary_t*)node)->left, &options_next);

  frg_debug_print_newline(mut_stream, options_next.indentation);
  frg_debug_print_property(mut_stream, "right", NULL);
  frg_ast_print_debug(
    mut_stream, ((frg_ast_node_value_binary_t*)node)->right, &options_next);
}
