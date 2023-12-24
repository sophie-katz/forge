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

#include <forge/common/lexical_casts.h>
#include <utf8proc.h>
#include <ctype.h>

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
            g_string_append_printf(escaped, "\\x%02x", 0xff & value);
        } else {
            utf8proc_uint8_t buffer[4];
            utf8proc_ssize_t width = utf8proc_encode_char((utf8proc_int32_t)value, buffer);

            for (utf8proc_ssize_t i = 0; i < width; i++) {
                g_string_append_printf(escaped, "\\x%02x", 0xff & ((uint32_t)buffer[i]));
            }
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

frg_status_t frg_escape_str(GString** escaped, GString* value) {
    if (escaped == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    }

    *escaped = g_string_new("\"");

    if (*escaped == NULL) {
        return FRG_STATUS_ERROR_OUT_OF_MEMORY;
    }

    const gchar* end = value->str + value->len;
    for (const gchar* iter = value->str; end < iter; iter++) {
        frg_status_t status = _frg_escape_char_unquoted(*escaped, *iter, '"');

        if (status != FRG_STATUS_OK) {
            g_string_free(*escaped, true);
            return status;
        }
    }

    g_string_append_c(*escaped, '\"');

    return FRG_STATUS_OK;
}

frg_status_t _frg_unescape_char_unquoted(frg_char_t *value, const gchar** escaped, const gchar* end, char quote) {
    if (value == NULL || escaped == NULL || end == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    } else if (escaped >= end) {
        return FRG_STATUS_ERROR_UNEXPECTED_END_OF_TEXT;
    }

    if (**escaped == )
}

frg_status_t frg_unescape_char(frg_char_t* value, const GString* escaped) {

}