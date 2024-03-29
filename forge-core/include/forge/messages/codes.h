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

// See docs/for-developers/guides/writing-messsage-codes.md for details on how to add to
// this file.

#pragma once

#include <forge/messages/message_buffer.h>

// Does FFS mean for fucks sake, facial feminization surgery, or fatal/filesystem?
// You'll never find out.

// TODO: Add assertions to these

#define frg_message_emit_ffs_1_open_for_writing(message_buffer, path, error_message)   \
  {                                                                                    \
    frg_assert_pointer_non_null(message_buffer);                                       \
    frg_assert_string_non_empty(path);                                                 \
                                                                                       \
    frg_message_t* __frg_ffs_1_message                                                 \
      = frg_message_emit((message_buffer),                                             \
                         FRG_MESSAGE_SEVERITY_FATAL_ERROR,                             \
                         "FFS-1",                                                      \
                         "Could not open '%s' for writing",                            \
                         (path));                                                      \
                                                                                       \
    if ((error_message) != NULL) {                                                     \
      frg_assert_string_non_empty(error_message);                                      \
                                                                                       \
      frg_message_emit_child((message_buffer),                                         \
                             __frg_ffs_1_message,                                      \
                             FRG_MESSAGE_SEVERITY_NOTE,                                \
                             NULL,                                                     \
                             "Underlying cause: %s",                                   \
                             (error_message));                                         \
    }                                                                                  \
  }

#define frg_message_emit_ffs_2_open_for_reading(message_buffer, path, error_message)   \
  {                                                                                    \
    frg_message_t* __frg_ffs_2_message                                                 \
      = frg_message_emit((message_buffer),                                             \
                         FRG_MESSAGE_SEVERITY_FATAL_ERROR,                             \
                         "FFS-2",                                                      \
                         "Could not open '%s' for reading",                            \
                         (path));                                                      \
                                                                                       \
    if ((error_message) != NULL) {                                                     \
      frg_message_emit_child((message_buffer),                                         \
                             __frg_ffs_2_message,                                      \
                             FRG_MESSAGE_SEVERITY_NOTE,                                \
                             NULL,                                                     \
                             "Underlying cause: %s",                                   \
                             (error_message));                                         \
    }                                                                                  \
  }

#define frg_message_emit_eft_1_unsupported_requirement(                                \
  message_buffer, source_range, requirement_number, requirement_title)                 \
  {                                                                                    \
    frg_message_t* __frg_eft_1_message = frg_message_emit_from_source_range(           \
      (message_buffer),                                                                \
      (source_range),                                                                  \
      FRG_MESSAGE_SEVERITY_ERROR,                                                      \
      "EFT-1",                                                                         \
      "Language feature is not yet supported ('%s')",                                  \
      (requirement_title));                                                            \
                                                                                       \
    frg_message_emit_child((message_buffer),                                           \
                           __frg_eft_1_message,                                        \
                           FRG_MESSAGE_SEVERITY_NOTE,                                  \
                           NULL,                                                       \
                           "It is described in Requirement-%i",                        \
                           (requirement_number));                                      \
  }

#define frg_message_emit_eft_2_unsupported_requirement_subitem(message_buffer,         \
                                                               source_range,           \
                                                               requirement_number,     \
                                                               requirement_title,      \
                                                               subitem_number,         \
                                                               subitem_title)          \
  {                                                                                    \
    frg_assert((subitem_title)[0] >= 'a' && (subitem_title)[0] <= 'z');                \
                                                                                       \
    frg_message_t* __frg_eft_2_message = frg_message_emit_from_source_range(           \
      (message_buffer),                                                                \
      (source_range),                                                                  \
      FRG_MESSAGE_SEVERITY_ERROR,                                                      \
      "EFT-2",                                                                         \
      "Language feature subitem is not yet supported ('%s' in '%s')",                  \
      (subitem_title),                                                                 \
      (requirement_title));                                                            \
                                                                                       \
    frg_message_emit_child((message_buffer),                                           \
                           __frg_eft_2_message,                                        \
                           FRG_MESSAGE_SEVERITY_NOTE,                                  \
                           NULL,                                                       \
                           "It is described in Requirement-%i.%i",                     \
                           (requirement_number),                                       \
                           (subitem_number));                                          \
  }

#define frg_message_emit_eft_3_unsupported_proposal(                                   \
  message_buffer, source_range, proposal_number, proposal_title)                       \
  {                                                                                    \
    frg_message_t* __frg_eft_3_message = frg_message_emit_from_source_range(           \
      (message_buffer),                                                                \
      (source_range),                                                                  \
      FRG_MESSAGE_SEVERITY_ERROR,                                                      \
      "EFT-3",                                                                         \
      "Language feature is not yet supported ('%s')",                                  \
      (proposal_title));                                                               \
                                                                                       \
    frg_message_emit_child((message_buffer),                                           \
                           __frg_eft_3_message,                                        \
                           FRG_MESSAGE_SEVERITY_NOTE,                                  \
                           NULL,                                                       \
                           "It is described in Proposal-%i",                           \
                           (proposal_number));                                         \
  }

