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

bool _frg_is_char_printable(frg_char_t value) {
    if (value < 0x80) {
        return isprint(value);
    }

    switch (utf8proc_category((utf8proc_int32_t)value)) {
        case UTF8PROC_CATEGORY_CN:
        case UTF8PROC_CATEGORY_ZL:
        case UTF8PROC_CATEGORY_ZP:
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

frg_recoverable_status_t _frg_parse_char_unquoted(
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
            frg_log_error_at_source_char(
                frg_parsing_token_reader_get_current_location(reader)->path,
                frg_parsing_token_reader_get_current_location(reader)->lineno,
                frg_parsing_token_reader_get_current_location(reader)->columnno,
                "'\\' in string expects at least one character afterwards"
            );
            return FRG_RECOVERABLE_STATUS_ERROR_UNEXPECTED_END_OF_TEXT;
        } else if (frg_parsing_token_reader_get_current_char(reader) == 'a') {
            *value = '\a';
        } else if (frg_parsing_token_reader_get_current_char(reader) == 'b') {
            *value = '\b';
        } else if (frg_parsing_token_reader_get_current_char(reader) == 'e') {
            *value = '\033';
        } else if (frg_parsing_token_reader_get_current_char(reader) == 'f') {
            *value = '\f';
        } else if (frg_parsing_token_reader_get_current_char(reader) == 'n') {
            *value = '\n';
        } else if (frg_parsing_token_reader_get_current_char(reader) == 'r') {
            *value = '\r';
        } else if (frg_parsing_token_reader_get_current_char(reader) == 't') {
            *value = '\t';
        } else if (frg_parsing_token_reader_get_current_char(reader) == 'v') {
            *value = '\v';
        } else if (frg_parsing_token_reader_get_current_char(reader) == '0') {
            *value = '\0';
        } else if (frg_parsing_token_reader_get_current_char(reader) == 'x') {
            *value = 0;

            for (int digit = 0; digit < 2; digit++) {
                frg_parsing_token_reader_step(reader);

                if (frg_parsing_token_reader_get_current_char(reader) == 0) {
                    frg_log_error_at_source_char(
                        frg_parsing_token_reader_get_current_location(reader)->path,
                        frg_parsing_token_reader_get_current_location(reader)->lineno,
                        frg_parsing_token_reader_get_current_location(reader)->columnno,
                        "'\\x' in string expects two hexadecimal digits afterwards"
                    );
                    return FRG_RECOVERABLE_STATUS_ERROR_UNEXPECTED_END_OF_TEXT;
                } else if (frg_parsing_token_reader_get_current_char(reader) >= '0' && frg_parsing_token_reader_get_current_char(reader) <= '9') {
                    *value = (*value << 4) | (frg_parsing_token_reader_get_current_char(reader) - '0');
                } else if (frg_parsing_token_reader_get_current_char(reader) >= 'a' && frg_parsing_token_reader_get_current_char(reader) <= 'f') {
                    *value = (*value << 4) | (frg_parsing_token_reader_get_current_char(reader) - 'a' + 10);
                } else if (frg_parsing_token_reader_get_current_char(reader) >= 'A' && frg_parsing_token_reader_get_current_char(reader) <= 'F') {
                    *value = (*value << 4) | (frg_parsing_token_reader_get_current_char(reader) - 'A' + 10);
                } else {
                    // TODO: Make error handling better here
                    frg_log_error_at_source_char(
                        frg_parsing_token_reader_get_current_location(reader)->path,
                        frg_parsing_token_reader_get_current_location(reader)->lineno,
                        frg_parsing_token_reader_get_current_location(reader)->columnno,
                        "character is not a valid hexadecimal digit to come after '\\x'"
                    );
                    return FRG_RECOVERABLE_STATUS_ERROR_UNEXPECTED_CHARACTER;
                }
            }
        } else if (frg_parsing_token_reader_get_current_char(reader) == 'u') {
            *value = 0;

            frg_parsing_token_reader_step(reader);

            if (frg_parsing_token_reader_get_current_char(reader) == 0) {
                frg_log_error_at_source_char(
                    frg_parsing_token_reader_get_current_location(reader)->path,
                    frg_parsing_token_reader_get_current_location(reader)->lineno,
                    frg_parsing_token_reader_get_current_location(reader)->columnno,
                    "'\\u' in string expects '{' afterwards, not end of string"
                );
                return FRG_RECOVERABLE_STATUS_ERROR_UNEXPECTED_END_OF_TEXT;
            } else if (frg_parsing_token_reader_get_current_char(reader) != '{') {
                // TODO: Make error handling better here
                frg_log_error_at_source_char(
                    frg_parsing_token_reader_get_current_location(reader)->path,
                    frg_parsing_token_reader_get_current_location(reader)->lineno,
                    frg_parsing_token_reader_get_current_location(reader)->columnno,
                    "'\\u' in string expects '{' afterwards"
                );
                return FRG_RECOVERABLE_STATUS_ERROR_UNEXPECTED_CHARACTER;
            }

            for (int digit = 0; digit < 6; digit++) {
                frg_parsing_token_reader_step(reader);

                if (frg_parsing_token_reader_get_current_char(reader) == 0) {
                    frg_log_error_at_source_char(
                        frg_parsing_token_reader_get_current_location(reader)->path,
                        frg_parsing_token_reader_get_current_location(reader)->lineno,
                        frg_parsing_token_reader_get_current_location(reader)->columnno,
                        "'\\u{' in string expects closing '}', not end of string"
                    );
                    return FRG_RECOVERABLE_STATUS_ERROR_UNEXPECTED_END_OF_TEXT;
                } else if (frg_parsing_token_reader_get_current_char(reader) == '}') {
                    break;
                } else if (frg_parsing_token_reader_get_current_char(reader) >= '0' && frg_parsing_token_reader_get_current_char(reader) <= '9') {
                    *value = (*value << 4) | (frg_parsing_token_reader_get_current_char(reader) - '0');
                } else if (frg_parsing_token_reader_get_current_char(reader) >= 'a' && frg_parsing_token_reader_get_current_char(reader) <= 'f') {
                    *value = (*value << 4) | (frg_parsing_token_reader_get_current_char(reader) - 'a' + 10);
                } else if (frg_parsing_token_reader_get_current_char(reader) >= 'A' && frg_parsing_token_reader_get_current_char(reader) <= 'F') {
                    *value = (*value << 4) | (frg_parsing_token_reader_get_current_char(reader) - 'A' + 10);
                } else {
                    // TODO: Make error handling better here
                    frg_log_error_at_source_char(
                        frg_parsing_token_reader_get_current_location(reader)->path,
                        frg_parsing_token_reader_get_current_location(reader)->lineno,
                        frg_parsing_token_reader_get_current_location(reader)->columnno,
                        "character is not a valid hexadecimal digit to come after '\\u{'"
                    );
                    return FRG_RECOVERABLE_STATUS_ERROR_UNEXPECTED_CHARACTER;
                }
            }
        } else {
            *value = frg_parsing_token_reader_get_current_char(reader);
        }

        return FRG_RECOVERABLE_STATUS_OK;
    } else if (frg_parsing_token_reader_get_current_char(reader) == quote) {
        return FRG_RECOVERABLE_STATUS_OK;
    } else {
        *value = frg_parsing_token_reader_get_current_char(reader);
        return FRG_RECOVERABLE_STATUS_OK;
    }
}

