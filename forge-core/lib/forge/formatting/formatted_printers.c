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
#include <forge/formatting/formatted_printers.h>
#include <forge/formatting/print.h>

void frg_formatting_formatted_printer_type_void(frg_stream_output_t* mut_stream,
                                                const frg_ast_node_t* node,
                                                frg_indentation_t indentation) {
  frg_assert_pointer_non_null(mut_stream);
  frg_assert_pointer_non_null(node);
  frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_TYPE_VOID);
  frg_assert_int_non_negative(indentation);

  frg_stream_output_write_string(mut_stream, "void");
}

void frg_formatting_formatted_printer_type_bool(frg_stream_output_t* mut_stream,
                                                const frg_ast_node_t* node,
                                                frg_indentation_t indentation) {
  frg_assert_pointer_non_null(mut_stream);
  frg_assert_pointer_non_null(node);
  frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_TYPE_BOOL);
  frg_assert_int_non_negative(indentation);

  frg_stream_output_write_string(mut_stream, "bool");
}

void frg_formatting_formatted_printer_type_int(frg_stream_output_t* mut_stream,
                                               const frg_ast_node_t* node,
                                               frg_indentation_t indentation) {
  frg_assert_pointer_non_null(mut_stream);
  frg_assert_pointer_non_null(node);
  frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_TYPE_INT);
  frg_assert_int_non_negative(indentation);

  frg_stream_output_write_printf(mut_stream,
                                 "%c%u",
                                 (((const frg_ast_node_type_int_t*)node)->flags
                                  & FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED)
                                     != 0
                                   ? 'u'
                                   : 'i',
                                 ((const frg_ast_node_type_int_t*)node)->bit_width);
}

void frg_formatting_formatted_printer_type_float(frg_stream_output_t* mut_stream,
                                                 const frg_ast_node_t* node,
                                                 frg_indentation_t indentation) {
  frg_assert_pointer_non_null(mut_stream);
  frg_assert_pointer_non_null(node);
  frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_TYPE_FLOAT);
  frg_assert_int_non_negative(indentation);

  frg_stream_output_write_printf(
    mut_stream, "f%u", ((const frg_ast_node_type_float_t*)node)->bit_width);
}

void frg_formatting_formatted_printer_type_symbol(frg_stream_output_t* mut_stream,
                                                  const frg_ast_node_t* node,
                                                  frg_indentation_t indentation) {
  frg_assert_pointer_non_null(mut_stream);
  frg_assert_pointer_non_null(node);
  frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_TYPE_SYMBOL);
  frg_assert_int_non_negative(indentation);

  frg_assert_gstring_non_empty(((const frg_ast_node_type_symbol_t*)node)->name);
  frg_stream_output_write_string(mut_stream,
                                 ((const frg_ast_node_type_symbol_t*)node)->name->str);
}

void frg_formatting_formatted_printer_type_pointer(frg_stream_output_t* mut_stream,
                                                   const frg_ast_node_t* node,
                                                   frg_indentation_t indentation) {
  frg_assert_pointer_non_null(mut_stream);
  frg_assert_pointer_non_null(node);
  frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_TYPE_POINTER);
  frg_assert_int_non_negative(indentation);

  if (((const frg_ast_node_type_pointer_t*)node)->flags
      & FRG_AST_NODE_TYPE_POINTER_FLAG_IMPLICIT_DEREFERENCE) {
    frg_stream_output_write_character(mut_stream, '&');
  } else {
    frg_stream_output_write_character(mut_stream, '*');
  }

  if (((const frg_ast_node_type_pointer_t*)node)->flags
      & FRG_AST_NODE_TYPE_POINTER_FLAG_CONSTANT) {
    frg_stream_output_write_string(mut_stream, "const ");
  }

  frg_formatting_print_formatted(
    mut_stream, ((const frg_ast_node_type_pointer_t*)node)->value, indentation);
}

void frg_formatting_formatted_printer_type_array(frg_stream_output_t* mut_stream,
                                                 const frg_ast_node_t* node,
                                                 frg_indentation_t indentation) {
  frg_assert_pointer_non_null(mut_stream);
  frg_assert_pointer_non_null(node);
  frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_TYPE_ARRAY);
  frg_assert_int_non_negative(indentation);

  frg_stream_output_write_printf(
    mut_stream, "[%lu: ", ((const frg_ast_node_type_array_t*)node)->length);

  frg_formatting_print_formatted(
    mut_stream, ((const frg_ast_node_type_array_t*)node)->value, indentation);

  frg_stream_output_write_character(mut_stream, ']');
}

