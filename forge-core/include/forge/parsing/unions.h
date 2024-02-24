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

#include <forge/lexical_casts.h>
#include <forge/parsing/domain.h>

typedef struct {
    frg_parsing_range_t source_range;
    frg_parse_uint_result_t value;
} frg_parsing_union_uint_t;

typedef struct {
    frg_parsing_range_t source_range;
    frg_parse_float_result_t value;
} frg_parsing_union_float_t;

typedef struct {
    frg_parsing_range_t source_range;
    GString* value;
} frg_parsing_union_symbol_t;
