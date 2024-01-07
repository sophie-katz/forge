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

#include <forge/common/types.h>
#include <stddef.h>

#define FRG_DEBUG_DEFAULT_STREAM stdout
#define FRG_DEBUG_INDENT_WIDTH 2

void frg_debug_print_newline(frg_indent_t indent);
void frg_debug_print_node(const char* format, ...);
void frg_debug_print_property(const char* label, const char* format, ...);
void frg_debug_print_property_with_index(
    const char* label,
    size_t index,
    const char* format,
    ...
);
