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
#include <forge/common/log.h>
#include <forge/common/memory.h>
#include <forge/parse/parse.h>

extern FILE* yyin;
extern int yyparse(frg_ast_t** ast);
extern int yy_scan_buffer(char* base, size_t size);

const char* _frg_current_path = NULL;

frg_ast_t* frg_parse_file(FILE* file, const char* path) {
    frg_assert_pointer_non_null(file);
    frg_assert(!ferror(file));
    frg_assert_string_non_empty(path);

    _frg_current_path = path;
    yyin = file;

    frg_ast_t* ast = NULL;
    if (yyparse(&ast) != 0) {
        frg_ast_destroy(&ast);
    }

    _frg_current_path = NULL;

    return ast;
}

frg_ast_t* frg_parse_file_at_path(const char* path) {
    frg_assert_string_non_empty(path);

    FILE *file = fopen(path, "r");
    if (file == NULL) {
        frg_log_fatal_error("unable to open source file: %s (%s)", path, strerror(errno));
        return NULL;
    }

    frg_ast_t* ast = frg_parse_file(
        file,
        path
    );

    fclose(file);

    return ast;
}

frg_ast_t* frg_parse_buffer(char* buffer, size_t length, const char* path) {
    frg_assert_pointer_non_null(buffer);
    frg_assert_int_ge(length, 2);
    frg_assert_int_eq(buffer[length - 2], 0);
    frg_assert_int_eq(buffer[length - 1], 0);
    frg_assert_string_non_empty(path);

    _frg_current_path = path;

    yy_scan_buffer(buffer, length);
    
    frg_ast_t* ast = NULL;
    if (yyparse(&ast) != 0) {
        frg_ast_destroy(&ast);
    }

    _frg_current_path = NULL;

    return ast;
}

frg_ast_t* frg_parse_string(const char* text, const char* path) {
    frg_assert_string_non_empty(text);
    frg_assert_string_non_empty(path);

    size_t length = strlen(text);

    char* buffer = (char*)frg_safe_malloc(length + 2);

    memcpy(buffer, text, length);
    buffer[length] = buffer[length + 1] = 0;

    frg_ast_t* ast = frg_parse_buffer(buffer, length + 2, path);

    frg_safe_free((void**)&buffer);

    return ast;
}
