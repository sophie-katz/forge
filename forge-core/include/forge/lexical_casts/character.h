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

void frg_print_character_fragment(frg_stream_output_t* mut_stream,
                                  frg_character_t value,
                                  char quote);

void frg_print_character(frg_stream_output_t* mut_stream, frg_character_t value);

bool frg_parse_character_fragment(frg_character_t* out_value,
                                  frg_message_buffer_t* mut_message_buffer,
                                  frg_parsing_token_reader_t* mut_reader,
                                  char quote);

bool frg_parse_character(frg_character_t* out_value,
                         frg_message_buffer_t* mut_message_buffer,
                         frg_parsing_token_reader_t* mut_reader);
