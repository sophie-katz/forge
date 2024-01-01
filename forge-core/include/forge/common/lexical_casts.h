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

#include <forge/common/enums.h>
#include <forge/common/types.h>
#include <glib.h>

frg_status_t frg_escape_char(GString** escaped, frg_char_t value);

frg_status_t frg_unescape_char(frg_char_t* value, const char* escaped);

frg_status_t frg_escape_str(GString** escaped, const char* value);

frg_status_t frg_unescape_str(GString** value, const char* escaped);

frg_status_t frg_uint_to_str(
    GString** str,
    uint64_t value,
    uint32_t base
);

frg_status_t frg_str_to_uint(
    uint64_t* value,
    const char* str
);

frg_status_t frg_float_to_str(
    GString** str,
    frg_f64_t value,
    uint32_t base
);

frg_status_t frg_str_to_float(
    frg_f64_t* value,
    const char* str
);
