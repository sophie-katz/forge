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

#include <forge/streams/output.h>
#include <forge/types.h>
#include <stddef.h>
#include <stdio.h>

#define FRG_DEBUG_DEFAULT_STREAM    stdout
#define FRG_DEBUG_INDENTATION_WIDTH 2

void frg_debug_print_newline(frg_stream_output_t* mut_stream,
                             frg_indentation_t indentation);

void frg_debug_print_node(frg_stream_output_t* mut_stream, const char* format, ...);

void frg_debug_print_property(frg_stream_output_t* mut_stream,
                              const char* label,
                              const char* format,
                              ...);

void frg_debug_print_property_with_index(frg_stream_output_t* mut_stream,
                                         const char* label,
                                         size_t index,
                                         const char* format,
                                         ...);
