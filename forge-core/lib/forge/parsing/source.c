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
#include <forge/parsing/source.h>

frg_parsing_source_t* frg_parsing_source_new_file(
    FILE* file,
    const char* path,
    bool owned
) {
    frg_assert_pointer_non_null(file);
    frg_assert_string_non_empty(path);

    frg_parsing_source_t* source = frg_safe_malloc(sizeof(frg_parsing_source_t));
    
    source->flags = FRG_PARSING_SOURCE_FLAG_NONE;

    if (owned) {
        source->flags |= FRG_PARSING_SOURCE_FLAG_OWNED_FILE;
    }

    source->file = file;
    source->path = path;
    source->text = NULL;
    source->length = -1;

    return source;
}

frg_parsing_source_t* frg_parsing_source_new_buffer(
    char* text,
    ssize_t length,
    const char* path,
    bool owned
) {
    frg_assert_pointer_non_null(text);
    frg_assert_string_non_empty(path);
    frg_assert_int_non_negative(length);

    frg_parsing_source_t* source = frg_safe_malloc(sizeof(frg_parsing_source_t));

    source->flags = FRG_PARSING_SOURCE_FLAG_NONE;

    if (owned) {
        source->flags |= FRG_PARSING_SOURCE_FLAG_OWNED_TEXT;
    }
    
    source->file = NULL;
    source->path = path;
    source->text = text;
    source->length = length;

    return source;
}

void frg_parsing_source_destroy(frg_parsing_source_t** source) {
    frg_assert_pointer_non_null(source);
    frg_assert_pointer_non_null(*source);

    if ((*source)->file != NULL && ((*source)->flags & FRG_PARSING_SOURCE_FLAG_OWNED_FILE) != 0) {
        fclose((*source)->file);
    }

    if ((*source)->text != NULL && ((*source)->flags & FRG_PARSING_SOURCE_FLAG_OWNED_TEXT) != 0) {
        frg_safe_free((void**)&(*source)->text);
    }

    frg_safe_free((void**)source);
}

ssize_t _frg_parsing_source_length_get(frg_parsing_source_t* source) {
    frg_assert_pointer_non_null(source);

    if (source->length == -1) {
        frg_assert_pointer_non_null(source->file);
        fseek(source->file, 0, SEEK_END);
        source->length = ftell(source->file);
    }

    return source->length;
}

const char* _frg_parsing_source_get_line_start_buffer(
    frg_parsing_source_t* source,
    const frg_parsing_location_t* start
) {
    frg_assert_pointer_non_null(source);
    frg_assert_pointer_non_null(source->text);
    frg_assert_int_ge(start->lineno, 1);
    frg_assert_int_ge(start->columnno, 1);
    frg_assert(strcmp(start->path, source->path) == 0);
    frg_assert_int_lt(start->offset, _frg_parsing_source_length_get(source));
    frg_assert_int_ge(start->columnno, start->offset);

    const char* result = source->text + start->offset - (start->columnno - 1);

    while (*result == '\n' || *result == '\r') {
        result++;
    }

    return result;
}

const char* _frg_parsing_source_get_line_end_buffer(
    frg_parsing_source_t* source,
    const frg_parsing_range_t* range
) {
    frg_assert_pointer_non_null(source);
    frg_assert_pointer_non_null(source->text);
    frg_assert_int_ge(range->start.lineno, 1);
    frg_assert_int_ge(range->start.columnno, 1);
    frg_assert(strcmp(range->start.path, source->path) == 0);
    frg_assert_int_lt(range->start.offset, _frg_parsing_source_length_get(source));
    frg_assert_int_ge(range->start.columnno, range->start.offset);

    const char* result = source->text + range->start.offset + range->length;

    while (*result != 0 && (*result != '\n' && *result != '\r')) {
        result++;
    }

    return result;
}

void _frg_parsing_source_seek_to_line_start_file(
    frg_parsing_source_t* source,
    const frg_parsing_location_t* start
) {
    frg_assert_pointer_non_null(source);
    frg_assert_pointer_non_null(source->file);
    frg_assert_int_ge(start->lineno, 1);
    frg_assert_int_ge(start->columnno, 1);
    frg_assert(strcmp(start->path, source->path) == 0);
    frg_assert_int_lt(start->offset, _frg_parsing_source_length_get(source));

    fseek(source->file, start->offset - start->columnno, SEEK_SET);

    while (!feof(source->file)) {
        int c = getc(source->file);
        if (c != '\n' && c != '\r') {
            ungetc(c, source->file);
            break;
        }
    }
}

GString* frg_parsing_source_load_range(
    frg_parsing_source_t* source,
    const frg_parsing_range_t* range
) {
    frg_assert_pointer_non_null(source);
    frg_assert_int_ge(range->start.lineno, 1);
    frg_assert_int_ge(range->start.columnno, 1);
    frg_assert(strcmp(range->start.path, source->path) == 0);
    frg_assert_int_lt(range->start.offset, _frg_parsing_source_length_get(source));
    frg_assert_int_le(
        range->start.offset + range->length,
        _frg_parsing_source_length_get(source)
    );

    if (source->text != NULL) {
        const char* line_start = _frg_parsing_source_get_line_start_buffer(
            source,
            &range->start
        );
        const char* line_end = _frg_parsing_source_get_line_end_buffer(source, range);
        return g_string_new_len(line_start, line_end - line_start);
    } else {
        _frg_parsing_source_seek_to_line_start_file(source, &range->start);

        size_t string_length = range->start.columnno + range->length - 1;

        GString* result = g_string_new(NULL);

        g_string_set_size(result, string_length);

        size_t read_bytes = fread(
            result->str,
            1,
            string_length,
            source->file
        );

        frg_assert_int_eq(read_bytes, string_length);

        while (!feof(source->file)) {
            int c = getc(source->file);
            if (c == '\n' || c == '\r') {
                break;
            } else {
                g_string_append_c(result, c);
            }
        }

        return result;
    }
}
