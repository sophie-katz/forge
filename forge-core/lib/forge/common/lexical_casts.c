// Copyright (c) 2024 Sophie Katz
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
#include <forge/common/error.h>
#include <forge/common/lexical_casts.h>
#include <math.h>
#include <utf8proc.h>

bool _frg_is_valid_base(frg_int_base_t base) {
    return base == 2 || base == 8 || base == 10 || base == 16;
}

bool _frg_is_valid_bit_width(frg_bit_width_t bit_width) {
    return bit_width == 8 || bit_width == 16 || bit_width == 32 || bit_width == 64;
}

bool _frg_is_char_printable(frg_char_t value) {
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

void _frg_format_char_unquoted(GString* formatted, frg_char_t value, char quote) {
    frg_assert_pointer_non_null(formatted);
    frg_assert(quote == '"' || quote == '\'');

    if (value == quote) {
        g_string_append_printf(formatted, "\\%c", quote);
    } else if (value == 0) {
        g_string_append(formatted, "\\0");
    } else if (value == '\a') {
        g_string_append(formatted, "\\a");
    } else if (value == '\b') {
        g_string_append(formatted, "\\b");
    } else if (value == '\033') {
        g_string_append(formatted, "\\e");
    } else if (value == '\f') {
        g_string_append(formatted, "\\f");
    } else if (value == '\n') {
        g_string_append(formatted, "\\n");
    } else if (value == '\r') {
        g_string_append(formatted, "\\r");
    } else if (value == '\t') {
        g_string_append(formatted, "\\t");
    } else if (value == '\v') {
        g_string_append(formatted, "\\v");
    } else if (value == '\\') {
        g_string_append(formatted, "\\\\");
    } else if (_frg_is_char_printable(value)) {
        if (value < 0x80) {
            g_string_append_c(formatted, value);
        } else {
            utf8proc_uint8_t buffer[4];
            utf8proc_ssize_t width = utf8proc_encode_char((utf8proc_int32_t)value, buffer);
            g_string_append_len(formatted, (const gchar*)buffer, width);
        }
    } else {
        if (value < 0x80) {
            g_string_append_printf(formatted, "\\x%02x", value & 0xff);
        } else if (value <= 0xff) {
            g_string_append_printf(formatted, "\\u{%02x}", value & 0xff);
        } else if (value <= 0xffff) {
            g_string_append_printf(formatted, "\\u{%04x}", value & 0xffff);
        } else {
            g_string_append_printf(formatted, "\\u{%06x}", value);
        }
    }
}

GString* frg_format_char(frg_char_t value) {
    GString* formatted = g_string_new("'");

    _frg_format_char_unquoted(formatted, value, '\'');

    g_string_append_c(formatted, '\'');

    return formatted;
}

size_t _frg_parse_uint_partial(
    uint64_t *value,
    frg_parsing_token_reader_t* reader,
    frg_int_base_t base,
    size_t max_digits,
    bool allow_underscores
) {
    frg_assert_pointer_non_null(value);
    frg_assert_pointer_non_null(reader);
    frg_assert(_frg_is_valid_base(base));

    *value = 0;

    size_t digit_counter = 0;

    while (true) {
        char current_char = frg_parsing_token_reader_get_current_char(reader);

        if (allow_underscores) {
            while (current_char == '_') {
                frg_parsing_token_reader_step(reader);
                current_char = frg_parsing_token_reader_get_current_char(reader);
            }
        }

        if (base == 2) {
            if (current_char >= '0' && current_char <= '1') {
                *value = (*value * base) + (current_char - '0');
            } else {
                break;
            }
        } else if (base == 8) {
            if (current_char >= '0' && current_char <= '7') {
                *value = (*value * base) + (current_char - '0');
            } else {
                break;
            }
        } else if (base == 10) {
            if (current_char >= '0' && current_char <= '9') {
                *value = (*value * base) + (current_char - '0');
            } else {
                break;
            }
        } else if (base == 16) {
            if (current_char >= '0' && current_char <= '9') {
                *value = (*value * base) + (current_char - '0');
            } else if (current_char >= 'a' && current_char <= 'f') {
                *value = (*value * base) + (current_char - 'a') + 10;
            } else if (current_char >= 'A' && current_char <= 'F') {
                *value = (*value * base) + (current_char - 'A') + 10;
            } else {
                break;
            }
        } else {
            frg_die("unexpected base %i", base);
        }

        frg_parsing_token_reader_step(reader);
        digit_counter++;

        if (max_digits > 0 && digit_counter >= max_digits) {
            break;
        }
    }

    if (allow_underscores) {
        while (frg_parsing_token_reader_get_current_char(reader) == '_') {
            frg_parsing_token_reader_step(reader);
        }
    }

    return digit_counter;
}

bool _frg_parse_char_unquoted(
    frg_message_buffer_t* message_buffer,
    frg_char_t *value,
    frg_parsing_token_reader_t* reader,
    char quote
) {
    frg_assert_pointer_non_null(value);
    frg_assert_pointer_non_null(reader);
    frg_assert(quote == '"' || quote == '\'');

    if (frg_parsing_token_reader_get_current_char(reader) == '\\') {
        frg_parsing_token_reader_step(reader);

        if (frg_parsing_token_reader_get_current_char(reader) == 0) {
            frg_parsing_range_t source_range = {
                .start = *frg_parsing_token_reader_get_current_location(reader),
                .length = 1
            };

            frg_message_emit_from_source_range(
                message_buffer,
                &source_range,
                FRG_MESSAGE_SEVERITY_ERROR,
                "'\\' in string expects at least one character afterwards"
            );

            return false;
        } else if (frg_parsing_token_reader_get_current_char(reader) == 'a') {
            *value = '\a';
            frg_parsing_token_reader_step(reader);
        } else if (frg_parsing_token_reader_get_current_char(reader) == 'b') {
            *value = '\b';
            frg_parsing_token_reader_step(reader);
        } else if (frg_parsing_token_reader_get_current_char(reader) == 'e') {
            *value = '\033';
            frg_parsing_token_reader_step(reader);
        } else if (frg_parsing_token_reader_get_current_char(reader) == 'f') {
            *value = '\f';
            frg_parsing_token_reader_step(reader);
        } else if (frg_parsing_token_reader_get_current_char(reader) == 'n') {
            *value = '\n';
            frg_parsing_token_reader_step(reader);
        } else if (frg_parsing_token_reader_get_current_char(reader) == 'r') {
            *value = '\r';
            frg_parsing_token_reader_step(reader);
        } else if (frg_parsing_token_reader_get_current_char(reader) == 't') {
            *value = '\t';
            frg_parsing_token_reader_step(reader);
        } else if (frg_parsing_token_reader_get_current_char(reader) == 'v') {
            *value = '\v';
            frg_parsing_token_reader_step(reader);
        } else if (frg_parsing_token_reader_get_current_char(reader) == '0') {
            *value = '\0';
            frg_parsing_token_reader_step(reader);
        } else if (frg_parsing_token_reader_get_current_char(reader) == 'x') {
            frg_parsing_token_reader_step(reader);

            uint64_t value_uncasted = 0;

            size_t digit_count = _frg_parse_uint_partial(
                &value_uncasted,
                reader,
                16,
                2,
                false
            );

            *value = value_uncasted;

            frg_parsing_range_t source_range = {
                .start = *frg_parsing_token_reader_get_current_location(reader),
                .length = 1
            };

            if (digit_count == 0) {
                frg_message_emit_from_source_range(
                    message_buffer,
                    &source_range,
                    FRG_MESSAGE_SEVERITY_ERROR,
                    "Escape code '\\x' expects 2 hexadecimal digits afterwards, but got none",
                    digit_count
                );
            } else if (digit_count == 1) {
                frg_message_emit_from_source_range(
                    message_buffer,
                    &source_range,
                    FRG_MESSAGE_SEVERITY_ERROR,
                    "Escape code '\\x' expects 2 hexadecimal digits afterwards, but only got 1",
                    digit_count
                );
            } else if (digit_count > 2) {
                frg_message_emit_from_source_range(
                    message_buffer,
                    &source_range,
                    FRG_MESSAGE_SEVERITY_INTERNAL_ERROR,
                    "Escape code '\\x' expects 2 hexadecimal digits afterwards, but got %i",
                    digit_count
                );
            }
        } else if (frg_parsing_token_reader_get_current_char(reader) == 'u') {
            *value = 0;

            frg_parsing_token_reader_step(reader);

            if (frg_parsing_token_reader_get_current_char(reader) == 0) {
                frg_parsing_range_t source_range = {
                    .start = *frg_parsing_token_reader_get_current_location(reader),
                    .length = 1
                };

                frg_message_emit_from_source_range(
                    message_buffer,
                    &source_range,
                    FRG_MESSAGE_SEVERITY_ERROR,
                    "'\\u' in string expects '{' afterwards, not end of string"
                );

                return false;
            } else if (frg_parsing_token_reader_get_current_char(reader) != '{') {
                frg_parsing_range_t source_range = {
                    .start = *frg_parsing_token_reader_get_current_location(reader),
                    .length = 1
                };

                frg_message_emit_from_source_range(
                    message_buffer,
                    &source_range,
                    FRG_MESSAGE_SEVERITY_ERROR,
                    "'\\u' in string expects '{' afterwards"
                );

                return false;
            }

            frg_parsing_token_reader_step(reader);

            uint64_t value_uncasted = 0;

            size_t digit_count = _frg_parse_uint_partial(
                &value_uncasted,
                reader,
                16,
                0,
                true
            );

            if (value_uncasted > 0xffffff) {
                frg_parsing_range_t source_range = {
                    .start = *frg_parsing_token_reader_get_current_location(reader),
                    .length = 1
                };

                frg_message_emit_from_source_range(
                    message_buffer,
                    &source_range,
                    FRG_MESSAGE_SEVERITY_INTERNAL_ERROR,
                    "Unicode code point specified exceeds 0xffffff (%Lx)",
                    value_uncasted
                );

                return false;
            }

            *value = value_uncasted;

            if (digit_count == 0) {
                frg_parsing_range_t source_range = {
                    .start = *frg_parsing_token_reader_get_current_location(reader),
                    .length = 1
                };

                frg_message_emit_from_source_range(
                    message_buffer,
                    &source_range,
                    FRG_MESSAGE_SEVERITY_ERROR,
                    "Escape code '\\u{' expects at least 1 hexadecimal digit afterwards, but got none"
                );

                return false;
            } else if (digit_count > 6) {
                frg_parsing_range_t source_range = {
                    .start = *frg_parsing_token_reader_get_current_location(reader),
                    .length = 1
                };

                frg_message_emit_from_source_range(
                    message_buffer,
                    &source_range,
                    FRG_MESSAGE_SEVERITY_ERROR,
                    "Escape code '\\u{' expects at most 6 hexadecimal digits afterwards, but got %i",
                    digit_count
                );

                return false;
            }

            if (frg_parsing_token_reader_get_current_char(reader) != '}') {
                frg_parsing_range_t source_range = {
                    .start = *frg_parsing_token_reader_get_current_location(reader),
                    .length = 1
                };

                frg_message_emit_from_source_range(
                    message_buffer,
                    &source_range,
                    FRG_MESSAGE_SEVERITY_ERROR,
                    "Escape code '\\u{' in string expects closing '}'"
                );

                return false;
            }

            frg_parsing_token_reader_step(reader);
        } else {
            *value = frg_parsing_token_reader_get_current_char(reader);
            frg_parsing_token_reader_step(reader);
        }

        return true;
    } else if (frg_parsing_token_reader_get_current_char(reader) == quote) {
        return true;
    } else {
        *value = frg_parsing_token_reader_get_current_char(reader);
        frg_parsing_token_reader_step(reader);
        return true;
    }
}

GString* _frg_parse_string_with_quote_char(
    frg_message_buffer_t* message_buffer,
    frg_parsing_token_reader_t* reader,
    char quote
) {
    frg_assert_pointer_non_null(message_buffer);
    frg_assert_pointer_non_null(reader);

    if (frg_parsing_token_reader_get_current_char(reader) == 0) {
        frg_message_emit(
            message_buffer,
            FRG_MESSAGE_SEVERITY_INTERNAL_ERROR,
            "Cannot parse empty string as literal"
        );

        return NULL;
    }

    if (frg_parsing_token_reader_get_current_char(reader) != quote) {
        frg_parsing_range_t source_range = {
            .start = *frg_parsing_token_reader_get_current_location(reader),
            .length = 1
        };
        
        frg_message_emit_from_source_range(
            message_buffer,
            &source_range,
            FRG_MESSAGE_SEVERITY_INTERNAL_ERROR,
            "Literal must start with a quote"
        );

        return NULL;
    }

    frg_parsing_token_reader_step(reader);

    if (frg_parsing_token_reader_get_current_char(reader) == 0) {
        // TODO: Write a function for token readers to do this automatically
        frg_parsing_range_t source_range = {
            .start = *frg_parsing_token_reader_get_current_location(reader),
            .length = 1
        };
        
        frg_message_emit_from_source_range(
            message_buffer,
            &source_range,
            FRG_MESSAGE_SEVERITY_ERROR,
            "Unexpected end of character literal"
        );

        return NULL;
    }

    GString* value = g_string_new(NULL);

    while (frg_parsing_token_reader_get_current_char(reader) != 0) {
        if (frg_parsing_token_reader_get_current_char(reader) == quote) {
            break;
        }

        frg_char_t current_character;
        if (!_frg_parse_char_unquoted(
            message_buffer,
            &current_character,
            reader,
            quote
        )) {
            return NULL;
        }

        g_string_append_c(value, current_character);
    }

    if (frg_parsing_token_reader_get_current_char(reader) == 0) {
        frg_parsing_range_t source_range = {
            .start = *frg_parsing_token_reader_get_current_location(reader),
            .length = 1
        };
        
        frg_message_emit_from_source_range(
            message_buffer,
            &source_range,
            FRG_MESSAGE_SEVERITY_ERROR,
            "Unexpected end of literal"
        );

        return NULL;
    } else if (frg_parsing_token_reader_get_current_char(reader) != quote) {
        frg_parsing_range_t source_range = {
            .start = *frg_parsing_token_reader_get_current_location(reader),
            .length = 1
        };
        
        frg_message_emit_from_source_range(
            message_buffer,
            &source_range,
            FRG_MESSAGE_SEVERITY_ERROR,
            "Opening quote expects closing quote"
        );

        return NULL;
    }

    return value;
}

bool frg_parse_char(
    frg_message_buffer_t* message_buffer,
    frg_char_t* value,
    frg_parsing_token_reader_t* reader
) {
    GString* value_string = _frg_parse_string_with_quote_char(
        message_buffer,
        reader,
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

    frg_assert_int_le(bytes_read, value_string->len);

    *value = codepoint;

    if (bytes_read < value_string->len) {
        frg_parsing_range_t source_range = {
            .start = *frg_parsing_token_reader_get_current_location(reader),
            .length = 1
        };
        
        frg_message_t* message = frg_message_emit_from_source_range(
            message_buffer,
            &source_range,
            FRG_MESSAGE_SEVERITY_ERROR,
            "Character literal contains more than one character"
        );

        frg_message_emit_child_from_source_range(
            message_buffer,
            message,
            &source_range,
            FRG_MESSAGE_SEVERITY_NOTE,
            "If this was supposed to be a string, use double quotes instead"
        );

        return false;
    }

    return true;
}

GString* frg_format_str(const char* value) {
    frg_assert_pointer_non_null(value);

    size_t length = strlen(value);

    GString* formatted = g_string_new("\"");

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
        
        _frg_format_char_unquoted(formatted, codepoint, '"');
    }

    g_string_append_c(formatted, '\"');

    return formatted;
}

bool frg_parse_str(
    frg_message_buffer_t* message_buffer,
    GString** value,
    frg_parsing_token_reader_t* reader
) {
    *value = _frg_parse_string_with_quote_char(
        message_buffer,
        reader,
        '"'
    );

    return *value != NULL;
}

void _frg_format_base_prefix(
    GString* formatted,
    frg_int_base_t base
) {
    if (base == 2) {
        g_string_append(formatted, "0b");
    } else if (base == 8) {
        g_string_append(formatted, "0o");
    } else if (base == 16) {
        g_string_append(formatted, "0x");
    }
}

void _frg_format_uint_without_base_prefix(
    GString* str,
    uint64_t value,
    frg_int_base_t base
) {
    frg_assert(base == 2 || base == 8 || base == 10 || base == 16);

    if (value == 0) {
        g_string_append_c(str, '0');
    } else {
        gchar* after_prefix = str->str + str->len;
        uint64_t current_value = value;

        while (current_value != 0) {
            uint64_t digit = current_value % base;
            current_value /= base;

            if (digit < 10) {
                g_string_append_c(str, '0' + digit);
            } else {
                g_string_append_c(str, 'a' + digit - 10);
            }
        }

        g_strreverse(after_prefix);
    }
}

GString* frg_format_uint(
    uint64_t value,
    frg_int_base_t base
) {
    frg_assert(base == 2 || base == 8 || base == 10 || base == 16);

    GString* str = g_string_new(NULL);

    _frg_format_base_prefix(str, base);

    _frg_format_uint_without_base_prefix(
        str,
        value,
        base
    );
    
    return str;
}

GString* frg_format_uint_with_suffix(
    uint64_t value,
    frg_int_base_t base,
    bool is_signed,
    frg_bit_width_t bit_width
) {
    frg_assert(bit_width == 8 || bit_width == 16 || bit_width == 32 || bit_width == 64);

    GString* result = frg_format_uint(
        value,
        base
    );

    if (is_signed) {
        g_string_append_c(result, 'i');
    } else {
        g_string_append_c(result, 'u');
    }

    g_string_append_printf(result, "%u", bit_width);

    return result;
}

void _frg_parse_base_prefix(
    frg_int_base_t* base,
    frg_parsing_token_reader_t* reader
) {
    frg_assert_pointer_non_null(base);
    frg_assert_pointer_non_null(reader);

    frg_parsing_token_reader_t* state = frg_parsing_token_reader_save(reader);

    if (frg_parsing_token_reader_get_current_char(reader) == '0') {
        frg_parsing_token_reader_step(reader);

        if (frg_parsing_token_reader_get_current_char(reader) == 'b') {
            *base = 2;
            frg_parsing_token_reader_step(reader);
        } else if (frg_parsing_token_reader_get_current_char(reader) == 'o') {
            *base = 8;
            frg_parsing_token_reader_step(reader);
        } else if (frg_parsing_token_reader_get_current_char(reader) == 'x') {
            *base = 16;
            frg_parsing_token_reader_step(reader);
        } else {
            *base = 10;
            frg_parsing_token_reader_restore(reader, &state);
        }
    } else {
        *base = 10;
    }
}

bool frg_parse_uint(
    frg_message_buffer_t* message_buffer,
    frg_parse_uint_result_t* result,
    frg_parsing_token_reader_t* reader
) {
    frg_assert_pointer_non_null(message_buffer);
    frg_assert_pointer_non_null(result);
    frg_assert_pointer_non_null(reader);

    if (frg_parsing_token_reader_get_current_char(reader) == 0) {
        frg_message_emit(
            message_buffer,
            FRG_MESSAGE_SEVERITY_INTERNAL_ERROR,
            "Cannot parse empty string as literal"
        );

        return false;
    }

    result->value = 0;

    uint32_t base = 10;
    _frg_parse_base_prefix(&base, reader);

    if (frg_parsing_token_reader_get_current_char(reader) == 0) {
        frg_parsing_range_t source_range = {
            .start = *frg_parsing_token_reader_get_current_location(reader),
            .length = 1
        };

        frg_message_emit_from_source_range(
            message_buffer,
            &source_range,
            FRG_MESSAGE_SEVERITY_INTERNAL_ERROR,
            "Unexpected end of literal"
        );

        return false;
    }

    size_t digit_count = _frg_parse_uint_partial(
        &result->value,
        reader,
        base,
        0,
        true
    );

    if (digit_count == 0) {
        frg_parsing_range_t source_range = {
            .start = *frg_parsing_token_reader_get_current_location(reader),
            .length = 1
        };

        frg_message_emit_from_source_range(
            message_buffer,
            &source_range,
            FRG_MESSAGE_SEVERITY_INTERNAL_ERROR,
            "Unexpected character in literal"
        );

        return false;
    } else if (frg_parsing_token_reader_get_current_char(reader) == 0) {
        result->is_signed = false;
        result->bit_width = 64;
    } else if (frg_parsing_token_reader_get_current_char(reader) == 'u' || frg_parsing_token_reader_get_current_char(reader) == 'i') {
        result->is_signed = frg_parsing_token_reader_get_current_char(reader) == 'i';
        frg_parsing_token_reader_step(reader);

        uint64_t bit_width_uncasted = 0;

        size_t digit_count = _frg_parse_uint_partial(
            &bit_width_uncasted,
            reader,
            10,
            0,
            false
        );

        if (!_frg_is_valid_bit_width(bit_width_uncasted)) {
            frg_parsing_range_t source_range = {
                .start = *frg_parsing_token_reader_get_current_location(reader),
                .length = 1
            };

            frg_message_emit_from_source_range(
                message_buffer,
                &source_range,
                FRG_MESSAGE_SEVERITY_INTERNAL_ERROR,
                "Invalid bit width for literal: %Lu",
                bit_width_uncasted
            );

            return false;
        }

        result->bit_width = bit_width_uncasted;

        if (digit_count == 0 || frg_parsing_token_reader_get_current_char(reader) != 0) {
            frg_parsing_range_t source_range = {
                .start = *frg_parsing_token_reader_get_current_location(reader),
                .length = 1
            };

            frg_message_emit_from_source_range(
                message_buffer,
                &source_range,
                FRG_MESSAGE_SEVERITY_INTERNAL_ERROR,
                "Unexpected character in literal"
            );

            return false;
        }
    } else {
        frg_parsing_range_t source_range = {
            .start = *frg_parsing_token_reader_get_current_location(reader),
            .length = 1
        };

        frg_message_emit_from_source_range(
            message_buffer,
            &source_range,
            FRG_MESSAGE_SEVERITY_INTERNAL_ERROR,
            "Unexpected character in literal"
        );

        return false;
    }

    return true;
}

void _frg_float_decompose(
    frg_f64_t value,
    frg_f64_t *integral_part,
    frg_f64_t *fractional_part
) {
    frg_assert_pointer_non_null(integral_part);
    frg_assert_pointer_non_null(fractional_part);

    *integral_part = floor(value);
    *fractional_part = value - *integral_part;
}

GString* _frg_format_float_without_scientific_notation(
    frg_f64_t value,
    frg_int_base_t base
) {
    frg_assert(base == 2 || base == 8 || base == 10 || base == 16);

    GString* str = g_string_new(NULL);

    _frg_format_base_prefix(str, base);

    frg_f64_t integral_part = 0.0;
    frg_f64_t fractional_part = 0.0;

    _frg_float_decompose(fabs(value), &integral_part, &fractional_part);

    if (value < 0.0) {
        g_string_append_c(str, '-');
    }

    if (integral_part == 0.0) {
        g_string_append_c(str, '0');
    } else {
        gchar* after_prefix = str->str + str->len;

        while (integral_part != 0.0) {
            uint32_t digit = (uint32_t)fmod(integral_part, base);

            if (digit < 10) {
                g_string_append_c(str, '0' + digit);
            } else {
                g_string_append_c(str, 'a' + digit - 10);
            }

            integral_part /= base;
        }

        g_strreverse(after_prefix);
    }

    g_string_append_c(str, '.');

    if (fractional_part == 0.0) {
        g_string_append_c(str, '0');
    } else {
        while (fractional_part != 0.0) {
            uint32_t digit = (uint32_t)fmod(fractional_part * base, base);

            if (digit < 10) {
                g_string_append_c(str, '0' + digit);
            } else {
                g_string_append_c(str, 'a' + digit - 10);
            }

            fractional_part *= base;
            fractional_part -= floor(fractional_part);
        }
    }

    return str;
}

GString* frg_format_float(
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

        GString* str = _frg_format_float_without_scientific_notation(value * negative_factor, base);

        g_string_append_c(str, 'e');

        _frg_format_uint_without_base_prefix(
            str,
            exponent,
            base
        );

        return str;
    } else {
        return _frg_format_float_without_scientific_notation(value, base);
    }
}

