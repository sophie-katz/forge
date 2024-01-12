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

#include <forge/parsing/source.h>
#include <glib.h>

typedef struct {
    GHashTable* sources;
} frg_parsing_source_context_t;

frg_parsing_source_context_t* frg_parsing_source_context_new();
void frg_parsing_source_context_destroy(frg_parsing_source_context_t** source_context);

frg_parsing_source_t* frg_parsing_source_context_open_file(
    frg_parsing_source_context_t* source_context,
    FILE* file,
    const char* path,
    bool owned
);

frg_parsing_source_t* frg_parsing_source_context_open_path(
    void* message_buffer,
    frg_parsing_source_context_t* source_context,
    const char* path
);

frg_parsing_source_t* frg_parsing_source_context_open_buffer(
    frg_parsing_source_context_t* source_context,
    char* text,
    ssize_t length,
    const char* path,
    bool owned
);

frg_parsing_source_t* frg_parsing_source_context_open_string(
    frg_parsing_source_context_t* source_context,
    const char* text,
    ssize_t length,
    const char* path
);

frg_parsing_source_t* frg_parsing_source_context_get_source_by_path(
    frg_parsing_source_context_t* source_context,
    const char* path
);
