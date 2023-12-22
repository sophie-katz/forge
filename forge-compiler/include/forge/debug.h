#pragma once

#include <forge/types.h>
#include <forge/ast.h>

void frg_debug_print_newline(frg_indent_t indent);
void frg_debug_print_node(const char* format, ...);
void frg_debug_print_property(const char* label, const char* format, ...);
void frg_debug_print_node_property(const char* label, const char* format, ...);

// void frg_debug_print_ast_at_indent(frg_ast_t* ast, frg_indent_t indent);

// void frg_debug_print_ast(frg_ast_t* ast);
