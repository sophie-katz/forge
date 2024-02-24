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

#include <forge/ast/debug_printers.h>
#include <forge/assert.h>
#include <forge/debug.h>
#include <forge/ast/utilities.h>
#include <forge/ast/node_kind_info.h>
#include <forge/enum_printing.h>

void _frg_ast_print_debug_list(frg_stream_output_t* mut_stream, const GList* list, const char* label, const frg_ast_print_debug_options_t* options) {
    frg_ast_node_count_t index = 0;

    frg_ast_print_debug_options_t options_next = frg_ast_print_debug_options_get_next(
        options,
        FRG_DEBUG_INDENTATION_WIDTH
    );

    for (const GList* iter = list; iter != NULL; iter = iter->next) {
        if (index >= options->max_list_length) {
            frg_debug_print_newline(mut_stream, options->indentation);
            frg_debug_print_property_with_index(mut_stream, label, index, "...");
            break;
        }

        frg_debug_print_newline(mut_stream, options->indentation);
        frg_debug_print_property_with_index(
            mut_stream,
            label,
            index,
            NULL
        );

        frg_ast_print_debug(mut_stream, (const frg_ast_node_t*)iter->data, &options_next);

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

    frg_debug_print_newline(mut_stream, options->indentation);
    frg_debug_print_property(mut_stream, "flags", NULL);
    frg_ast_node_type_int_flags_print(mut_stream, ((const frg_ast_node_type_int_t*)node)->flags);

    frg_debug_print_newline(mut_stream, options->indentation);
    frg_debug_print_property(mut_stream, "bit-width", "%u", ((const frg_ast_node_type_int_t*)node)->bit_width);
}

void frg_ast_debug_printer_type_float(frg_stream_output_t* mut_stream,
    const frg_ast_node_t* node,
    const frg_ast_print_debug_options_t* options) {
    frg_assert_pointer_non_null(mut_stream);
    frg_assert_pointer_non_null(node);
    frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_TYPE_FLOAT);
    frg_assert_int_non_negative(options->indentation);

    frg_debug_print_newline(mut_stream, options->indentation);
    frg_debug_print_property(mut_stream, "bit-width", "%u", ((const frg_ast_node_type_float_t*)node)->bit_width);
}

void frg_ast_debug_printer_type_symbol(frg_stream_output_t* mut_stream,
    const frg_ast_node_t* node,
    const frg_ast_print_debug_options_t* options) {
    frg_assert_pointer_non_null(mut_stream);
    frg_assert_pointer_non_null(node);
    frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_TYPE_SYMBOL);
    frg_assert_int_non_negative(options->indentation);

    frg_debug_print_newline(mut_stream, options->indentation);
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

    frg_ast_print_debug_options_t options_next = frg_ast_print_debug_options_get_next(
        options,
        FRG_DEBUG_INDENTATION_WIDTH
    );

    frg_debug_print_newline(mut_stream, options->indentation);
    frg_ast_print_debug(mut_stream, ((const frg_ast_node_type_pointer_t*)node)->value, &options_next);
}

void frg_ast_debug_printer_type_function(frg_stream_output_t* mut_stream,
    const frg_ast_node_t* node,
    const frg_ast_print_debug_options_t* options) {
    frg_assert_pointer_non_null(mut_stream);
    frg_assert_pointer_non_null(node);
    frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_TYPE_FUNCTION);
    frg_assert_int_non_negative(options->indentation);

    frg_ast_print_debug_options_t options_next = frg_ast_print_debug_options_get_next(
        options,
        FRG_DEBUG_INDENTATION_WIDTH
    );

    _frg_ast_print_debug_list(
        mut_stream,
        ((const frg_ast_node_type_function_t*)node)->arguments,
        "arguments",
        &options_next
    );

    frg_debug_print_newline(mut_stream, options->indentation);
    frg_debug_print_property(mut_stream, "variadic-positional-arguments", NULL);
    frg_ast_print_debug(mut_stream, ((const frg_ast_node_type_function_t*)node)->variadic_positional_arguments, &options_next);

    frg_debug_print_newline(mut_stream, options->indentation);
    frg_debug_print_property(mut_stream, "variadic-keyword-arguments", NULL);
    frg_ast_print_debug(mut_stream, ((const frg_ast_node_type_function_t*)node)->variadic_keyword_arguments, &options_next);

    frg_debug_print_newline(mut_stream, options->indentation);
    frg_debug_print_property(mut_stream, "return-type", NULL);
    frg_ast_print_debug(mut_stream, ((const frg_ast_node_type_function_t*)node)->return_type, &options_next);
}

