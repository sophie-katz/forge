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

#include <ctype.h>
#include <forge/assert.h>
#include <forge/lexical_casts.h>
#include <forge/messages/codes.h>
#include <math.h>
#include <utf8proc.h>

bool frg_is_char_printable(frg_character_t value) {
    if (value < 0x80) {
        return isprint(value);
    }

    switch (utf8proc_category((utf8proc_int32_t)value)) {
        case UTF8PROC_CATEGORY_CN:
        case UTF8PROC_CATEGORY_CC:
        case UTF8PROC_CATEGORY_CF:
        case UTF8PROC_CATEGORY_CS:
        case UTF8PROC_CATEGORY_CO:
            return false;
        default:
            return true;
    }
}

void _frg_print_char_unquoted(
    frg_stream_output_t* mut_stream,
    frg_character_t value,
    char quote
) {
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
            frg_stream_output_write_string_with_length(mut_stream, (const gchar*)buffer, width);
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

    _frg_print_char_unquoted(mut_stream, value, '\'');

    frg_stream_output_write_character(mut_stream, '\'');
}

size_t _frg_parse_uint_partial(
    uint64_t *out_value,
    frg_parsing_token_reader_t* mut_reader,
    frg_int_base_t base,
    size_t max_digits,
    bool allow_underscores
) {
    frg_assert_pointer_non_null(out_value);
    frg_assert_pointer_non_null(mut_reader);
    frg_assert(frg_is_valid_base(base));

    *out_value = 0;

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

bool _frg_parse_char_unquoted(
    frg_character_t *out_value,
    frg_message_buffer_t* mut_message_buffer,
    frg_parsing_token_reader_t* mut_reader,
    char quote
) {
    frg_assert_pointer_non_null(out_value);
    frg_assert_pointer_non_null(mut_reader);
    frg_assert(quote == '"' || quote == '\'');

    if (frg_parsing_token_reader_get_current_character(mut_reader) == '\\') {
        frg_parsing_token_reader_step(mut_reader);

        if (frg_parsing_token_reader_get_current_character(mut_reader) == 0) {
            frg_parsing_range_t source_range = {
                .start = *frg_parsing_token_reader_get_current_location(mut_reader),
                .length = 1
            };

            frg_message_emit_es_3_string_ends_with_backslash(
                mut_message_buffer,
                &source_range
            );

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

            size_t digit_count = _frg_parse_uint_partial(
                &value_uncasted,
                mut_reader,
                16,
                2,
                false
            );

            *out_value = value_uncasted;

            if (digit_count != 2) {
                frg_parsing_range_t source_range = {
                    .start = *frg_parsing_token_reader_get_current_location(mut_reader),
                    .length = 1
                };

                frg_message_emit_es_4_escape_x_expects_2_hex_digits(
                    mut_message_buffer,
                    &source_range,
                    digit_count
                );
            }
        } else if (frg_parsing_token_reader_get_current_character(mut_reader) == 'u') {
            *out_value = 0;

            frg_parsing_token_reader_step(mut_reader);

            if (frg_parsing_token_reader_get_current_character(mut_reader) == 0) {
                frg_parsing_range_t source_range = {
                    .start = *frg_parsing_token_reader_get_current_location(mut_reader),
                    .length = 1
                };

                frg_message_emit_es_5_escape_u_expects_opening_brace(
                    mut_message_buffer,
                    &source_range
                );

                return false;
            } else if (frg_parsing_token_reader_get_current_character(mut_reader) != '{') {
                frg_parsing_range_t source_range = {
                    .start = *frg_parsing_token_reader_get_current_location(mut_reader),
                    .length = 1
                };

                frg_message_emit_es_5_escape_u_expects_opening_brace(
                    mut_message_buffer,
                    &source_range
                );

                return false;
            }

            frg_parsing_token_reader_step(mut_reader);

            uint64_t value_uncasted = 0;

            size_t digit_count = _frg_parse_uint_partial(
                &value_uncasted,
                mut_reader,
                16,
                0,
                true
            );

            if (digit_count == 0 || digit_count > 6) {
                frg_parsing_range_t source_range = {
                    .start = *frg_parsing_token_reader_get_current_location(mut_reader),
                    .length = 1
                };

                frg_message_emit_es_7_escape_u_expects_hex_digits(
                    mut_message_buffer,
                    &source_range,
                    digit_count
                );

                return false;
            }

            if (value_uncasted > 0xffffff) {
                frg_parsing_range_t source_range = {
                    .start = *frg_parsing_token_reader_get_current_location(mut_reader),
                    .length = 1
                };

                frg_message_emit_is_1_code_point_limit(
                    mut_message_buffer,
                    &source_range,
                    value_uncasted
                );

                return false;
            }

            *out_value = value_uncasted;

            if (frg_parsing_token_reader_get_current_character(mut_reader) != '}') {
                frg_parsing_range_t source_range = {
                    .start = *frg_parsing_token_reader_get_current_location(mut_reader),
                    .length = 1
                };

                frg_message_emit_es_6_escape_u_expects_closing_brace(
                    mut_message_buffer,
                    &source_range
                );

                return false;
            }

            frg_parsing_token_reader_step(mut_reader);
        } else {
            *out_value = frg_parsing_token_reader_get_current_character(mut_reader);
            frg_parsing_token_reader_step(mut_reader);
        }

        return true;
    } else if (frg_parsing_token_reader_get_current_character(mut_reader) == quote) {
        return true;
    } else {
        *out_value = frg_parsing_token_reader_get_current_character(mut_reader);
        frg_parsing_token_reader_step(mut_reader);
        return true;
    }
}

GString* _frg_parse_string_with_quote_character(
    frg_message_buffer_t* mut_message_buffer,
    frg_parsing_token_reader_t* mut_reader,
    char quote
) {
    frg_assert_pointer_non_null(mut_message_buffer);
    frg_assert_pointer_non_null(mut_reader);

    if (frg_parsing_token_reader_get_current_character(mut_reader) == 0) {
        frg_message_emit_is_2_empty_literal(
            mut_message_buffer
        );

        return NULL;
    }

    if (frg_parsing_token_reader_get_current_character(mut_reader) != quote) {
        frg_message_emit_is_3_literal_must_start_with_quote(
            mut_message_buffer
        );

        return NULL;
    }

    frg_parsing_token_reader_step(mut_reader);

    if (frg_parsing_token_reader_get_current_character(mut_reader) == 0) {
        // TODO: Write a function for token readers to do this automatically
        frg_parsing_range_t source_range = {
            .start = *frg_parsing_token_reader_get_current_location(mut_reader),
            .length = 1
        };
        
        frg_message_emit_es_8_unexpected_end_of_literal(
            mut_message_buffer,
            &source_range
        );

        return NULL;
    }

    GString* value = g_string_new(NULL);

    while (frg_parsing_token_reader_get_current_character(mut_reader) != 0) {
        if (frg_parsing_token_reader_get_current_character(mut_reader) == quote) {
            break;
        }

        frg_character_t current_character;
        if (!_frg_parse_char_unquoted(
            &current_character,
            mut_message_buffer,
            mut_reader,
            quote
        )) {
            return NULL;
        }

        g_string_append_c(value, current_character);
    }

    if (frg_parsing_token_reader_get_current_character(mut_reader) == 0) {
        frg_parsing_range_t source_range = {
            .start = *frg_parsing_token_reader_get_current_location(mut_reader),
            .length = 1
        };
        
        frg_message_emit_es_8_unexpected_end_of_literal(
            mut_message_buffer,
            &source_range
        );

        return NULL;
    } else if (frg_parsing_token_reader_get_current_character(mut_reader) != quote) {
        frg_parsing_range_t source_range = {
            .start = *frg_parsing_token_reader_get_current_location(mut_reader),
            .length = 1
        };
        
        frg_message_emit_es_8_unexpected_end_of_literal(
            mut_message_buffer,
            &source_range
        );

        return NULL;
    }

    return value;
}

bool frg_parse_character(
    frg_character_t* out_value,
    frg_message_buffer_t* mut_message_buffer,
    frg_parsing_token_reader_t* mut_reader
) {
    GString* value_string = _frg_parse_string_with_quote_character(
        mut_message_buffer,
        mut_reader,
        '\''
    );

    if (value_string == NULL) {
        return false;
    }

    utf8proc_int32_t codepoint = 0;

    utf8proc_ssize_t bytes_read = utf8proc_iterate(
        (const utf8proc_uint8_t*)value_string->str,
        value_string->len,
        &codepoint
    );

    // If the character is an invalid UTF-8 sequence, we just read it as a standalone
    // byte value
    if (bytes_read < 0) {
        bytes_read = 1;
        codepoint = value_string->str[0] & 0xff;
    }

    frg_assert_int_less_than_or_equal_to(bytes_read, value_string->len);

    *out_value = codepoint;

    if (bytes_read < value_string->len) {
        frg_parsing_range_t source_range = {
            .start = *frg_parsing_token_reader_get_current_location(mut_reader),
            .length = 1
        };
        
        frg_message_emit_es_9_multiple_characters_in_character_literal(
            mut_message_buffer,
            &source_range
        );

        g_string_free(value_string, TRUE);

        return false;
    }

    g_string_free(value_string, TRUE);

    return true;
}

void frg_print_string(frg_stream_output_t* mut_stream, const char* value) {
    frg_assert_pointer_non_null(value);

    size_t length = strlen(value);

    frg_stream_output_write_character(mut_stream, '"');

    for (const char* iter = value; *iter != 0;) {
        utf8proc_int32_t codepoint = 0;

        utf8proc_ssize_t bytes_read = utf8proc_iterate(
            (const utf8proc_uint8_t*)iter,
            length - (iter - value),
            &codepoint
        );

        // If the character is an invalid UTF-8 sequence, we just read it as a standalone
        // byte value
        if (bytes_read < 0) {
            bytes_read = 1;
            codepoint = *iter & 0xff;
            iter++;
        } else {
            iter += bytes_read;
        }
        
        _frg_print_char_unquoted(mut_stream, codepoint, '"');
    }

    frg_stream_output_write_character(mut_stream, '"');
}

bool frg_parse_string(
    GString** out_value,
    frg_message_buffer_t* mut_message_buffer,
    frg_parsing_token_reader_t* reader
) {
    *out_value = _frg_parse_string_with_quote_character(
        mut_message_buffer,
        reader,
        '"'
    );

    return *out_value != NULL;
}

void _frg_print_base_prefix(
    frg_stream_output_t* mut_stream,
    frg_int_base_t base
) {
    if (base == 2) {
        frg_stream_output_write_string(mut_stream, "0b");
    } else if (base == 8) {
        frg_stream_output_write_string(mut_stream, "0o");
    } else if (base == 16) {
        frg_stream_output_write_string(mut_stream, "0x");
    }
}

void _frg_print_uint_without_base_prefix(
    frg_stream_output_t* mut_stream,
    uint64_t value,
    frg_int_base_t base
) {
    frg_assert(frg_is_valid_base(base));

    if (value == 0) {
        frg_stream_output_write_character(mut_stream, '0');
    } else {
        GString* buffer = g_string_new(NULL);
        uint64_t current_value = value;

        while (current_value != 0) {
            uint64_t digit = current_value % base;
            current_value /= base;

            if (digit < 10) {
                g_string_append_c(buffer, '0' + digit);
            } else {
                g_string_append_c(buffer, 'a' + digit - 10);
            }
        }

        g_strreverse(buffer->str);

        frg_stream_output_write_string_with_length(
            mut_stream,
            buffer->str,
            buffer->len
        );

        g_string_free(buffer, TRUE);
    }
}

void frg_print_uint(
    frg_stream_output_t* mut_stream,
    uint64_t value,
    frg_int_base_t base
) {
    frg_assert(frg_is_valid_base(base));

    _frg_print_base_prefix(mut_stream, base);

    _frg_print_uint_without_base_prefix(
        mut_stream,
        value,
        base
    );
}

void frg_print_uint_with_suffix(
    frg_stream_output_t* mut_stream,
    uint64_t value,
    frg_int_base_t base,
    bool is_signed,
    frg_bit_width_t bit_width
) {
    frg_assert(frg_is_valid_base(base));

    frg_print_uint(
        mut_stream,
        value,
        base
    );

    if (is_signed) {
        frg_stream_output_write_character(mut_stream, 'i');
    } else {
        frg_stream_output_write_character(mut_stream, 'u');
    }

    frg_stream_output_write_printf(mut_stream, "%u", bit_width);
}

void _frg_parse_base_prefix(
    frg_int_base_t* out_base,
    frg_message_buffer_t* mut_message_buffer,
    frg_parsing_token_reader_t* mut_reader
) {
    frg_assert_pointer_non_null(out_base);
    frg_assert_pointer_non_null(mut_reader);

    if (frg_parsing_token_reader_get_current_character(mut_reader) == '0') {
        frg_parsing_token_reader_t* save = frg_parsing_token_reader_clone(mut_reader);
        frg_parsing_token_reader_step(mut_reader);

        if (frg_parsing_token_reader_get_current_character(mut_reader) == 'b') {
            *out_base = 2;
            frg_parsing_token_reader_step(mut_reader);
        } else if (frg_parsing_token_reader_get_current_character(mut_reader) == 'o') {
            *out_base = 8;
            frg_parsing_token_reader_step(mut_reader);
        } else if (frg_parsing_token_reader_get_current_character(mut_reader) == 'x') {
            *out_base = 16;
            frg_parsing_token_reader_step(mut_reader);
        } else {
            frg_parsing_token_reader_restore(mut_reader, save);
            *out_base = 10;
        }

        frg_parsing_token_reader_destroy(save);
    } else {
        *out_base = 10;
    }
}

bool frg_parse_uint(
    frg_parse_uint_result_t* out_result,
    frg_message_buffer_t* mut_message_buffer,
    frg_parsing_token_reader_t* mut_reader
) {
    frg_assert_pointer_non_null(mut_message_buffer);
    frg_assert_pointer_non_null(out_result);
    frg_assert_pointer_non_null(mut_reader);

    if (frg_parsing_token_reader_get_current_character(mut_reader) == 0) {
        frg_parsing_range_t source_range = {
            .start = *frg_parsing_token_reader_get_current_location(mut_reader),
            .length = 1
        };

        frg_message_emit_es_8_unexpected_end_of_literal(
            mut_message_buffer,
            &source_range
        );

        return false;
    }

    out_result->value = 0;

    uint32_t base = 10;
    _frg_parse_base_prefix(&base, mut_message_buffer, mut_reader);

    if (frg_parsing_token_reader_get_current_character(mut_reader) == 0) {
        frg_parsing_range_t source_range = {
            .start = *frg_parsing_token_reader_get_current_location(mut_reader),
            .length = 1
        };

        frg_message_emit_es_8_unexpected_end_of_literal(
            mut_message_buffer,
            &source_range
        );

        return false;
    }

    size_t digit_count = _frg_parse_uint_partial(
        &out_result->value,
        mut_reader,
        base,
        0,
        true
    );

    if (digit_count == 0) {
        frg_parsing_range_t source_range = {
            .start = *frg_parsing_token_reader_get_current_location(mut_reader),
            .length = 1
        };

        frg_message_emit_es_1_unexpected_character(
            mut_message_buffer,
            &source_range,
            frg_parsing_token_reader_get_current_character(mut_reader)
        );

        return false;
    } else if (frg_parsing_token_reader_get_current_character(mut_reader) == 0) {
        out_result->is_signed = false;
        out_result->bit_width = 64;
    } else if (frg_parsing_token_reader_get_current_character(mut_reader) == 'u' || frg_parsing_token_reader_get_current_character(mut_reader) == 'i') {
        out_result->is_signed = frg_parsing_token_reader_get_current_character(mut_reader) == 'i';
        frg_parsing_token_reader_step(mut_reader);

        uint64_t bit_width_uncasted = 0;

        size_t digit_count = _frg_parse_uint_partial(
            &bit_width_uncasted,
            mut_reader,
            10,
            0,
            false
        );

        if (!frg_is_valid_bit_width_int(bit_width_uncasted)) {
            frg_parsing_range_t source_range = {
                .start = *frg_parsing_token_reader_get_current_location(mut_reader),
                .length = 1
            };

            frg_message_emit_es_10_invalid_bit_width_for_int(
                mut_message_buffer,
                &source_range,
                bit_width_uncasted
            );

            return false;
        }

        out_result->bit_width = bit_width_uncasted;

        if (digit_count == 0 || frg_parsing_token_reader_get_current_character(mut_reader) != 0) {
            frg_parsing_range_t source_range = {
                .start = *frg_parsing_token_reader_get_current_location(mut_reader),
                .length = 1
            };

            frg_message_emit_es_1_unexpected_character(
                mut_message_buffer,
                &source_range,
                frg_parsing_token_reader_get_current_character(mut_reader)
            );

            return false;
        }
    } else {
        frg_parsing_range_t source_range = {
            .start = *frg_parsing_token_reader_get_current_location(mut_reader),
            .length = 1
        };

        frg_message_emit_es_1_unexpected_character(
            mut_message_buffer,
            &source_range,
            frg_parsing_token_reader_get_current_character(mut_reader)
        );

        return false;
    }

    return true;
}

void _frg_float_decompose(
    frg_f64_t *out_integral_part,
    frg_f64_t *out_fractional_part,
    frg_f64_t value
) {
    frg_assert_pointer_non_null(out_integral_part);
    frg_assert_pointer_non_null(out_fractional_part);

    *out_integral_part = floor(value);
    *out_fractional_part = value - *out_integral_part;
}

void _frg_print_float_without_scientific_notation(
    frg_stream_output_t* mut_stream,
    frg_f64_t value,
    frg_int_base_t base
) {
    frg_assert(frg_is_valid_base(base));

    _frg_print_base_prefix(mut_stream, base);

    frg_f64_t integral_part = 0.0;
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

        frg_stream_output_write_string_with_length(
            mut_stream,
            buffer->str,
            buffer->len
        );

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

void frg_print_float(
    frg_stream_output_t* mut_stream,
    frg_f64_t value,
    frg_int_base_t base,
    bool scientific_notation
) {
    if (scientific_notation) {
        int32_t exponent = 0;
        frg_f64_t negative_factor = 1.0;

        if (value < 0.0) {
            negative_factor = -1.0;
            value = -value;
        }

        while (value < 1.0) {
            value *= base;
            exponent--;
        }

        while (value > base) {
            value /= base;
            exponent++;
        }

        _frg_print_float_without_scientific_notation(mut_stream, value * negative_factor, base);

        frg_stream_output_write_character(mut_stream, 'e');

        _frg_print_uint_without_base_prefix(
            mut_stream,
            exponent,
            base
        );
    } else {
        return _frg_print_float_without_scientific_notation(mut_stream, value, base);
    }
}

void frg_print_float_with_suffix(
    frg_stream_output_t* mut_stream,
    frg_f64_t value,
    frg_int_base_t base,
    bool scientific_notation,
    frg_bit_width_t bit_width
) {
    frg_assert(frg_is_valid_bit_width_float(bit_width));

    frg_print_float(
        mut_stream,
        value,
        base,
        scientific_notation
    );

    frg_stream_output_write_printf(mut_stream, "f%u", bit_width);
}

bool _frg_parse_float_without_scientific_notation(
    frg_parse_float_result_t* out_result,
    frg_message_buffer_t* mut_message_buffer,
    frg_parsing_token_reader_t* mut_reader,
    uint32_t base
) {
    frg_assert_pointer_non_null(mut_message_buffer);
    frg_assert_pointer_non_null(out_result);
    frg_assert_pointer_non_null(mut_reader);

    if (frg_parsing_token_reader_get_current_character(mut_reader) == 0) {
        frg_message_emit_is_2_empty_literal(
            mut_message_buffer
        );

        return false;
    }

    out_result->value = 0.0;
    out_result->bit_width = 64;

    if (frg_parsing_token_reader_get_current_character(mut_reader) == 0) {
        frg_parsing_range_t source_range = {
            .start = *frg_parsing_token_reader_get_current_location(mut_reader),
            .length = 1
        };

        frg_message_emit_es_8_unexpected_end_of_literal(
            mut_message_buffer,
            &source_range
        );

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
        frg_parsing_range_t source_range = {
            .start = *frg_parsing_token_reader_get_current_location(mut_reader),
            .length = 1
        };

        frg_message_emit_is_4_float_literal_missing_dot(
            mut_message_buffer,
            &source_range
        );

        return false;
    } else if (frg_parsing_token_reader_get_current_character(mut_reader) != '.') {
        frg_parsing_range_t source_range = {
            .start = *frg_parsing_token_reader_get_current_location(mut_reader),
            .length = 1
        };

        frg_message_emit_es_1_unexpected_character(
            mut_message_buffer,
            &source_range,
            frg_parsing_token_reader_get_current_character(mut_reader)
        );

        return false;
    }

    frg_parsing_token_reader_step(mut_reader);

    frg_f64_t fractional_part = 0.0;
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

bool frg_parse_float(
    frg_parse_float_result_t* out_result,
    frg_message_buffer_t* mut_message_buffer,
    frg_parsing_token_reader_t* mut_reader
) {
    frg_parse_float_result_t result_without_scientific_notation;

    uint32_t base = 0;
    _frg_parse_base_prefix(&base, mut_message_buffer, mut_reader);

    if (!_frg_parse_float_without_scientific_notation(
        &result_without_scientific_notation,
        mut_message_buffer,
        mut_reader,
        base
    )) {
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
            &result_exponent,
            mut_message_buffer,
            mut_reader,
            base
        )) {
            return false;
        }

        out_result->value *= pow(
            base,
            result_exponent.value * exponent_negative_factor
        );
    }

    if (frg_parsing_token_reader_get_current_character(mut_reader) == 'f') {
        frg_parsing_token_reader_step(mut_reader);

        uint64_t bit_width_uncasted = 0;

        size_t digit_count = _frg_parse_uint_partial(
            &bit_width_uncasted,
            mut_reader,
            10,
            0,
            false
        );

        if (digit_count == 0) {
            frg_parsing_range_t source_range = {
                .start = *frg_parsing_token_reader_get_current_location(mut_reader),
                .length = 1
            };

            frg_message_emit_es_1_unexpected_character(
                mut_message_buffer,
                &source_range,
                frg_parsing_token_reader_get_current_character(mut_reader)
            );

            return false;
        } else if (bit_width_uncasted != 32 && bit_width_uncasted != 64) {
            frg_parsing_range_t source_range = {
                .start = *frg_parsing_token_reader_get_current_location(mut_reader),
                .length = 1
            };

            frg_message_emit_es_11_invalid_bit_width_for_float(
                mut_message_buffer,
                &source_range,
                bit_width_uncasted
            );

            out_result->bit_width = bit_width_uncasted;
        }
    }

    if (frg_parsing_token_reader_get_current_character(mut_reader) != 0) {
        frg_parsing_range_t source_range = {
            .start = *frg_parsing_token_reader_get_current_location(mut_reader),
            .length = 1
        };

        frg_message_emit_es_1_unexpected_character(
                mut_message_buffer,
                &source_range,
                frg_parsing_token_reader_get_current_character(mut_reader)
            );

        return false;
    }

    return true;
}
