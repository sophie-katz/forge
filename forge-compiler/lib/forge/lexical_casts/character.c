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

#include <forge/assert.h>
#include <forge/lexical_casts/character.h>
#include <forge/lexical_casts/string.h>
#include <forge/lexical_casts/uint.h>
#include <forge/lexical_casts/utilities.h>
#include <forge/messages/codes.h>
#include <utf8proc.h>

void frg_print_character_fragment(frg_stream_output_t* mut_stream,
                                  frg_character_t value,
                                  char quote) {
  frg_assert_pointer_non_null(mut_stream);
  frg_assert(quote == '"' || quote == '\'');

  if (value == quote) {
    frg_stream_output_write_printf(mut_stream, "\\%c", quote);
  } else if (value == 0) {
    frg_stream_output_write_string(mut_stream, "\\0");
  } else if (value == '\a') {
    frg_stream_output_write_string(mut_stream, "\\a");
  } else if (value == '\b') {
    frg_stream_output_write_string(mut_stream, "\\b");
  } else if (value == '\033') {
    frg_stream_output_write_string(mut_stream, "\\e");
  } else if (value == '\f') {
    frg_stream_output_write_string(mut_stream, "\\f");
  } else if (value == '\n') {
    frg_stream_output_write_string(mut_stream, "\\n");
  } else if (value == '\r') {
    frg_stream_output_write_string(mut_stream, "\\r");
  } else if (value == '\t') {
    frg_stream_output_write_string(mut_stream, "\\t");
  } else if (value == '\v') {
    frg_stream_output_write_string(mut_stream, "\\v");
  } else if (value == '\\') {
    frg_stream_output_write_string(mut_stream, "\\\\");
  } else if (frg_is_char_printable(value)) {
    if (value < 0x80) {
      frg_stream_output_write_character(mut_stream, value);
    } else {
      utf8proc_uint8_t buffer[4];
      utf8proc_ssize_t width = utf8proc_encode_char((utf8proc_int32_t)value, buffer);
      frg_stream_output_write_string_with_length(
        mut_stream, (const gchar*)buffer, width);
    }
  } else {
    if (value < 0x80) {
      frg_stream_output_write_printf(mut_stream, "\\x%02x", value & 0xff);
    } else if (value <= 0xff) {
      frg_stream_output_write_printf(mut_stream, "\\u{%02x}", value & 0xff);
    } else if (value <= 0xffff) {
      frg_stream_output_write_printf(mut_stream, "\\u{%04x}", value & 0xffff);
    } else {
      frg_stream_output_write_printf(mut_stream, "\\u{%06x}", value);
    }
  }
}

void frg_print_character(frg_stream_output_t* mut_stream, frg_character_t value) {
  frg_stream_output_write_character(mut_stream, '\'');

  frg_print_character_fragment(mut_stream, value, '\'');

  frg_stream_output_write_character(mut_stream, '\'');
}