#define frg_message_emit_eft_4_unsupported_proposal_subitem(message_buffer,            \
                                                            source_range,              \
                                                            proposal_number,           \
                                                            proposal_title,            \
                                                            subitem_number,            \
                                                            subitem_title)             \
  {                                                                                    \
    frg_message_t* __frg_eft_4_message = frg_message_emit_from_source_range(           \
      (message_buffer),                                                                \
      (source_range),                                                                  \
      FRG_MESSAGE_SEVERITY_ERROR,                                                      \
      "EFT-4",                                                                         \
      "Language feature is not yet supported ('%s' in '%s')",                          \
      (subitem_title),                                                                 \
      (proposal_title));                                                               \
                                                                                       \
    frg_message_emit_child((message_buffer),                                           \
                           __frg_eft_4_message,                                        \
                           FRG_MESSAGE_SEVERITY_NOTE,                                  \
                           NULL,                                                       \
                           "It is described in Proposal-%i.%i",                        \
                           (proposal_number),                                          \
                           (subitem_number));                                          \
  }

#define frg_message_emit_fc_1_argument_expects_value(                                  \
  message_buffer, argument, value_name)                                                \
  {                                                                                    \
    frg_message_t* __frg_fc_1_message                                                  \
      = frg_message_emit((message_buffer),                                             \
                         FRG_MESSAGE_SEVERITY_FATAL_ERROR,                             \
                         "FC-1",                                                       \
                         "Argument '%s <%s>' must have a value passed",                \
                         (argument),                                                   \
                         (value_name));                                                \
                                                                                       \
    frg_message_emit_child((message_buffer),                                           \
                           __frg_fc_1_message,                                         \
                           FRG_MESSAGE_SEVERITY_NOTE,                                  \
                           NULL,                                                       \
                           "Use '--help' for more details");                           \
  }

#define frg_message_emit_fc_2_argument_expects_value_not_argument(                     \
  message_buffer, argument, value_name, value)                                         \
  {                                                                                    \
    frg_message_t* __frg_fc_2_message = frg_message_emit(                              \
      (message_buffer),                                                                \
      FRG_MESSAGE_SEVERITY_FATAL_ERROR,                                                \
      "FC-2",                                                                          \
      "Argument '--%s <%s>' expects to have a value passed, but '%s' is an argument "  \
      "not a value",                                                                   \
      (argument),                                                                      \
      (value_name),                                                                    \
      (value));                                                                        \
                                                                                       \
    frg_message_emit_child((message_buffer),                                           \
                           __frg_fc_2_message,                                         \
                           FRG_MESSAGE_SEVERITY_NOTE,                                  \
                           NULL,                                                       \
                           "Use '--help' for more details");                           \
  }

#define frg_message_emit_fc_3_invalid_choice(message_buffer, argument, option)         \
  {                                                                                    \
    GString* __frg_fc_3_allowed_values = g_string_new("");                             \
                                                                                       \
    for (GList* __frg_fc_3_choice = (option)->choices; __frg_fc_3_choice != NULL;      \
         __frg_fc_3_choice        = __frg_fc_3_choice->next) {                                \
      const frg_cli_choice_t* __frg_fc_3_choice_casted                                 \
        = (const frg_cli_choice_t*)__frg_fc_3_choice->data;                            \
      g_string_append_printf(__frg_fc_3_allowed_values,                                \
                             "\n  '%s' - %s",                                          \
                             __frg_fc_3_choice_casted->name,                           \
                             __frg_fc_3_choice_casted->help);                          \
    }                                                                                  \
                                                                                       \
    frg_message_emit((message_buffer),                                                 \
                     FRG_MESSAGE_SEVERITY_FATAL_ERROR,                                 \
                     "FC-3",                                                           \
                     "Value for argument '--%s' must be one of:%s",                    \
                     (argument),                                                       \
                     __frg_fc_3_allowed_values->str);                                  \
                                                                                       \
    g_string_free(__frg_fc_3_allowed_values, TRUE);                                    \
  }

#define frg_message_emit_fc_4_expects_source_file(message_buffer)                      \
  frg_message_emit((message_buffer),                                                   \
                   FRG_MESSAGE_SEVERITY_FATAL_ERROR,                                   \
                   "FC-4",                                                             \
                   "Command expects a source file as an argument")

#define frg_message_emit_fc_5_expects_only_one_source_file(message_buffer, value)      \
  {                                                                                    \
    frg_message_t* __frg_fc_5_message                                                  \
      = frg_message_emit((message_buffer),                                             \
                         FRG_MESSAGE_SEVERITY_FATAL_ERROR,                             \
                         "FC-5",                                                       \
                         "Command expects only one source file as an argument");       \
                                                                                       \
    frg_message_emit_child((message_buffer),                                           \
                           __frg_fc_5_message,                                         \
                           FRG_MESSAGE_SEVERITY_NOTE,                                  \
                           NULL,                                                       \
                           "'%s' is one too many",                                     \
                           (value));                                                   \
  }

#define frg_message_emit_fc_6_expected_command(message_buffer, binary_name)            \
  {                                                                                    \
    frg_message_t* __frg_fc_6_message                                                  \
      = frg_message_emit((message_buffer),                                             \
                         FRG_MESSAGE_SEVERITY_FATAL_ERROR,                             \
                         "FC-6",                                                       \
                         "Program must be called with a subcommand");                  \
                                                                                       \
    frg_message_emit_child((message_buffer),                                           \
                           __frg_fc_6_message,                                         \
                           FRG_MESSAGE_SEVERITY_NOTE,                                  \
                           NULL,                                                       \
                           "Run '%s help' to see a list of available commands",        \
                           (binary_name));                                             \
  }

