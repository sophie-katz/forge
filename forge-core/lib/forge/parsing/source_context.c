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

#include <forge/parsing/source_context.h>
#include <forge/common/memory.h>
#include <forge/messages/message_buffer.h>
#include <forge/common/error.h>

frg_parsing_source_context_t* frg_parsing_source_context_new() {
    frg_parsing_source_context_t* source_context = frg_safe_malloc(sizeof(frg_parsing_source_context_t));

    source_context->sources = g_hash_table_new(
        g_str_hash,
        g_str_equal
    );

    return source_context;
}

void frg_parsing_source_context_destroy(frg_parsing_source_context_t** source_context) {
    frg_assert_pointer_non_null(source_context);
    frg_assert_pointer_non_null(*source_context);

    for (GList* source = g_hash_table_get_values((*source_context)->sources); source != NULL; source = source->next) {
        frg_parsing_source_destroy((frg_parsing_source_t**)&source->data);
    }

    g_hash_table_destroy((*source_context)->sources);

    frg_safe_free((void**)source_context);
}

frg_parsing_source_t* frg_parsing_source_context_open_file(
    frg_parsing_source_context_t* source_context,
    FILE* file,
    const char* path,
    bool owned
) {
    frg_assert_pointer_non_null(source_context);
    frg_assert_pointer_non_null(file);
    frg_assert_string_non_empty(path);

    frg_parsing_source_t* source = frg_parsing_source_new_file(
        file,
        path,
        owned
    );

    g_hash_table_insert(
        source_context->sources,
        (gpointer)path,
        source
    );

    return source;
}

frg_parsing_source_t* frg_parsing_source_context_open_path(
    void* message_buffer,
    frg_parsing_source_context_t* source_context,
    const char* path
) {
    frg_assert_pointer_non_null(message_buffer);
    frg_assert_pointer_non_null(source_context);
    frg_assert_string_non_empty(path);

    FILE* file = fopen(path, "r");

    if (file == NULL) {
        frg_message_emit(
            message_buffer,
            FRG_MESSAGE_SEVERITY_FATAL_ERROR,
            "unable to open source file '%s'",
            path
        );

        return NULL;
    }

    return frg_parsing_source_context_open_file(
        source_context,
        file,
        path,
        true
    );
}

frg_parsing_source_t* frg_parsing_source_context_open_buffer(
    frg_parsing_source_context_t* source_context,
    char* text,
    ssize_t length,
    const char* path,
    bool owned
) {
    frg_assert_pointer_non_null(source_context);
    frg_assert_pointer_non_null(text);
    frg_assert_int_ge(length, 2);
    frg_assert_string_non_empty(path);
    frg_assert_int_eq(text[length - 1], 0);
    frg_assert_int_eq(text[length - 2], 0);

    frg_parsing_source_t* source = frg_parsing_source_new_buffer(
        text,
        length,
        path,
        owned
    );

    g_hash_table_insert(
        source_context->sources,
        (gpointer)path,
        source
    );

    return source;
}

frg_parsing_source_t* frg_parsing_source_context_open_string(
    frg_parsing_source_context_t* source_context,
    const char* text,
    ssize_t length,
    const char* path
) {
    frg_assert_pointer_non_null(source_context);
    frg_assert_pointer_non_null(text);
    frg_assert_string_non_empty(path);

    if (length < 0) {
        length = strlen(text);
    }

    char* buffer = frg_safe_malloc(length + 2);
    memcpy(buffer, text, length);
    buffer[length] = 0;
    buffer[length + 1] = 0;

    return frg_parsing_source_context_open_buffer(
        source_context,
        buffer,
        length + 2,
        path,
        true
    );
}

frg_parsing_source_t* frg_parsing_source_context_get_source_by_path(
    frg_parsing_source_context_t* source_context,
    const char* path
) {
    frg_assert_pointer_non_null(source_context);
    frg_assert_string_non_empty(path);

    return (frg_parsing_source_t*)g_hash_table_lookup(
        source_context->sources,
        path
    );
}
