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

#include <stdio.h>
#include <forge/parsing/domain.h>
#include <glib.h>
#include <stdbool.h>
#include <forge/common/enums.h>

typedef struct {
    frg_parsing_source_flags_t flags;
    FILE* file;
    const char* path;
    char* text;
    ssize_t length;
} frg_parsing_source_t;

frg_parsing_source_t* frg_parsing_source_new_file(
    FILE* file,
    const char* path,
    bool owned
);

frg_parsing_source_t* frg_parsing_source_new_buffer(
    char* text,
    ssize_t length,
    const char* path,
    bool owned
);

void frg_parsing_source_destroy(frg_parsing_source_t** source);

GString* frg_parsing_source_load_range(
    frg_parsing_source_t* source,
    const frg_parsing_range_t* range
);
