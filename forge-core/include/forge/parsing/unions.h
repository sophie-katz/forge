// This program is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software Foundation,
// either version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
// PARTICULAR PURPOSE. See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with this
// program. If not, see <https://www.gnu.org/licenses/>.
//
// Copyright 2023-2024 Sophie Katz

#pragma once

#include <forge/lexical_casts/float.h>
#include <forge/lexical_casts/uint.h>
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