bool _frg_parse_character_fragment_escape(frg_character_t* out_value,
                                          frg_message_buffer_t* mut_message_buffer,
                                          frg_parsing_token_reader_t* mut_reader,
                                          char quote) {
  frg_assert_pointer_non_null(out_value);
  frg_assert_pointer_non_null(mut_message_buffer);
  frg_assert_pointer_non_null(mut_reader);
  frg_assert(quote == '"' || quote == '\'');
  frg_assert_int_equal_to(frg_parsing_token_reader_get_current_character(mut_reader),
                          '\\');

  frg_parsing_token_reader_step(mut_reader);

  if (frg_parsing_token_reader_get_current_character(mut_reader) == 0) {
    frg_parsing_range_t source_range
      = { .start  = *frg_parsing_token_reader_get_current_location(mut_reader),
          .length = 1 };

    frg_message_emit_es_3_string_ends_with_backslash(mut_message_buffer, &source_range);

    return false;
  } else if (frg_parsing_token_reader_get_current_character(mut_reader) == 'a') {
    *out_value = '\a';
    frg_parsing_token_reader_step(mut_reader);
  } else if (frg_parsing_token_reader_get_current_character(mut_reader) == 'b') {
    *out_value = '\b';
    frg_parsing_token_reader_step(mut_reader);
  } else if (frg_parsing_token_reader_get_current_character(mut_reader) == 'e') {
    *out_value = '\033';
    frg_parsing_token_reader_step(mut_reader);
  } else if (frg_parsing_token_reader_get_current_character(mut_reader) == 'f') {
    *out_value = '\f';
    frg_parsing_token_reader_step(mut_reader);
  } else if (frg_parsing_token_reader_get_current_character(mut_reader) == 'n') {
    *out_value = '\n';
    frg_parsing_token_reader_step(mut_reader);
  } else if (frg_parsing_token_reader_get_current_character(mut_reader) == 'r') {
    *out_value = '\r';
    frg_parsing_token_reader_step(mut_reader);
  } else if (frg_parsing_token_reader_get_current_character(mut_reader) == 't') {
    *out_value = '\t';
    frg_parsing_token_reader_step(mut_reader);
  } else if (frg_parsing_token_reader_get_current_character(mut_reader) == 'v') {
    *out_value = '\v';
    frg_parsing_token_reader_step(mut_reader);
  } else if (frg_parsing_token_reader_get_current_character(mut_reader) == '0') {
    *out_value = '\0';
    frg_parsing_token_reader_step(mut_reader);
  } else if (frg_parsing_token_reader_get_current_character(mut_reader) == 'x') {
    frg_parsing_token_reader_step(mut_reader);

    uint64_t value_uncasted = 0;

    size_t digit_count
      = frg_parse_uint_fragment(&value_uncasted, mut_reader, 16, 2, false);

    *out_value = value_uncasted;

    if (digit_count != 2) {
      frg_parsing_range_t source_range
        = { .start  = *frg_parsing_token_reader_get_current_location(mut_reader),
            .length = 1 };

      frg_message_emit_es_4_escape_x_expects_2_hex_digits(
        mut_message_buffer, &source_range, digit_count);
    }
  } else if (frg_parsing_token_reader_get_current_character(mut_reader) == 'u') {
    *out_value = 0;

    frg_parsing_token_reader_step(mut_reader);

    if (frg_parsing_token_reader_get_current_character(mut_reader) == 0) {
      frg_parsing_range_t source_range
        = { .start  = *frg_parsing_token_reader_get_current_location(mut_reader),
            .length = 1 };

      frg_message_emit_es_5_escape_u_expects_opening_brace(mut_message_buffer,
                                                           &source_range);

      return false;
    } else if (frg_parsing_token_reader_get_current_character(mut_reader) != '{') {
      frg_parsing_range_t source_range
        = { .start  = *frg_parsing_token_reader_get_current_location(mut_reader),
            .length = 1 };

      frg_message_emit_es_5_escape_u_expects_opening_brace(mut_message_buffer,
                                                           &source_range);

      return false;
    }

    frg_parsing_token_reader_step(mut_reader);

    uint64_t value_uncasted = 0;

    size_t digit_count
      = frg_parse_uint_fragment(&value_uncasted, mut_reader, 16, 0, true);

    if (digit_count == 0 || digit_count > 6) {
      frg_parsing_range_t source_range
        = { .start  = *frg_parsing_token_reader_get_current_location(mut_reader),
            .length = 1 };

      frg_message_emit_es_7_escape_u_expects_hex_digits(
        mut_message_buffer, &source_range, digit_count);

      return false;
    }

    if (value_uncasted > 0xffffff) {
      frg_parsing_range_t source_range
        = { .start  = *frg_parsing_token_reader_get_current_location(mut_reader),
            .length = 1 };

      frg_message_emit_is_1_code_point_limit(
        mut_message_buffer, &source_range, value_uncasted);

      return false;
    }

    *out_value = value_uncasted;

    if (frg_parsing_token_reader_get_current_character(mut_reader) != '}') {
      frg_parsing_range_t source_range
        = { .start  = *frg_parsing_token_reader_get_current_location(mut_reader),
            .length = 1 };

      frg_message_emit_es_6_escape_u_expects_closing_brace(mut_message_buffer,
                                                           &source_range);

      return false;
    }

    frg_parsing_token_reader_step(mut_reader);
  } else {
    *out_value = frg_parsing_token_reader_get_current_character(mut_reader);
    frg_parsing_token_reader_step(mut_reader);
  }

  return true;
}

bool frg_parse_character_fragment(frg_character_t* out_value,
                                  frg_message_buffer_t* mut_message_buffer,
                                  frg_parsing_token_reader_t* mut_reader,
                                  char quote) {
  frg_assert_pointer_non_null(out_value);
  frg_assert_pointer_non_null(mut_message_buffer);
  frg_assert_pointer_non_null(mut_reader);
  frg_assert(quote == '"' || quote == '\'');

  if (frg_parsing_token_reader_get_current_character(mut_reader) == '\\') {
    return _frg_parse_character_fragment_escape(
      out_value, mut_message_buffer, mut_reader, quote);
  } else if (frg_parsing_token_reader_get_current_character(mut_reader) == quote) {
    return true;
  } else {
    *out_value = frg_parsing_token_reader_get_current_character(mut_reader);
    frg_parsing_token_reader_step(mut_reader);
    return true;
  }
}

bool frg_parse_character(frg_character_t* out_value,
                         frg_message_buffer_t* mut_message_buffer,
                         frg_parsing_token_reader_t* mut_reader) {
  frg_assert_pointer_non_null(out_value);
  frg_assert_pointer_non_null(mut_message_buffer);
  frg_assert_pointer_non_null(mut_reader);

  GString* value_string = NULL;

  if (!frg_parse_string_with_quote_character(
        &value_string, mut_message_buffer, mut_reader, '\'')) {
    return false;
  }

  utf8proc_int32_t codepoint  = 0;

  utf8proc_ssize_t bytes_read = utf8proc_iterate(
    (const utf8proc_uint8_t*)value_string->str, value_string->len, &codepoint);

  // If the character is an invalid UTF-8 sequence, we just read it as a standalone
  // byte value
  if (bytes_read < 0) {
    bytes_read = 1;
    codepoint  = value_string->str[0] & 0xff;
  }

  frg_assert_int_less_than_or_equal_to(bytes_read, value_string->len);

  *out_value = codepoint;

  if (bytes_read < value_string->len) {
    frg_parsing_range_t source_range
      = { .start  = *frg_parsing_token_reader_get_current_location(mut_reader),
          .length = 1 };

    frg_message_emit_es_9_multiple_characters_in_character_literal(mut_message_buffer,
                                                                   &source_range);

    g_string_free(value_string, TRUE);

    return false;
  }

  g_string_free(value_string, TRUE);

  return true;
}
