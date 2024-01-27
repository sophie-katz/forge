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

#include <stdbool.h>
#include <stdio.h>
#include <forge/common/types.h>
#include <glib.h>

/// A mode for whether or not to use color for console output
typedef enum {
    /// Do not use color output
    FRG_COLOR_MODE_DISABLED,
    /// Use color if it is available, but otherwise disable it
    FRG_COLOR_MODE_AUTO,
    /// Force color output no matter what
    FRG_COLOR_MODE_ENABLED,
} frg_color_mode_t;

// TODO: Add suggestion or fix severity
typedef enum {
    FRG_MESSAGE_SEVERITY_DEBUG,
    FRG_MESSAGE_SEVERITY_NOTE,
    FRG_MESSAGE_SEVERITY_WARNING,
    FRG_MESSAGE_SEVERITY_ERROR,
    FRG_MESSAGE_SEVERITY_FATAL_ERROR,
    FRG_MESSAGE_SEVERITY_INTERNAL_ERROR,
} frg_message_severity_t;

typedef enum {
    FRG_AST_KIND_FLAG_NONE = 0x0,
    FRG_AST_KIND_FLAG_TY = 0x1,
    FRG_AST_KIND_FLAG_TY_PRIMARY = 0x1,
    FRG_AST_KIND_FLAG_TY_INT = 0x2,
    FRG_AST_KIND_FLAG_TY_INT_SIGNED = 0x2,
    FRG_AST_KIND_FLAG_TY_INT_UNSIGNED = 0x2,
    FRG_AST_KIND_FLAG_TY_FLOAT = 0x4,
    FRG_AST_KIND_FLAG_DECL = 0x1,
    FRG_AST_KIND_FLAG_STMT = 0x1,
    FRG_AST_KIND_FLAG_VALUE = 0x1,
    FRG_AST_KIND_FLAG_VALUE_PRIMARY = 0x1,
    FRG_AST_KIND_FLAG_VALUE_UNARY = 0x1,
    FRG_AST_KIND_FLAG_VALUE_BINARY = 0x1,
} frg_ast_kind_flags_t;

/// @brief The list of possible types of AST nodes.
typedef enum {
    FRG_AST_KIND_TY_BOOL,
    FRG_AST_KIND_TY_U8,
    FRG_AST_KIND_TY_U16,
    FRG_AST_KIND_TY_U32,
    FRG_AST_KIND_TY_U64,
    FRG_AST_KIND_TY_I8,
    FRG_AST_KIND_TY_I16,
    FRG_AST_KIND_TY_I32,
    FRG_AST_KIND_TY_I64,
    FRG_AST_KIND_TY_F32,
    FRG_AST_KIND_TY_F64,
    FRG_AST_KIND_TY_SYMBOL,
    FRG_AST_KIND_TY_POINTER,
    FRG_AST_KIND_TY_FN,
    FRG_AST_KIND_DECL_UNION,
    FRG_AST_KIND_DECL_STRUCT,
    FRG_AST_KIND_DECL_PROP,
    FRG_AST_KIND_DECL_IFACE,
    FRG_AST_KIND_DECL_FN_ARG,
    FRG_AST_KIND_DECL_FN,
    FRG_AST_KIND_DECL_VAR,
    FRG_AST_KIND_DECL_BLOCK,
    FRG_AST_KIND_STMT_RETURN,
    FRG_AST_KIND_STMT_IF,
    FRG_AST_KIND_STMT_WHILE,
    FRG_AST_KIND_STMT_BLOCK,
    FRG_AST_KIND_VALUE_TRUE,
    FRG_AST_KIND_VALUE_FALSE,
    FRG_AST_KIND_VALUE_INT,
    FRG_AST_KIND_VALUE_FLOAT,
    FRG_AST_KIND_VALUE_CHAR,
    FRG_AST_KIND_VALUE_STR,
    FRG_AST_KIND_VALUE_SYMBOL,
    FRG_AST_KIND_VALUE_DEREF,
    FRG_AST_KIND_VALUE_GETADDR,
    FRG_AST_KIND_VALUE_CALL_KW_ARG,
    FRG_AST_KIND_VALUE_CALL,
    FRG_AST_KIND_VALUE_ACCESS,
    FRG_AST_KIND_VALUE_BIT_NOT,
    FRG_AST_KIND_VALUE_BIT_AND,
    FRG_AST_KIND_VALUE_BIT_OR,
    FRG_AST_KIND_VALUE_BIT_XOR,
    FRG_AST_KIND_VALUE_BIT_SHL,
    FRG_AST_KIND_VALUE_BIT_SHR,
    FRG_AST_KIND_VALUE_NEG,
    FRG_AST_KIND_VALUE_ADD,
    FRG_AST_KIND_VALUE_SUB,
    FRG_AST_KIND_VALUE_MUL,
    FRG_AST_KIND_VALUE_DIV,
    FRG_AST_KIND_VALUE_DIV_INT,
    FRG_AST_KIND_VALUE_MOD,
    FRG_AST_KIND_VALUE_EXP,
    FRG_AST_KIND_VALUE_EQ,
    FRG_AST_KIND_VALUE_NE,
    FRG_AST_KIND_VALUE_LT,
    FRG_AST_KIND_VALUE_LE,
    FRG_AST_KIND_VALUE_GT,
    FRG_AST_KIND_VALUE_GE,
    FRG_AST_KIND_VALUE_LOG_NOT,
    FRG_AST_KIND_VALUE_LOG_AND,
    FRG_AST_KIND_VALUE_LOG_OR,
    FRG_AST_KIND_VALUE_ASSIGN,
    FRG_AST_KIND_VALUE_BIT_AND_ASSIGN,
    FRG_AST_KIND_VALUE_BIT_OR_ASSIGN,
    FRG_AST_KIND_VALUE_BIT_XOR_ASSIGN,
    FRG_AST_KIND_VALUE_BIT_SHL_ASSIGN,
    FRG_AST_KIND_VALUE_BIT_SHR_ASSIGN,
    FRG_AST_KIND_VALUE_ADD_ASSIGN,
    FRG_AST_KIND_VALUE_INC,
    FRG_AST_KIND_VALUE_SUB_ASSIGN,
    FRG_AST_KIND_VALUE_DEC,
    FRG_AST_KIND_VALUE_MUL_ASSIGN,
    FRG_AST_KIND_VALUE_DIV_ASSIGN,
    FRG_AST_KIND_VALUE_DIV_INT_ASSIGN,
    FRG_AST_KIND_VALUE_MOD_ASSIGN,
    FRG_AST_KIND_VALUE_EXP_ASSIGN,
    FRG_AST_KIND_VALUE_LOG_AND_ASSIGN,
    FRG_AST_KIND_VALUE_LOG_OR_ASSIGN,

    FRG_AST_KIND_FIRST = FRG_AST_KIND_TY_BOOL,
    FRG_AST_KIND_LAST = FRG_AST_KIND_VALUE_LOG_OR_ASSIGN,
} frg_ast_kind_t;