#define frg_message_emit_fc_7_invalid_argument_dash_dash(message_buffer)               \
  {                                                                                    \
    frg_message_t* __frg_fc_7_message                                                  \
      = frg_message_emit((message_buffer),                                             \
                         FRG_MESSAGE_SEVERITY_FATAL_ERROR,                             \
                         "FC-7",                                                       \
                         "Invalid argument '--'");                                     \
                                                                                       \
    frg_message_emit_child((message_buffer),                                           \
                           __frg_fc_7_message,                                         \
                           FRG_MESSAGE_SEVERITY_NOTE,                                  \
                           NULL,                                                       \
                           "Use '--help' to see a list of valid arguments");           \
  }

#define frg_message_emit_fc_8_unknown_short_argument(message_buffer, argument)         \
  {                                                                                    \
    frg_message_t* __frg_fc_8_message                                                  \
      = frg_message_emit((message_buffer),                                             \
                         FRG_MESSAGE_SEVERITY_FATAL_ERROR,                             \
                         "FC-8",                                                       \
                         "Unknown argument '-%c'",                                     \
                         (argument));                                                  \
                                                                                       \
    frg_message_emit_child((message_buffer),                                           \
                           __frg_fc_8_message,                                         \
                           FRG_MESSAGE_SEVERITY_NOTE,                                  \
                           NULL,                                                       \
                           "Use '--help' to see a list of available arguments");       \
  }

#define frg_message_emit_fc_9_unknown_long_argument(message_buffer, argument)          \
  {                                                                                    \
    frg_message_t* __frg_fc_9_message                                                  \
      = frg_message_emit((message_buffer),                                             \
                         FRG_MESSAGE_SEVERITY_FATAL_ERROR,                             \
                         "FC-9",                                                       \
                         "Unknown argument '--%s'",                                    \
                         (argument));                                                  \
    frg_message_emit_child((message_buffer),                                           \
                           __frg_fc_9_message,                                         \
                           FRG_MESSAGE_SEVERITY_NOTE,                                  \
                           NULL,                                                       \
                           "Use '--help' to see a list of available arguments");       \
  }

#define frg_message_emit_fc_10_long_argument_with_single_dash(message_buffer,          \
                                                              argument)                \
  {                                                                                    \
    frg_message_t* __frg_fc_10_message = frg_message_emit(                             \
      (message_buffer),                                                                \
      FRG_MESSAGE_SEVERITY_FATAL_ERROR,                                                \
      "FC-10",                                                                         \
      "Long arguments like '%s' must be prefixed with '--', not just a single '-'",    \
      (argument));                                                                     \
    frg_message_emit_child((message_buffer),                                           \
                           __frg_fc_10_message,                                        \
                           FRG_MESSAGE_SEVERITY_NOTE,                                  \
                           NULL,                                                       \
                           "Use '--help' to see a list of available arguments");       \
  }

#define frg_message_emit_fc_11_unknown_command(message_buffer, command, binary_name)   \
  {                                                                                    \
    frg_message_t* __frg_fc_11_message                                                 \
      = frg_message_emit((message_buffer),                                             \
                         FRG_MESSAGE_SEVERITY_FATAL_ERROR,                             \
                         "FC-11",                                                      \
                         "Unknown subcommand '%s'",                                    \
                         (command));                                                   \
                                                                                       \
    frg_message_emit_child((message_buffer),                                           \
                           __frg_fc_11_message,                                        \
                           FRG_MESSAGE_SEVERITY_NOTE,                                  \
                           NULL,                                                       \
                           "Run '%s help' to see a list of available commands",        \
                           (binary_name));                                             \
  }

#define frg_message_emit_fc_12_invalid_boolean_shell_environment_variable(             \
  message_buffer, variable_name, variable_value)                                       \
  {                                                                                    \
    frg_message_t* __frg_fc_12_message                                                 \
      = frg_message_emit((message_buffer),                                             \
                         FRG_MESSAGE_SEVERITY_FATAL_ERROR,                             \
                         "FC-12",                                                      \
                         "Invalid boolean value for environment variable '%s': '%s'",  \
                         (variable_name),                                              \
                         (variable_value));                                            \
                                                                                       \
    frg_message_emit_child((message_buffer),                                           \
                           __frg_fc_12_message,                                        \
                           FRG_MESSAGE_SEVERITY_NOTE,                                  \
                           NULL,                                                       \
                           "Value can be either 'true' or 'false'");                   \
  }

#define frg_message_emit_fc_13_unknown_color_mode(message_buffer, value)               \
  {                                                                                    \
    frg_message_t* __frg_fc_13_message                                                 \
      = frg_message_emit((message_buffer),                                             \
                         FRG_MESSAGE_SEVERITY_FATAL_ERROR,                             \
                         "FC-13",                                                      \
                         "Invalid color mode: '%s'",                                   \
                         (value));                                                     \
                                                                                       \
    frg_message_emit_child((message_buffer),                                           \
                           __frg_fc_13_message,                                        \
                           FRG_MESSAGE_SEVERITY_NOTE,                                  \
                           NULL,                                                       \
                           "Value can be 'disabled', 'auto', or 'enabled'");           \
  }

