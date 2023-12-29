// Copyright (c) 2023 Sophie Katz
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

frg_status_t _frg_escape_char_unquoted(GString* escaped, frg_char_t value, char quote) {
    if (escaped == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    } else if (quote != '"' && quote != '\'') {
        return FRG_STATUS_ERROR_UNEXPECTED_ARGUMENT_VALUE;
    }

    if (value == quote) {
        g_string_append_printf(escaped, "\\%c", quote);
    } else if (value == 0) {
        g_string_append(escaped, "\\0");
    } else if (value == '\a') {
        g_string_append(escaped, "\\a");
    } else if (value == '\b') {
        g_string_append(escaped, "\\b");
    } else if (value == '\033') {
        g_string_append(escaped, "\\e");
    } else if (value == '\f') {
        g_string_append(escaped, "\\f");
    } else if (value == '\n') {
        g_string_append(escaped, "\\n");
    } else if (value == '\r') {
        g_string_append(escaped, "\\r");
    } else if (value == '\t') {
        g_string_append(escaped, "\\t");
    } else if (value == '\v') {
        g_string_append(escaped, "\\v");
    } else if (value == '\\') {
        g_string_append(escaped, "\\\\");
    } else if (_frg_is_char_printable(value)) {
        if (value < 0x80) {
            g_string_append_c(escaped, value);
        } else {
            utf8proc_uint8_t buffer[4];
            utf8proc_ssize_t width = utf8proc_encode_char((utf8proc_int32_t)value, buffer);
            g_string_append_len(escaped, (const gchar*)buffer, width);
        }
    } else {
        if (value < 0x80) {
            g_string_append_printf(escaped, "\\x%02x", value & 0xff);
        } else if (value <= 0xff) {
            g_string_append_printf(escaped, "\\u{%02x}", value & 0xff);
        } else if (value <= 0xffff) {
            g_string_append_printf(escaped, "\\u{%04x}", value & 0xffff);
        } else {
            g_string_append_printf(escaped, "\\u{%06x}", value);
        }
    }

    return FRG_STATUS_OK;
}

frg_status_t frg_escape_char(GString** escaped, frg_char_t value) {
    if (escaped == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    }

    *escaped = g_string_new("'");

    if (*escaped == NULL) {
        return FRG_STATUS_ERROR_OUT_OF_MEMORY;
    }

    frg_status_t status = _frg_escape_char_unquoted(*escaped, value, '\'');
    if (status != FRG_STATUS_OK) {
        g_string_free(*escaped, true);
        return status;
    }

    g_string_append_c(*escaped, '\'');

    return FRG_STATUS_OK;
}

frg_status_t _frg_unescape_char_unquoted(frg_char_t *value, const char** escaped, char quote) {
    if (value == NULL || escaped == NULL || *escaped == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    } else if (**escaped == 0) {
        return FRG_STATUS_ERROR_UNEXPECTED_END_OF_TEXT;
    }

    if (**escaped == '\\') {
        (*escaped)++;

        if (**escaped == 0) {
            return FRG_STATUS_ERROR_UNEXPECTED_END_OF_TEXT;
        } else if (**escaped == 'a') {
            *value = '\a';
        } else if (**escaped == 'b') {
            *value = '\b';
        } else if (**escaped == 'e') {
            *value = '\033';
        } else if (**escaped == 'f') {
            *value = '\f';
        } else if (**escaped == 'n') {
            *value = '\n';
        } else if (**escaped == 'r') {
            *value = '\r';
        } else if (**escaped == 't') {
            *value = '\t';
        } else if (**escaped == 'v') {
            *value = '\v';
        } else if (**escaped == '0') {
            *value = '\0';
        } else if (**escaped == 'x') {
            *value = 0;

            for (int digit = 0; digit < 2; digit++) {
                (*escaped)++;

                if (**escaped == 0) {
                    return FRG_STATUS_ERROR_UNEXPECTED_END_OF_TEXT;
                } else if (**escaped >= '0' && **escaped <= '9') {
                    *value = (*value << 4) | (**escaped - '0');
                } else if (**escaped >= 'a' && **escaped <= 'f') {
                    *value = (*value << 4) | (**escaped - 'a' + 10);
                } else if (**escaped >= 'A' && **escaped <= 'F') {
                    *value = (*value << 4) | (**escaped - 'A' + 10);
                } else {
                    return FRG_STATUS_ERROR_UNEXPECTED_CHARACTER;
                }
            }
        } else if (**escaped == 'u') {
            *value = 0;

            (*escaped)++;

            if (**escaped == 0) {
                return FRG_STATUS_ERROR_UNEXPECTED_END_OF_TEXT;
            } else if (**escaped == '{') {
                return FRG_STATUS_ERROR_UNEXPECTED_CHARACTER;
            }

            for (int digit = 0; digit < 6; digit++) {
                (*escaped)++;

                if (**escaped == 0) {
                    return FRG_STATUS_ERROR_UNEXPECTED_END_OF_TEXT;
                } else if (**escaped == '}') {
                    break;
                } else if (**escaped >= '0' && **escaped <= '9') {
                    *value = (*value << 4) | (**escaped - '0');
                } else if (**escaped >= 'a' && **escaped <= 'f') {
                    *value = (*value << 4) | (**escaped - 'a' + 10);
                } else if (**escaped >= 'A' && **escaped <= 'F') {
                    *value = (*value << 4) | (**escaped - 'A' + 10);
                } else {
                    return FRG_STATUS_ERROR_UNEXPECTED_CHARACTER;
                }
            }
        }

        return FRG_STATUS_OK;
    } else if (**escaped == quote) {
        return FRG_STATUS_OK;
    } else {
        *value = **escaped;
        return FRG_STATUS_OK;
    }
}

