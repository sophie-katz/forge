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

#include <stdbool.h>
#include <stdint.h>

typedef int32_t frg_line_number_t;
typedef int32_t frg_column_number_t;
typedef int32_t frg_indentation_t;
typedef float frg_f32_t;
typedef double frg_f64_t;
typedef uint32_t frg_character_t;
typedef uint32_t frg_int_base_t;
typedef uint32_t frg_bit_width_t;
typedef uint32_t frg_requirement_number_t;
typedef uint32_t frg_proposal_number_t;
typedef uint32_t frg_subitem_number_t;
typedef uint32_t frg_message_count_t;
typedef int32_t frg_recursion_depth_t;
typedef uint32_t frg_ast_node_count_t;

bool frg_is_valid_base(frg_int_base_t base);

bool frg_is_valid_bit_width_int(frg_bit_width_t bit_width);

bool frg_is_valid_bit_width_float(frg_bit_width_t bit_width);
