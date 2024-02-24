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

#include <forge/types.h>

bool frg_is_valid_base(frg_int_base_t base) {
    return base == 2 || base == 8 || base == 10 || base == 16;
}

bool frg_is_valid_bit_width_int(frg_bit_width_t bit_width) {
    return bit_width == 8 || bit_width == 16 || bit_width == 32 || bit_width == 64;
}

bool frg_is_valid_bit_width_float(frg_bit_width_t bit_width) {
    return bit_width == 32 || bit_width == 64;
}
