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

#include <forge/ast/type_resolution.h>

frg_status_t frg_ast_resolve_value_type(
    frg_ast_t** type,
    frg_ast_scope_t* scope,
    frg_ast_t* value
) {
    if (type == NULL || scope == NULL || value == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    } else if (*type != NULL) {
        return FRG_STATUS_ERROR_UNEXPECTED_ARGUMENT_VALUE;
    }

    frg_ast_value_int_t* value_int = NULL;
    frg_ast_value_float_t* value_float = NULL;
    frg_ast_value_symbol_t* value_symbol = NULL;
    frg_ast_t* scope_ast = NULL;

    switch (value->id) {
        case FRG_AST_ID_VALUE_TRUE:
        case FRG_AST_ID_VALUE_FALSE:
            frg_check(
                frg_ast_new_ty_primary(type, FRG_AST_ID_TY_BOOL)
            );
            break;
        case FRG_AST_ID_VALUE_INT:
            value_int = (frg_ast_value_int_t*)value;

            frg_check(
                frg_ast_new_ty_primary(type, value_int->ty->id)
            );
            break;
        case FRG_AST_ID_VALUE_FLOAT:
            value_float = (frg_ast_value_float_t*)value;

            frg_check(
                frg_ast_new_ty_primary(type, value_float->ty->id)
            );
            break;
        case FRG_AST_ID_VALUE_SYMBOL:
            value_symbol = (frg_ast_value_symbol_t*)value;

            frg_check(
                frg_ast_scope_get_ast(&scope_ast, scope, value_symbol->name->str)
            );
            break;
        default:
            return FRG_STATUS_ERROR_UNEXPECTED_ENUM_VALUE;
    }

    return FRG_STATUS_OK;
}
