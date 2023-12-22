#pragma once

/// A status result to be returned by functions in Forge
typedef enum {
    /// Function completed successfully
    FRG_STATUS_OK = 0,
    /// An unexpected null argument was passed into the function
    FRG_STATUS_ERROR_NULL_ARGUMENT = 1,
    /// An unexpected value was passed into the function
    FRG_STATUS_ERROR_UNEXPECTED_ARGUMENT_VALUE = 2,
    /// An unexpected enum variant was used
    FRG_STATUS_ERROR_UNEXPECTED_ENUM_VALUE = 3,
    /// A string was unexpectedly empty
    FRG_STATUS_ERROR_EMPTY_STRING = 4,
} frg_status_t;

const char* frg_status_to_string(frg_status_t status);

/// A mode for whether or not to use color for console output
typedef enum {
    /// Do not use color output
    FRG_COLOR_MODE_DISABLED = 0,
    /// Use color if it is available, but otherwise disable it
    FRG_COLOR_MODE_AUTO = 1,
    /// Force color output no matter what
    FRG_COLOR_MODE_ENABLED = 2,
} frg_color_mode_t;

/// An identifier for which color code to output
///
/// Codes are a subset of https://gist.github.com/fnky/458719343aabd01cfb17a3a4f7296797
/// that will actually get used by the Forge compiler.
typedef enum {
    FRG_COLOR_ID_RESET = 0,
    FRG_COLOR_ID_BOLD = 1,
    FRG_COLOR_ID_DIM = 2,
    FRG_COLOR_ID_ITALIC = 3,
    FRG_COLOR_ID_UNDERLINE = 4,
    FRG_COLOR_ID_BLACK = 5,
    FRG_COLOR_ID_RED = 6,
    FRG_COLOR_ID_GREEN = 7,
    FRG_COLOR_ID_YELLOW = 8,
    FRG_COLOR_ID_BLUE = 9,
    FRG_COLOR_ID_MAGENTA = 10,
    FRG_COLOR_ID_CYAN = 11,
    FRG_COLOR_ID_WHITE = 12,
    FRG_COLOR_ID_BRIGHT_BLACK = 13,
    FRG_COLOR_ID_BRIGHT_RED = 14,
    FRG_COLOR_ID_BRIGHT_GREEN = 15,
    FRG_COLOR_ID_BRIGHT_YELLOW = 16,
    FRG_COLOR_ID_BRIGHT_BLUE = 17,
    FRG_COLOR_ID_BRIGHT_MAGENTA = 18,
    FRG_COLOR_ID_BRIGHT_CYAN = 19,
    FRG_COLOR_ID_BRIGHT_WHITE = 20,
} frg_color_id_t;

typedef enum {
    FRG_LOG_SEVERITY_NOTE = 0,
    FRG_LOG_SEVERITY_DEBUG = 1,
    FRG_LOG_SEVERITY_WARNING = 2,
    FRG_LOG_SEVERITY_ERROR = 3,
    FRG_LOG_SEVERITY_FATAL_ERROR = 4,
    FRG_LOG_SEVERITY_INTERNAL_ERROR = 5,
} frg_log_severity_t;

/// Different types of values according to the interpreter
typedef enum {
    FRG_EXEC_TYPE_ID_BOOL = 0,
} frg_exec_type_id_t;

