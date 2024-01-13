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

#include <forge/common/error.h>
#include <forge/common/memory.h>
#include <forge/streams/input.h>
#include <string.h>
#include <glib.h>

frg_stream_input_t* frg_stream_input_new_file(
    FILE* file,
    frg_stream_input_flags_t flags
) {
    frg_assert_pointer_non_null(file);
    frg_assert(file != stdin);
    frg_assert((flags & FRG_STREAM_INPUT_FLAG_LENGTH_SET) == 0);
    frg_assert((flags & FRG_STREAM_INPUT_FLAG_EXTRA_NULL_BYTE) == 0);

    frg_stream_input_t* source = frg_safe_malloc(sizeof(frg_stream_input_t));
    
    source->flags = flags;
    source->file = file;
    source->text = NULL;
    source->length = 0;
    source->text_offset = NULL;

    return source;
}

frg_stream_input_t* frg_stream_input_new_buffer(
    char* text,
    frg_stream_input_flags_t flags
) {
    frg_assert_pointer_non_null(text);
    frg_assert((flags & FRG_STREAM_INPUT_FLAG_LENGTH_SET) == 0);

    size_t length = strlen(text) + 1;

    if (flags & FRG_STREAM_INPUT_FLAG_EXTRA_NULL_BYTE) {
        length++;

        frg_assert_int_eq(text[length - 1], 0);
        frg_assert_int_eq(text[length - 2], 0);
    }

    return frg_stream_input_new_buffer_with_length(text, length, flags);
}

frg_stream_input_t* frg_stream_input_new_buffer_with_length(
    char* text,
    size_t length,
    frg_stream_input_flags_t flags
) {
    frg_assert_pointer_non_null(text);
    frg_assert((flags & FRG_STREAM_INPUT_FLAG_LENGTH_SET) == 0);

    frg_stream_input_t* source = frg_safe_malloc(sizeof(frg_stream_input_t));
    
    source->flags = flags | FRG_STREAM_INPUT_FLAG_LENGTH_SET;
    source->file = NULL;
    source->text = text;
    source->length = length;
    source->text_offset = text;

    return source;
}

void frg_stream_input_destroy(frg_stream_input_t** input) {
    frg_assert_pointer_non_null(input);
    frg_assert_pointer_non_null(*input);

    if ((*input)->file != NULL && ((*input)->flags & FRG_STREAM_INPUT_FLAG_OWNED)) {
        fclose((*input)->file);
    }

    if ((*input)->text != NULL && ((*input)->flags & FRG_STREAM_INPUT_FLAG_OWNED)) {
        frg_safe_free((void**)&(*input)->text);
    }

    frg_safe_free((void**)input);
}

bool frg_stream_input_is_buffer(const frg_stream_input_t* input) {
    frg_assert_pointer_non_null(input);

    return input->text != NULL && input->file == NULL;
}

bool frg_stream_input_is_file(const frg_stream_input_t* input) {
    frg_assert_pointer_non_null(input);

    return input->text == NULL && input->file != NULL;
}

bool _frg_stream_input_is_at_end_buffer(const frg_stream_input_t* input) {
    frg_assert_pointer_non_null(input);
    frg_assert(frg_stream_input_is_buffer(input));

    return input->text_offset >= input->text + input->length;
}

bool _frg_stream_input_is_at_end_file(const frg_stream_input_t* input) {
    frg_assert_pointer_non_null(input);
    frg_assert(frg_stream_input_is_file(input));

    return feof(input->file);
}

bool frg_stream_input_is_at_end(const frg_stream_input_t* input) {
    if (frg_stream_input_is_buffer(input)) {
        return _frg_stream_input_is_at_end_buffer(input);
    } else if (frg_stream_input_is_file(input)) {
        return _frg_stream_input_is_at_end_file(input);
    } else {
        frg_die("'input' is incorrectly initialized (looks like both a file and a buffer stream)");
    }
}

bool _frg_stream_input_has_error_buffer(const frg_stream_input_t* input) {
    frg_assert_pointer_non_null(input);
    frg_assert(frg_stream_input_is_buffer(input));

    return input->text_offset < input->text ||
        input->text_offset > input->text + input->length;
}

bool _frg_stream_input_has_error_file(const frg_stream_input_t* input) {
    frg_assert_pointer_non_null(input);
    frg_assert(frg_stream_input_is_file(input));

    return ferror(input->file);
}

bool frg_stream_input_has_error(const frg_stream_input_t* input) {
    if (frg_stream_input_is_buffer(input)) {
        return _frg_stream_input_has_error_buffer(input);
    } else if (frg_stream_input_is_file(input)) {
        return _frg_stream_input_has_error_file(input);
    } else {
        frg_die("'input' is incorrectly initialized (looks like both a file and a buffer stream)");
    }
}

char _frg_stream_input_peek_char_buffer(const frg_stream_input_t* input) {
    frg_assert_pointer_non_null(input);
    frg_assert(frg_stream_input_is_buffer(input));
    
    return *input->text_offset;
}

char _frg_stream_input_peek_char_file(const frg_stream_input_t* input) {
    frg_assert_pointer_non_null(input);
    frg_assert(frg_stream_input_is_file(input));

    int result = getc(input->file);
    if (result < 0) {
        return 0;
    }

    ungetc(result, input->file);

    return result;
}

