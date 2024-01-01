// Copyright (c) 2023 Sophie Katz
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

/// A status result to be returned by functions in Forge
typedef enum {
    /// Function completed successfully
    FRG_STATUS_OK,
    /// An unexpected null argument was passed into the function
    FRG_STATUS_ERROR_NULL_ARGUMENT,
    /// An unexpected value was passed into the function
    FRG_STATUS_ERROR_UNEXPECTED_ARGUMENT_VALUE,
    /// An unexpected enum variant was used
    FRG_STATUS_ERROR_UNEXPECTED_ENUM_VALUE,
    /// A string was unexpectedly empty
    FRG_STATUS_ERROR_EMPTY_STRING,
    /// \brief A file is being accessed which has its error bit set
    ///
    /// This is detected with \c ferror
    FRG_STATUS_ERROR_FILE_ERROR_BIT_SET,
    /// A memory allocation failed
    FRG_STATUS_ERROR_OUT_OF_MEMORY,
    /// Parsing reached out of text unexpectedly
    FRG_STATUS_ERROR_UNEXPECTED_END_OF_TEXT,
    /// An unexpected character was encountered while parsing text
    FRG_STATUS_ERROR_UNEXPECTED_CHARACTER,
    /// A duplicate entry or object was found
    FRG_STATUS_ERROR_DUPLICATE,
    FRG_STATUS_ERROR_KEY_NOT_FOUND,
    FRG_STATUS_CLI_NO_MORE_ARGUMENTS,
    FRG_STATUS_CLI_REACHED_POSITIONAL_ARGUMENT,
    FRG_STATUS_CLI_ERROR
} frg_status_t;

const char* frg_status_to_string(frg_status_t status);

/// A mode for whether or not to use color for console output
typedef enum {
    /// Do not use color output
    FRG_COLOR_MODE_DISABLED,
    /// Use color if it is available, but otherwise disable it
    FRG_COLOR_MODE_AUTO,
    /// Force color output no matter what
    FRG_COLOR_MODE_ENABLED,
} frg_color_mode_t;

/// An identifier for which color code to output
///
/// Codes are a subset of https://gist.github.com/fnky/458719343aabd01cfb17a3a4f7296797
/// that will actually get used by the Forge compiler.
typedef enum {
    FRG_COLOR_ID_RESET,
    FRG_COLOR_ID_BOLD,
    FRG_COLOR_ID_DIM,
    FRG_COLOR_ID_ITALIC,
    FRG_COLOR_ID_UNDERLINE,
    FRG_COLOR_ID_BLACK,
    FRG_COLOR_ID_RED,
    FRG_COLOR_ID_GREEN,
    FRG_COLOR_ID_YELLOW,
    FRG_COLOR_ID_BLUE,
    FRG_COLOR_ID_MAGENTA,
    FRG_COLOR_ID_CYAN,
    FRG_COLOR_ID_WHITE,
    FRG_COLOR_ID_BRIGHT_BLACK,
    FRG_COLOR_ID_BRIGHT_RED,
    FRG_COLOR_ID_BRIGHT_GREEN,
    FRG_COLOR_ID_BRIGHT_YELLOW,
    FRG_COLOR_ID_BRIGHT_BLUE,
    FRG_COLOR_ID_BRIGHT_MAGENTA,
    FRG_COLOR_ID_BRIGHT_CYAN,
    FRG_COLOR_ID_BRIGHT_WHITE,
} frg_color_id_t;

typedef enum {
    FRG_LOG_SEVERITY_TRACE,
    FRG_LOG_SEVERITY_DEBUG,
    FRG_LOG_SEVERITY_NOTE,
    FRG_LOG_SEVERITY_WARNING,
    FRG_LOG_SEVERITY_ERROR,
    FRG_LOG_SEVERITY_FATAL_ERROR,
    FRG_LOG_SEVERITY_INTERNAL_ERROR,
} frg_log_severity_t;

/// Different types of values according to the interpreter
typedef enum {
    FRG_EXEC_TYPE_ID_BOOL,
} frg_exec_type_id_t;