void frg_ast_debug_printer_declaration_union(frg_stream_output_t* mut_stream,
    const frg_ast_node_t* node,
    const frg_ast_print_debug_options_t* options) {
    frg_assert_pointer_non_null(mut_stream);
    frg_assert_pointer_non_null(node);
    frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_DECLARATION_UNION);
    frg_assert_int_non_negative(options->indentation);

    frg_ast_print_debug_options_t options_next = frg_ast_print_debug_options_get_next(
        options,
        FRG_DEBUG_INDENTATION_WIDTH
    );

    frg_debug_print_newline(mut_stream, options->indentation);
    frg_debug_print_property(mut_stream, "name", NULL);
    frg_print_string(mut_stream, ((const frg_ast_node_declaration_union_t*)node)->name->str);

    _frg_ast_print_debug_list(
        mut_stream,
        ((const frg_ast_node_declaration_union_t*)node)->properties,
        "properties",
        &options_next
    );
}

void frg_ast_debug_printer_declaration_structure(frg_stream_output_t* mut_stream,
    const frg_ast_node_t* node,
    const frg_ast_print_debug_options_t* options) {
    frg_assert_pointer_non_null(mut_stream);
    frg_assert_pointer_non_null(node);
    frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_DECLARATION_STRUCTURE);
    frg_assert_int_non_negative(options->indentation);

    frg_ast_print_debug_options_t options_next = frg_ast_print_debug_options_get_next(
        options,
        FRG_DEBUG_INDENTATION_WIDTH
    );

    frg_debug_print_newline(mut_stream, options->indentation);
    frg_debug_print_property(mut_stream, "name", NULL);
    frg_print_string(mut_stream, ((const frg_ast_node_declaration_structure_t*)node)->name->str);

    _frg_ast_print_debug_list(
        mut_stream,
        ((const frg_ast_node_declaration_structure_t*)node)->declarations,
        "declarations",
        &options_next
    );
}

void frg_ast_debug_printer_declaration_property(frg_stream_output_t* mut_stream,
    const frg_ast_node_t* node,
    const frg_ast_print_debug_options_t* options) {
    frg_assert_pointer_non_null(mut_stream);
    frg_assert_pointer_non_null(node);
    frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_DECLARATION_PROPERTY);
    frg_assert_int_non_negative(options->indentation);

    frg_ast_print_debug_options_t options_next = frg_ast_print_debug_options_get_next(
        options,
        FRG_DEBUG_INDENTATION_WIDTH
    );

    frg_debug_print_newline(mut_stream, options->indentation);
    frg_debug_print_property(mut_stream, "flags", NULL);
    frg_ast_node_declaration_property_flags_print(mut_stream, ((const frg_ast_node_declaration_property_t*)node)->flags);

    frg_debug_print_newline(mut_stream, options->indentation);
    frg_debug_print_property(mut_stream, "name", NULL);
    frg_print_string(mut_stream, ((const frg_ast_node_declaration_property_t*)node)->name->str);

    frg_debug_print_newline(mut_stream, options->indentation);
    frg_debug_print_property(mut_stream, "type", NULL);
    frg_ast_print_debug(mut_stream, ((const frg_ast_node_declaration_property_t*)node)->type, &options_next);
}

