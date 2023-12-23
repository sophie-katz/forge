#include <forge/exec.h>

frg_value_t* frg_value_new_bool(bool data) {
    frg_value_t* value = malloc(sizeof(frg_value_t));
    value->type_id = FRG_EXEC_TYPE_ID_BOOL;
    value->data.as_bool = data;
    return value;
}

void frg_value_destroy(frg_value_t** value) {
    if (value == NULL || *value == NULL) {
        return;
    }

    free(*value);
    *value = NULL;
}

bool frg_value_is_bool(const frg_value_t* value) {
    return value->type_id == FRG_EXEC_TYPE_ID_BOOL;
}

bool frg_value_get_bool(const frg_value_t* value) {
    return value->data.as_bool;
}

void frg_value_set_bool(frg_value_t* value, bool data) {
    value->data.as_bool = data;
}

void frg_value_print(FILE* file, const frg_value_t* value) {
    switch (value->type_id) {
        case FRG_EXEC_TYPE_ID_BOOL:
            fprintf(file, "%s", value->data.as_bool ? "true" : "false");
            break;
        default:
            fprintf(file, "unknown");
            break;
    }
}

// frg_value_t* frg_exec(const frg_ast_t* ast) {
//     if (ast == NULL) {
//         return NULL;
//     }

//     switch (ast->id) {
//         case FRG_AST_ID_VALUE_TRUE:
//             return frg_value_new_bool(true);
//         case FRG_AST_ID_VALUE_FALSE:
//             return frg_value_new_bool(false);
//         case FRG_AST_ID_VALUE_LOG_NOT: {
//             frg_ast_value_unary_t* data = ast->data;
//             return frg_value_new_bool(!frg_value_get_bool(frg_exec(data->operand)));
//         }
//         case FRG_AST_ID_VALUE_LOG_AND: {
//             frg_ast_value_binary_t* data = ast->data;
//             return frg_value_new_bool(
//                 frg_value_get_bool(frg_exec(data->lhs)) &&
//                 frg_value_get_bool(frg_exec(data->rhs))
//             );
//         }
//         case FRG_AST_ID_VALUE_LOG_OR: {
//             frg_ast_value_binary_t* data = ast->data;
//             return frg_value_new_bool(
//                 frg_value_get_bool(frg_exec(data->lhs)) ||
//                 frg_value_get_bool(frg_exec(data->rhs))
//             );
//         }
//         default:
//             return NULL;
//     }
// }
