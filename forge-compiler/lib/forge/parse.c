#include <forge/parse.h>
#include <forge/log.h>
#include <forge/memory.h>

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