void frg_ast_debug_printer_declaration_interface(frg_stream_output_t* mut_stream,
    const frg_ast_node_t* node,
    const frg_ast_print_debug_options_t* options) {
    frg_assert_pointer_non_null(mut_stream);
    frg_assert_pointer_non_null(node);
    frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_DECLARATION_INTERFACE);
    frg_assert_int_non_negative(options->indentation);

    frg_ast_print_debug_options_t options_next = frg_ast_print_debug_options_get_next(
        options,
        FRG_DEBUG_INDENTATION_WIDTH
    );

    frg_debug_print_newline(mut_stream, options->indentation);
    frg_debug_print_property(mut_stream, "flags", NULL);
    frg_ast_node_declaration_interface_flags_print(mut_stream, ((const frg_ast_node_declaration_interface_t*)node)->flags);

    frg_debug_print_newline(mut_stream, options->indentation);
    frg_debug_print_property(mut_stream, "name", NULL);
    frg_print_string(mut_stream, ((const frg_ast_node_declaration_interface_t*)node)->name->str);

    _frg_ast_print_debug_list(
        mut_stream,
        ((const frg_ast_node_declaration_interface_t*)node)->extends,
        "extends",
        &options_next
    );

    _frg_ast_print_debug_list(
        mut_stream,
        ((const frg_ast_node_declaration_interface_t*)node)->declarations,
        "declarations",
        &options_next
    );
}

void frg_ast_debug_printer_declaration_function_argument(frg_stream_output_t* mut_stream,
    const frg_ast_node_t* node,
    const frg_ast_print_debug_options_t* options) {
    frg_assert_pointer_non_null(mut_stream);
    frg_assert_pointer_non_null(node);
    frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_DECLARATION_FUNCTION_ARGUMENT);
    frg_assert_int_non_negative(options->indentation);

    frg_ast_print_debug_options_t options_next = frg_ast_print_debug_options_get_next(
        options,
        FRG_DEBUG_INDENTATION_WIDTH
    );

    frg_debug_print_newline(mut_stream, options->indentation);
            
    frg_debug_print_property(mut_stream, "flags", NULL);
    frg_ast_node_declaration_function_argument_flags_print(mut_stream, ((const frg_ast_node_declaration_function_argument_t*)node)->flags);

    frg_debug_print_newline(mut_stream, options->indentation);
    frg_debug_print_property(mut_stream, "property", NULL);
    frg_ast_print_debug(mut_stream, (const frg_ast_node_t*)((const frg_ast_node_declaration_function_argument_t*)node)->property, &options_next);

    frg_debug_print_newline(mut_stream, options->indentation);
    frg_debug_print_property(mut_stream, "default-value", NULL);
    frg_ast_print_debug(mut_stream, ((const frg_ast_node_declaration_function_argument_t*)node)->default_value, &options_next);
}

void frg_ast_debug_printer_declaration_function(frg_stream_output_t* mut_stream,
    const frg_ast_node_t* node,
    const frg_ast_print_debug_options_t* options) {
    frg_assert_pointer_non_null(mut_stream);
    frg_assert_pointer_non_null(node);
    frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_DECLARATION_FUNCTION);
    frg_assert_int_non_negative(options->indentation);

    frg_ast_print_debug_options_t options_next = frg_ast_print_debug_options_get_next(
        options,
        FRG_DEBUG_INDENTATION_WIDTH
    );

    frg_debug_print_newline(mut_stream, options->indentation);
    frg_debug_print_property(mut_stream, "flags", NULL);
    frg_ast_node_declaration_function_flags_print(mut_stream, ((const frg_ast_node_declaration_function_t*)node)->flags);

    frg_debug_print_newline(mut_stream, options->indentation);
    frg_debug_print_property(mut_stream, "name", NULL);
    frg_print_string(mut_stream, ((const frg_ast_node_declaration_function_t*)node)->name->str);

    frg_debug_print_newline(mut_stream, options->indentation);
    frg_debug_print_property(mut_stream, "type", NULL);
    frg_ast_print_debug(mut_stream, (const frg_ast_node_t*)((const frg_ast_node_declaration_function_t*)node)->type, &options_next);

    frg_debug_print_newline(mut_stream, options->indentation);
    frg_debug_print_property(mut_stream, "body", NULL);
    frg_ast_print_debug(mut_stream, ((const frg_ast_node_declaration_function_t*)node)->body, &options_next);
}