#define frg_message_emit_fc_14_unknown_unicode_mode(message_buffer, value)             \
  {                                                                                    \
    frg_message_t* __frg_fc_14_message                                                 \
      = frg_message_emit((message_buffer),                                             \
                         FRG_MESSAGE_SEVERITY_FATAL_ERROR,                             \
                         "FC-14",                                                      \
                         "Invalid unicode mode: '%s'",                                 \
                         (value));                                                     \
                                                                                       \
    frg_message_emit_child((message_buffer),                                           \
                           __frg_fc_14_message,                                        \
                           FRG_MESSAGE_SEVERITY_NOTE,                                  \
                           NULL,                                                       \
                           "Value can be 'disabled', 'auto', or 'enabled'");           \
  }

#define frg_message_emit_fc_15_missing_required_argument(                              \
  message_buffer, argument_long, argument_short)                                       \
  frg_message_emit((message_buffer),                                                   \
                   FRG_MESSAGE_SEVERITY_FATAL_ERROR,                                   \
                   "FC-15",                                                            \
                   "Missing required argument '--%s'%s%s%s",                           \
                   (argument_long),                                                    \
                   ((argument_short) == NULL) ? "" : " ('-",                           \
                   (argument_short) ? (argument_short) : "",                           \
                   ((argument_short) == NULL) ? "" : "')")

#define frg_message_emit_is_1_code_point_limit(                                        \
  message_buffer, source_range, code_point)                                            \
  frg_message_emit_from_source_range(                                                  \
    (message_buffer),                                                                  \
    (source_range),                                                                    \
    FRG_MESSAGE_SEVERITY_INTERNAL_ERROR,                                               \
    "IS-1",                                                                            \
    "Unicode code point specified exceeds 0xffffff (%Lx)",                             \
    (code_point))

// TODO: Add a source range to this
#define frg_message_emit_is_2_empty_literal(message_buffer)                            \
  frg_message_emit((message_buffer),                                                   \
                   FRG_MESSAGE_SEVERITY_INTERNAL_ERROR,                                \
                   "IS-2",                                                             \
                   "Cannot parse empty string as literal")

// TODO: Add a source range to this
#define frg_message_emit_is_3_literal_must_start_with_quote(message_buffer)            \
  frg_message_emit((message_buffer),                                                   \
                   FRG_MESSAGE_SEVERITY_INTERNAL_ERROR,                                \
                   "IS-3",                                                             \
                   "String or character literal must start with a quote")

#define frg_message_emit_is_4_float_literal_missing_dot(message_buffer, source_range)  \
  frg_message_emit_from_source_range(                                                  \
    (message_buffer),                                                                  \
    (source_range),                                                                    \
    FRG_MESSAGE_SEVERITY_INTERNAL_ERROR,                                               \
    "IS-4",                                                                            \
    "Cannot parse string without '.' as float literal")

#define frg_message_emit_is_5_ast_node_property_null(                                  \
  message_buffer, source_range, ast_node_kind, property_name)                          \
  frg_message_emit_from_source_range((message_buffer),                                 \
                                     (source_range),                                   \
                                     FRG_MESSAGE_SEVERITY_INTERNAL_ERROR,              \
                                     "IS-5",                                           \
                                     "AST node '%s' cannot have null '%s'",            \
                                     frg_ast_node_kind_info_get(ast_node_kind)->name,  \
                                     (property_name))

#define frg_message_emit_is_6_ast_node_property_empty_string(                          \
  message_buffer, source_range, ast_node_kind, property_name)                          \
  frg_message_emit_from_source_range(                                                  \
    (message_buffer),                                                                  \
    (source_range),                                                                    \
    FRG_MESSAGE_SEVERITY_INTERNAL_ERROR,                                               \
    "IS-6",                                                                            \
    "AST node '%s' cannot have '%s' that is an empty string",                          \
    frg_ast_node_kind_info_get(ast_node_kind)->name,                                   \
    (property_name))

#define frg_message_emit_is_7_invalid_symbol(                                          \
  message_buffer, source_range, ast_node_kind, property_name)                          \
  {                                                                                    \
    frg_message_t* __frg_is_7_message = frg_message_emit_from_source_range(            \
      (message_buffer),                                                                \
      (source_range),                                                                  \
      FRG_MESSAGE_SEVERITY_INTERNAL_ERROR,                                             \
      "IS-7",                                                                          \
      "Symbol '%s' is invalid",                                                        \
      frg_ast_node_kind_info_get(ast_node_kind)->name,                                 \
      (property_name));                                                                \
                                                                                       \
    frg_message_emit_child(                                                            \
      (message_buffer),                                                                \
      __frg_is_7_message,                                                              \
      FRG_MESSAGE_SEVERITY_NOTE,                                                       \
      NULL,                                                                            \
      "Symbols must start with an underscore or a letter and contain underscores, "    \
      "letters, and numbers");                                                         \
  }

