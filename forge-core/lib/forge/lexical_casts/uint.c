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

#include <forge/assert.h>
#include <forge/lexical_casts/uint.h>
#include <forge/lexical_casts/utilities.h>
#include <forge/messages/codes.h>

void frg_print_uint_without_base_prefix(frg_stream_output_t* mut_stream,
                                        uint64_t value,
                                        frg_base_t base,
                                        frg_column_number_t separate_every_n) {
  frg_assert(frg_is_valid_base(base));
  frg_assert_int_non_negative(separate_every_n);
  frg_assert_with_message(separate_every_n == 0,
                          "separate_every_n is not yet implemented");

  if (value == 0) {
    frg_stream_output_write_character(mut_stream, '0');
  } else {
    GString* buffer        = g_string_new(NULL);
    uint64_t current_value = value;

    while (current_value != 0) {
      uint64_t digit  = current_value % base;
      current_value  /= base;

      if (digit < 10) {
        g_string_append_c(buffer, '0' + digit);
      } else {
        g_string_append_c(buffer, 'a' + digit - 10);
      }
    }

    g_strreverse(buffer->str);

    frg_stream_output_write_string_with_length(mut_stream, buffer->str, buffer->len);

    g_string_free(buffer, TRUE);
  }
}

void frg_print_uint(frg_stream_output_t* mut_stream,
                    uint64_t value,
                    frg_base_t base,
                    frg_column_number_t separate_every_n) {
  frg_assert_pointer_non_null(mut_stream);
  frg_assert(frg_is_valid_base(base));
  frg_assert_int_non_negative(separate_every_n);
  frg_assert_with_message(separate_every_n == 0,
                          "separate_every_n is not yet implemented");

  frg_print_base_prefix(mut_stream, base);

  frg_print_uint_without_base_prefix(mut_stream, value, base, separate_every_n);
}

void frg_print_uint_suffix(frg_stream_output_t* mut_stream,
                           bool is_signed,
                           frg_bit_width_t bit_width) {
  frg_assert_pointer_non_null(mut_stream);

  if (is_signed) {
    frg_stream_output_write_character(mut_stream, 'i');
  } else {
    frg_stream_output_write_character(mut_stream, 'u');
  }

  frg_stream_output_write_printf(mut_stream, "%u", bit_width);
}

size_t frg_parse_uint_fragment(uint64_t* out_value,
                               frg_parsing_token_reader_t* mut_reader,
                               frg_base_t base,
                               size_t max_digits,
                               bool allow_underscores) {
  frg_assert_pointer_non_null(out_value);
  frg_assert_pointer_non_null(mut_reader);
  frg_assert(frg_is_valid_base(base));

  *out_value           = 0;

  size_t digit_counter = 0;

  while (true) {
    char current_char = frg_parsing_token_reader_get_current_character(mut_reader);

    if (allow_underscores) {
      while (current_char == '_') {
        frg_parsing_token_reader_step(mut_reader);
        current_char = frg_parsing_token_reader_get_current_character(mut_reader);
      }
    }

    if (base == 2) {
      if (current_char >= '0' && current_char <= '1') {
        *out_value = (*out_value * base) + (current_char - '0');
      } else {
        break;
      }
    } else if (base == 8) {
      if (current_char >= '0' && current_char <= '7') {
        *out_value = (*out_value * base) + (current_char - '0');
      } else {
        break;
      }
    } else if (base == 10) {
      if (current_char >= '0' && current_char <= '9') {
        *out_value = (*out_value * base) + (current_char - '0');
      } else {
        break;
      }
    } else if (base == 16) {
      if (current_char >= '0' && current_char <= '9') {
        *out_value = (*out_value * base) + (current_char - '0');
      } else if (current_char >= 'a' && current_char <= 'f') {
        *out_value = (*out_value * base) + (current_char - 'a') + 10;
      } else if (current_char >= 'A' && current_char <= 'F') {
        *out_value = (*out_value * base) + (current_char - 'A') + 10;
      } else {
        break;
      }
    } else {
      frg_die("unexpected base %i", base);
    }

    frg_parsing_token_reader_step(mut_reader);
    digit_counter++;

    if (max_digits > 0 && digit_counter >= max_digits) {
      break;
    }
  }

  if (allow_underscores) {
    while (frg_parsing_token_reader_get_current_character(mut_reader) == '_') {
      frg_parsing_token_reader_step(mut_reader);
    }
  }

  return digit_counter;
}