frg_recoverable_status_t frg_parse_char(frg_char_t* value, frg_parsing_token_reader_t* reader) {
    frg_assert_pointer_non_null(value);
    frg_assert_pointer_non_null(reader);

    if (frg_parsing_token_reader_get_current_char(reader) != '\'') {
        // TODO: Make error handling better here
        frg_log_error_at_source_char(
            frg_parsing_token_reader_get_current_location(reader)->path,
            frg_parsing_token_reader_get_current_location(reader)->lineno,
            frg_parsing_token_reader_get_current_location(reader)->columnno,
            "character literal must start with single quote"
        );
        return FRG_RECOVERABLE_STATUS_ERROR_UNEXPECTED_CHARACTER;
    }

    frg_parsing_token_reader_step(reader);

    if (frg_parsing_token_reader_get_current_char(reader) == 0) {
        frg_log_error_at_source_char(
            frg_parsing_token_reader_get_current_location(reader)->path,
            frg_parsing_token_reader_get_current_location(reader)->lineno,
            frg_parsing_token_reader_get_current_location(reader)->columnno,
            "single quote expects literal character afterwards, not end of string"
        );
        return FRG_RECOVERABLE_STATUS_ERROR_UNEXPECTED_END_OF_TEXT;
    }

    frg_recoverable_status_t result = _frg_parse_char_unquoted(value, reader, '\'');
    if (result != FRG_RECOVERABLE_STATUS_OK) {
        return result;
    }

    frg_parsing_token_reader_step(reader);

    if (frg_parsing_token_reader_get_current_char(reader) == 0) {
        frg_log_error_at_source_char(
            frg_parsing_token_reader_get_current_location(reader)->path,
            frg_parsing_token_reader_get_current_location(reader)->lineno,
            frg_parsing_token_reader_get_current_location(reader)->columnno,
            "single quote expects closing single quote, not end of string"
        );
        return FRG_RECOVERABLE_STATUS_ERROR_UNEXPECTED_END_OF_TEXT;
    } else if (frg_parsing_token_reader_get_current_char(reader) != '\'') {
        // TODO: Make error handling better here
        frg_log_error_at_source_char(
            frg_parsing_token_reader_get_current_location(reader)->path,
            frg_parsing_token_reader_get_current_location(reader)->lineno,
            frg_parsing_token_reader_get_current_location(reader)->columnno,
            "character literal must end with single quote"
        );
        return FRG_RECOVERABLE_STATUS_ERROR_UNEXPECTED_CHARACTER;
    }

    return FRG_RECOVERABLE_STATUS_OK;
}