#define frg_message_emit_is_8_ast_node_property_null_element(                          \
  message_buffer, source_range, ast_node_kind, property_name, index)                   \
  frg_message_emit_from_source_range(                                                  \
    (message_buffer),                                                                  \
    (source_range),                                                                    \
    FRG_MESSAGE_SEVERITY_INTERNAL_ERROR,                                               \
    "IS-8",                                                                            \
    "AST node '%s' property '%s' has invalid null element at index %i",                \
    frg_ast_node_kind_info_get(ast_node_kind)->name,                                   \
    (property_name),                                                                   \
    (index))

#define frg_message_emit_is_9_ast_node_property_unexpected_element(                    \
  message_buffer, source_range, ast_node_kind, property_name, index)                   \
  frg_message_emit_from_source_range(                                                  \
    (message_buffer),                                                                  \
    (source_range),                                                                    \
    FRG_MESSAGE_SEVERITY_INTERNAL_ERROR,                                               \
    "IS-9",                                                                            \
    "AST node '%s' property '%s' has element of unexpected AST kind at index %i",      \
    frg_ast_node_kind_info_get(ast_node_kind)->name,                                   \
    (property_name),                                                                   \
    (index))

#define frg_message_emit_es_1_unexpected_character(                                    \
  message_buffer, source_range, value)                                                 \
  frg_message_emit_from_source_range((message_buffer),                                 \
                                     (source_range),                                   \
                                     FRG_MESSAGE_SEVERITY_ERROR,                       \
                                     "ES-1",                                           \
                                     "Unexpected character '%c' (0x%02x)",             \
                                     (char)(value),                                    \
                                     ((int)(value)) & 0xff)

#define frg_message_emit_es_2_unexpected_byte(message_buffer, source_range, value)     \
  frg_message_emit_from_source_range((message_buffer),                                 \
                                     (source_range),                                   \
                                     FRG_MESSAGE_SEVERITY_ERROR,                       \
                                     "ES-2",                                           \
                                     "Unexpected byte 0x%02x",                         \
                                     ((int)(value)) & 0xff)

#define frg_message_emit_es_3_string_ends_with_backslash(message_buffer, source_range) \
  frg_message_emit_from_source_range(                                                  \
    (message_buffer),                                                                  \
    (source_range),                                                                    \
    FRG_MESSAGE_SEVERITY_ERROR,                                                        \
    "ES-3",                                                                            \
    "'\\' in string must have at least one character afterwards")

#define frg_message_emit_es_4_escape_x_expects_2_hex_digits(                           \
  message_buffer, source_range, digit_count)                                           \
  {                                                                                    \
    frg_assert_int_less_than((digit_count), 2);                                        \
                                                                                       \
    frg_message_t* __frg_es_4_message = frg_message_emit_from_source_range(            \
      (message_buffer),                                                                \
      (source_range),                                                                  \
      FRG_MESSAGE_SEVERITY_ERROR,                                                      \
      "ES-4",                                                                          \
      "'\\x' must be followed by exactly 2 hexadecimal digits (%s)",                   \
      (digit_count) == 1 ? "only found one" : "found none");                           \
                                                                                       \
    frg_message_emit_child((message_buffer),                                           \
                           __frg_es_4_message,                                         \
                           FRG_MESSAGE_SEVERITY_NOTE,                                  \
                           NULL,                                                       \
                           "For example: '\\x20'");                                    \
  }

#define frg_message_emit_es_5_escape_u_expects_opening_brace(message_buffer,           \
                                                             source_range)             \
  {                                                                                    \
    frg_message_t* __frg_es_5_message                                                  \
      = frg_message_emit_from_source_range((message_buffer),                           \
                                           (source_range),                             \
                                           FRG_MESSAGE_SEVERITY_ERROR,                 \
                                           "ES-5",                                     \
                                           "'\\u' must be followed by '{'");           \
                                                                                       \
    frg_message_emit_child((message_buffer),                                           \
                           __frg_es_5_message,                                         \
                           FRG_MESSAGE_SEVERITY_NOTE,                                  \
                           NULL,                                                       \
                           "For example: '\\u{20}'");                                  \
  }

#define frg_message_emit_es_6_escape_u_expects_closing_brace(message_buffer,           \
                                                             source_range)             \
  {                                                                                    \
    frg_message_t* __frg_es_6_message = frg_message_emit_from_source_range(            \
      (message_buffer),                                                                \
      (source_range),                                                                  \
      FRG_MESSAGE_SEVERITY_ERROR,                                                      \
      "ES-6",                                                                          \
      "'\\u{' must be followed by a closing '}'");                                     \
                                                                                       \
    frg_message_emit_child((message_buffer),                                           \
                           __frg_es_6_message,                                         \
                           FRG_MESSAGE_SEVERITY_NOTE,                                  \
                           NULL,                                                       \
                           "For example: '\\u{20}'");                                  \
  }

#define frg_message_emit_es_7_escape_u_expects_hex_digits(                             \
  message_buffer, source_range, digit_count)                                           \
  {                                                                                    \
    frg_assert((digit_count) == 0 || (digit_count) > 6);                               \
                                                                                       \
    frg_message_t* __frg_es_7_message = frg_message_emit_from_source_range(            \
      (message_buffer),                                                                \
      (source_range),                                                                  \
      FRG_MESSAGE_SEVERITY_ERROR,                                                      \
      "ES-6",                                                                          \
      "'\\u{' must be followed by between 1 and 6 hexadecimal digits (%Lu were "       \
      "provided)",                                                                     \
      (digit_count));                                                                  \
                                                                                       \
    frg_message_emit_child((message_buffer),                                           \
                           __frg_es_7_message,                                         \
                           FRG_MESSAGE_SEVERITY_NOTE,                                  \
                           NULL,                                                       \
                           "For example: '\\u{20}'");                                  \
  }