typedef enum {
    FRG_AST_ID_PROP = 0,
    FRG_AST_ID_TY_BOOL = 1,
    FRG_AST_ID_TY_U8 = 2,
    FRG_AST_ID_TY_U16 = 3,
    FRG_AST_ID_TY_U32 = 4,
    FRG_AST_ID_TY_U64 = 5,
    FRG_AST_ID_TY_I8 = 6,
    FRG_AST_ID_TY_I16 = 7,
    FRG_AST_ID_TY_I32 = 8,
    FRG_AST_ID_TY_I64 = 9,
    FRG_AST_ID_TY_F32 = 10,
    FRG_AST_ID_TY_F64 = 11,
    FRG_AST_ID_TY_ALIAS = 12,
    FRG_AST_ID_TY_UNION = 13,
    FRG_AST_ID_TY_STRUCT = 14,
    FRG_AST_ID_IFACE = 15,
    FRG_AST_ID_FN_ARG = 16,
    FRG_AST_ID_FN = 17,
    FRG_AST_ID_VAR = 18,
    FRG_AST_ID_RETURN = 19,
    FRG_AST_ID_WHILE = 20,
    FRG_AST_ID_VALUE_TRUE = 21,
    FRG_AST_ID_VALUE_FALSE = 22,
    FRG_AST_ID_VALUE_INT = 23,
    FRG_AST_ID_VALUE_FLOAT = 24,
    FRG_AST_ID_VALUE_CHAR = 25,
    FRG_AST_ID_VALUE_STR = 26,
    FRG_AST_ID_VALUE_BIT_NEG = 27,
    FRG_AST_ID_VALUE_BIT_AND = 28,
    FRG_AST_ID_VALUE_BIT_OR = 29,
    FRG_AST_ID_VALUE_BIT_XOR = 30,
    FRG_AST_ID_VALUE_BIT_SHL = 31,
    FRG_AST_ID_VALUE_BIT_SHR = 32,
    FRG_AST_ID_VALUE_NEG = 33,
    FRG_AST_ID_VALUE_ADD = 34,
    FRG_AST_ID_VALUE_SUB = 35,
    FRG_AST_ID_VALUE_MUL = 36,
    FRG_AST_ID_VALUE_DIV = 37,
    FRG_AST_ID_VALUE_DIV_INT = 38,
    FRG_AST_ID_VALUE_MOD = 39,
    FRG_AST_ID_VALUE_EXP = 40,
    FRG_AST_ID_VALUE_EQ = 41,
    FRG_AST_ID_VALUE_NE = 42,
    FRG_AST_ID_VALUE_LT = 43,
    FRG_AST_ID_VALUE_LE = 44,
    FRG_AST_ID_VALUE_GT = 45,
    FRG_AST_ID_VALUE_GE = 46,
    FRG_AST_ID_VALUE_LOG_NOT = 47,
    FRG_AST_ID_VALUE_LOG_AND = 48,
    FRG_AST_ID_VALUE_LOG_OR = 49,
    FRG_AST_ID_VALUE_FOR = 50,
    FRG_AST_ID_VALUE_IF = 51,
    FRG_AST_ID_VALUE_MATCH = 52,
} frg_ast_id_t;

typedef enum {
    FRG_AST_PROP_FLAG_NONE = 0x0,
    FRG_AST_PROP_FLAG_OPTIONAL = 0x1,
    FRG_AST_PROP_FLAG_NON_OPTIONAL = 0x2,
    FRG_AST_PROP_FLAG_SPREAD = 0x4,
    FRG_AST_PROP_FLAG_CONST = 0x8,
} frg_ast_prop_flags_t;

typedef enum {
    FRG_AST_IFACE_FLAG_NONE = 0x0,
    FRG_AST_IFACE_FLAG_ABSTRACT = 0x1,
} frg_ast_iface_flags_t;

typedef enum {
    FRG_AST_FN_ARG_FLAG_NONE = 0x0,
    FRG_AST_FN_ARG_FLAG_CONST = 0x1,
    FRG_AST_FN_ARG_FLAG_MUT = 0x2,
    FRG_AST_FN_ARG_FLAG_KW = 0x4,
} frg_ast_fn_arg_flags_t;

typedef enum {
    FRG_AST_FN_FLAG_NONE = 0x0,
    FRG_AST_FN_FLAG_CONST = 0x1,
    FRG_AST_FN_FLAG_MUT = 0x2,
    FRG_AST_FN_FLAG_OVERRIDE = 0x4,
} frg_ast_fn_flags_t;

typedef enum {
    FRG_AST_VAR_FLAG_NONE = 0x0,
    FRG_AST_VAR_FLAG_CONST = 0x1,
    FRG_AST_VAR_FLAG_MUT = 0x2,
} frg_ast_var_flags_t;
