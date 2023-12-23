#pragma once

#include <forge/common/types.h>
#include <stddef.h>

#define FRG_DEBUG_INDENT_WIDTH 2

void frg_debug_print_newline(frg_indent_t indent);
void frg_debug_print_node(const char* format, ...);
void frg_debug_print_property(const char* label, const char* format, ...);
void frg_debug_print_property_with_index(
    const char* label,
    size_t index,
    const char* format,
    ...
);