void frg_ast_debug_printer_declaration_variable(frg_stream_output_t* mut_stream,
    const frg_ast_node_t* node,
    const frg_ast_print_debug_options_t* options) {
    frg_assert_pointer_non_null(mut_stream);
    frg_assert_pointer_non_null(node);
    frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_DECLARATION_VARIABLE);
    frg_assert_int_non_negative(options->indentation);

    frg_ast_print_debug_options_t options_next = frg_ast_print_debug_options_get_next(
        options,
        FRG_DEBUG_INDENTATION_WIDTH
    );

    frg_debug_print_newline(mut_stream, options->indentation);
    frg_debug_print_property(mut_stream, "property", NULL);
    frg_ast_print_debug(mut_stream, (const frg_ast_node_t*)((const frg_ast_node_declaration_variable_t*)node)->property, &options_next);

    frg_debug_print_newline(mut_stream, options->indentation);
    frg_debug_print_property(mut_stream, "initial-value", NULL);
    frg_ast_print_debug(mut_stream, ((const frg_ast_node_declaration_variable_t*)node)->initial_value, &options_next);
}

void frg_ast_debug_printer_declaration_block(frg_stream_output_t* mut_stream,
    const frg_ast_node_t* node,
    const frg_ast_print_debug_options_t* options) {
    frg_assert_pointer_non_null(mut_stream);
    frg_assert_pointer_non_null(node);
    frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_DECLARATION_BLOCK);
    frg_assert_int_non_negative(options->indentation);

    frg_ast_print_debug_options_t options_next = frg_ast_print_debug_options_get_next(
        options,
        FRG_DEBUG_INDENTATION_WIDTH
    );

    _frg_ast_print_debug_list(
        mut_stream,
        ((const frg_ast_node_declaration_block_t*)node)->declarations,
        "declarations",
        &options_next
    );
}

void frg_ast_debug_printer_statement_return(frg_stream_output_t* mut_stream,
    const frg_ast_node_t* node,
    const frg_ast_print_debug_options_t* options) {
    frg_assert_pointer_non_null(mut_stream);
    frg_assert_pointer_non_null(node);
    frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_STATEMENT_RETURN);
    frg_assert_int_non_negative(options->indentation);

    frg_ast_print_debug_options_t options_next = frg_ast_print_debug_options_get_next(
        options,
        FRG_DEBUG_INDENTATION_WIDTH
    );

    frg_debug_print_newline(mut_stream, options->indentation);
    frg_debug_print_property(mut_stream, "value", NULL);
    frg_ast_print_debug(mut_stream, ((const frg_ast_node_statement_return_t*)node)->value, &options_next);
}

void frg_ast_debug_printer_statement_if(frg_stream_output_t* mut_stream,
    const frg_ast_node_t* node,
    const frg_ast_print_debug_options_t* options) {
    frg_assert_pointer_non_null(mut_stream);
    frg_assert_pointer_non_null(node);
    frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_STATEMENT_IF);
    frg_assert_int_non_negative(options->indentation);

    frg_ast_print_debug_options_t options_next = frg_ast_print_debug_options_get_next(
        options,
        FRG_DEBUG_INDENTATION_WIDTH
    );

    frg_debug_print_newline(mut_stream, options->indentation);
    frg_debug_print_property(mut_stream, "condition", NULL);
    frg_ast_print_debug(mut_stream, ((const frg_ast_node_statement_if_t*)node)->condition, &options_next);

    frg_debug_print_newline(mut_stream, options->indentation);
    frg_debug_print_property(mut_stream, "then-clause", NULL);
    frg_ast_print_debug(mut_stream, ((const frg_ast_node_statement_if_t*)node)->then_clause, &options_next);

    frg_debug_print_newline(mut_stream, options->indentation);
    frg_debug_print_property(mut_stream, "else-clause", NULL);
    frg_ast_print_debug(mut_stream, ((const frg_ast_node_statement_if_t*)node)->else_clause, &options_next);
}

