#pragma once

#include <forge/ast.h>



frg_ast_t* frg_parse(const char* source, frg_size_t length, const char* filename);
