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

#include <forge/messages/message_buffer.h>
#include <forge/parsing/token_reader.h>
#include <forge/streams/output.h>
#include <forge/types.h>

void frg_print_float(frg_stream_output_t* mut_stream,
                     frg_f64_t value,
                     frg_base_t base,
                     frg_column_number_t separate_every_n,
                     bool scientific_notation);

void frg_print_float_suffix(frg_stream_output_t* mut_stream, frg_bit_width_t bit_width);

typedef struct {
  frg_f64_t value;
  frg_bit_width_t bit_width;
} frg_parse_float_result_t;

bool frg_parse_float(frg_parse_float_result_t* out_result,
                     frg_message_buffer_t* mut_message_buffer,
                     frg_parsing_token_reader_t* mut_reader);
