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

#pragma once

#include <forge/types.h>
#include <glib.h>
#include <stdbool.h>
#include <stdio.h>

int frg_get_enum_mask_from_max(int max);

/// A mode for whether or not to use color for console output
typedef enum {
  /// Do not use color output
  FRG_STREAM_COLOR_MODE_DISABLED,
  /// Use color if it is available, but otherwise disable it
  FRG_STREAM_COLOR_MODE_AUTO,
  /// Force color output no matter what
  FRG_STREAM_COLOR_MODE_ENABLED,

  FRG_STREAM_COLOR_MODE_LAST = FRG_STREAM_COLOR_MODE_ENABLED,
} frg_stream_color_mode_t;

#define FRG_STREAM_COLOR_MODE_COUNT (FRG_STREAM_COLOR_MODE_LAST + 1)

// TODO: Add suggestion or fix severity
typedef enum {
  FRG_MESSAGE_SEVERITY_DEBUG,
  FRG_MESSAGE_SEVERITY_NOTE,
  FRG_MESSAGE_SEVERITY_WARNING,
  FRG_MESSAGE_SEVERITY_ERROR,
  FRG_MESSAGE_SEVERITY_FATAL_ERROR,
  FRG_MESSAGE_SEVERITY_INTERNAL_ERROR,

  FRG_MESSAGE_SEVERITY_LAST = FRG_MESSAGE_SEVERITY_INTERNAL_ERROR,
} frg_message_severity_t;

#define FRG_MESSAGE_SEVERITY_COUNT (FRG_MESSAGE_SEVERITY_LAST + 1)

typedef enum {
  FRG_AST_NODE_KIND_FLAG_NONE         = 0,
  FRG_AST_NODE_KIND_FLAG_TYPE         = 1,
  FRG_AST_NODE_KIND_FLAG_TYPE_PRIMARY = 1 << 1,
  FRG_AST_NODE_KIND_FLAG_DECLARATION  = 1 << 2,
  FRG_AST_NODE_KIND_FLAG_STATEMENT    = 1 << 3,
  FRG_AST_NODE_KIND_FLAG_VALUE        = 1 << 4,
  FRG_AST_NODE_KIND_FLAG_VALUE_UNARY  = 1 << 5,
  FRG_AST_NODE_KIND_FLAG_VALUE_BINARY = 1 << 6,
  FRG_AST_NODE_KIND_FLAG_HAS_CHILDREN = 1 << 7,

  FRG_AST_NODE_KIND_FLAG_MAX          = FRG_AST_NODE_KIND_FLAG_HAS_CHILDREN,
} frg_ast_node_kind_flags_t;