void frg_formatting_formatted_printer_type_function(frg_stream_output_t* mut_stream,
                                                    const frg_ast_node_t* node,
                                                    frg_indentation_t indentation) {
  frg_assert_pointer_non_null(mut_stream);
  frg_assert_pointer_non_null(node);
  frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_TYPE_FUNCTION);
  frg_assert_int_non_negative(indentation);

  frg_assert_pointer_null(
    ((const frg_ast_node_type_function_t*)node)->variadic_positional_arguments);
  frg_assert_pointer_null(
    ((const frg_ast_node_type_function_t*)node)->variadic_keyword_arguments);
  frg_assert_pointer_non_null(((const frg_ast_node_type_function_t*)node)->return_type);

  frg_stream_output_write_character(mut_stream, '(');

  for (GList* iter = ((const frg_ast_node_type_function_t*)node)->arguments;
       iter != NULL;
       iter = iter->next) {
    frg_formatting_print_formatted(mut_stream, iter->data, indentation);

    if (iter->next != NULL) {
      frg_stream_output_write_string(mut_stream, ", ");
    }
  }

  frg_stream_output_write_string(mut_stream, ") -> ");

  frg_formatting_print_formatted(
    mut_stream, ((const frg_ast_node_type_function_t*)node)->return_type, indentation);
}

void frg_formatting_formatted_printer_declaration_property(
  frg_stream_output_t* mut_stream,
  const frg_ast_node_t* node,
  frg_indentation_t indentation) {
  frg_assert_pointer_non_null(mut_stream);
  frg_assert_pointer_non_null(node);
  frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_DECLARATION_PROPERTY);
  frg_assert_int_non_negative(indentation);

  if ((((const frg_ast_node_declaration_property_t*)node)->flags
       & FRG_AST_NODE_DECLARATION_PROPERTY_FLAG_SPREAD)
      != 0) {
    frg_stream_output_write_string(mut_stream, "...");
  }

  frg_stream_output_write_string(
    mut_stream, ((const frg_ast_node_declaration_property_t*)node)->name->str);

  if ((((const frg_ast_node_declaration_property_t*)node)->flags
       & FRG_AST_NODE_DECLARATION_PROPERTY_FLAG_OPTIONAL)
      != 0) {
    frg_stream_output_write_character(mut_stream, '?');
  } else if ((((const frg_ast_node_declaration_property_t*)node)->flags
              & FRG_AST_NODE_DECLARATION_PROPERTY_FLAG_NON_OPTIONAL)
             != 0) {
    frg_stream_output_write_character(mut_stream, '!');
  }

  if (((const frg_ast_node_declaration_property_t*)node)->type != NULL) {
    frg_stream_output_write_string(mut_stream, ": ");
    frg_formatting_print_formatted(
      mut_stream,
      ((const frg_ast_node_declaration_property_t*)node)->type,
      indentation);
  }
}

void frg_formatting_formatted_printer_declaration_function_argument(
  frg_stream_output_t* mut_stream,
  const frg_ast_node_t* node,
  frg_indentation_t indentation) {
  frg_assert_pointer_non_null(mut_stream);
  frg_assert_pointer_non_null(node);
  frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_DECLARATION_FUNCTION_ARGUMENT);
  frg_assert_int_non_negative(indentation);

  if ((((frg_ast_node_declaration_function_argument_t*)node)->flags
       & FRG_AST_NODE_DECLARATION_FUNCTION_ARGUMENT_FLAG_KEYWORD)
      != 0) {
    frg_stream_output_write_string(mut_stream, "kw ");
  }

  frg_formatting_print_formatted(
    mut_stream,
    (frg_ast_node_t*)((frg_ast_node_declaration_function_argument_t*)node)->property,
    indentation);

  if (((frg_ast_node_declaration_function_argument_t*)node)->default_value != NULL) {
    frg_stream_output_write_string(mut_stream, " = ");
    frg_formatting_print_formatted(
      mut_stream,
      ((frg_ast_node_declaration_function_argument_t*)node)->default_value,
      indentation);
  }
}