GString* frg_format_float_with_suffix(
    frg_f64_t value,
    frg_int_base_t base,
    bool scientific_notation,
    frg_bit_width_t bit_width
) {
    frg_assert(bit_width == 32 || bit_width == 64);

    GString* result = frg_format_float(
        value,
        base,
        scientific_notation
    );

    g_string_append_printf(result, "f%u", bit_width);

    return result;
}

bool _frg_parse_float_without_scientific_notation(
    frg_message_buffer_t* message_buffer,
    frg_parse_float_result_t* result,
    frg_parsing_token_reader_t* reader,
    uint32_t base
) {
    frg_assert_pointer_non_null(message_buffer);
    frg_assert_pointer_non_null(result);
    frg_assert_pointer_non_null(reader);

    if (frg_parsing_token_reader_get_current_char(reader) == 0) {
        frg_message_emit(
            message_buffer,
            FRG_MESSAGE_SEVERITY_INTERNAL_ERROR,
            "Cannot parse empty string as literal"
        );

        return false;
    }

    result->value = 0.0;
    result->bit_width = 64;

    if (frg_parsing_token_reader_get_current_char(reader) == 0) {
        frg_parsing_range_t source_range = {
            .start = *frg_parsing_token_reader_get_current_location(reader),
            .length = 1
        };

        frg_message_emit_from_source_range(
            message_buffer,
            &source_range,
            FRG_MESSAGE_SEVERITY_INTERNAL_ERROR,
            "Unexpected end of literal"
        );

        return false;
    }

    frg_f64_t integral_part = 0.0;

    while (true) {
        char current_char = frg_parsing_token_reader_get_current_char(reader);

        while (current_char == '_') {
            frg_parsing_token_reader_step(reader);
            current_char = frg_parsing_token_reader_get_current_char(reader);
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

        frg_parsing_token_reader_step(reader);
    }

    while (frg_parsing_token_reader_get_current_char(reader) == '_') {
        frg_parsing_token_reader_step(reader);
    }

    if (frg_parsing_token_reader_get_current_char(reader) == 0) {
        frg_parsing_range_t source_range = {
            .start = *frg_parsing_token_reader_get_current_location(reader),
            .length = 1
        };

        frg_message_emit_from_source_range(
            message_buffer,
            &source_range,
            FRG_MESSAGE_SEVERITY_INTERNAL_ERROR,
            "Cannot parse string without '.' as float literal"
        );

        return false;
    } else if (frg_parsing_token_reader_get_current_char(reader) != '.') {
        frg_parsing_range_t source_range = {
            .start = *frg_parsing_token_reader_get_current_location(reader),
            .length = 1
        };

        frg_message_emit_from_source_range(
            message_buffer,
            &source_range,
            FRG_MESSAGE_SEVERITY_ERROR,
            "Unexpected character in literal"
        );

        return false;
    }

    frg_parsing_token_reader_step(reader);

    frg_f64_t fractional_part = 0.0;
    frg_f64_t fractional_factor = 1.0;

    while (true) {
        char current_char = frg_parsing_token_reader_get_current_char(reader);

        while (current_char == '_') {
            frg_parsing_token_reader_step(reader);
            current_char = frg_parsing_token_reader_get_current_char(reader);
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
        frg_parsing_token_reader_step(reader);
    }

    result->value = integral_part + fractional_part * fractional_factor;

    return true;
}



bool frg_parse_float(
    frg_message_buffer_t* message_buffer,
    frg_parse_float_result_t* result,
    frg_parsing_token_reader_t* reader
) {
    frg_parse_float_result_t result_without_scientific_notation;

    uint32_t base = 0;
    _frg_parse_base_prefix(&base, reader);

    if (!_frg_parse_float_without_scientific_notation(
        message_buffer,
        &result_without_scientific_notation,
        reader,
        base
    )) {
        return false;
    }

    *result = result_without_scientific_notation;

    if (frg_parsing_token_reader_get_current_char(reader) == 'e') {
        frg_parsing_token_reader_step(reader);

        double exponent_negative_factor = 1.0;

        if (frg_parsing_token_reader_get_current_char(reader) == '-') {
            exponent_negative_factor = -1.0;
            frg_parsing_token_reader_step(reader);
        }

        frg_parse_float_result_t result_exponent;

        if (!_frg_parse_float_without_scientific_notation(
            message_buffer,
            &result_exponent,
            reader,
            base
        )) {
            return false;
        }

        result->value *= pow(
            base,
            result_exponent.value * exponent_negative_factor
        );
    }

    if (frg_parsing_token_reader_get_current_char(reader) == 'f') {
        frg_parsing_token_reader_step(reader);

        uint64_t bit_width_uncasted = 0;

        size_t digit_count = _frg_parse_uint_partial(
            &bit_width_uncasted,
            reader,
            10,
            0,
            false
        );

        if (digit_count == 0) {
            frg_parsing_range_t source_range = {
                .start = *frg_parsing_token_reader_get_current_location(reader),
                .length = 1
            };

            frg_message_emit_from_source_range(
                message_buffer,
                &source_range,
                FRG_MESSAGE_SEVERITY_INTERNAL_ERROR,
                "Unexpected character in literal"
            );

            return false;
        } else if (bit_width_uncasted != 32 && bit_width_uncasted != 64) {
            frg_parsing_range_t source_range = {
                .start = *frg_parsing_token_reader_get_current_location(reader),
                .length = 1
            };

            frg_message_emit_from_source_range(
                message_buffer,
                &source_range,
                FRG_MESSAGE_SEVERITY_INTERNAL_ERROR,
                "Invalid bit width for literal: %Lu",
                bit_width_uncasted
            );

            result->bit_width = bit_width_uncasted;
        }
    }

    if (frg_parsing_token_reader_get_current_char(reader) != 0) {
        frg_parsing_range_t source_range = {
            .start = *frg_parsing_token_reader_get_current_location(reader),
            .length = 1
        };

        frg_message_emit_from_source_range(
            message_buffer,
            &source_range,
            FRG_MESSAGE_SEVERITY_INTERNAL_ERROR,
            "Unexpected character in literal"
        );

        return false;
    }

    return true;
}