void frg_ast_debug_printer_statement_while(frg_stream_output_t* mut_stream,
    const frg_ast_node_t* node,
    const frg_ast_print_debug_options_t* options) {
    frg_assert_pointer_non_null(mut_stream);
    frg_assert_pointer_non_null(node);
    frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_STATEMENT_WHILE);
    frg_assert_int_non_negative(options->indentation);

    frg_ast_print_debug_options_t options_next = frg_ast_print_debug_options_get_next(
        options,
        FRG_DEBUG_INDENTATION_WIDTH
    );

    frg_debug_print_newline(mut_stream, options->indentation);
    frg_debug_print_property(mut_stream, "condition", NULL);
    frg_ast_print_debug(mut_stream, ((const frg_ast_node_statement_while_t*)node)->condition, &options_next);

    frg_debug_print_newline(mut_stream, options->indentation);
    frg_debug_print_property(mut_stream, "body", NULL);
    frg_ast_print_debug(mut_stream, ((const frg_ast_node_statement_while_t*)node)->body, &options_next);
}

void frg_ast_debug_printer_statement_block(frg_stream_output_t* mut_stream,
    const frg_ast_node_t* node,
    const frg_ast_print_debug_options_t* options) {
    frg_assert_pointer_non_null(mut_stream);
    frg_assert_pointer_non_null(node);
    frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_STATEMENT_BLOCK);
    frg_assert_int_non_negative(options->indentation);

    frg_ast_print_debug_options_t options_next = frg_ast_print_debug_options_get_next(
        options,
        0
    );

    _frg_ast_print_debug_list(
        mut_stream,
        ((const frg_ast_node_statement_block_t*)node)->statements,
        "statements",
        &options_next
    );
}

void frg_ast_debug_printer_value_bool(
    frg_stream_output_t* mut_stream,
    const frg_ast_node_t* node,
    const frg_ast_print_debug_options_t* options
) {
    frg_assert_pointer_non_null(mut_stream);
    frg_assert_pointer_non_null(node);
    frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_VALUE_BOOL);
    frg_assert_int_non_negative(options->indentation);

    frg_debug_print_newline(mut_stream, options->indentation);
    frg_debug_print_property(mut_stream, "value", "%s", ((const frg_ast_node_value_bool_t*)node)->value ? "true" : "false");
}

void frg_ast_debug_printer_value_int(frg_stream_output_t* mut_stream,
    const frg_ast_node_t* node,
    const frg_ast_print_debug_options_t* options) {
    frg_assert_pointer_non_null(mut_stream);
    frg_assert_pointer_non_null(node);
    frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_VALUE_INT);
    frg_assert_int_non_negative(options->indentation);

    frg_ast_print_debug_options_t options_next = frg_ast_print_debug_options_get_next(
        options,
        FRG_DEBUG_INDENTATION_WIDTH
    );

    frg_debug_print_newline(mut_stream, options->indentation);
    frg_debug_print_property(mut_stream, "type", NULL);
    frg_ast_print_debug(mut_stream, (const frg_ast_node_t*)&((const frg_ast_node_value_int_t*)node)->type, &options_next);

    frg_debug_print_newline(mut_stream, options->indentation);
    if ((((const frg_ast_node_value_int_t*)node)->type.flags & FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED) == 0) {
        switch (((const frg_ast_node_value_int_t*)node)->type.bit_width) {
            case 8:
                frg_debug_print_property(mut_stream, "value", "%ii8", ((const frg_ast_node_value_int_t*)node)->value.as_i8);
                break;
            case 16:
                frg_debug_print_property(mut_stream, "value", "%ii16", ((const frg_ast_node_value_int_t*)node)->value.as_i16);
                break;
            case 32:
                frg_debug_print_property(mut_stream, "value", "%ii32", ((const frg_ast_node_value_int_t*)node)->value.as_i32);
                break;
            case 64:
                frg_debug_print_property(mut_stream, "value", "%lii64", ((const frg_ast_node_value_int_t*)node)->value.as_i64);
                break;
            default:
                frg_debug_print_property(mut_stream, "value", "%li (defaulting to i64 since bit width %u is not supported)", ((const frg_ast_node_value_int_t*)node)->value.as_i64, ((const frg_ast_node_value_int_t*)node)->type.bit_width);
                break;
        }
    } else {
        switch (((const frg_ast_node_value_int_t*)node)->type.bit_width) {
            case 8:
                frg_debug_print_property(mut_stream, "value", "%uu8", ((const frg_ast_node_value_int_t*)node)->value.as_u8);
                break;
            case 16:
                frg_debug_print_property(mut_stream, "value", "%uu16", ((const frg_ast_node_value_int_t*)node)->value.as_u16);
                break;
            case 32:
                frg_debug_print_property(mut_stream, "value", "%uu32", ((const frg_ast_node_value_int_t*)node)->value.as_u32);
                break;
            case 64:
                frg_debug_print_property(mut_stream, "value", "%luu64", ((const frg_ast_node_value_int_t*)node)->value.as_u64);
                break;
            default:
                frg_debug_print_property(mut_stream, "value", "%lu (defaulting to u64 since bit width %u is not supported)", ((const frg_ast_node_value_int_t*)node)->value.as_u64, ((const frg_ast_node_value_int_t*)node)->type.bit_width);
                break;
        }
    }
}

