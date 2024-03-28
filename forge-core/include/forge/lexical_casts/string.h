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

void frg_print_string(frg_stream_output_t* mut_stream, const char* value);

bool frg_parse_string_with_quote_character(GString** out_value,
                                           frg_message_buffer_t* mut_message_buffer,
                                           frg_parsing_token_reader_t* mut_reader,
                                           char quote);

bool frg_parse_string(GString** out_value,
                      frg_message_buffer_t* mut_message_buffer,
                      frg_parsing_token_reader_t* mut_reader);
