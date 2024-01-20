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

#include <forge/common/enums.h>
#include <forge/common/types.h>
#include <forge/parsing/domain.h>
#include <forge/parsing/source_context.h>
#include <forge/streams/output.h>
#include <glib.h>

typedef struct frg_message_t {
    const char* log_path;
    frg_lineno_t log_lineno;
    frg_parsing_range_t source_range;
    frg_message_severity_t severity;
    const char* code;
    GString* text;
    GList* children;
} frg_message_t;

frg_message_t* frg_message_new(
    const char* log_path,
    frg_lineno_t log_lineno,
    const frg_parsing_range_t* source_range,
    frg_message_severity_t severity,
    const char* code,
    GString* text
);

void frg_message_destroy(frg_message_t** message);

void frg_message_add_child(frg_message_t* message, frg_message_t* child);

void frg_message_print(
    frg_stream_output_t* stream,
    const frg_message_t* message,
    frg_columnno_t lineno_pad_to_width,
    frg_parsing_source_context_t* source_context
);
