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

#include <forge/parsing/token_reader.h>
#include <forge/common/memory.h>
#include <forge/common/error.h>
#include <string.h>

frg_parsing_token_reader_t* frg_parsing_token_reader_new(frg_parsing_location_t start, const char* text) {
    frg_assert_string_non_empty(start.path);
    frg_assert_int_ge(start.lineno, 1);
    frg_assert_int_ge(start.columnno, 1);
    frg_assert_string_non_empty(text);

    frg_parsing_token_reader_t* reader = frg_safe_malloc(sizeof(frg_parsing_token_reader_t));

    reader->start = start;
    reader->text = text;
    reader->current_location = start;
    reader->current_char = text;

    return reader;
}

void frg_parsing_token_reader_destroy(frg_parsing_token_reader_t** reader) {
    frg_assert(reader != NULL);
    frg_assert(*reader != NULL);

    frg_safe_free((void**)reader);
}

const frg_parsing_location_t* frg_parsing_token_reader_get_current_location(const frg_parsing_token_reader_t* reader) {
    frg_assert(reader != NULL);

    return &reader->current_location;
}

char frg_parsing_token_reader_get_current_char(const frg_parsing_token_reader_t* reader) {
    frg_assert(reader != NULL);

    return *reader->current_char;
}

void frg_parsing_token_reader_step(frg_parsing_token_reader_t* reader) {
    frg_assert(reader != NULL);

    if (*reader->current_char == 0) {
        return;
    } else if (*reader->current_char == '\n') {
        reader->current_location.lineno++;
        reader->current_location.columnno = 1;
    } else {
        reader->current_location.columnno++;
    }

    reader->current_location.offset++;
    reader->current_char++;
}

frg_parsing_token_reader_t* frg_parsing_token_reader_save(const frg_parsing_token_reader_t* reader) {
    frg_assert_pointer_non_null(reader);

    frg_parsing_token_reader_t* state = frg_parsing_token_reader_new(reader->start, reader->text);
    state->current_location = reader->current_location;
    state->current_char = reader->current_char;

    return state;
}

void frg_parsing_token_reader_restore(frg_parsing_token_reader_t* reader, frg_parsing_token_reader_t** state) {
    frg_assert_pointer_non_null(reader);
    frg_assert_pointer_non_null(state);
    frg_assert_pointer_non_null(*state);
    frg_assert_int_eq(reader->start.offset, (*state)->start.offset);
    frg_assert_int_eq(reader->start.lineno, (*state)->start.lineno);
    frg_assert_int_eq(reader->start.columnno, (*state)->start.columnno);
    frg_assert_int_eq(reader->start.path, (*state)->start.path);
    frg_assert_int_eq(reader->text, (*state)->text);

    reader->current_location = (*state)->current_location;
    reader->current_char = (*state)->current_char;

    frg_parsing_token_reader_destroy(state);
}
