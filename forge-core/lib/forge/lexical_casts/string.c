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

#include <ctype.h>
#include <forge/assert.h>
#include <forge/lexical_casts/character.h>
#include <forge/lexical_casts/utilities.h>
#include <forge/messages/codes.h>
#include <math.h>
#include <utf8proc.h>

bool frg_parse_string_with_quote_character(GString** out_value,
                                           frg_message_buffer_t* mut_message_buffer,
                                           frg_parsing_token_reader_t* mut_reader,
                                           char quote) {
  frg_assert_pointer_non_null(out_value);
  frg_assert_pointer_non_null(mut_message_buffer);
  frg_assert_pointer_non_null(mut_reader);

  if (frg_parsing_token_reader_get_current_character(mut_reader) == 0) {
    frg_message_emit_is_2_empty_literal(mut_message_buffer);

    return false;
  }

  if (frg_parsing_token_reader_get_current_character(mut_reader) != quote) {
    frg_message_emit_is_3_literal_must_start_with_quote(mut_message_buffer);

    return false;
  }

  frg_parsing_token_reader_step(mut_reader);

  if (frg_parsing_token_reader_get_current_character(mut_reader) == 0) {
    // TODO: Write a function for token readers to do this automatically
    frg_parsing_range_t source_range
      = { .start  = *frg_parsing_token_reader_get_current_location(mut_reader),
          .length = 1 };

    frg_message_emit_es_8_unexpected_end_of_literal(mut_message_buffer, &source_range);

    return false;
  }

  *out_value = g_string_new(NULL);

  while (frg_parsing_token_reader_get_current_character(mut_reader) != 0) {
    if (frg_parsing_token_reader_get_current_character(mut_reader) == quote) {
      break;
    }

    frg_character_t current_character = 0;
    if (!frg_parse_character_fragment(
          &current_character, mut_message_buffer, mut_reader, quote)) {
      return false;
    }

    g_string_append_c(*out_value, current_character);
  }

  if (frg_parsing_token_reader_get_current_character(mut_reader) == 0) {
    frg_parsing_range_t source_range
      = { .start  = *frg_parsing_token_reader_get_current_location(mut_reader),
          .length = 1 };

    frg_message_emit_es_8_unexpected_end_of_literal(mut_message_buffer, &source_range);

    return false;
  } else if (frg_parsing_token_reader_get_current_character(mut_reader) != quote) {
    frg_parsing_range_t source_range
      = { .start  = *frg_parsing_token_reader_get_current_location(mut_reader),
          .length = 1 };

    frg_message_emit_es_8_unexpected_end_of_literal(mut_message_buffer, &source_range);

    return false;
  }

  return true;
}

void frg_print_string(frg_stream_output_t* mut_stream, const char* value) {
  frg_assert_pointer_non_null(value);

  size_t length = strlen(value);

  frg_stream_output_write_character(mut_stream, '"');

  for (const char* iter = value; *iter != 0;) {
    utf8proc_int32_t codepoint  = 0;

    utf8proc_ssize_t bytes_read = utf8proc_iterate(
      (const utf8proc_uint8_t*)iter, length - (iter - value), &codepoint);

    // If the character is an invalid UTF-8 sequence, we just read it as a standalone
    // byte value
    if (bytes_read < 0) {
      codepoint = *iter & 0xff;
      iter++;
    } else {
      iter += bytes_read;
    }

    frg_print_character_fragment(mut_stream, codepoint, '"');
  }

  frg_stream_output_write_character(mut_stream, '"');
}

bool frg_parse_string(GString** out_value,
                      frg_message_buffer_t* mut_message_buffer,
                      frg_parsing_token_reader_t* reader) {
  return frg_parse_string_with_quote_character(
    out_value, mut_message_buffer, reader, '"');
}