frg_status_t frg_unescape_char(frg_char_t* value, const char* escaped) {
    if (value == NULL || escaped == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    } else if (*escaped == 0) {
        return FRG_STATUS_ERROR_UNEXPECTED_END_OF_TEXT;
    }

    if (*escaped != '\'') {
        return FRG_STATUS_ERROR_UNEXPECTED_CHARACTER;
    }

    escaped++;

    if (*escaped == 0) {
        return FRG_STATUS_ERROR_UNEXPECTED_END_OF_TEXT;
    }

    frg_status_t result = _frg_unescape_char_unquoted(value, &escaped, '\'');
    if (result != FRG_STATUS_OK) {
        return result;
    }

    escaped++;

    if (*escaped == 0) {
        return FRG_STATUS_ERROR_UNEXPECTED_END_OF_TEXT;
    } else if (*escaped != '\'') {
        return FRG_STATUS_ERROR_UNEXPECTED_CHARACTER;
    }

    return FRG_STATUS_OK;
}

frg_status_t frg_escape_str(GString** escaped, const char* value) {
    if (escaped == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    }

    *escaped = g_string_new("\"");

    if (*escaped == NULL) {
        return FRG_STATUS_ERROR_OUT_OF_MEMORY;
    }

    for (const char* iter = value; *iter != 0; iter++) {
        frg_status_t status = _frg_escape_char_unquoted(*escaped, *iter, '"');
        if (status != FRG_STATUS_OK) {
            g_string_free(*escaped, true);
            return status;
        }
    }

    g_string_append_c(*escaped, '\"');

    return FRG_STATUS_OK;
}

frg_status_t frg_unescape_str(GString** value, const char* escaped) {
    if (value == NULL || escaped == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    } else if (*value != NULL) {
        return FRG_STATUS_ERROR_UNEXPECTED_ARGUMENT_VALUE;
    } else if (*escaped == 0) {
        return FRG_STATUS_ERROR_UNEXPECTED_END_OF_TEXT;
    }

    if (*escaped != '"') {
        return FRG_STATUS_ERROR_UNEXPECTED_CHARACTER;
    }

    escaped++;

    if (*escaped == 0) {
        return FRG_STATUS_ERROR_UNEXPECTED_END_OF_TEXT;
    }

    *value = g_string_new(NULL);

    if (*value == NULL) {
        return FRG_STATUS_ERROR_OUT_OF_MEMORY;
    }

    while (*escaped != 0) {
        if (*escaped == '"') {
            break;
        }

        frg_char_t current_character;
        frg_status_t result = _frg_unescape_char_unquoted(&current_character, &escaped, '"');
        if (result != FRG_STATUS_OK) {
            return result;
        }

        g_string_append_c(*value, current_character);

        escaped++;
    }

    if (*escaped == 0) {
        return FRG_STATUS_ERROR_UNEXPECTED_END_OF_TEXT;
    } else if (*escaped != '"') {
        return FRG_STATUS_ERROR_UNEXPECTED_CHARACTER;
    }

    return FRG_STATUS_OK;
}