char frg_stream_input_peek_char(const frg_stream_input_t* input) {
    if (frg_stream_input_is_buffer(input)) {
        return _frg_stream_input_peek_char_buffer(input);
    } else if (frg_stream_input_is_file(input)) {
        return _frg_stream_input_peek_char_file(input);
    } else {
        frg_die("'input' is incorrectly initialized (looks like both a file and a buffer stream)");
    }
}

char _frg_stream_input_read_char_buffer(frg_stream_input_t* input) {
    frg_assert_pointer_non_null(input);
    frg_assert(frg_stream_input_is_buffer(input));

    return *(input->text_offset++);
}

char _frg_stream_input_read_char_file(frg_stream_input_t* input) {
    frg_assert_pointer_non_null(input);
    frg_assert(frg_stream_input_is_file(input));

    int result = getc(input->file);
    if (result < 0) {
        return 0;
    }

    return result;
}

char frg_stream_input_read_char(frg_stream_input_t* input) {
    if (frg_stream_input_is_buffer(input)) {
        return _frg_stream_input_read_char_buffer(input);
    } else if (frg_stream_input_is_file(input)) {
        return _frg_stream_input_read_char_file(input);
    } else {
        frg_die("'input' is incorrectly initialized (looks like both a file and a buffer stream)");
    }
}

size_t _frg_stream_input_read_buffer_buffer(
    frg_stream_input_t* input,
    char* buffer,
    size_t length
) {
    frg_assert_pointer_non_null(input);
    frg_assert(frg_stream_input_is_buffer(input));

    length = MIN(length, input->text + input->length - input->text_offset);

    if (length > 0) {
        memcpy(buffer, input->text_offset, length);
    }

    input->text_offset += length;

    return length;
}

size_t _frg_stream_input_read_buffer_file(
    frg_stream_input_t* input,
    char* buffer,
    size_t length
) {
    frg_assert_pointer_non_null(input);
    frg_assert(frg_stream_input_is_file(input));

    return fread(buffer, 1, length, input->file);
}

size_t frg_stream_input_read_buffer(
    frg_stream_input_t* input,
    char* buffer,
    size_t length
) {
    if (frg_stream_input_is_buffer(input)) {
        return _frg_stream_input_read_buffer_buffer(input, buffer, length);
    } else if (frg_stream_input_is_file(input)) {
        return _frg_stream_input_read_buffer_file(input, buffer, length);
    } else {
        frg_die("'input' is incorrectly initialized (looks like both a file and a buffer stream)");
    }
}

size_t _frg_stream_input_get_offset_buffer(const frg_stream_input_t* input) {
    frg_assert_pointer_non_null(input);
    frg_assert(frg_stream_input_is_buffer(input));

    return input->text_offset - input->text;
}

size_t _frg_stream_input_get_offset_file(const frg_stream_input_t* input) {
    frg_assert_pointer_non_null(input);
    frg_assert(frg_stream_input_is_file(input));

    return ftell(input->file);
}

size_t frg_stream_input_get_offset(const frg_stream_input_t* input) {
    if (frg_stream_input_is_buffer(input)) {
        return _frg_stream_input_get_offset_buffer(input);
    } else if (frg_stream_input_is_file(input)) {
        return _frg_stream_input_get_offset_file(input);
    } else {
        frg_die("'input' is incorrectly initialized (looks like both a file and a buffer stream)");
    }
}

void _frg_stream_input_seek_to_offset_buffer(frg_stream_input_t* input, size_t offset) {
    frg_assert_pointer_non_null(input);
    frg_assert(frg_stream_input_is_buffer(input));
    frg_assert_int_ge(offset, 0);
    frg_assert_int_le(offset, input->length);

    offset = MIN(offset, input->length);

    input->text_offset = input->text + offset;
}

void _frg_stream_input_seek_to_offset_file(frg_stream_input_t* input, size_t offset) {
    frg_assert_pointer_non_null(input);
    frg_assert(frg_stream_input_is_file(input));

    fseek(input->file, offset, SEEK_SET);
}

void frg_stream_input_seek_to_offset(frg_stream_input_t* input, size_t offset) {
    if (frg_stream_input_is_buffer(input)) {
        _frg_stream_input_seek_to_offset_buffer(input, offset);
    } else if (frg_stream_input_is_file(input)) {
        _frg_stream_input_seek_to_offset_file(input, offset);
    } else {
        frg_die("'input' is incorrectly initialized (looks like both a file and a buffer stream)");
    }
}

size_t frg_stream_get_length(const frg_stream_input_t* input) {
    frg_assert_pointer_non_null(input);

    if ((input->flags & FRG_STREAM_INPUT_FLAG_LENGTH_SET) == 0 && frg_stream_input_is_file(input)) {
        size_t current_offset = frg_stream_input_get_offset(input);
        fseek(input->file, 0, SEEK_END);
        ((frg_stream_input_t*)input)->length = ftell(input->file);
        frg_stream_input_seek_to_offset((frg_stream_input_t*)input, current_offset);
        ((frg_stream_input_t*)input)->flags |= FRG_STREAM_INPUT_FLAG_LENGTH_SET;
    }

    frg_assert((input->flags & FRG_STREAM_INPUT_FLAG_LENGTH_SET) != 0);

    return input->length;
}

char* frg_stream_get_buffer(frg_stream_input_t* input) {
    frg_assert_pointer_non_null(input);
    frg_assert(frg_stream_input_is_buffer(input));

    return input->text;
}

FILE* frg_stream_get_file(frg_stream_input_t* input) {
    frg_assert_pointer_non_null(input);
    frg_assert(frg_stream_input_is_file(input));

    return input->file;
}
