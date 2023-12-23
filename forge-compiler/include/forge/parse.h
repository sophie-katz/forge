#pragma once

#include <stdio.h>
#include <forge/ast.h>
#include <forge/enums.h>

/// Parse from a file
frg_status_t frg_parse_file(frg_ast_t** ast, FILE* file, const char* filename);

/// Parse from a pre-allocated buffer
frg_status_t frg_parse_buffer(frg_ast_t** ast, char* buffer, size_t length, const char* filename);

/// \brief Parse from a string
///
/// Note that this will copy the string. Use \c frg_parse_buffer if you want to avoid
/// this.
frg_status_t frg_parse_string(frg_ast_t** ast, const char* text, const char* filename);
