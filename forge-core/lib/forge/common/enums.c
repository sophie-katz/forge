// Copyright (c) 2024 Sophie Katz
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

#include <forge/common/enums.h>
#include <forge/common/error.h>

// const char* frg_recoverable_status_to_string(frg_recoverable_status_t status) {
//     switch (status) {
//         case FRG_RECOVERABLE_STATUS_OK:
//             return "ok";
//         case FRG_RECOVERABLE_STATUS_ERROR_WAS_LOGGED:
//             return "error has already been logged";
//         case FRG_RECOVERABLE_STATUS_ERROR_UNEXPECTED_END_OF_TEXT:
//             return "unexpected end of text";
//         case FRG_RECOVERABLE_STATUS_ERROR_UNEXPECTED_CHARACTER:
//             return "unexpected character";
//         default:
//             frg_die_unexpected_enum_value(status);
//     }
// }

const char* frg_ast_id_to_string(frg_ast_id_t id) {
    switch (id) {
        case FRG_AST_ID_TY_BOOL:
            return "ty-bool";
        case FRG_AST_ID_TY_U8:
            return "ty-u8";
        case FRG_AST_ID_TY_U16:
            return "ty-u16";
        case FRG_AST_ID_TY_U32:
            return "ty-u32";
        case FRG_AST_ID_TY_U64:
            return "ty-u64";
        case FRG_AST_ID_TY_I8:
            return "ty-i8";
        case FRG_AST_ID_TY_I16:
            return "ty-i16";
        case FRG_AST_ID_TY_I32:
            return "ty-i32";
        case FRG_AST_ID_TY_I64:
            return "ty-i64";
        case FRG_AST_ID_TY_F32:
            return "ty-f32";
        case FRG_AST_ID_TY_F64:
            return "ty-f64";
        case FRG_AST_ID_TY_SYMBOL:
            return "ty-symbol";
        case FRG_AST_ID_TY_POINTER:
            return "ty-pointer";
        case FRG_AST_ID_TY_FN:
            return "ty-fn";
        case FRG_AST_ID_DECL_UNION:
            return "decl-union";
        case FRG_AST_ID_DECL_STRUCT:
            return "decl-struct";
        case FRG_AST_ID_DECL_PROP:
            return "decl-prop";
        case FRG_AST_ID_DECL_IFACE:
            return "decl-iface";
        case FRG_AST_ID_DECL_FN_ARG:
            return "decl-fn-arg";
        case FRG_AST_ID_DECL_FN:
            return "decl-fn";
        case FRG_AST_ID_DECL_VAR:
            return "decl-var";
        case FRG_AST_ID_DECL_BLOCK:
            return "decl-block";
        case FRG_AST_ID_STMT_RETURN:
            return "stmt-return";
        case FRG_AST_ID_STMT_IF:
            return "stmt-if";
        case FRG_AST_ID_STMT_WHILE:
            return "stmt-while";
        case FRG_AST_ID_STMT_BLOCK:
            return "stmt-block";
        case FRG_AST_ID_VALUE_TRUE:
            return "value-true";
        case FRG_AST_ID_VALUE_FALSE:
            return "value-false";
        case FRG_AST_ID_VALUE_INT:
            return "value-int";
        case FRG_AST_ID_VALUE_FLOAT:
            return "value-float";
        case FRG_AST_ID_VALUE_CHAR:
            return "value-char";
        case FRG_AST_ID_VALUE_STR:
            return "value-str";
        case FRG_AST_ID_VALUE_SYMBOL:
            return "value-symbol";
        case FRG_AST_ID_VALUE_DEREF:
            return "value-deref";
        case FRG_AST_ID_VALUE_GETADDR:
            return "value-getaddr";
        case FRG_AST_ID_VALUE_CALL_KW_ARG:
            return "value-call-kw-arg";
        case FRG_AST_ID_VALUE_CALL:
            return "value-call";
        case FRG_AST_ID_VALUE_ACCESS:
            return "value-access";
        case FRG_AST_ID_VALUE_BIT_NOT:
            return "value-bit-not";
        case FRG_AST_ID_VALUE_BIT_AND:
            return "value-bit-and";
        case FRG_AST_ID_VALUE_BIT_OR:
            return "value-bit-or";
        case FRG_AST_ID_VALUE_BIT_XOR:
            return "value-bit-xor";
        case FRG_AST_ID_VALUE_BIT_SHL:
            return "value-bit-shl";
        case FRG_AST_ID_VALUE_BIT_SHR:
            return "value-bit-shr";
        case FRG_AST_ID_VALUE_NEG:
            return "value-neg";
        case FRG_AST_ID_VALUE_ADD:
            return "value-add";
        case FRG_AST_ID_VALUE_SUB:
            return "value-sub";
        case FRG_AST_ID_VALUE_MUL:
            return "value-mul";
        case FRG_AST_ID_VALUE_DIV:
            return "value-div";
        case FRG_AST_ID_VALUE_DIV_INT:
            return "value-div-int";
        case FRG_AST_ID_VALUE_MOD:
            return "value-mod";
        case FRG_AST_ID_VALUE_EXP:
            return "value-exp";
        case FRG_AST_ID_VALUE_EQ:
            return "value-eq";
        case FRG_AST_ID_VALUE_NE:
            return "value-ne";
        case FRG_AST_ID_VALUE_LT:
            return "value-lt";
        case FRG_AST_ID_VALUE_LE:
            return "value-le";
        case FRG_AST_ID_VALUE_GT:
            return "value-gt";
        case FRG_AST_ID_VALUE_GE:
            return "value-ge";
        case FRG_AST_ID_VALUE_LOG_NOT:
            return "value-log-not";
        case FRG_AST_ID_VALUE_LOG_AND:
            return "value-log-and";
        case FRG_AST_ID_VALUE_LOG_OR:
            return "value-log-or";
        case FRG_AST_ID_VALUE_ASSIGN:
            return "value-assign";
        case FRG_AST_ID_VALUE_BIT_AND_ASSIGN:
            return "value-bit-and-assign";
        case FRG_AST_ID_VALUE_BIT_OR_ASSIGN:
            return "value-bit-or-assign";
        case FRG_AST_ID_VALUE_BIT_XOR_ASSIGN:
            return "value-bit-xor-assign";
        case FRG_AST_ID_VALUE_BIT_SHL_ASSIGN:
            return "value-bit-shl-assign";
        case FRG_AST_ID_VALUE_BIT_SHR_ASSIGN:
            return "value-bit-shr-assign";
        case FRG_AST_ID_VALUE_ADD_ASSIGN:
            return "value-add-assign";
        case FRG_AST_ID_VALUE_INC:
            return "value-inc";
        case FRG_AST_ID_VALUE_SUB_ASSIGN:
            return "value-sub-assign";
        case FRG_AST_ID_VALUE_DEC:
            return "value-dec";
        case FRG_AST_ID_VALUE_MUL_ASSIGN:
            return "value-mul-assign";
        case FRG_AST_ID_VALUE_DIV_ASSIGN:
            return "value-div-assign";
        case FRG_AST_ID_VALUE_DIV_INT_ASSIGN:
            return "value-div-int-assign";
        case FRG_AST_ID_VALUE_MOD_ASSIGN:
            return "value-mod-assign";
        case FRG_AST_ID_VALUE_EXP_ASSIGN:
            return "value-exp-assign";
        case FRG_AST_ID_VALUE_LOG_AND_ASSIGN:
            return "value-log-and-assign";
        case FRG_AST_ID_VALUE_LOG_OR_ASSIGN:
            return "value-log-or-assign";
        default:
            return "(unknown value for frg_ast_id_t)";
    }
}