#define frg_message_emit_es_8_unexpected_end_of_literal(message_buffer, source_range)  \
  frg_message_emit_from_source_range((message_buffer),                                 \
                                     (source_range),                                   \
                                     FRG_MESSAGE_SEVERITY_ERROR,                       \
                                     "ES-8",                                           \
                                     "Unexpected end of literal")

#define frg_message_emit_es_9_multiple_characters_in_character_literal(message_buffer, \
                                                                       source_range)   \
  {                                                                                    \
    frg_message_t* __frg_es_9_message = frg_message_emit_from_source_range(            \
      (message_buffer),                                                                \
      (source_range),                                                                  \
      FRG_MESSAGE_SEVERITY_ERROR,                                                      \
      "ES-9",                                                                          \
      "Character literal contains more than one character");                           \
                                                                                       \
    frg_message_emit_child(                                                            \
      (message_buffer),                                                                \
      __frg_es_9_message,                                                              \
      FRG_MESSAGE_SEVERITY_NOTE,                                                       \
      NULL,                                                                            \
      "If this was supposed to be a string, use double quotes instead");               \
  }

#define frg_message_emit_es_10_invalid_bit_width_for_int(                              \
  message_buffer, source_range, bit_width)                                             \
  {                                                                                    \
    frg_message_t* __frg_es_10_message = frg_message_emit_from_source_range(           \
      (message_buffer),                                                                \
      (source_range),                                                                  \
      FRG_MESSAGE_SEVERITY_ERROR,                                                      \
      "ES-10",                                                                         \
      "Invalid bit width for integer literal: %Lu",                                    \
      (bit_width));                                                                    \
                                                                                       \
    frg_message_emit_child((message_buffer),                                           \
                           __frg_es_10_message,                                        \
                           FRG_MESSAGE_SEVERITY_NOTE,                                  \
                           NULL,                                                       \
                           "Valid bit widths are: 8, 16, 32, 64");                     \
  }

#define frg_message_emit_es_11_invalid_bit_width_for_float(                            \
  message_buffer, source_range, bit_width)                                             \
  {                                                                                    \
    frg_message_t* __frg_es_11_message                                                 \
      = frg_message_emit_from_source_range((message_buffer),                           \
                                           (source_range),                             \
                                           FRG_MESSAGE_SEVERITY_ERROR,                 \
                                           "ES-11",                                    \
                                           "Invalid bit width for float literal: %Lu", \
                                           (bit_width));                               \
                                                                                       \
    frg_message_emit_child((message_buffer),                                           \
                           __frg_es_11_message,                                        \
                           FRG_MESSAGE_SEVERITY_NOTE,                                  \
                           NULL,                                                       \
                           "Valid bit widths are: 32, 64");                            \
  }

#define frg_message_emit_fg_1_cannot_emit_object_file(message_buffer, target_triple)   \
  frg_message_emit((message_buffer),                                                   \
                   FRG_MESSAGE_SEVERITY_FATAL_ERROR,                                   \
                   "FG-1",                                                             \
                   "Target machine '%s' cannot emit an object file",                   \
                   (target_triple))

#define frg_message_emit_fg_2_invalid_target_triple(                                   \
  message_buffer, target_triple, error_message)                                        \
  {                                                                                    \
    frg_message_t* __frg_fg_2_message                                                  \
      = frg_message_emit((message_buffer),                                             \
                         FRG_MESSAGE_SEVERITY_FATAL_ERROR,                             \
                         "FG-2",                                                       \
                         "Unable find target for '%s'",                                \
                         (target_triple));                                             \
                                                                                       \
    frg_message_emit_child((message_buffer),                                           \
                           __frg_fg_2_message,                                         \
                           FRG_MESSAGE_SEVERITY_NOTE,                                  \
                           NULL,                                                       \
                           "Underlying cause: %s",                                     \
                           (error_message));                                           \
  }

#define _frg_message_emit_fl_helper(                                                   \
  message_buffer, message, executable, argv, working_directory)                        \
  {                                                                                    \
    frg_message_emit_child((message_buffer),                                           \
                           (message),                                                  \
                           FRG_MESSAGE_SEVERITY_NOTE,                                  \
                           NULL,                                                       \
                           "Linker executable called: %s",                             \
                           (executable));                                              \
                                                                                       \
    gchar* __frg_fl_argv_joined = g_strjoinv("', '", (gchar**)(argv));                 \
                                                                                       \
    frg_message_emit_child((message_buffer),                                           \
                           (message),                                                  \
                           FRG_MESSAGE_SEVERITY_NOTE,                                  \
                           NULL,                                                       \
                           "Linker command-line arguments: '%s'",                      \
                           (__frg_fl_argv_joined));                                    \
                                                                                       \
    if ((working_directory) != NULL) {                                                 \
      frg_message_emit_child((message_buffer),                                         \
                             (message),                                                \
                             FRG_MESSAGE_SEVERITY_NOTE,                                \
                             NULL,                                                     \
                             "Linker working directory: '%s'",                         \
                             (working_directory));                                     \
    } else {                                                                           \
      frg_message_emit_child((message_buffer),                                         \
                             (message),                                                \
                             FRG_MESSAGE_SEVERITY_NOTE,                                \
                             NULL,                                                     \
                             "Linker working directory: (inherited)");                 \
    }                                                                                  \
                                                                                       \
    g_free(__frg_fl_argv_joined);                                                      \
  }

