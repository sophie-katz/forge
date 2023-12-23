#include <forge/common/color.h>
#include <forge/common/debug.h>
#include <forge/common/stream.h>
#include <stdarg.h>
#include <stdio.h>

#define FRG_DEBUG_INDENT_WIDTH 2

void frg_debug_print_newline(frg_indent_t indent) {
    fputc('\n', FRG_STREAM_DEFAULT);

    for (size_t i = 0; i < indent; i++) {
        fputc(' ', FRG_STREAM_DEFAULT);
    }
}

void frg_debug_print_node(const char* format, ...) {
    frg_set_color(FRG_STREAM_DEFAULT, FRG_COLOR_ID_BRIGHT_BLACK);
    fprintf(FRG_STREAM_DEFAULT, "[");

    va_list args;
    va_start(args, format);

    frg_set_color(FRG_STREAM_DEFAULT, FRG_COLOR_ID_BOLD);
    frg_set_color(FRG_STREAM_DEFAULT, FRG_COLOR_ID_GREEN);
    vfprintf(FRG_STREAM_DEFAULT, format, args);

    va_end(args);

    frg_set_color(FRG_STREAM_DEFAULT, FRG_COLOR_ID_RESET);
    frg_set_color(FRG_STREAM_DEFAULT, FRG_COLOR_ID_BRIGHT_BLACK);
    fprintf(FRG_STREAM_DEFAULT, "]");

    frg_set_color(FRG_STREAM_DEFAULT, FRG_COLOR_ID_RESET);
}

void frg_debug_print_property(const char* label, const char* format, ...) {
    frg_set_color(FRG_STREAM_DEFAULT, FRG_COLOR_ID_BOLD);
    frg_set_color(FRG_STREAM_DEFAULT, FRG_COLOR_ID_CYAN);
    fprintf(FRG_STREAM_DEFAULT, "%s = ", label);
    
    frg_set_color(FRG_STREAM_DEFAULT, FRG_COLOR_ID_RESET);

    va_list args;
    va_start(args, format);

    vfprintf(FRG_STREAM_DEFAULT, format, args);

    va_end(args);
}

void frg_debug_print_node_property(const char* label, const char* format, ...) {
    frg_set_color(FRG_STREAM_DEFAULT, FRG_COLOR_ID_BOLD);
    frg_set_color(FRG_STREAM_DEFAULT, FRG_COLOR_ID_CYAN);
    fprintf(FRG_STREAM_DEFAULT, "%s = ", label);
    
    frg_set_color(FRG_STREAM_DEFAULT, FRG_COLOR_ID_RESET);
    frg_set_color(FRG_STREAM_DEFAULT, FRG_COLOR_ID_BRIGHT_BLACK);
    fprintf(FRG_STREAM_DEFAULT, "[");

    va_list args;
    va_start(args, format);

    frg_set_color(FRG_STREAM_DEFAULT, FRG_COLOR_ID_BOLD);
    frg_set_color(FRG_STREAM_DEFAULT, FRG_COLOR_ID_GREEN);
    vfprintf(FRG_STREAM_DEFAULT, format, args);

    frg_set_color(FRG_STREAM_DEFAULT, FRG_COLOR_ID_RESET);
    frg_set_color(FRG_STREAM_DEFAULT, FRG_COLOR_ID_BRIGHT_BLACK);
    fprintf(FRG_STREAM_DEFAULT, "]");

    va_end(args);
}

// const char* _frg_debug_get_ast_id_name(frg_ast_id_t id) {
//     switch (id) {
//         case FRG_AST_ID_VALUE_TRUE:
//             return "VALUE_TRUE";
//         case FRG_AST_ID_VALUE_FALSE:
//             return "VALUE_FALSE";
//         case FRG_AST_ID_VALUE_LOG_NOT:
//             return "VALUE_LOG_NOT";
//         case FRG_AST_ID_VALUE_LOG_AND:
//             return "VALUE_LOG_AND";
//         case FRG_AST_ID_VALUE_LOG_OR:
//             return "VALUE_LOG_OR";
//         default:
//             return "???";
//     }
// }

// void _frg_debug_print_indent(size_t indent) {
//     for (size_t i = 0; i < indent; i++) {
//         printf(" ");
//     }
// }

// void _frg_debug_print_newline(size_t indent) {
//     printf("\n");
//     _frg_debug_print_indent(indent);
// }

// void frg_debug_print_ast_at_indent(frg_ast_t* ast, size_t indent) {
//     if (ast == NULL) {
//         printf("null");
//     }

//     switch (ast->id) {
//         case FRG_AST_ID_VALUE_TRUE:
//             printf("VALUE_TRUE");
//             break;
//         case FRG_AST_ID_VALUE_FALSE:
//             printf("VALUE_FALSE");
//             break;
//         case FRG_AST_ID_VALUE_LOG_NOT:
//             printf("LOG_NOT");
//             _frg_debug_print_newline(indent + FRG_DEBUG_INDENT_WIDTH);
//             printf("operand: ");
//             frg_debug_print_ast_at_indent(frg_ast_as_value_unary(ast)->operand, indent + FRG_DEBUG_INDENT_WIDTH);
//             break;
//         case FRG_AST_ID_VALUE_LOG_AND:
//             printf("LOG_AND");
//             _frg_debug_print_newline(indent + FRG_DEBUG_INDENT_WIDTH);
//             printf("lhs: ");
//             frg_debug_print_ast_at_indent(frg_ast_as_value_binary(ast)->lhs, indent + FRG_DEBUG_INDENT_WIDTH);
//             _frg_debug_print_newline(indent + FRG_DEBUG_INDENT_WIDTH);
//             printf("rhs: ");
//             frg_debug_print_ast_at_indent(frg_ast_as_value_binary(ast)->rhs, indent + FRG_DEBUG_INDENT_WIDTH);
//             break;
//         case FRG_AST_ID_VALUE_LOG_OR:
//             printf("LOG_OR");
//             _frg_debug_print_newline(indent + FRG_DEBUG_INDENT_WIDTH);
//             printf("lhs: ");
//             frg_debug_print_ast_at_indent(frg_ast_as_value_binary(ast)->lhs, indent + FRG_DEBUG_INDENT_WIDTH);
//             _frg_debug_print_newline(indent + FRG_DEBUG_INDENT_WIDTH);
//             printf("rhs: ");
//             frg_debug_print_ast_at_indent(frg_ast_as_value_binary(ast)->rhs, indent + FRG_DEBUG_INDENT_WIDTH);
//             break;
//         default:
//             printf("UNKNOWN(%d)", ast->id);
//             break;
//     }
// }

// void frg_debug_print_ast(frg_ast_t* ast) {
//     frg_debug_print_ast_at_indent(ast, 0);
// }