bool frg_ast_id_is_ty_primary(frg_ast_id_t id) {
    switch (id) {
        case FRG_AST_ID_TY_BOOL:
        case FRG_AST_ID_TY_U8:
        case FRG_AST_ID_TY_U16:
        case FRG_AST_ID_TY_U32:
        case FRG_AST_ID_TY_U64:
        case FRG_AST_ID_TY_I8:
        case FRG_AST_ID_TY_I16:
        case FRG_AST_ID_TY_I32:
        case FRG_AST_ID_TY_I64:
        case FRG_AST_ID_TY_F32:
        case FRG_AST_ID_TY_F64:
            return true;
        default:
            return false;
    }
}

bool frg_ast_id_is_value_primary(frg_ast_id_t id) {
    switch (id) {
        case FRG_AST_ID_VALUE_TRUE:
        case FRG_AST_ID_VALUE_FALSE:
            return true;
        default:
            return false;
    }
}

bool frg_ast_id_is_value_unary(frg_ast_id_t id) {
    switch (id) {
        case FRG_AST_ID_VALUE_BIT_NOT:
        case FRG_AST_ID_VALUE_NEG:
        case FRG_AST_ID_VALUE_LOG_NOT:
        case FRG_AST_ID_VALUE_INC:
        case FRG_AST_ID_VALUE_DEC:
        case FRG_AST_ID_VALUE_DEREF:
        case FRG_AST_ID_VALUE_GETADDR:
            return true;
        default:
            return false;
    }
}