#define frg_message_emit_fl_1_unable_to_spawn(                                         \
  message_buffer, cause, executable, argv, working_directory)                          \
  {                                                                                    \
    frg_message_t* __frg_fl_1_message                                                  \
      = frg_message_emit((message_buffer),                                             \
                         FRG_MESSAGE_SEVERITY_FATAL_ERROR,                             \
                         "FL-1",                                                       \
                         "Could not spawn linker process");                            \
                                                                                       \
    frg_message_emit_child((message_buffer),                                           \
                           __frg_fl_1_message,                                         \
                           FRG_MESSAGE_SEVERITY_NOTE,                                  \
                           NULL,                                                       \
                           "Underlying cause: %s",                                     \
                           (cause));                                                   \
                                                                                       \
    _frg_message_emit_fl_helper((message_buffer),                                      \
                                __frg_fl_1_message,                                    \
                                (executable),                                          \
                                (argv),                                                \
                                (working_directory));                                  \
  }

#define frg_message_emit_fl_2_exited_nonzero(                                          \
  message_buffer, exit_code, executable, argv, working_directory)                      \
  {                                                                                    \
    frg_message_t* __frg_fl_2_message                                                  \
      = frg_message_emit((message_buffer),                                             \
                         FRG_MESSAGE_SEVERITY_FATAL_ERROR,                             \
                         "FL-2",                                                       \
                         "Linker process exited with non-zero status %i",              \
                         (exit_code));                                                 \
                                                                                       \
    _frg_message_emit_fl_helper((message_buffer),                                      \
                                __frg_fl_2_message,                                    \
                                (executable),                                          \
                                (argv),                                                \
                                (working_directory));                                  \
  }

#define frg_message_emit_fl_3_no_linker(message_buffer)                                \
  {                                                                                    \
    frg_message_t* __frg_fl_3_message                                                  \
      = frg_message_emit((message_buffer),                                             \
                         FRG_MESSAGE_SEVERITY_FATAL_ERROR,                             \
                         "FL-3",                                                       \
                         NULL,                                                         \
                         "No linker could be found");                                  \
                                                                                       \
    frg_message_emit_child(                                                            \
      (message_buffer),                                                                \
      __frg_fl_3_message,                                                              \
      FRG_MESSAGE_SEVERITY_NOTE,                                                       \
      NULL,                                                                            \
      "Currently only LLD is supported - please install and try again");               \
  }

#define frg_emit_message_it_1_no_containing_type(                                      \
  message_buffer, source_range, type_a, type_b)                                        \
  {                                                                                    \
    frg_stream_output_t* __frg_it_1_stream_a                                           \
      = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);                     \
                                                                                       \
    frg_formatting_print_formatted(__frg_it_1_stream_a, (type_a), 0);                  \
                                                                                       \
    GString* __frg_it_1_buffer_a                                                       \
      = frg_stream_output_destroy_take_buffer(__frg_it_1_stream_a);                    \
                                                                                       \
    frg_stream_output_t* __frg_it_1_stream_b                                           \
      = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);                     \
                                                                                       \
    frg_formatting_print_formatted(__frg_it_1_stream_b, (type_b), 0);                  \
                                                                                       \
    GString* __frg_it_1_buffer_b                                                       \
      = frg_stream_output_destroy_take_buffer(__frg_it_1_stream_b);                    \
                                                                                       \
    frg_message_emit_from_source_range(                                                \
      (message_buffer),                                                                \
      (source_range),                                                                  \
      FRG_MESSAGE_SEVERITY_INTERNAL_ERROR,                                             \
      "IT-1",                                                                          \
      "Unable to find a type that can contain both '%s' and '%s'",                     \
      __frg_it_1_buffer_a->str,                                                        \
      __frg_it_1_buffer_b->str);                                                       \
                                                                                       \
    g_string_free(__frg_it_1_buffer_a, TRUE);                                          \
    g_string_free(__frg_it_1_buffer_b, TRUE);                                          \
  }

#define frg_emit_message_et_1_undeclared_symbol(                                       \
  message_buffer, source_range, symbol_name)                                           \
  frg_message_emit_from_source_range((message_buffer),                                 \
                                     (source_range),                                   \
                                     FRG_MESSAGE_SEVERITY_ERROR,                       \
                                     "ET-1",                                           \
                                     "Symbol '%s' must be declared",                   \
                                     (symbol_name))

