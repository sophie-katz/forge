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

#include <forge/common/error.h>
#include <forge/common/memory.h>
#include <forge/streams/output.h>
#include <unistd.h>

static frg_stream_output_t* _frg_stream_output_stdout = NULL;
static frg_stream_output_t* _frg_stream_output_stderr = NULL;

bool _frg_stream_output_detect_color(
    FILE* file
) {
    frg_assert(file == stdout || file == stderr);

    return isatty(fileno(file));
}

bool _frg_stream_output_detect_unicode() {
    // This is basically stolen from:
    // https://github.com/sindresorhus/is-unicode-supported/blob/main/index.js

    const char* value;

#ifdef G_OS_WIN32
    value = g_getenv("WT_SESSION");
    if (value != NULL && *value != 0) {
        return true;
    }

    value = g_getenv("TERMINUS_SUBLIME");
    if (value != NULL && *value != 0) {
        return true;
    }

    value = g_getenv("ConEmuTask");
    if (value != NULL && strcmp(value, "{cmd::Cmder}") == 0) {
        return true;
    }

    value = g_getenv("TERM_PROGRAM");
    if (value != NULL && strcmp(value, "Terminus-Sublime") == 0) {
        return true;
    } else if (value != NULL && strcmp(value, "vscode") == 0) {
        return true;
    }

    value = g_getenv("TERM");
    if (value != NULL && strcmp(value, "xterm-256color") == 0) {
        return true;
    } else if (value != NULL && strcmp(value, "alacritty") == 0) {
        return true;
    }

    value = g_getenv("TERMINAL_EMULATOR");
    if (value != NULL && strcmp(value, "JetBrains-JediTerm") == 0) {
        return true;
    }

    return false;
#else
    value = g_getenv("TERM");
    if (value != NULL && strcmp(value, "linux") == 0) {
        // Linux console (kernel)
        return false;
    }

    return true;
#endif
}

frg_stream_output_t* frg_stream_output_new_file(
    FILE* file,
    frg_stream_output_flags_t flags
) {
    frg_assert_pointer_non_null(file);

    if (file == stdout || file == stderr) {
        frg_assert((flags & FRG_STREAM_OUTPUT_FLAG_OWNED) == 0);

        if ((flags & FRG_STREAM_OUTPUT_FLAG_DETECT_COLOR) != 0) {
            if (_frg_stream_output_detect_color(file)) {
                flags |= FRG_STREAM_OUTPUT_FLAG_COLOR;
            } else {
                flags &= ~FRG_STREAM_OUTPUT_FLAG_COLOR;
            }

            flags &= ~FRG_STREAM_OUTPUT_FLAG_DETECT_COLOR;
        }

        if ((flags & FRG_STREAM_OUTPUT_FLAG_DETECT_UNICODE) != 0) {
            if (_frg_stream_output_detect_unicode()) {
                flags |= FRG_STREAM_OUTPUT_FLAG_UNICODE;
            } else {
                flags &= ~FRG_STREAM_OUTPUT_FLAG_UNICODE;
            }

            flags &= ~FRG_STREAM_OUTPUT_FLAG_DETECT_UNICODE;
        }
    } else {
        frg_assert((flags & FRG_STREAM_OUTPUT_FLAG_DETECT_COLOR) == 0);
        frg_assert((flags & FRG_STREAM_OUTPUT_FLAG_DETECT_UNICODE) == 0);
    }

    frg_stream_output_t* output = frg_safe_malloc(sizeof(frg_stream_output_t));
    
    output->flags = flags;
    output->file = file;
    output->text = NULL;

    return output;
}

frg_stream_output_t* frg_stream_output_new_buffer(
    frg_stream_output_flags_t flags
) {
    frg_assert((flags & FRG_STREAM_OUTPUT_FLAG_DETECT_COLOR) == 0);
    frg_assert((flags & FRG_STREAM_OUTPUT_FLAG_DETECT_UNICODE) == 0);

    frg_stream_output_t* output = frg_safe_malloc(sizeof(frg_stream_output_t));

    output->flags = flags;
    output->file = NULL;
    output->text = g_string_new(NULL);

    return output;
}