bool frg_ast_id_is_value_binary(frg_ast_id_t id) {
    switch (id) {
        case FRG_AST_ID_VALUE_ACCESS:
        case FRG_AST_ID_VALUE_BIT_AND:
        case FRG_AST_ID_VALUE_BIT_OR:
        case FRG_AST_ID_VALUE_BIT_XOR:
        case FRG_AST_ID_VALUE_BIT_SHL:
        case FRG_AST_ID_VALUE_BIT_SHR:
        case FRG_AST_ID_VALUE_ADD:
        case FRG_AST_ID_VALUE_SUB:
        case FRG_AST_ID_VALUE_MUL:
        case FRG_AST_ID_VALUE_DIV:
        case FRG_AST_ID_VALUE_DIV_INT:
        case FRG_AST_ID_VALUE_MOD:
        case FRG_AST_ID_VALUE_EXP:
        case FRG_AST_ID_VALUE_EQ:
        case FRG_AST_ID_VALUE_NE:
        case FRG_AST_ID_VALUE_LT:
        case FRG_AST_ID_VALUE_LE:
        case FRG_AST_ID_VALUE_GT:
        case FRG_AST_ID_VALUE_GE:
        case FRG_AST_ID_VALUE_LOG_AND:
        case FRG_AST_ID_VALUE_LOG_OR:
        case FRG_AST_ID_VALUE_ASSIGN:
        case FRG_AST_ID_VALUE_BIT_AND_ASSIGN:
        case FRG_AST_ID_VALUE_BIT_OR_ASSIGN:
        case FRG_AST_ID_VALUE_BIT_XOR_ASSIGN:
        case FRG_AST_ID_VALUE_BIT_SHL_ASSIGN:
        case FRG_AST_ID_VALUE_BIT_SHR_ASSIGN:
        case FRG_AST_ID_VALUE_ADD_ASSIGN:
        case FRG_AST_ID_VALUE_SUB_ASSIGN:
        case FRG_AST_ID_VALUE_MUL_ASSIGN:
        case FRG_AST_ID_VALUE_DIV_ASSIGN:
        case FRG_AST_ID_VALUE_DIV_INT_ASSIGN:
        case FRG_AST_ID_VALUE_MOD_ASSIGN:
        case FRG_AST_ID_VALUE_EXP_ASSIGN:
        case FRG_AST_ID_VALUE_LOG_AND_ASSIGN:
        case FRG_AST_ID_VALUE_LOG_OR_ASSIGN:
            return true;
        default:
            return false;
    }
}

GString* frg_ast_decl_prop_flags_to_string(frg_ast_decl_prop_flags_t flags) {
    GString* result = g_string_new(NULL);
    bool first = true;

    if (flags & FRG_AST_DECL_PROP_FLAG_OPTIONAL) {
        if (!first) {
            g_string_append_printf(result, ",");
        }

        g_string_append_printf(result, "optional");

        first = false;
    }

    if (flags & FRG_AST_DECL_PROP_FLAG_NON_OPTIONAL) {
        if (!first) {
            g_string_append_printf(result, ",");
        }

        g_string_append_printf(result, "non-optional");

        first = false;
    }

    if (flags & FRG_AST_DECL_PROP_FLAG_SPREAD) {
        if (!first) {
            g_string_append_printf(result, ",");
        }

        g_string_append_printf(result, "spread");

        first = false;
    }

    if (first) {
        g_string_append_printf(result, "none");
    }

    return result;
}

GString* frg_ast_decl_iface_flags_to_string(frg_ast_decl_iface_flags_t flags) {
    GString* result = g_string_new(NULL);
    bool first = true;

    if (flags & FRG_AST_DECL_IFACE_FLAG_ABSTRACT) {
        if (!first) {
            g_string_append_printf(result, ",");
        }

        g_string_append_printf(result, "abstract");

        first = false;
    }

    if (first) {
        g_string_append_printf(result, "none");
    }

    return result;
}

GString* frg_ast_decl_fn_arg_flags_to_string(frg_ast_decl_fn_arg_flags_t flags) {
    GString* result = g_string_new(NULL);
    bool first = true;

    if (flags & FRG_AST_DECL_FN_ARG_FLAG_KW) {
        if (!first) {
            g_string_append_printf(result, ",");
        }

        g_string_append_printf(result, "kw");

        first = false;
    }

    if (first) {
        g_string_append_printf(result, "none");
    }

    return result;
}

GString* frg_ast_decl_fn_flags_to_string(frg_ast_decl_fn_flags_t flags) {
    GString* result = g_string_new(NULL);
    bool first = true;

    if (flags & FRG_AST_DECL_FN_FLAG_MUT) {
        if (!first) {
            g_string_append_printf(result, ",");
        }

        g_string_append_printf(result, "mut");

        first = false;
    }

    if (flags & FRG_AST_DECL_FN_FLAG_OVERRIDE) {
        if (!first) {
            g_string_append_printf(result, ",");
        }

        g_string_append_printf(result, "override");

        first = false;
    }

    if (first) {
        g_string_append_printf(result, "none");
    }

    return result;
}