#define frg_emit_message_et_2_cannot_dereference_non_pointer(                          \
  message_buffer, source_range, type)                                                  \
  {                                                                                    \
    frg_stream_output_t* __frg_et_2_stream                                             \
      = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);                     \
                                                                                       \
    frg_formatting_print_formatted(__frg_et_2_stream, (type), 0);                      \
                                                                                       \
    GString* __frg_et_2_buffer                                                         \
      = frg_stream_output_destroy_take_buffer(__frg_et_2_stream);                      \
                                                                                       \
    frg_message_emit_from_source_range((message_buffer),                               \
                                       (source_range),                                 \
                                       FRG_MESSAGE_SEVERITY_ERROR,                     \
                                       "ET-2",                                         \
                                       "Cannot dereference non-pointer type '%s'",     \
                                       __frg_et_2_buffer->str);                        \
                                                                                       \
    g_string_free(__frg_et_2_buffer, TRUE);                                            \
  }

#define frg_emit_message_et_3_cannot_call_non_function(                                \
  message_buffer, source_range, type)                                                  \
  {                                                                                    \
    frg_stream_output_t* __frg_et_3_stream                                             \
      = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);                     \
                                                                                       \
    frg_formatting_print_formatted(__frg_et_3_stream, (type), 0);                      \
                                                                                       \
    GString* __frg_et_3_buffer                                                         \
      = frg_stream_output_destroy_take_buffer(__frg_et_3_stream);                      \
                                                                                       \
    frg_message_emit_from_source_range((message_buffer),                               \
                                       (source_range),                                 \
                                       FRG_MESSAGE_SEVERITY_ERROR,                     \
                                       "ET-3",                                         \
                                       "Cannot call non-function type '%s'",           \
                                       __frg_et_3_buffer->str);                        \
                                                                                       \
    g_string_free(__frg_et_3_buffer, TRUE);                                            \
  }

#define frg_emit_message_et_4_cannot_get_address_non_reference(                        \
  message_buffer, source_range, type)                                                  \
  {                                                                                    \
    frg_stream_output_t* __frg_et_4_stream                                             \
      = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);                     \
                                                                                       \
    frg_formatting_print_formatted(__frg_et_4_stream, (type), 0);                      \
                                                                                       \
    GString* __frg_et_4_buffer                                                         \
      = frg_stream_output_destroy_take_buffer(__frg_et_4_stream);                      \
                                                                                       \
    frg_message_emit_from_source_range(                                                \
      (message_buffer),                                                                \
      (source_range),                                                                  \
      FRG_MESSAGE_SEVERITY_ERROR,                                                      \
      "ET-4",                                                                          \
      "Cannot get address of value with non-reference type '%s'",                      \
      __frg_et_4_buffer->str);                                                         \
                                                                                       \
    g_string_free(__frg_et_4_buffer, TRUE);                                            \
  }

#define frg_emit_message_et_5_mismatched_return_type(                                  \
  message_buffer, source_range, expected_type, actual_type)                            \
  {                                                                                    \
    frg_stream_output_t* __frg_et_5_stream_expected                                    \
      = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);                     \
                                                                                       \
    frg_formatting_print_formatted(__frg_et_5_stream_expected, (expected_type), 0);    \
                                                                                       \
    GString* __frg_et_5_buffer_expected                                                \
      = frg_stream_output_destroy_take_buffer(__frg_et_5_stream_expected);             \
                                                                                       \
    frg_stream_output_t* __frg_et_5_stream_actual                                      \
      = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);                     \
                                                                                       \
    frg_formatting_print_formatted(__frg_et_5_stream_actual, (actual_type), 0);        \
                                                                                       \
    GString* __frg_et_5_buffer_actual                                                  \
      = frg_stream_output_destroy_take_buffer(__frg_et_5_stream_actual);               \
                                                                                       \
    frg_message_emit_from_source_range(                                                \
      (message_buffer),                                                                \
      (source_range),                                                                  \
      FRG_MESSAGE_SEVERITY_ERROR,                                                      \
      "ET-5",                                                                          \
      "Cannot return value of type '%s' in function with return type '%s'",            \
      __frg_et_5_buffer_actual->str,                                                   \
      __frg_et_5_buffer_expected->str);                                                \
                                                                                       \
    g_string_free(__frg_et_5_buffer_expected, TRUE);                                   \
    g_string_free(__frg_et_5_buffer_actual, TRUE);                                     \
  }

#define frg_emit_message_et_6_operator_unexpected_operand_type(message_buffer,         \
                                                               source_range,           \
                                                               operator_name,          \
                                                               operand_name,           \
                                                               expected_type_name,     \
                                                               operand_type)           \
  {                                                                                    \
    frg_stream_output_t* __frg_et_6_stream                                             \
      = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);                     \
                                                                                       \
    frg_formatting_print_formatted(__frg_et_6_stream, (operand_type), 0);              \
                                                                                       \
    GString* __frg_et_6_buffer                                                         \
      = frg_stream_output_destroy_take_buffer(__frg_et_6_stream);                      \
                                                                                       \
    frg_message_emit_from_source_range((message_buffer),                               \
                                       (source_range),                                 \
                                       FRG_MESSAGE_SEVERITY_ERROR,                     \
                                       "ET-6",                                         \
                                       "Operator %s's %s must be %s, but is '%s'",     \
                                       (operator_name),                                \
                                       (operand_name),                                 \
                                       (expected_type_name),                           \
                                       __frg_et_6_buffer->str);                        \
                                                                                       \
    g_string_free(__frg_et_6_buffer, TRUE);                                            \
  }
