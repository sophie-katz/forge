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

#include <forge/parsing/source_context.h>
#include <forge/memory.h>
#include <forge/messages/message_buffer.h>
#include <forge/assert.h>

frg_parsing_source_context_t* frg_parsing_source_context_new() {
    frg_parsing_source_context_t* source_context = frg_malloc(sizeof(frg_parsing_source_context_t));

    source_context->_sources = g_hash_table_new(
        (GHashFunc)g_str_hash,
        (GEqualFunc)g_str_equal
    );

    return source_context;
}

void frg_parsing_source_context_destroy(frg_parsing_source_context_t* source_context) {
    frg_assert_pointer_non_null(source_context);

    GList* values = g_hash_table_get_values(source_context->_sources);

    for (GList* source = values; source != NULL; source = source->next) {
        frg_parsing_source_destroy((frg_parsing_source_t*)source->data);
    }

    g_list_free(values);

    g_hash_table_destroy(source_context->_sources);

    frg_free(source_context);
}

void frg_parsing_source_context_add_source(
    frg_parsing_source_context_t* mut_source_context,
    frg_parsing_source_t* source
) {
    frg_assert_pointer_non_null(mut_source_context);
    frg_assert_pointer_non_null(source);

    g_hash_table_insert(
        mut_source_context->_sources,
        (gpointer)source->path,
        source
    );
}

frg_parsing_source_t* frg_parsing_source_context_get_source_by_path(
    frg_parsing_source_context_t* mut_source_context,
    const char* path
) {
    frg_assert_pointer_non_null(mut_source_context);
    frg_assert_string_non_empty(path);

    return (frg_parsing_source_t*)g_hash_table_lookup(
        mut_source_context->_sources,
        path
    );
}