bool frg_parse_uint(frg_parse_uint_result_t* out_result,
                    frg_message_buffer_t* mut_message_buffer,
                    frg_parsing_token_reader_t* mut_reader) {
  frg_assert_pointer_non_null(mut_message_buffer);
  frg_assert_pointer_non_null(out_result);
  frg_assert_pointer_non_null(mut_reader);

  if (frg_parsing_token_reader_get_current_character(mut_reader) == 0) {
    frg_parsing_range_t source_range
      = { .start  = *frg_parsing_token_reader_get_current_location(mut_reader),
          .length = 1 };

    frg_message_emit_es_8_unexpected_end_of_literal(mut_message_buffer, &source_range);

    return false;
  }

  out_result->value = 0;

  uint32_t base     = 10;
  frg_parse_base_prefix(&base, mut_message_buffer, mut_reader);

  if (frg_parsing_token_reader_get_current_character(mut_reader) == 0) {
    frg_parsing_range_t source_range
      = { .start  = *frg_parsing_token_reader_get_current_location(mut_reader),
          .length = 1 };

    frg_message_emit_es_8_unexpected_end_of_literal(mut_message_buffer, &source_range);

    return false;
  }

  size_t digit_count
    = frg_parse_uint_fragment(&out_result->value, mut_reader, base, 0, true);

  if (digit_count == 0) {
    frg_parsing_range_t source_range
      = { .start  = *frg_parsing_token_reader_get_current_location(mut_reader),
          .length = 1 };

    frg_message_emit_es_1_unexpected_character(
      mut_message_buffer,
      &source_range,
      frg_parsing_token_reader_get_current_character(mut_reader));

    return false;
  } else if (frg_parsing_token_reader_get_current_character(mut_reader) == 0) {
    out_result->is_signed = false;
    out_result->bit_width = 64;
  } else if (frg_parsing_token_reader_get_current_character(mut_reader) == 'u'
             || frg_parsing_token_reader_get_current_character(mut_reader) == 'i') {
    out_result->is_signed
      = frg_parsing_token_reader_get_current_character(mut_reader) == 'i';
    frg_parsing_token_reader_step(mut_reader);

    uint64_t bit_width_uncasted = 0;

    size_t digit_count
      = frg_parse_uint_fragment(&bit_width_uncasted, mut_reader, 10, 0, false);

    if (!frg_is_valid_bit_width_int(bit_width_uncasted)) {
      frg_parsing_range_t source_range
        = { .start  = *frg_parsing_token_reader_get_current_location(mut_reader),
            .length = 1 };

      frg_message_emit_es_10_invalid_bit_width_for_int(
        mut_message_buffer, &source_range, bit_width_uncasted);

      return false;
    }

    out_result->bit_width = bit_width_uncasted;

    if (digit_count == 0
        || frg_parsing_token_reader_get_current_character(mut_reader) != 0) {
      frg_parsing_range_t source_range
        = { .start  = *frg_parsing_token_reader_get_current_location(mut_reader),
            .length = 1 };

      frg_message_emit_es_1_unexpected_character(
        mut_message_buffer,
        &source_range,
        frg_parsing_token_reader_get_current_character(mut_reader));

      return false;
    }
  } else {
    frg_parsing_range_t source_range
      = { .start  = *frg_parsing_token_reader_get_current_location(mut_reader),
          .length = 1 };

    frg_message_emit_es_1_unexpected_character(
      mut_message_buffer,
      &source_range,
      frg_parsing_token_reader_get_current_character(mut_reader));

    return false;
  }

  return true;
}