void frg_ast_debug_printer_value_float(frg_stream_output_t* mut_stream,
    const frg_ast_node_t* node,
    const frg_ast_print_debug_options_t* options) {
    frg_assert_pointer_non_null(mut_stream);
    frg_assert_pointer_non_null(node);
    frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_VALUE_FLOAT);
    frg_assert_int_non_negative(options->indentation);

    frg_ast_print_debug_options_t options_next = frg_ast_print_debug_options_get_next(
        options,
        FRG_DEBUG_INDENTATION_WIDTH
    );

    frg_debug_print_newline(mut_stream, options->indentation);
    frg_debug_print_property(mut_stream, "type", NULL);
    frg_ast_print_debug(mut_stream, (const frg_ast_node_t*)&((const frg_ast_node_value_float_t*)node)->type, &options_next);

    frg_debug_print_newline(mut_stream, options->indentation);
    switch (((const frg_ast_node_value_float_t*)node)->type.bit_width) {
        case 32:
            frg_debug_print_property(mut_stream, "value", "%ff32", ((const frg_ast_node_value_float_t*)node)->value.as_f32);
            break;
        case 64:
            frg_debug_print_property(mut_stream, "value", "%lff64", ((const frg_ast_node_value_float_t*)node)->value.as_f64);
            break;
        default:
            frg_debug_print_property(mut_stream, "value", "%lf (defaulting to f64 since bit width %u is not supported)", ((const frg_ast_node_value_float_t*)node)->value.as_f64, ((const frg_ast_node_value_float_t*)node)->type.bit_width);
            break;
    }
}

void frg_ast_debug_printer_value_character(frg_stream_output_t* mut_stream,
    const frg_ast_node_t* node,
    const frg_ast_print_debug_options_t* options) {
    frg_assert_pointer_non_null(mut_stream);
    frg_assert_pointer_non_null(node);
    frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_VALUE_CHARACTER);
    frg_assert_int_non_negative(options->indentation);

    frg_debug_print_newline(mut_stream, options->indentation);
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

    frg_debug_print_newline(mut_stream, options->indentation);
    frg_debug_print_property(mut_stream, "value", NULL);
    frg_print_string(mut_stream, ((frg_ast_node_value_string_t*)node)->value->str);
}

void frg_ast_debug_printer_value_symbol(frg_stream_output_t* mut_stream,
    const frg_ast_node_t* node,
    const frg_ast_print_debug_options_t* options) {
    frg_assert_pointer_non_null(mut_stream);
    frg_assert_pointer_non_null(node);
    frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_VALUE_SYMBOL);
    frg_assert_int_non_negative(options->indentation);

    frg_debug_print_newline(mut_stream, options->indentation);
    frg_debug_print_property(mut_stream, "name", NULL);
    frg_print_string(mut_stream, ((frg_ast_node_value_symbol_t*)node)->name->str);
}

