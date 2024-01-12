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
#include <forge/parsing/parsing.h>

extern FILE* yyin;
extern int yyparse(frg_ast_t** ast);
extern int yy_scan_buffer(char* base, size_t size);

const char* _frg_parsing_current_path = NULL;
size_t _frg_parsing_current_offset = 0;
frg_message_buffer_t* _frg_parsing_current_message_buffer = NULL;

frg_ast_t* frg_parse(
    frg_message_buffer_t* message_buffer,
    frg_parsing_source_t* source
) {
    // Error checking
    frg_assert_pointer_non_null(source);
    frg_assert_string_non_empty(source->path);

    // Set globals
    _frg_parsing_current_path = source->path;
    _frg_parsing_current_offset = 0;
    _frg_parsing_current_message_buffer = message_buffer;

    if (source->text != NULL) {
        // We are parsing a buffer
        frg_assert_int_ge(source->length, 2);
        frg_assert_int_eq(source->text[source->length - 2], 0);
        frg_assert_int_eq(source->text[source->length - 1], 0);

        // Set the scan buffer
        yy_scan_buffer(source->text, source->length);
    } else {
        // We are parsing a file
        frg_assert_pointer_non_null(source->file);
        frg_assert(!ferror(source->file));

        // Set the input file handle
        yyin = source->file;
    }

    // Actually parse the text
    frg_ast_t* ast = NULL;
    if (yyparse(&ast) != 0 && ast != NULL) {
        frg_ast_destroy(&ast);
    }

    // Unset globals
    _frg_parsing_current_path = NULL;
    _frg_parsing_current_offset = 0;
    _frg_parsing_current_message_buffer = NULL;

    // Return AST
    return ast;
}
