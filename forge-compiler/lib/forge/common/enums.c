#include <forge/common/enums.h>

const char* frg_status_to_string(frg_status_t status) {
    switch (status) {
        case FRG_STATUS_OK:
            return "ok";
        case FRG_STATUS_ERROR_NULL_ARGUMENT:
            return "null argument";
        case FRG_STATUS_ERROR_UNEXPECTED_ARGUMENT_VALUE:
            return "unexpected argument value";
        case FRG_STATUS_ERROR_UNEXPECTED_ENUM_VALUE:
            return "unexpected enum value";
        case FRG_STATUS_ERROR_EMPTY_STRING:
            return "empty string";
        default:
            return "(unknown value for frg_status_t)";
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
            return true;
        default:
            return false;
    }
}

bool frg_ast_id_is_value_binary(frg_ast_id_t id) {
    switch (id) {
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
