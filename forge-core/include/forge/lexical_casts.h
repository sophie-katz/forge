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

#include <forge/enums.h>
#include <forge/messages/message_buffer.h>
#include <forge/parsing/token_reader.h>
#include <forge/types.h>
#include <glib.h>

bool frg_is_char_printable(frg_character_t value);

void frg_print_character(frg_stream_output_t* mut_stream, frg_character_t value);

bool frg_parse_character(frg_character_t* out_value,
                         frg_message_buffer_t* mut_message_buffer,
                         frg_parsing_token_reader_t* mut_reader);

void frg_print_string(frg_stream_output_t* mut_stream, const char* value);

bool frg_parse_string(GString** out_value,
                      frg_message_buffer_t* mut_message_buffer,
                      frg_parsing_token_reader_t* mut_reader);

void frg_print_uint(frg_stream_output_t* mut_stream,
                    uint64_t value,
                    frg_int_base_t base);

void frg_print_uint_with_suffix(frg_stream_output_t* mut_stream,
                                uint64_t value,
                                frg_int_base_t base,
                                bool is_signed,
                                frg_bit_width_t bit_width);

typedef struct {
  uint64_t value;
  bool is_signed;
  frg_bit_width_t bit_width;
} frg_parse_uint_result_t;

bool frg_parse_uint(frg_parse_uint_result_t* out_result,
                    frg_message_buffer_t* mut_message_buffer,
                    frg_parsing_token_reader_t* mut_reader);

void frg_print_float(frg_stream_output_t* mut_stream,
                     frg_f64_t value,
                     frg_int_base_t base,
                     bool scientific_notation);

void frg_print_float_with_suffix(frg_stream_output_t* mut_stream,
                                 frg_f64_t value,
                                 frg_int_base_t base,
                                 bool scientific_notation,
                                 frg_bit_width_t bit_width);

typedef struct {
  frg_f64_t value;
  frg_bit_width_t bit_width;
} frg_parse_float_result_t;

bool frg_parse_float(frg_parse_float_result_t* out_result,
                     frg_message_buffer_t* mut_message_buffer,
                     frg_parsing_token_reader_t* mut_reader);
