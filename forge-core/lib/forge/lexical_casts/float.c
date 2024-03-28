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
#include <forge/lexical_casts/float.h>
#include <forge/lexical_casts/uint.h>
#include <forge/lexical_casts/utilities.h>
#include <forge/messages/codes.h>
#include <math.h>

void _frg_float_decompose(frg_f64_t* out_integral_part,
                          frg_f64_t* out_fractional_part,
                          frg_f64_t value) {
  frg_assert_pointer_non_null(out_integral_part);
  frg_assert_pointer_non_null(out_fractional_part);

  *out_integral_part   = floor(value);
  *out_fractional_part = value - *out_integral_part;
}

void _frg_print_float_without_scientific_notation(
  frg_stream_output_t* mut_stream,
  frg_f64_t value,
  frg_base_t base,
  frg_column_number_t separate_every_n) {
  frg_assert(frg_is_valid_base(base));
  frg_assert_int_non_negative(separate_every_n);
  frg_assert_with_message(separate_every_n == 0,
                          "separate_every_n is not yet implemented");

  frg_print_base_prefix(mut_stream, base);

  frg_f64_t integral_part   = 0.0;
  frg_f64_t fractional_part = 0.0;

  _frg_float_decompose(&integral_part, &fractional_part, fabs(value));

  if (value < 0.0) {
    frg_stream_output_write_character(mut_stream, '-');
  }

  if (integral_part == 0.0) {
    frg_stream_output_write_character(mut_stream, '0');
  } else {
    GString* buffer = g_string_new(NULL);

    while (integral_part != 0.0) {
      uint32_t digit = (uint32_t)fmod(integral_part, base);

      if (digit < 10) {
        g_string_append_c(buffer, '0' + digit);
      } else {
        g_string_append_c(buffer, 'a' + digit - 10);
      }

      integral_part /= base;
    }

    g_strreverse(buffer->str);

    frg_stream_output_write_string_with_length(mut_stream, buffer->str, buffer->len);

    g_string_free(buffer, TRUE);
  }

  frg_stream_output_write_character(mut_stream, '.');

  if (fractional_part == 0.0) {
    frg_stream_output_write_character(mut_stream, '0');
  } else {
    while (fractional_part != 0.0) {
      uint32_t digit = (uint32_t)fmod(fractional_part * base, base);

      if (digit < 10) {
        frg_stream_output_write_character(mut_stream, '0' + digit);
      } else {
        frg_stream_output_write_character(mut_stream, 'a' + digit - 10);
      }

      fractional_part *= base;
      fractional_part -= floor(fractional_part);
    }
  }
}

void frg_print_float(frg_stream_output_t* mut_stream,
                     frg_f64_t value,
                     frg_base_t base,
                     frg_column_number_t separate_every_n,
                     bool scientific_notation) {
  frg_assert_pointer_non_null(mut_stream);
  frg_assert(frg_is_valid_base(base));
  frg_assert_int_non_negative(separate_every_n);
  frg_assert_with_message(separate_every_n == 0,
                          "separate_every_n is not yet implemented");

  if (scientific_notation) {
    int32_t exponent          = 0;
    frg_f64_t negative_factor = 1.0;

    if (value < 0.0) {
      negative_factor = -1.0;
      value           = -value;
    }

    while (value < 1.0) {
      value *= base;
      exponent--;
    }

    while (value > base) {
      value /= base;
      exponent++;
    }

    _frg_print_float_without_scientific_notation(
      mut_stream, value * negative_factor, base, separate_every_n);

    frg_stream_output_write_character(mut_stream, 'e');

    frg_print_uint_without_base_prefix(mut_stream, exponent, base, separate_every_n);
  } else {
    return _frg_print_float_without_scientific_notation(
      mut_stream, value, base, separate_every_n);
  }
}

void frg_print_float_suffix(frg_stream_output_t* mut_stream,
                            frg_bit_width_t bit_width) {
  frg_assert(frg_is_valid_bit_width_float(bit_width));

  frg_stream_output_write_printf(mut_stream, "f%u", bit_width);
}