GString* frg_format_str(const char* value) {
    frg_assert_pointer_non_null(value);

    GString* formatted = g_string_new("\"");

    for (const char* iter = value; *iter != 0; iter++) {
        _frg_format_char_unquoted(formatted, *iter, '"');
    }

    g_string_append_c(formatted, '\"');

    return formatted;
}

frg_recoverable_status_t frg_parse_str(GString** value, frg_parsing_token_reader_t* reader) {
    frg_assert_pointer_non_null(value);
    frg_assert_pointer_non_null(reader);

    if (frg_parsing_token_reader_get_current_char(reader) == 0) {
        return FRG_RECOVERABLE_STATUS_ERROR_UNEXPECTED_END_OF_TEXT;
    }

    if (frg_parsing_token_reader_get_current_char(reader) != '"') {
        // TODO: Make error handling better here
        return FRG_RECOVERABLE_STATUS_ERROR_UNEXPECTED_CHARACTER;
    }

    frg_parsing_token_reader_step(reader);

    if (frg_parsing_token_reader_get_current_char(reader) == 0) {
        return FRG_RECOVERABLE_STATUS_ERROR_UNEXPECTED_END_OF_TEXT;
    }

    *value = g_string_new(NULL);

    while (frg_parsing_token_reader_get_current_char(reader) != 0) {
        if (frg_parsing_token_reader_get_current_char(reader) == '"') {
            break;
        }

        frg_char_t current_character;
        frg_recoverable_status_t result = _frg_parse_char_unquoted(&current_character, reader, '"');
        if (result != FRG_RECOVERABLE_STATUS_OK) {
            return result;
        }

        g_string_append_c(*value, current_character);

        frg_parsing_token_reader_step(reader);
    }

    if (frg_parsing_token_reader_get_current_char(reader) == 0) {
        return FRG_RECOVERABLE_STATUS_ERROR_UNEXPECTED_END_OF_TEXT;
    } else if (frg_parsing_token_reader_get_current_char(reader) != '"') {
        // TODO: Make error handling better here
        return FRG_RECOVERABLE_STATUS_ERROR_UNEXPECTED_CHARACTER;
    }

    return FRG_RECOVERABLE_STATUS_OK;
}

GString* frg_format_uint(
    uint64_t value,
    uint32_t base
) {
    frg_assert(base == 2 || base == 8 || base == 10 || base == 16);

    GString* str = g_string_new(NULL);

    if (base == 2) {
        g_string_append(str, "0b");
    } else if (base == 8) {
        g_string_append(str, "0o");
    } else if (base == 16) {
        g_string_append(str, "0x");
    }

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

    return str;
}

frg_recoverable_status_t _frg_parse_base_prefix(
    frg_int_base_t* base,
    frg_parsing_token_reader_t* reader
) {
    frg_assert_pointer_non_null(base);
    frg_assert_pointer_non_null(reader);

    if (frg_parsing_token_reader_get_current_char(reader) == 0) {
        return FRG_RECOVERABLE_STATUS_ERROR_UNEXPECTED_END_OF_TEXT;
    }

    frg_parsing_token_reader_t* state = frg_parsing_token_reader_save(reader);

    if (frg_parsing_token_reader_get_current_char(reader) == '0') {
        frg_parsing_token_reader_step(reader);

        if (frg_parsing_token_reader_get_current_char(reader) == 0) {
            *base = 10;
            frg_parsing_token_reader_restore(reader, &state);
            return FRG_RECOVERABLE_STATUS_OK;
        } else if (frg_parsing_token_reader_get_current_char(reader) == 'b') {
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
        }
    } else {
        *base = 10;
    }

    return FRG_RECOVERABLE_STATUS_OK;
}

