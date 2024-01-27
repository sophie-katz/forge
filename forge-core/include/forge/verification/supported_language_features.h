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

#include <forge/ast/visitor.h>

frg_ast_visitor_status_t frg_verification_supported_language_features_callback_ty_fn(
    GList* parents,
    frg_ast_t** ast,
    void* user_data
);

frg_ast_visitor_status_t frg_verification_supported_language_features_callback_pre_decl_union(
    GList* parents,
    frg_ast_t** ast,
    void* user_data
);

frg_ast_visitor_status_t frg_verification_supported_language_features_callback_pre_decl_prop(
    GList* parents,
    frg_ast_t** ast,
    void* user_data
);

frg_ast_visitor_status_t frg_verification_supported_language_features_callback_pre_decl_iface(
    GList* parents,
    frg_ast_t** ast,
    void* user_data
);

frg_ast_visitor_status_t frg_verification_supported_language_features_callback_pre_decl_fn_arg(
    GList* parents,
    frg_ast_t** ast,
    void* user_data
);

frg_ast_visitor_status_t frg_verification_supported_language_features_callback_pre_decl_fn(
    GList* parents,
    frg_ast_t** ast,
    void* user_data
);

frg_ast_visitor_status_t frg_verification_supported_language_features_callback_pre_value_char(
    GList* parents,
    frg_ast_t** ast,
    void* user_data
);

frg_ast_visitor_status_t frg_verification_supported_language_features_callback_pre_value_str(
    GList* parents,
    frg_ast_t** ast,
    void* user_data
);

frg_ast_visitor_status_t frg_verification_supported_language_features_callback_pre_value_call_kw_arg(
    GList* parents,
    frg_ast_t** ast,
    void* user_data
);

frg_ast_visitor_status_t frg_verification_supported_language_features_callback_pre_value_call(
    GList* parents,
    frg_ast_t** ast,
    void* user_data
);

frg_ast_visitor_status_t frg_verification_supported_language_features_callback_pre_operator(
    GList* parents,
    frg_ast_t** ast,
    void* user_data
);