bool _frg_parse_float_without_scientific_notation(
  frg_parse_float_result_t* out_result,
  frg_message_buffer_t* mut_message_buffer,
  frg_parsing_token_reader_t* mut_reader,
  uint32_t base) {
  frg_assert_pointer_non_null(mut_message_buffer);
  frg_assert_pointer_non_null(out_result);
  frg_assert_pointer_non_null(mut_reader);

  if (frg_parsing_token_reader_get_current_character(mut_reader) == 0) {
    frg_message_emit_is_2_empty_literal(mut_message_buffer);

    return false;
  }

  out_result->value     = 0.0;
  out_result->bit_width = 64;

  if (frg_parsing_token_reader_get_current_character(mut_reader) == 0) {
    frg_parsing_range_t source_range
      = { .start  = *frg_parsing_token_reader_get_current_location(mut_reader),
          .length = 1 };

    frg_message_emit_es_8_unexpected_end_of_literal(mut_message_buffer, &source_range);

    return false;
  }

  frg_f64_t integral_part = 0.0;

  while (true) {
    char current_char = frg_parsing_token_reader_get_current_character(mut_reader);

    while (current_char == '_') {
      frg_parsing_token_reader_step(mut_reader);
      current_char = frg_parsing_token_reader_get_current_character(mut_reader);
    }

    if (base == 2) {
      if (current_char >= '0' && current_char <= '1') {
        integral_part = (integral_part * base) + (current_char - '0');
      } else {
        break;
      }
    } else if (base == 8) {
      if (current_char >= '0' && current_char <= '7') {
        integral_part = (integral_part * base) + (current_char - '0');
      } else {
        break;
      }
    } else if (base == 10) {
      if (current_char >= '0' && current_char <= '9') {
        integral_part = (integral_part * base) + (current_char - '0');
      } else {
        break;
      }
    } else if (base == 16) {
      if (current_char >= '0' && current_char <= '9') {
        integral_part = (integral_part * base) + (current_char - '0');
      } else if (current_char >= 'a' && current_char <= 'f') {
        integral_part = (integral_part * base) + (current_char - 'a') + 10;
      } else if (current_char >= 'A' && current_char <= 'F') {
        integral_part = (integral_part * base) + (current_char - 'A') + 10;
      } else {
        break;
      }
    } else {
      frg_die("unexpected base %i", base);
    }

    frg_parsing_token_reader_step(mut_reader);
  }

  while (frg_parsing_token_reader_get_current_character(mut_reader) == '_') {
    frg_parsing_token_reader_step(mut_reader);
  }

  if (frg_parsing_token_reader_get_current_character(mut_reader) == 0) {
    frg_parsing_range_t source_range
      = { .start  = *frg_parsing_token_reader_get_current_location(mut_reader),
          .length = 1 };

    frg_message_emit_is_4_float_literal_missing_dot(mut_message_buffer, &source_range);

    return false;
  } else if (frg_parsing_token_reader_get_current_character(mut_reader) != '.') {
    frg_parsing_range_t source_range
      = { .start  = *frg_parsing_token_reader_get_current_location(mut_reader),
          .length = 1 };

    frg_message_emit_es_1_unexpected_character(
      mut_message_buffer,
      &source_range,
      frg_parsing_token_reader_get_current_character(mut_reader));

    return false;
  }

  frg_parsing_token_reader_step(mut_reader);

  frg_f64_t fractional_part   = 0.0;
  frg_f64_t fractional_factor = 1.0;

  while (true) {
    char current_char = frg_parsing_token_reader_get_current_character(mut_reader);

    while (current_char == '_') {
      frg_parsing_token_reader_step(mut_reader);
      current_char = frg_parsing_token_reader_get_current_character(mut_reader);
    }

    if (base == 2) {
      if (current_char >= '0' && current_char <= '1') {
        fractional_part = (fractional_part * base) + (current_char - '0');
      } else {
        break;
      }
    } else if (base == 8) {
      if (current_char >= '0' && current_char <= '7') {
        fractional_part = (fractional_part * base) + (current_char - '0');
      } else {
        break;
      }
    } else if (base == 10) {
      if (current_char >= '0' && current_char <= '9') {
        fractional_part = (fractional_part * base) + (current_char - '0');
      } else {
        break;
      }
    } else if (base == 16) {
      if (current_char >= '0' && current_char <= '9') {
        fractional_part = (fractional_part * base) + (current_char - '0');
      } else if (current_char >= 'a' && current_char <= 'f') {
        fractional_part = (fractional_part * base) + (current_char - 'a') + 10;
      } else if (current_char >= 'A' && current_char <= 'F') {
        fractional_part = (fractional_part * base) + (current_char - 'A') + 10;
      } else {
        break;
      }
    } else {
      frg_die("unexpected base %i", base);
    }

    fractional_factor /= base;
    frg_parsing_token_reader_step(mut_reader);
  }

  out_result->value = integral_part + fractional_part * fractional_factor;

  return true;
}

bool frg_parse_float(frg_parse_float_result_t* out_result,
                     frg_message_buffer_t* mut_message_buffer,
                     frg_parsing_token_reader_t* mut_reader) {
  frg_parse_float_result_t result_without_scientific_notation;

  uint32_t base = 10;
  frg_parse_base_prefix(&base, mut_message_buffer, mut_reader);

  if (!_frg_parse_float_without_scientific_notation(
        &result_without_scientific_notation, mut_message_buffer, mut_reader, base)) {
    return false;
  }

  *out_result = result_without_scientific_notation;

  if (frg_parsing_token_reader_get_current_character(mut_reader) == 'e') {
    frg_parsing_token_reader_step(mut_reader);

    double exponent_negative_factor = 1.0;

    if (frg_parsing_token_reader_get_current_character(mut_reader) == '-') {
      exponent_negative_factor = -1.0;
      frg_parsing_token_reader_step(mut_reader);
    }

    frg_parse_float_result_t result_exponent;

    if (!_frg_parse_float_without_scientific_notation(
          &result_exponent, mut_message_buffer, mut_reader, base)) {
      return false;
    }

    out_result->value *= pow(base, result_exponent.value * exponent_negative_factor);
  }

  if (frg_parsing_token_reader_get_current_character(mut_reader) == 'f') {
    frg_parsing_token_reader_step(mut_reader);

    uint64_t bit_width_uncasted = 0;

    size_t digit_count
      = frg_parse_uint_fragment(&bit_width_uncasted, mut_reader, 10, 0, false);

    if (digit_count == 0) {
      frg_parsing_range_t source_range
        = { .start  = *frg_parsing_token_reader_get_current_location(mut_reader),
            .length = 1 };

      frg_message_emit_es_1_unexpected_character(
        mut_message_buffer,
        &source_range,
        frg_parsing_token_reader_get_current_character(mut_reader));

      return false;
    } else if (bit_width_uncasted != 32 && bit_width_uncasted != 64) {
      frg_parsing_range_t source_range
        = { .start  = *frg_parsing_token_reader_get_current_location(mut_reader),
            .length = 1 };

      frg_message_emit_es_11_invalid_bit_width_for_float(
        mut_message_buffer, &source_range, bit_width_uncasted);

      out_result->bit_width = bit_width_uncasted;
    }
  }

  if (frg_parsing_token_reader_get_current_character(mut_reader) != 0) {
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
