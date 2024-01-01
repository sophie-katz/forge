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

#pragma once

#include <forge/ast/ast.h>
#include <forge/common/enums.h>
#include <stdio.h>

/// Parse from a file
frg_status_t frg_parse_file(frg_ast_t** ast, FILE* file, const char* filename);

frg_status_t frg_parse_file_at_path(frg_ast_t** ast, const char* path);

/// Parse from a pre-allocated buffer
frg_status_t frg_parse_buffer(frg_ast_t** ast, char* buffer, size_t length, const char* filename);

/// \brief Parse from a string
///
/// Note that this will copy the string. Use \c frg_parse_buffer if you want to avoid
/// this.
frg_status_t frg_parse_string(frg_ast_t** ast, const char* text, const char* filename);