frg_recoverable_status_t _frg_get_next_digit(
    uint32_t* value,
    frg_int_base_t base,
    frg_parsing_token_reader_t* reader
) {
    frg_assert_pointer_non_null(value);
    frg_assert(base == 2 || base == 8 || base == 10 || base == 16);
    frg_assert_pointer_non_null(reader);

    while (frg_parsing_token_reader_get_current_char(reader) != 0 && frg_parsing_token_reader_get_current_char(reader) == '_') {
        frg_parsing_token_reader_step(reader);
    }

    if (frg_parsing_token_reader_get_current_char(reader) == 0) {
        return FRG_RECOVERABLE_STATUS_ERROR_UNEXPECTED_END_OF_TEXT;
    }

    if (frg_parsing_token_reader_get_current_char(reader) >= '0' && frg_parsing_token_reader_get_current_char(reader) <= '9') {
        *value = frg_parsing_token_reader_get_current_char(reader) - '0';
    } else if (frg_parsing_token_reader_get_current_char(reader) >= 'a' && frg_parsing_token_reader_get_current_char(reader) <= 'z') {
        *value = frg_parsing_token_reader_get_current_char(reader) - 'a' + 10;
    } else if (frg_parsing_token_reader_get_current_char(reader) >= 'A' && frg_parsing_token_reader_get_current_char(reader) <= 'Z') {
        *value = frg_parsing_token_reader_get_current_char(reader) - 'A' + 10;
    } else {
        // TODO: Make error handling better here
        return FRG_RECOVERABLE_STATUS_ERROR_UNEXPECTED_CHARACTER;
    }

    if (*value >= base) {
        // TODO: Make error handling better here
        return FRG_RECOVERABLE_STATUS_ERROR_UNEXPECTED_CHARACTER;
    }

    frg_parsing_token_reader_step(reader);

    return FRG_RECOVERABLE_STATUS_OK;
}

frg_recoverable_status_t frg_parse_uint(
    uint64_t* value,
    frg_parsing_token_reader_t* reader
) {
    frg_assert_pointer_non_null(value);
    frg_assert_pointer_non_null(reader);

    if (frg_parsing_token_reader_get_current_char(reader) == 0) {
        return FRG_RECOVERABLE_STATUS_ERROR_UNEXPECTED_END_OF_TEXT;
    }

    *value = 0;

    uint32_t base = 10;

    frg_recoverable_status_t result = _frg_parse_base_prefix(&base, reader);
    if (result != FRG_RECOVERABLE_STATUS_OK) {
        return result;
    }

    if (frg_parsing_token_reader_get_current_char(reader) == 0) {
        return FRG_RECOVERABLE_STATUS_ERROR_UNEXPECTED_END_OF_TEXT;
    }

    while (frg_parsing_token_reader_get_current_char(reader) != 0) {
        uint32_t digit;
        frg_recoverable_status_t result = _frg_get_next_digit(&digit, base, reader);
        if (result != FRG_RECOVERABLE_STATUS_OK) {
            return result;
        }

        *value = (*value * base) + digit;
    }

    return FRG_RECOVERABLE_STATUS_OK;
}

// frg_status_t frg_float_to_str(
//     GString** str,
//     frg_f64_t value,
//     uint32_t base
// ) {
//     if (str == NULL) {
//         return FRG_STATUS_ERROR_NULL_ARGUMENT;
//     } else if (*str != NULL || (!(base == 2 || base == 8 || base == 10 || base == 16))) {
//         return FRG_STATUS_ERROR_UNEXPECTED_ARGUMENT_VALUE;
//     }

//     *str = g_string_new(NULL);

//     if (*str == NULL) {
//         return FRG_STATUS_ERROR_OUT_OF_MEMORY;
//     }

//     if (base == 2) {
//         g_string_append(*str, "0b");
//     } else if (base == 8) {
//         g_string_append(*str, "0o");
//     } else if (base == 16) {
//         g_string_append(*str, "0x");
//     }

//     double integer_part = floor(value);
//     double fractional_part = value - integer_part;

//     if (integer_part == 0.0) {
//         g_string_append_c(*str, '0');
//     } else {
//         gchar* after_prefix = (*str)->str + (*str)->len;

//         while (integer_part != 0.0) {
//             uint32_t digit = (uint32_t)fmod(integer_part, base);