/// @brief The list of possible types of AST nodes.
typedef enum {
  FRG_AST_NODE_KIND_TYPE_VOID,
  FRG_AST_NODE_KIND_TYPE_BOOL,
  FRG_AST_NODE_KIND_TYPE_INT,
  FRG_AST_NODE_KIND_TYPE_FLOAT,
  FRG_AST_NODE_KIND_TYPE_SYMBOL,
  FRG_AST_NODE_KIND_TYPE_POINTER,
  FRG_AST_NODE_KIND_TYPE_ARRAY,
  FRG_AST_NODE_KIND_TYPE_FUNCTION,
  FRG_AST_NODE_KIND_DECLARATION_UNION,
  FRG_AST_NODE_KIND_DECLARATION_STRUCTURE,
  FRG_AST_NODE_KIND_DECLARATION_PROPERTY,
  FRG_AST_NODE_KIND_DECLARATION_INTERFACE,
  FRG_AST_NODE_KIND_DECLARATION_FUNCTION_ARGUMENT,
  FRG_AST_NODE_KIND_DECLARATION_FUNCTION,
  FRG_AST_NODE_KIND_DECLARATION_ASSIGNMENT,
  FRG_AST_NODE_KIND_DECLARATION_BLOCK,
  FRG_AST_NODE_KIND_STATEMENT_RETURN,
  FRG_AST_NODE_KIND_STATEMENT_IF_CONDITIONAL_CLAUSE,
  FRG_AST_NODE_KIND_STATEMENT_IF,
  FRG_AST_NODE_KIND_STATEMENT_WHILE,
  FRG_AST_NODE_KIND_STATEMENT_BLOCK,
  FRG_AST_NODE_KIND_VALUE_BOOL,
  FRG_AST_NODE_KIND_VALUE_INT,
  FRG_AST_NODE_KIND_VALUE_FLOAT,
  FRG_AST_NODE_KIND_VALUE_CHARACTER,
  FRG_AST_NODE_KIND_VALUE_STRING,
  FRG_AST_NODE_KIND_VALUE_ARRAY,
  FRG_AST_NODE_KIND_VALUE_ARRAY_REPEATED,
  FRG_AST_NODE_KIND_VALUE_STRUCTURE,
  FRG_AST_NODE_KIND_VALUE_SYMBOL,
  FRG_AST_NODE_KIND_VALUE_DEREFERENCE,
  FRG_AST_NODE_KIND_VALUE_GET_ADDRESS,
  FRG_AST_NODE_KIND_VALUE_CALL_KEYWORD_ARGUMENT,
  FRG_AST_NODE_KIND_VALUE_CALL,
  FRG_AST_NODE_KIND_VALUE_CAST,
  FRG_AST_NODE_KIND_VALUE_ACCESS,
  FRG_AST_NODE_KIND_VALUE_BIT_NOT,
  FRG_AST_NODE_KIND_VALUE_BIT_AND,
  FRG_AST_NODE_KIND_VALUE_BIT_OR,
  FRG_AST_NODE_KIND_VALUE_BIT_XOR,
  FRG_AST_NODE_KIND_VALUE_BIT_SHIFT_LEFT,
  FRG_AST_NODE_KIND_VALUE_BIT_SHIFT_RIGHT,
  FRG_AST_NODE_KIND_VALUE_NEGATE,
  FRG_AST_NODE_KIND_VALUE_ADD,
  FRG_AST_NODE_KIND_VALUE_SUBTRACT,
  FRG_AST_NODE_KIND_VALUE_MULTIPLY,
  FRG_AST_NODE_KIND_VALUE_DIVIDE,
  FRG_AST_NODE_KIND_VALUE_DIVIDE_INT,
  FRG_AST_NODE_KIND_VALUE_MODULO,
  FRG_AST_NODE_KIND_VALUE_EXPONENTIATE,
  FRG_AST_NODE_KIND_VALUE_EQUALS,
  FRG_AST_NODE_KIND_VALUE_NOT_EQUALS,
  FRG_AST_NODE_KIND_VALUE_IS_LESS_THAN,
  FRG_AST_NODE_KIND_VALUE_IS_LESS_THAN_OR_EQUAL_TO,
  FRG_AST_NODE_KIND_VALUE_IS_GREATER_THAN,
  FRG_AST_NODE_KIND_VALUE_IS_GREATER_THAN_OR_EQUAL_TO,
  FRG_AST_NODE_KIND_VALUE_LOGICAL_NOT,
  FRG_AST_NODE_KIND_VALUE_LOGICAL_AND,
  FRG_AST_NODE_KIND_VALUE_LOGICAL_OR,
  FRG_AST_NODE_KIND_VALUE_ASSIGN,
  FRG_AST_NODE_KIND_VALUE_BIT_AND_ASSIGN,
  FRG_AST_NODE_KIND_VALUE_BIT_OR_ASSIGN,
  FRG_AST_NODE_KIND_VALUE_BIT_XOR_ASSIGN,
  FRG_AST_NODE_KIND_VALUE_BIT_SHIFT_LEFT_ASSIGN,
  FRG_AST_NODE_KIND_VALUE_BIT_SHIFT_RIGHT_ASSIGN,
  FRG_AST_NODE_KIND_VALUE_ADD_ASSIGN,
  FRG_AST_NODE_KIND_VALUE_INCREMENT,
  FRG_AST_NODE_KIND_VALUE_SUBTRACT_ASSIGN,
  FRG_AST_NODE_KIND_VALUE_DECREMENT,
  FRG_AST_NODE_KIND_VALUE_MULTIPLY_ASSIGN,
  FRG_AST_NODE_KIND_VALUE_DIVIDE_ASSIGN,
  FRG_AST_NODE_KIND_VALUE_DIVIDE_INT_ASSIGN,
  FRG_AST_NODE_KIND_VALUE_MODULO_ASSIGN,
  FRG_AST_NODE_KIND_VALUE_EXPONENTIATE_ASSIGN,
  FRG_AST_NODE_KIND_VALUE_LOGICAL_AND_ASSIGN,
  FRG_AST_NODE_KIND_VALUE_LOGICAL_OR_ASSIGN,

  FRG_AST_NODE_KIND_LAST = FRG_AST_NODE_KIND_VALUE_LOGICAL_OR_ASSIGN,
} frg_ast_node_kind_t;

#define FRG_AST_NODE_KIND_COUNT (FRG_AST_NODE_KIND_LAST + 1)