void frg_stream_output_destroy(frg_stream_output_t** output) {
    GString* buffer = frg_stream_output_destroy_take_buffer(output);

    if (buffer != NULL) {
        g_string_free(buffer, TRUE);
    }
}

GString* frg_stream_output_destroy_take_buffer(frg_stream_output_t** output) {
    frg_assert_pointer_non_null(output);
    frg_assert_pointer_non_null(*output);

    GString* buffer = (*output)->text;

    if (((*output)->flags & FRG_STREAM_OUTPUT_FLAG_OWNED) != 0 && (*output)->file != NULL) {
        fclose((*output)->file);
    }

    frg_safe_free((void**)output);

    return buffer;
}

void frg_stream_output_write_char(
    frg_stream_output_t* output,
    char value
) {
    frg_assert_pointer_non_null(output);

    if (output->file != NULL) {
        putc(value, output->file);
    } else {
        g_string_append_c(output->text, value);
    }
}

void frg_stream_output_write_string(
    frg_stream_output_t* output,
    const char* value
) {
    frg_assert_pointer_non_null(output);
    frg_assert_pointer_non_null(value);

    if (*value == 0) {
        return;
    }

    if (output->file != NULL) {
        fputs(value, output->file);
    } else {
        g_string_append(output->text, value);
    }
}

void frg_stream_output_write_string_with_length(
    frg_stream_output_t* output,
    const char* value,
    size_t length
) {
    frg_assert_pointer_non_null(output);
    frg_assert_pointer_non_null(value);

    if (length == 0) {
        return;
    }

    if (output->file != NULL) {
        fwrite(value, sizeof(char), length, output->file);
    } else {
        g_string_append_len(output->text, value, length);
    }
}

void frg_stream_output_write_printf(
    frg_stream_output_t* output,
    const char* format,
    ...
) {
    frg_assert_pointer_non_null(output);
    frg_assert_string_non_empty(format);

    va_list args;
    va_start(args, format);

    frg_stream_output_write_vprintf(
        output,
        format,
        args
    );

    va_end(args);
}

void frg_stream_output_write_vprintf(
    frg_stream_output_t* output,
    const char* format,
    va_list args
) {
    frg_assert_pointer_non_null(output);
    frg_assert_string_non_empty(format);

    if (output->file != NULL) {
        vfprintf(output->file, format, args);
    } else {
        g_string_append_vprintf(output->text, format, args);
    }
}

void frg_stream_output_set_color(
    frg_stream_output_t* output,
    frg_stream_output_color_t color
) {
    frg_assert_pointer_non_null(output);

    if ((output->flags & FRG_STREAM_OUTPUT_FLAG_COLOR) == 0) {
        return;
    }

    switch (color) {
        case FRG_STREAM_OUTPUT_COLOR_RESET:
            frg_stream_output_write_string(output, "\033[0;0m");
            break;
        case FRG_STREAM_OUTPUT_COLOR_BOLD:
            frg_stream_output_write_string(output, "\033[1m");
            break;
        case FRG_STREAM_OUTPUT_COLOR_DIM:
            frg_stream_output_write_string(output, "\033[2m");
            break;
        case FRG_STREAM_OUTPUT_COLOR_ITALIC:
            frg_stream_output_write_string(output, "\033[3m");
            break;
        case FRG_STREAM_OUTPUT_COLOR_UNDERLINE:
            frg_stream_output_write_string(output, "\033[4m");
            break;
        case FRG_STREAM_OUTPUT_COLOR_BLACK:
            frg_stream_output_write_string(output, "\033[30m");
            break;
        case FRG_STREAM_OUTPUT_COLOR_RED:
            frg_stream_output_write_string(output, "\033[31m");
            break;
        case FRG_STREAM_OUTPUT_COLOR_GREEN:
            frg_stream_output_write_string(output, "\033[32m");
            break;
        case FRG_STREAM_OUTPUT_COLOR_YELLOW:
            frg_stream_output_write_string(output, "\033[33m");
            break;
        case FRG_STREAM_OUTPUT_COLOR_BLUE:
            frg_stream_output_write_string(output, "\033[34m");
            break;
        case FRG_STREAM_OUTPUT_COLOR_MAGENTA:
            frg_stream_output_write_string(output, "\033[35m");
            break;
        case FRG_STREAM_OUTPUT_COLOR_CYAN:
            frg_stream_output_write_string(output, "\033[36m");
            break;
        case FRG_STREAM_OUTPUT_COLOR_WHITE:
            frg_stream_output_write_string(output, "\033[37m");
            break;
        case FRG_STREAM_OUTPUT_COLOR_BRIGHT_BLACK:
            frg_stream_output_write_string(output, "\033[90m");
            break;
        case FRG_STREAM_OUTPUT_COLOR_BRIGHT_RED:
            frg_stream_output_write_string(output, "\033[91m");
            break;
        case FRG_STREAM_OUTPUT_COLOR_BRIGHT_GREEN:
            frg_stream_output_write_string(output, "\033[92m");
            break;
        case FRG_STREAM_OUTPUT_COLOR_BRIGHT_YELLOW:
            frg_stream_output_write_string(output, "\033[93m");
            break;
        case FRG_STREAM_OUTPUT_COLOR_BRIGHT_BLUE:
            frg_stream_output_write_string(output, "\033[94m");
            break;
        case FRG_STREAM_OUTPUT_COLOR_BRIGHT_MAGENTA:
            frg_stream_output_write_string(output, "\033[95m");
            break;
        case FRG_STREAM_OUTPUT_COLOR_BRIGHT_CYAN:
            frg_stream_output_write_string(output, "\033[96m");
            break;
        case FRG_STREAM_OUTPUT_COLOR_BRIGHT_WHITE:
            frg_stream_output_write_string(output, "\033[97m");
            break;
        default:
            frg_die_unexpected_enum_value(color);
    }
}