//             if (digit < 10) {
//                 g_string_append_c(*str, '0' + digit);
//             } else {
//                 g_string_append_c(*str, 'a' + digit - 10);
//             }

//             integer_part = floor(integer_part / base);
//         }

//         g_strreverse(after_prefix);
//     }

//     g_string_append_c(*str, '.');

//     if (fractional_part == 0.0) {
//         g_string_append_c(*str, '0');
//     } else {
//         while (fractional_part != 0.0) {
//             uint32_t digit = (uint32_t)fmod(fractional_part * base, base);

//             if (digit < 10) {
//                 g_string_append_c(*str, '0' + digit);
//             } else {
//                 g_string_append_c(*str, 'a' + digit - 10);
//             }

//             fractional_part *= base;
//             fractional_part -= floor(fractional_part);
//         }
//     }

//     return FRG_STATUS_OK;
// }

// frg_status_t frg_str_to_float(
//     frg_f64_t* value,
//     const char* str
// ) {
//     if (value == NULL || str == NULL) {
//         return FRG_STATUS_ERROR_NULL_ARGUMENT;
//     } else if (*str == 0) {
//         return FRG_STATUS_ERROR_UNEXPECTED_END_OF_TEXT;
//     }

//     uint32_t base = 10;

//     frg_check(
//         _frg_parse_base_prefix(&base, &str)
//     );

//     if (*str == 0) {
//         return FRG_STATUS_ERROR_UNEXPECTED_END_OF_TEXT;
//     }

//     double integer_part = 0.0;
//     double fractional_part = 0.0;
//     double fractional_factor = 1.0;

//     while (*str != 0) {
//         if (*str == '.') {
//             break;
//         }

//         uint32_t digit;
//         frg_check(
//             _frg_get_next_digit(&digit, base, &str)
//         );

//         integer_part = (integer_part * base) + digit;
//     }

//     if (*str == 0) {
//         return FRG_STATUS_ERROR_UNEXPECTED_END_OF_TEXT;
//     } else if (*str != '.') {
//         return FRG_STATUS_ERROR_UNEXPECTED_CHARACTER;
//     }

//     str++;

//     if (*str == 0) {
//         return FRG_STATUS_ERROR_UNEXPECTED_END_OF_TEXT;
//     }

//     while (*str != 0) {
//         if (*str == '.') {
//             break;
//         }

//         uint32_t digit;
//         frg_check(
//             _frg_get_next_digit(&digit, base, &str)
//         );

//         fractional_part = (fractional_part * base) + digit;
//         fractional_factor /= base;
//     }

//     if (*str == 0) {
//         *value = integer_part + fractional_part * fractional_factor;
//         return FRG_STATUS_OK;
//     } else if (*str != 'e') {
//         return FRG_STATUS_ERROR_UNEXPECTED_CHARACTER;
//     }

//     str++;

//     double exponent_integer_factor = 1.0;
//     double exponent_integer_part = 0.0;
//     double exponent_fractional_part = 0.0;
//     double exponent_fractional_factor = 1.0;

//     if (*str == 0) {
//         return FRG_STATUS_ERROR_UNEXPECTED_END_OF_TEXT;
//     } else if (*str == '-') {
//         exponent_integer_factor = -1.0;
//         str++;
//     }

//     while (*str != 0) {
//         if (*str == '.') {
//             break;
//         }

//         uint32_t digit;
//         frg_check(
//             _frg_get_next_digit(&digit, base, &str)
//         );

//         exponent_integer_part = (exponent_integer_part * base) + digit;
//     }

//     if (*str == 0) {
//         return FRG_STATUS_ERROR_UNEXPECTED_END_OF_TEXT;
//     } else if (*str != '.') {
//         return FRG_STATUS_ERROR_UNEXPECTED_CHARACTER;
//     }

//     str++;

//     if (*str == 0) {
//         return FRG_STATUS_ERROR_UNEXPECTED_END_OF_TEXT;
//     }

//     while (*str != 0) {
//         if (*str == '.') {
//             break;
//         }

//         uint32_t digit;
//         frg_check(
//             _frg_get_next_digit(&digit, base, &str)
//         );

//         exponent_fractional_part = (exponent_fractional_part * base) + digit;
//         exponent_fractional_factor /= base;
//     }

//     *value = pow(
//         integer_part + fractional_part * fractional_factor,
//         exponent_integer_part * exponent_integer_factor + exponent_fractional_part * exponent_fractional_factor
//     );

//     return FRG_STATUS_OK;
// }
