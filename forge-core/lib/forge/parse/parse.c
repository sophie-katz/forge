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

#include <forge/common/log.h>
#include <forge/common/memory.h>
#include <forge/parse/parse.h>

extern FILE* yyin;
extern int yyparse(frg_ast_t** ast);
extern int yy_scan_buffer(char* base, size_t size);

const char* _frg_current_filename = NULL;

frg_status_t frg_parse_file(frg_ast_t** ast, FILE* file, const char* filename) {
    if (file == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    } else if (ferror(file)) {
        return FRG_STATUS_ERROR_FILE_ERROR_BIT_SET;
    } else if (filename == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    } else if (*filename == 0) {
        return FRG_STATUS_ERROR_EMPTY_STRING;
    }

    _frg_current_filename = filename;

    yyin = file;

    *ast = NULL;
    if (yyparse(ast) != 0) {
        frg_ast_destroy(ast);
    }

    _frg_current_filename = NULL;

    return FRG_STATUS_OK;
}

frg_status_t frg_parse_file_at_path(frg_ast_t** ast, const char* path) {
    if (ast == NULL || path == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    } else if (*ast != NULL) {
        return FRG_STATUS_ERROR_UNEXPECTED_ARGUMENT_VALUE;
    } else if (*path == 0) {
        return FRG_STATUS_ERROR_EMPTY_STRING;
    }

    FILE *file = fopen(path, "r");
    if (file == NULL) {
        frg_log_fatal_error("unable to open source file: %s (%s)", path, strerror(errno));
        return FRG_STATUS_CLI_ERROR;
    }

    frg_status_t result = frg_parse_file(
        ast,
        file,
        path
    );
    if (result != FRG_STATUS_OK) {
        frg_log_internal_error("unable to parse source file: %s", frg_status_to_string(result));
        fclose(file);
        return FRG_STATUS_CLI_ERROR;
    }

    fclose(file);

    return FRG_STATUS_OK;
}

frg_status_t frg_parse_buffer(frg_ast_t** ast, char* buffer, size_t length, const char* filename) {
    if (buffer == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    } else if (length < 2) {
        return FRG_STATUS_ERROR_UNEXPECTED_ARGUMENT_VALUE;
    } else if (buffer[length - 2] != 0 || buffer[length - 1] != 0) {
        return FRG_STATUS_ERROR_UNEXPECTED_ARGUMENT_VALUE;
    } else if (filename == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    } else if (*filename == 0) {
        return FRG_STATUS_ERROR_EMPTY_STRING;
    }

    _frg_current_filename = filename;

    yy_scan_buffer(buffer, length);
    
    *ast = NULL;
    if (yyparse(ast) != 0) {
        frg_ast_destroy(ast);
    }

    _frg_current_filename = NULL;

    return FRG_STATUS_OK;
}

frg_status_t frg_parse_string(frg_ast_t** ast, const char* text, const char* filename) {
    if (text == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    } else if (filename == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    } else if (*filename == 0) {
        return FRG_STATUS_ERROR_EMPTY_STRING;
    }

    size_t length = strlen(text);

    char* buffer = NULL;
    frg_status_t result = frg_safe_malloc((void**)&buffer, length + 2);
    if (result != FRG_STATUS_OK) {
        return result;
    }

    memcpy(buffer, text, length);
    buffer[length] = buffer[length + 1] = 0;

    frg_status_t parse_result = frg_parse_buffer(ast, buffer, length + 2, filename);

    result = frg_safe_free((void**)&buffer);
    if (result != FRG_STATUS_OK) {
        return result;
    }

    return parse_result;
}