const char* frg_stream_output_choose_unicode(
    const frg_stream_output_t* output,
    const char* ascii,
    const char* unicode
) {
    frg_assert_pointer_non_null(output);
    frg_assert_pointer_non_null(ascii);
    frg_assert_pointer_non_null(unicode);

    if ((output->flags & FRG_STREAM_OUTPUT_FLAG_UNICODE) != 0) {
        return unicode;
    } else {
        return ascii;
    }
}

frg_stream_output_t* frg_stream_output_get_stdout() {
    if (_frg_stream_output_stdout == NULL) {
        _frg_stream_output_stdout = frg_stream_output_new_file(
            stdout,
            FRG_STREAM_OUTPUT_FLAG_DETECT_COLOR | FRG_STREAM_OUTPUT_FLAG_DETECT_UNICODE
        );
    }

    return _frg_stream_output_stdout;
}

frg_stream_output_t* frg_stream_output_get_stderr() {
    if (_frg_stream_output_stderr == NULL) {
        _frg_stream_output_stderr = frg_stream_output_new_file(
            stderr,
            FRG_STREAM_OUTPUT_FLAG_DETECT_COLOR | FRG_STREAM_OUTPUT_FLAG_DETECT_UNICODE
        );
    }

    return _frg_stream_output_stderr;
}

void frg_stream_output_set_console_color(
    bool enabled
) {
    if (enabled) {
        frg_stream_output_get_stdout()->flags |= FRG_STREAM_OUTPUT_FLAG_COLOR;
        frg_stream_output_get_stderr()->flags |= FRG_STREAM_OUTPUT_FLAG_COLOR;
    } else {
        frg_stream_output_get_stdout()->flags &= ~FRG_STREAM_OUTPUT_FLAG_COLOR;
        frg_stream_output_get_stderr()->flags &= ~FRG_STREAM_OUTPUT_FLAG_COLOR;
    }
}

void frg_stream_output_set_console_unicode(
    bool enabled
) {
    if (enabled) {
        frg_stream_output_get_stdout()->flags |= FRG_STREAM_OUTPUT_FLAG_UNICODE;
        frg_stream_output_get_stderr()->flags |= FRG_STREAM_OUTPUT_FLAG_UNICODE;
    } else {
        frg_stream_output_get_stdout()->flags &= ~FRG_STREAM_OUTPUT_FLAG_UNICODE;
        frg_stream_output_get_stderr()->flags &= ~FRG_STREAM_OUTPUT_FLAG_UNICODE;
    }
}
