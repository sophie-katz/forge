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
#include <glib.h>
#include <forge/parsing/token_reader.h>
#include <forge/messages/message_buffer.h>

GString* frg_format_char(frg_char_t value);

bool frg_parse_char(
    frg_message_buffer_t* message_buffer,
    frg_char_t* value,
    frg_parsing_token_reader_t* reader
);

GString* frg_format_str(const char* value);

bool frg_parse_str(
    frg_message_buffer_t* message_buffer,
    GString** value,
    frg_parsing_token_reader_t* reader
);

GString* frg_format_uint(
    uint64_t value,
    frg_int_base_t base
);

GString* frg_format_uint_with_suffix(
    uint64_t value,
    frg_int_base_t base,
    bool is_signed,
    frg_bit_width_t bit_width
);

typedef struct {
    uint64_t value;
    bool is_signed;
    frg_bit_width_t bit_width;
} frg_parse_uint_result_t;

bool frg_parse_uint(
    frg_message_buffer_t* message_buffer,
    frg_parse_uint_result_t* result,
    frg_parsing_token_reader_t* reader
);

GString* frg_format_float(
    frg_f64_t value,
    frg_int_base_t base,
    bool scientific_notation
);

GString* frg_format_float_with_suffix(
    frg_f64_t value,
    frg_int_base_t base,
    bool scientific_notation,
    frg_bit_width_t bit_width
);

typedef struct {
    frg_f64_t value;
    frg_bit_width_t bit_width;
} frg_parse_float_result_t;

bool frg_parse_float(
    frg_message_buffer_t* message_buffer,
    frg_parse_float_result_t* result,
    frg_parsing_token_reader_t* reader
);
