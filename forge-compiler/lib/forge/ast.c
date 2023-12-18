#include <stddef.h>
#include <stdlib.h>
#include <forge/ast.h>

void frg_ast_destroy(frg_ast_t** ast) {
    if (ast == NULL || *ast == NULL) {
        return;
    }

    switch ((*ast)->id) {
        case FRG_AST_ID_VALUE_LOG_NOT:
            frg_ast_value_unary_destroy((frg_ast_value_unary_t**)&(*ast)->data);
            break;
        case FRG_AST_ID_VALUE_LOG_AND:
        case FRG_AST_ID_VALUE_LOG_OR:
            frg_ast_value_binary_destroy((frg_ast_value_binary_t**)&(*ast)->data);
            break;
        default:
            break;
    }

    free(*ast);
    *ast = NULL;
}

frg_ast_t* _frg_ast_new_with_data(frg_ast_id_t id, void* data) {
    frg_ast_t* ast = malloc(sizeof(frg_ast_t));
    ast->id = id;
    ast->data = data;
    return ast;
}

frg_ast_t* frg_ast_new_value_true(void) {
    return _frg_ast_new_with_data(FRG_AST_ID_VALUE_TRUE, NULL);
}

frg_ast_t* frg_ast_new_value_false(void) {
    return _frg_ast_new_with_data(FRG_AST_ID_VALUE_FALSE, NULL);
}

frg_ast_t* frg_ast_new_value_log_not(frg_ast_t* operand) {
    frg_ast_value_unary_t* data = malloc(sizeof(frg_ast_value_unary_t));
    data->operand = operand;
    return _frg_ast_new_with_data(FRG_AST_ID_VALUE_LOG_NOT, data);
}

void frg_ast_value_unary_destroy(frg_ast_value_unary_t** ast_value_unary) {
    if (ast_value_unary == NULL || *ast_value_unary == NULL) {
        return;
    }

    frg_ast_destroy(&(*ast_value_unary)->operand);
    free(*ast_value_unary);
    *ast_value_unary = NULL;
}

frg_ast_value_unary_t* frg_ast_as_value_unary(frg_ast_t* ast) {
    if (ast == NULL) {
        return NULL;
    }

    switch (ast->id) {
        case FRG_AST_ID_VALUE_LOG_NOT:
            return ast->data;
        default:
            return NULL;
    }
}

frg_ast_t* frg_ast_new_value_log_and(frg_ast_t* lhs, frg_ast_t* rhs) {
    frg_ast_value_binary_t* data = malloc(sizeof(frg_ast_value_binary_t));
    data->lhs = lhs;
    data->rhs = rhs;
    return _frg_ast_new_with_data(FRG_AST_ID_VALUE_LOG_AND, data);
}

frg_ast_t* frg_ast_new_value_log_or(frg_ast_t* lhs, frg_ast_t* rhs) {
    frg_ast_value_binary_t* data = malloc(sizeof(frg_ast_value_binary_t));
    data->lhs = lhs;
    data->rhs = rhs;
    return _frg_ast_new_with_data(FRG_AST_ID_VALUE_LOG_OR, data);
}

void frg_ast_value_binary_destroy(frg_ast_value_binary_t** ast_value_binary) {
    if (ast_value_binary == NULL || *ast_value_binary == NULL) {
        return;
    }

    frg_ast_destroy(&(*ast_value_binary)->lhs);
    frg_ast_destroy(&(*ast_value_binary)->rhs);
    free(*ast_value_binary);
    *ast_value_binary = NULL;
}

frg_ast_value_binary_t* frg_ast_as_value_binary(frg_ast_t* ast) {
    if (ast == NULL) {
        return NULL;
    }

    switch (ast->id) {
        case FRG_AST_ID_VALUE_LOG_AND:
        case FRG_AST_ID_VALUE_LOG_OR:
            return ast->data;
        default:
            return NULL;
    }
}
