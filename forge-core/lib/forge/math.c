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

#include <forge/math.h>

uint32_t frg_get_uint32_digit_count(uint32_t value) {
    // Stolen from https://stackoverflow.com/a/1068937

    if (value < 10) {
        return 1;
    } else if (value < 100) {
        return 2;
    } else if (value < 1000) {
        return 3;
    } else if (value < 10000) {
        return 4;
    } else if (value < 100000) {
        return 5;
    } else if (value < 1000000) {
        return 6;
    } else if (value < 10000000) {
        return 7;
    } else if (value < 100000000) {
        return 8;
    } else if (value < 1000000000) {
        return 9;
    } else {
        // 2^32 - 1 is 4294967295, which has 10 digits
        return 10;
    }
}
