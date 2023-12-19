#pragma once

#include <stdio.h>
#include <forge/ast.h>

frg_ast_t* frg_parse_file(FILE* file, const char* filename);

frg_ast_t* frg_parse_buffer(char* buffer, size_t length, const char* filename);

frg_ast_t* frg_parse_string(const char* text, const char* filename);
