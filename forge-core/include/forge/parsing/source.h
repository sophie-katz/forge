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

#include <stdio.h>
#include <forge/parsing/domain.h>
#include <glib.h>
#include <stdbool.h>
#include <forge/common/enums.h>
#include <forge/streams/input.h>

typedef struct {
    const char* path;
    frg_stream_input_t* stream;
} frg_parsing_source_t;

frg_parsing_source_t* frg_parsing_source_new(
    const char* path,
    frg_stream_input_t* stream
);

void frg_parsing_source_destroy(frg_parsing_source_t** source);

GString* frg_parsing_source_load_range(
    frg_parsing_source_t* source,
    const frg_parsing_range_t* range
);
