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

#include <forge/verification/visitor/type_verification.h>
#include <forge/ast/kind_info.h>
#include <forge/common/error.h>

frg_ast_visitor_status_t frg_verification_type_verification_callback_ty_symbol(
    GList* parents,
    frg_ast_t** ast,
    void* user_data
) {
    frg_die_ast_kind_not_yet_supported((*ast)->kind);
}