#define FRG_AST_KIND_COUNT (FRG_AST_KIND_LAST - FRG_AST_KIND_FIRST + 1)

typedef enum {
    FRG_AST_DECL_PROP_FLAG_NONE = 0x0,
    FRG_AST_DECL_PROP_FLAG_OPTIONAL = 0x1,
    FRG_AST_DECL_PROP_FLAG_NON_OPTIONAL = 0x2,
    FRG_AST_DECL_PROP_FLAG_SPREAD = 0x4
} frg_ast_decl_prop_flags_t;

GString* frg_ast_decl_prop_flags_to_string(frg_ast_decl_prop_flags_t flags);

typedef enum {
    FRG_AST_DECL_IFACE_FLAG_NONE = 0x0,
    FRG_AST_DECL_IFACE_FLAG_ABSTRACT = 0x1,
} frg_ast_decl_iface_flags_t;

GString* frg_ast_decl_iface_flags_to_string(frg_ast_decl_iface_flags_t flags);

typedef enum {
    FRG_AST_DECL_FN_ARG_FLAG_NONE = 0x0,
    FRG_AST_DECL_FN_ARG_FLAG_KW = 0x1,
} frg_ast_decl_fn_arg_flags_t;

GString* frg_ast_decl_fn_arg_flags_to_string(frg_ast_decl_fn_arg_flags_t flags);

typedef enum {
    FRG_AST_DECL_FN_FLAG_NONE = 0x0,
    FRG_AST_DECL_FN_FLAG_MUT = 0x1,
    FRG_AST_DECL_FN_FLAG_OVERRIDE = 0x2,
} frg_ast_decl_fn_flags_t;

GString* frg_ast_decl_fn_flags_to_string(frg_ast_decl_fn_flags_t flags);

typedef enum {
    FRG_AST_VISITOR_STATUS_OK,
    FRG_AST_VISITOR_STATUS_SKIP,
    FRG_AST_VISITOR_STATUS_STOP,
} frg_ast_visitor_status_t;

typedef enum {
    FRG_STREAM_INPUT_FLAG_NONE = 0x0,
    FRG_STREAM_INPUT_FLAG_OWNED = 0x1,
    FRG_STREAM_INPUT_FLAG_LENGTH_SET = 0x2,
    FRG_STREAM_INPUT_FLAG_EXTRA_NULL_BYTE = 0x4,
} frg_stream_input_flags_t;

typedef enum {
    FRG_STREAM_OUTPUT_FLAG_NONE = 0x0,
    FRG_STREAM_OUTPUT_FLAG_OWNED = 0x1,
    FRG_STREAM_OUTPUT_FLAG_COLOR = 0x2,
    FRG_STREAM_OUTPUT_FLAG_DETECT_COLOR = 0x4,
    FRG_STREAM_OUTPUT_FLAG_UNICODE = 0x8,
    FRG_STREAM_OUTPUT_FLAG_DETECT_UNICODE = 0x10,
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
} frg_stream_output_color_t;

typedef enum {
    FRG_LINKER_KIND_NONE,
    FRG_LINKER_KIND_LD_LLD,
    FRG_LINKER_KIND_LD64_LLD,
    FRG_LINKER_KIND_LLD_LINK,
} frg_linker_kind_t;

typedef enum {
    FRG_LINKER_MODE_EXECUTABLE,
    FRG_LINKER_MODE_SHARED_LIBRARY,
    FRG_LINKER_MODE_STATIC_LIBRARY,
} frg_linker_mode_t;
