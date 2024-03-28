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

#include <forge/messages/message_buffer.h>
#include <forge/parsing/token_reader.h>
#include <forge/streams/output.h>
#include <forge/types.h>

void frg_print_uint_without_base_prefix(frg_stream_output_t* mut_stream,
                                        uint64_t value,
                                        frg_base_t base,
                                        frg_column_number_t separate_every_n);

void frg_print_uint(frg_stream_output_t* mut_stream,
                    uint64_t value,
                    frg_base_t base,
                    frg_column_number_t separate_every_n);

void frg_print_uint_suffix(frg_stream_output_t* mut_stream,
                           bool is_signed,
                           frg_bit_width_t bit_width);

typedef struct {
  uint64_t value;
  bool is_signed;
  frg_bit_width_t bit_width;
} frg_parse_uint_result_t;

size_t frg_parse_uint_fragment(uint64_t* out_value,
                               frg_parsing_token_reader_t* mut_reader,
                               frg_base_t base,
                               size_t max_digits,
                               bool allow_underscores);

bool frg_parse_uint(frg_parse_uint_result_t* out_result,
                    frg_message_buffer_t* mut_message_buffer,
                    frg_parsing_token_reader_t* mut_reader);