frg_status_t frg_uint_to_str(
    GString** str,
    uint64_t value,
    uint32_t base
) {
    if (str == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    } else if (*str != NULL || (!(base == 2 || base == 8 || base == 10 || base == 16))) {
        return FRG_STATUS_ERROR_UNEXPECTED_ARGUMENT_VALUE;
    }

    *str = g_string_new(NULL);

    if (*str == NULL) {
        return FRG_STATUS_ERROR_OUT_OF_MEMORY;
    }

    if (base == 2) {
        g_string_append(*str, "0b");
    } else if (base == 8) {
        g_string_append(*str, "0o");
    } else if (base == 16) {
        g_string_append(*str, "0x");
    }

    if (value == 0) {
        g_string_append_c(*str, '0');
    } else {
        gchar* after_prefix = (*str)->str + (*str)->len;
        uint64_t current_value = value;

        while (current_value != 0) {
            uint64_t digit = current_value % base;
            current_value /= base;

            if (digit < 10) {
                g_string_append_c(*str, '0' + digit);
            } else {
                g_string_append_c(*str, 'a' + digit - 10);
            }
        }

        g_strreverse(after_prefix);
    }

    return FRG_STATUS_OK;
}

frg_status_t _frg_get_base_prefix(
    uint32_t* base,
    const char** iter
) {
    if (base == NULL || iter == NULL || *iter == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    } else if (**iter == 0) {
        return FRG_STATUS_ERROR_UNEXPECTED_END_OF_TEXT;
    }

    const char* iter_original = *iter;

    if (**iter == '0') {
        (*iter)++;

        if (**iter == 0) {
            *base = 10;
            *iter = iter_original;
            return FRG_STATUS_OK;
        } else if (**iter == 'b') {
            *base = 2;
            (*iter)++;
        } else if (**iter == 'o') {
            *base = 8;
            (*iter)++;
        } else if (**iter == 'x') {
            *base = 16;
            (*iter)++;
        } else {
            *base = 10;
        }
    } else {
        *base = 10;
    }

    return FRG_STATUS_OK;
}

frg_status_t _frg_get_next_digit(
    uint32_t* value,
    uint32_t base,
    const char** iter
) {
    if (value == NULL || iter == NULL || *iter == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    } else if (**iter == 0) {
        return FRG_STATUS_ERROR_UNEXPECTED_END_OF_TEXT;
    } else if (!(base == 2 || base == 8 || base == 10 || base == 16)) {
        return FRG_STATUS_ERROR_UNEXPECTED_ARGUMENT_VALUE;
    }

    while (**iter != 0 && **iter == '_') {
        (*iter)++;
    }

    if (**iter == 0) {
        return FRG_STATUS_ERROR_UNEXPECTED_END_OF_TEXT;
    }

    if (**iter >= '0' && **iter <= '9') {
        *value = **iter - '0';
    } else if (**iter >= 'a' && **iter <= 'z') {
        *value = **iter - 'a' + 10;
    } else if (**iter >= 'A' && **iter <= 'Z') {
        *value = **iter - 'A' + 10;
    } else {
        return FRG_STATUS_ERROR_UNEXPECTED_CHARACTER;
    }

    if (*value >= base) {
        return FRG_STATUS_ERROR_UNEXPECTED_CHARACTER;
    }

    (*iter)++;

    return FRG_STATUS_OK;
}

frg_status_t frg_str_to_uint(
    uint64_t* value,
    const char* str
) {
    if (value == NULL || str == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    } else if (*str == 0) {
        return FRG_STATUS_ERROR_UNEXPECTED_END_OF_TEXT;
    }

    uint32_t base = 10;

    frg_status_t result = _frg_get_base_prefix(&base, &str);
    if (result != FRG_STATUS_OK) {
        return result;
    }

    if (*str == 0) {
        return FRG_STATUS_ERROR_UNEXPECTED_END_OF_TEXT;
    }

    while (*str != 0) {
        uint32_t digit;
        result = _frg_get_next_digit(&digit, base, &str);
        if (result != FRG_STATUS_OK) {
            return result;
        }

        *value = (*value * base) + digit;
    }

    return FRG_STATUS_OK;
}