typedef enum {
  FRG_AST_NODE_TYPE_POINTER_FLAG_NONE                 = 0,
  FRG_AST_NODE_TYPE_POINTER_FLAG_CONSTANT             = 1,
  FRG_AST_NODE_TYPE_POINTER_FLAG_IMPLICIT_DEREFERENCE = 1 << 1,

  FRG_AST_NODE_TYPE_POINTER_FLAG_MAX
    = FRG_AST_NODE_TYPE_POINTER_FLAG_IMPLICIT_DEREFERENCE,
} frg_ast_node_type_pointer_flags_t;

typedef enum {
  FRG_AST_NODE_DECLARATION_PROPERTY_FLAG_NONE         = 0,
  FRG_AST_NODE_DECLARATION_PROPERTY_FLAG_OPTIONAL     = 1,
  FRG_AST_NODE_DECLARATION_PROPERTY_FLAG_NON_OPTIONAL = 1 << 1,
  FRG_AST_NODE_DECLARATION_PROPERTY_FLAG_SPREAD       = 1 << 2,

  FRG_AST_NODE_DECLARATION_PROPERTY_FLAG_MAX
    = FRG_AST_NODE_DECLARATION_PROPERTY_FLAG_SPREAD,
} frg_ast_node_declaration_property_flags_t;

typedef enum {
  FRG_AST_NODE_DECLARATION_INTERFACE_FLAG_NONE     = 0,
  FRG_AST_NODE_DECLARATION_INTERFACE_FLAG_ABSTRACT = 1,

  FRG_AST_NODE_DECLARATION_INTERFACE_FLAG_MAX
    = FRG_AST_NODE_DECLARATION_INTERFACE_FLAG_ABSTRACT,
} frg_ast_node_declaration_interface_flags_t;

typedef enum {
  FRG_AST_NODE_DECLARATION_FUNCTION_ARGUMENT_FLAG_NONE    = 0,
  FRG_AST_NODE_DECLARATION_FUNCTION_ARGUMENT_FLAG_KEYWORD = 1,

  FRG_AST_NODE_DECLARATION_FUNCTION_ARGUMENT_FLAG_MAX
    = FRG_AST_NODE_DECLARATION_FUNCTION_ARGUMENT_FLAG_KEYWORD,
} frg_ast_node_declaration_function_argument_flags_t;

typedef enum {
  FRG_AST_NODE_DECLARATION_FUNCTION_FLAG_NONE     = 0,
  FRG_AST_NODE_DECLARATION_FUNCTION_FLAG_MUTABLE  = 1,
  FRG_AST_NODE_DECLARATION_FUNCTION_FLAG_OVERRIDE = 1 << 1,

  FRG_AST_NODE_DECLARATION_FUNCTION_FLAG_MAX
    = FRG_AST_NODE_DECLARATION_FUNCTION_FLAG_OVERRIDE,
} frg_ast_node_declaration_function_flags_t;

typedef enum {
  FRG_AST_VISITOR_STATUS_OK,
  FRG_AST_VISITOR_STATUS_SKIP,
  FRG_AST_VISITOR_STATUS_STOP,

  FRG_AST_VISITOR_STATUS_LAST = FRG_AST_VISITOR_STATUS_STOP,
} frg_ast_visitor_status_t;

#define FRG_AST_VISITOR_STATUS_COUNT (FRG_AST_VISITOR_STATUS_LAST + 1)

typedef enum {
  FRG_STREAM_INPUT_FLAG_NONE            = 0,
  FRG_STREAM_INPUT_FLAG_OWNED           = 1,
  FRG_STREAM_INPUT_FLAG_LENGTH_SET      = 1 << 1,
  FRG_STREAM_INPUT_FLAG_EXTRA_NULL_BYTE = 1 << 2,

  FRG_STREAM_INPUT_FLAG_MAX             = FRG_STREAM_INPUT_FLAG_EXTRA_NULL_BYTE,
} frg_stream_input_flags_t;

typedef enum {
  FRG_STREAM_OUTPUT_FLAG_NONE           = 0,
  FRG_STREAM_OUTPUT_FLAG_OWNED          = 1,
  FRG_STREAM_OUTPUT_FLAG_COLOR          = 1 << 1,
  FRG_STREAM_OUTPUT_FLAG_DETECT_COLOR   = 1 << 2,
  FRG_STREAM_OUTPUT_FLAG_UNICODE        = 1 << 3,
  FRG_STREAM_OUTPUT_FLAG_DETECT_UNICODE = 1 << 4,

  FRG_STREAM_OUTPUT_FLAG_MAX            = FRG_STREAM_OUTPUT_FLAG_DETECT_UNICODE,
} frg_stream_output_flags_t;

