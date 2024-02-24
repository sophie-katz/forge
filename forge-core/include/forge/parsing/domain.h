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

#include <forge/types.h>
#include <stddef.h>

typedef struct {
  const char* path;
  size_t offset;
  frg_line_number_t line_number;
  frg_column_number_t column_number;
} frg_parsing_location_t;

typedef struct {
  frg_parsing_location_t start;
  frg_column_number_t length;
} frg_parsing_range_t;

extern const frg_parsing_range_t frg_global_parsing_range_null;

frg_parsing_range_t frg_parsing_range_get_span(const frg_parsing_range_t* first,
                                               const frg_parsing_range_t* last);