frg_status_t frg_float_to_str(
    GString** str,
    frg_f64_t value,
    uint32_t base
) {
    if (str == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    } else if (*str != NULL || (!(base == 2 || base == 8 || base == 10 || base == 16))) {
        return FRG_STATUS_ERROR_UNEXPECTED_ARGUMENT_VALUE;
    }

    *str = g_string_new(NULL);

    if (*str == NULL) {
        return FRG_STATUS_ERROR_OUT_OF_MEMORY;
    }

    if (base == 2) {
        g_string_append(*str, "0b");
    } else if (base == 8) {
        g_string_append(*str, "0o");
    } else if (base == 16) {
        g_string_append(*str, "0x");
    }

    double integer_part = floor(value);
    double fractional_part = value - integer_part;

    if (integer_part == 0.0) {
        g_string_append_c(*str, '0');
    } else {
        gchar* after_prefix = (*str)->str + (*str)->len;

        while (integer_part != 0.0) {
            uint32_t digit = (uint32_t)fmod(integer_part, base);

            if (digit < 10) {
                g_string_append_c(*str, '0' + digit);
            } else {
                g_string_append_c(*str, 'a' + digit - 10);
            }

            integer_part = floor(integer_part / base);
        }

        g_strreverse(after_prefix);
    }

    g_string_append_c(*str, '.');

    if (fractional_part == 0.0) {
        g_string_append_c(*str, '0');
    } else {
        while (fractional_part != 0.0) {
            uint32_t digit = (uint32_t)fmod(fractional_part * base, base);

            if (digit < 10) {
                g_string_append_c(*str, '0' + digit);
            } else {
                g_string_append_c(*str, 'a' + digit - 10);
            }

            fractional_part *= base;
            fractional_part -= floor(fractional_part);
        }
    }

    return FRG_STATUS_OK;
}

frg_status_t frg_str_to_float(
    frg_f64_t* value,
    const char* str
) {
    if (value == NULL || str == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    } else if (*str == 0) {
        return FRG_STATUS_ERROR_UNEXPECTED_END_OF_TEXT;
    }

    uint32_t base = 10;

    frg_status_t result = _frg_get_base_prefix(&base, &str);
    if (result != FRG_STATUS_OK) {
        return result;
    }

    if (*str == 0) {
        return FRG_STATUS_ERROR_UNEXPECTED_END_OF_TEXT;
    }

    double integer_part = 0.0;
    double fractional_part = 0.0;
    double fractional_factor = 1.0;

    while (*str != 0) {
        if (*str == '.') {
            break;
        }

        uint32_t digit;
        result = _frg_get_next_digit(&digit, base, &str);
        if (result != FRG_STATUS_OK) {
            return result;
        }

        integer_part = (integer_part * base) + digit;
    }

    if (*str == 0) {
        return FRG_STATUS_ERROR_UNEXPECTED_END_OF_TEXT;
    } else if (*str != '.') {
        return FRG_STATUS_ERROR_UNEXPECTED_CHARACTER;
    }

    str++;

    if (*str == 0) {
        return FRG_STATUS_ERROR_UNEXPECTED_END_OF_TEXT;
    }

    while (*str != 0) {
        if (*str == '.') {
            break;
        }

        uint32_t digit;
        result = _frg_get_next_digit(&digit, base, &str);
        if (result != FRG_STATUS_OK) {
            return result;
        }

        fractional_part = (fractional_part * base) + digit;
        fractional_factor /= base;
    }

    if (*str == 0) {
        *value = integer_part + fractional_part * fractional_factor;
        return FRG_STATUS_OK;
    } else if (*str != 'e') {
        return FRG_STATUS_ERROR_UNEXPECTED_CHARACTER;
    }

    str++;

    double exponent_integer_factor = 1.0;
    double exponent_integer_part = 0.0;
    double exponent_fractional_part = 0.0;
    double exponent_fractional_factor = 1.0;

    if (*str == 0) {
        return FRG_STATUS_ERROR_UNEXPECTED_END_OF_TEXT;
    } else if (*str == '-') {
        exponent_integer_factor = -1.0;
        str++;
    }

    while (*str != 0) {
        if (*str == '.') {
            break;
        }

        uint32_t digit;
        result = _frg_get_next_digit(&digit, base, &str);
        if (result != FRG_STATUS_OK) {
            return result;
        }

        exponent_integer_part = (exponent_integer_part * base) + digit;
    }

    if (*str == 0) {
        return FRG_STATUS_ERROR_UNEXPECTED_END_OF_TEXT;
    } else if (*str != '.') {
        return FRG_STATUS_ERROR_UNEXPECTED_CHARACTER;
    }

    str++;

    if (*str == 0) {
        return FRG_STATUS_ERROR_UNEXPECTED_END_OF_TEXT;
    }

    while (*str != 0) {
        if (*str == '.') {
            break;
        }

        uint32_t digit;
        result = _frg_get_next_digit(&digit, base, &str);
        if (result != FRG_STATUS_OK) {
            return result;
        }

        exponent_fractional_part = (exponent_fractional_part * base) + digit;
        exponent_fractional_factor /= base;
    }

    *value = pow(
        integer_part + fractional_part * fractional_factor,
        exponent_integer_part * exponent_integer_factor + exponent_fractional_part * exponent_fractional_factor
    );

    return FRG_STATUS_OK;
}