void frg_ast_debug_printer_value_call_keyword_argument(frg_stream_output_t* mut_stream,
    const frg_ast_node_t* node,
    const frg_ast_print_debug_options_t* options) {
    frg_assert_pointer_non_null(mut_stream);
    frg_assert_pointer_non_null(node);
    frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_VALUE_CALL_KEYWORD_ARGUMENT);
    frg_assert_int_non_negative(options->indentation);

    frg_ast_print_debug_options_t options_next = frg_ast_print_debug_options_get_next(
        options,
        FRG_DEBUG_INDENTATION_WIDTH
    );

    frg_debug_print_newline(mut_stream, options->indentation);
    frg_debug_print_property(mut_stream, "name", NULL);
    frg_print_string(mut_stream, ((frg_ast_node_value_call_keyword_argument_t*)node)->name->str);

    frg_debug_print_newline(mut_stream, options->indentation);
    frg_debug_print_property(mut_stream, "value", NULL);
    frg_ast_print_debug(mut_stream, ((frg_ast_node_value_call_keyword_argument_t*)node)->value, &options_next);
}

void frg_ast_debug_printer_value_call(frg_stream_output_t* mut_stream,
    const frg_ast_node_t* node,
    const frg_ast_print_debug_options_t* options) {
    frg_assert_pointer_non_null(mut_stream);
    frg_assert_pointer_non_null(node);
    frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_VALUE_CALL);
    frg_assert_int_non_negative(options->indentation);

    frg_ast_print_debug_options_t options_next = frg_ast_print_debug_options_get_next(
        options,
        FRG_DEBUG_INDENTATION_WIDTH
    );

    frg_debug_print_newline(mut_stream, options->indentation);
    frg_debug_print_property(mut_stream, "callee", NULL);
    frg_ast_print_debug(mut_stream, ((frg_ast_node_value_call_t*)node)->callee, &options_next);

    _frg_ast_print_debug_list(
        mut_stream,
        ((frg_ast_node_value_call_t*)node)->arguments,
        "arguments",
        &options_next
    );

    _frg_ast_print_debug_list(
        mut_stream,
        ((frg_ast_node_value_call_t*)node)->keyword_arguments,
        "keyword-arguments",
        &options_next
    );
}

void frg_ast_debug_printer_value_unary(frg_stream_output_t* mut_stream,
    const frg_ast_node_t* node,
    const frg_ast_print_debug_options_t* options) {
    frg_assert_pointer_non_null(mut_stream);
    frg_assert_pointer_non_null(node);
    frg_assert(frg_ast_node_kind_info_get(node->kind)->flags & FRG_AST_NODE_KIND_FLAG_VALUE_UNARY);
    frg_assert_int_non_negative(options->indentation);

    frg_ast_print_debug_options_t options_next = frg_ast_print_debug_options_get_next(
        options,
        FRG_DEBUG_INDENTATION_WIDTH
    );

    frg_debug_print_newline(mut_stream, options->indentation);
    frg_debug_print_property(mut_stream, "operand", NULL);
    frg_ast_print_debug(mut_stream, ((frg_ast_node_value_unary_t*)node)->operand, &options_next);
}

void frg_ast_debug_printer_value_binary(frg_stream_output_t* mut_stream,
    const frg_ast_node_t* node,
    const frg_ast_print_debug_options_t* options) {
    frg_assert_pointer_non_null(mut_stream);
    frg_assert_pointer_non_null(node);
    frg_assert(frg_ast_node_kind_info_get(node->kind)->flags & FRG_AST_NODE_KIND_FLAG_VALUE_BINARY);
    frg_assert_int_non_negative(options->indentation);

    frg_ast_print_debug_options_t options_next = frg_ast_print_debug_options_get_next(
        options,
        FRG_DEBUG_INDENTATION_WIDTH
    );

    frg_debug_print_newline(mut_stream, options->indentation);
    frg_debug_print_property(mut_stream, "left", NULL);
    frg_ast_print_debug(mut_stream, ((frg_ast_node_value_binary_t*)node)->left, &options_next);

    frg_debug_print_newline(mut_stream, options->indentation);
    frg_debug_print_property(mut_stream, "right", NULL);
    frg_ast_print_debug(mut_stream, ((frg_ast_node_value_binary_t*)node)->right, &options_next);
}