/// An identifier for which color code to output
///
/// Codes are a subset of https://gist.github.com/fnky/458719343aabd01cfb17a3a4f7296797
/// that will actually get used by the Forge compiler.
typedef enum {
  FRG_STREAM_OUTPUT_COLOR_RESET,
  FRG_STREAM_OUTPUT_COLOR_BOLD,
  FRG_STREAM_OUTPUT_COLOR_DIM,
  FRG_STREAM_OUTPUT_COLOR_ITALIC,
  FRG_STREAM_OUTPUT_COLOR_UNDERLINE,
  FRG_STREAM_OUTPUT_COLOR_BLACK,
  FRG_STREAM_OUTPUT_COLOR_RED,
  FRG_STREAM_OUTPUT_COLOR_GREEN,
  FRG_STREAM_OUTPUT_COLOR_YELLOW,
  FRG_STREAM_OUTPUT_COLOR_BLUE,
  FRG_STREAM_OUTPUT_COLOR_MAGENTA,
  FRG_STREAM_OUTPUT_COLOR_CYAN,
  FRG_STREAM_OUTPUT_COLOR_WHITE,
  FRG_STREAM_OUTPUT_COLOR_BRIGHT_BLACK,
  FRG_STREAM_OUTPUT_COLOR_BRIGHT_RED,
  FRG_STREAM_OUTPUT_COLOR_BRIGHT_GREEN,
  FRG_STREAM_OUTPUT_COLOR_BRIGHT_YELLOW,
  FRG_STREAM_OUTPUT_COLOR_BRIGHT_BLUE,
  FRG_STREAM_OUTPUT_COLOR_BRIGHT_MAGENTA,
  FRG_STREAM_OUTPUT_COLOR_BRIGHT_CYAN,
  FRG_STREAM_OUTPUT_COLOR_BRIGHT_WHITE,

  FRG_STREAM_OUTPUT_COLOR_LAST = FRG_STREAM_OUTPUT_COLOR_BRIGHT_WHITE,
} frg_stream_output_color_t;

#define FRG_STREAM_OUTPUT_COLOR_COUNT (FRG_STREAM_OUTPUT_COLOR_LAST + 1)

typedef enum {
  FRG_LINKING_LINKER_KIND_NONE,
  FRG_LINKING_LINKER_KIND_LD_LLD,
  FRG_LINKING_LINKER_KIND_LD64_LLD,
  FRG_LINKING_LINKER_KIND_LLD_LINK,

  FRG_LINKING_LINKER_KIND_LAST = FRG_LINKING_LINKER_KIND_LLD_LINK,
} frg_linking_linker_kind_t;

#define FRG_LINKING_LINKER_KIND_COUNT (FRG_LINKING_LINKER_KIND_LAST + 1)

typedef enum {
  FRG_LINKING_MODE_EXECUTABLE,
  FRG_LINKING_MODE_SHARED_LIBRARY,
  FRG_LINKING_MODE_STATIC_LIBRARY,

  FRG_LINKING_MODE_LAST = FRG_LINKING_MODE_STATIC_LIBRARY,
} frg_linking_mode_t;

#define FRG_LINKING_MODE_COUNT (FRG_LINKING_MODE_LAST + 1)

typedef enum {
  FRG_AST_VISITOR_EVENT_ENTER,
  FRG_AST_VISITOR_EVENT_LEAVE,

  FRG_AST_VISITOR_EVENT_LAST = FRG_AST_VISITOR_EVENT_LEAVE,
} frg_ast_visitor_event_t;

#define FRG_AST_VISITOR_EVENT_COUNT (FRG_AST_VISITOR_EVENT_LAST + 1)

typedef enum {
  FRG_AST_NODE_TYPE_INT_FLAG_NONE     = 0,
  FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED = 1,

  FRG_AST_NODE_TYPE_INT_FLAG_MAX      = FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED,
} frg_ast_node_type_int_flags_t;

typedef enum {
  FRG_MESSAGE_BUFFER_QUERY_RESULT_SINGLE,
  FRG_MESSAGE_BUFFER_QUERY_RESULT_NONE,
  FRG_MESSAGE_BUFFER_QUERY_RESULT_MULTIPLE,

  FRG_MESSAGE_BUFFER_QUERY_RESULT_LAST = FRG_MESSAGE_BUFFER_QUERY_RESULT_MULTIPLE,
} frg_message_buffer_query_single_result_t;

#define FRG_MESSAGE_BUFFER_QUERY_RESULT_COUNT (FRG_MESSAGE_BUFFER_QUERY_RESULT_LAST + 1)