typedef enum {
    FRG_AST_ID_TY_BOOL,
    FRG_AST_ID_TY_U8,
    FRG_AST_ID_TY_U16,
    FRG_AST_ID_TY_U32,
    FRG_AST_ID_TY_U64,
    FRG_AST_ID_TY_I8,
    FRG_AST_ID_TY_I16,
    FRG_AST_ID_TY_I32,
    FRG_AST_ID_TY_I64,
    FRG_AST_ID_TY_F32,
    FRG_AST_ID_TY_F64,
    FRG_AST_ID_TY_SYMBOL,
    FRG_AST_ID_TY_POINTER,
    FRG_AST_ID_DECL_UNION,
    FRG_AST_ID_DECL_STRUCT,
    FRG_AST_ID_DECL_PROP,
    FRG_AST_ID_DECL_IFACE,
    FRG_AST_ID_DECL_FN_ARG,
    FRG_AST_ID_DECL_FN,
    FRG_AST_ID_DECL_VAR,
    FRG_AST_ID_DECL_BLOCK,
    FRG_AST_ID_STMT_RETURN,
    FRG_AST_ID_STMT_IF,
    FRG_AST_ID_STMT_WHILE,
    FRG_AST_ID_STMT_BLOCK,
    FRG_AST_ID_VALUE_TRUE,
    FRG_AST_ID_VALUE_FALSE,
    FRG_AST_ID_VALUE_INT,
    FRG_AST_ID_VALUE_FLOAT,
    FRG_AST_ID_VALUE_CHAR,
    FRG_AST_ID_VALUE_STR,
    FRG_AST_ID_VALUE_SYMBOL,
    FRG_AST_ID_VALUE_DEREF,
    FRG_AST_ID_VALUE_GETADDR,
    FRG_AST_ID_VALUE_CALL_KW_ARG,
    FRG_AST_ID_VALUE_CALL,
    FRG_AST_ID_VALUE_ACCESS,
    FRG_AST_ID_VALUE_BIT_NOT,
    FRG_AST_ID_VALUE_BIT_AND,
    FRG_AST_ID_VALUE_BIT_OR,
    FRG_AST_ID_VALUE_BIT_XOR,
    FRG_AST_ID_VALUE_BIT_SHL,
    FRG_AST_ID_VALUE_BIT_SHR,
    FRG_AST_ID_VALUE_NEG,
    FRG_AST_ID_VALUE_ADD,
    FRG_AST_ID_VALUE_SUB,
    FRG_AST_ID_VALUE_MUL,
    FRG_AST_ID_VALUE_DIV,
    FRG_AST_ID_VALUE_DIV_INT,
    FRG_AST_ID_VALUE_MOD,
    FRG_AST_ID_VALUE_EXP,
    FRG_AST_ID_VALUE_EQ,
    FRG_AST_ID_VALUE_NE,
    FRG_AST_ID_VALUE_LT,
    FRG_AST_ID_VALUE_LE,
    FRG_AST_ID_VALUE_GT,
    FRG_AST_ID_VALUE_GE,
    FRG_AST_ID_VALUE_LOG_NOT,
    FRG_AST_ID_VALUE_LOG_AND,
    FRG_AST_ID_VALUE_LOG_OR,
    FRG_AST_ID_VALUE_ASSIGN,
    FRG_AST_ID_VALUE_BIT_AND_ASSIGN,
    FRG_AST_ID_VALUE_BIT_OR_ASSIGN,
    FRG_AST_ID_VALUE_BIT_XOR_ASSIGN,
    FRG_AST_ID_VALUE_BIT_SHL_ASSIGN,
    FRG_AST_ID_VALUE_BIT_SHR_ASSIGN,
    FRG_AST_ID_VALUE_ADD_ASSIGN,
    FRG_AST_ID_VALUE_INC,
    FRG_AST_ID_VALUE_SUB_ASSIGN,
    FRG_AST_ID_VALUE_DEC,
    FRG_AST_ID_VALUE_MUL_ASSIGN,
    FRG_AST_ID_VALUE_DIV_ASSIGN,
    FRG_AST_ID_VALUE_DIV_INT_ASSIGN,
    FRG_AST_ID_VALUE_MOD_ASSIGN,
    FRG_AST_ID_VALUE_EXP_ASSIGN,
    FRG_AST_ID_VALUE_LOG_AND_ASSIGN,
    FRG_AST_ID_VALUE_LOG_OR_ASSIGN,
} frg_ast_id_t;

const char* frg_ast_id_to_string(frg_ast_id_t id);
bool frg_ast_id_is_ty_primary(frg_ast_id_t id);
bool frg_ast_id_is_value_primary(frg_ast_id_t id);
bool frg_ast_id_is_value_unary(frg_ast_id_t id);
bool frg_ast_id_is_value_binary(frg_ast_id_t id);

typedef enum {
    FRG_AST_DECL_PROP_FLAG_NONE = 0x0,
    FRG_AST_DECL_PROP_FLAG_OPTIONAL = 0x1,
    FRG_AST_DECL_PROP_FLAG_NON_OPTIONAL = 0x2,
    FRG_AST_DECL_PROP_FLAG_SPREAD = 0x4
} frg_ast_decl_prop_flags_t;

void frg_ast_decl_prop_flags_print(FILE* file, frg_ast_decl_prop_flags_t flags);

typedef enum {
    FRG_AST_DECL_IFACE_FLAG_NONE = 0x0,
    FRG_AST_DECL_IFACE_FLAG_ABSTRACT = 0x1,
} frg_ast_decl_iface_flags_t;

void frg_ast_decl_iface_flags_print(FILE* file, frg_ast_decl_iface_flags_t flags);

typedef enum {
    FRG_AST_DECL_FN_ARG_FLAG_NONE = 0x0,
    FRG_AST_DECL_FN_ARG_FLAG_KW = 0x1,
} frg_ast_decl_fn_arg_flags_t;

void frg_ast_decl_fn_arg_flags_print(FILE* file, frg_ast_decl_fn_arg_flags_t flags);

typedef enum {
    FRG_AST_DECL_FN_FLAG_NONE = 0x0,
    FRG_AST_DECL_FN_FLAG_MUT = 0x1,
    FRG_AST_DECL_FN_FLAG_OVERRIDE = 0x2,
} frg_ast_decl_fn_flags_t;

void frg_ast_decl_fn_flags_print(FILE* file, frg_ast_decl_fn_flags_t flags);
