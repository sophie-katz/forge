#include <forge/parse.h>
#include <forge/log.h>

extern FILE* yyin;
extern int yyparse(frg_ast_t** ast);
extern int yy_scan_buffer(char* base, size_t size);

const char* _frg_current_filename = NULL;

frg_ast_t* frg_parse_file(FILE* file, const char* filename) {
    if (file == NULL) {
        frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "cannot parse NULL file handle");
        return NULL;
    }

    if (ferror(file)) {
        frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "cannot parse from file handle with error bit set");
        return NULL;
    }

    if (filename == NULL) {
        frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "cannot parse file with NULL filename");
        return NULL;
    }

    if (*filename == 0) {
        frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "cannot parse file with empty filename");
        return NULL;
    }

    _frg_current_filename = filename;

    yyin = file;

    frg_ast_t* ast = NULL;
    if (yyparse(&ast) != 0) {
        frg_ast_destroy(&ast);
    }

    _frg_current_filename = NULL;

    return ast;
}

frg_ast_t* frg_parse_buffer(char* buffer, size_t length, const char* filename) {
    if (buffer == NULL) {
        frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "cannot parse NULL buffer");
        return NULL;
    }

    if (length < 2) {
        frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "cannot parse buffer shorter than 2 bytes (buffer must be terminated with two null bytes)");
        return NULL;
    }

    if (buffer[length - 2] != 0 || buffer[length - 1] != 0) {
        frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "cannot parse buffer that is not terminated with two null bytes");
        return NULL;
    }

    if (filename == NULL) {
        frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "cannot parse buffer with NULL filename");
        return NULL;
    }

    if (*filename == 0) {
        frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "cannot parse buffer with empty filename");
        return NULL;
    }

    _frg_current_filename = filename;

    yy_scan_buffer(buffer, length);
    
    frg_ast_t* ast = NULL;
    if (yyparse(&ast) != 0) {
        frg_ast_destroy(&ast);
    }

    _frg_current_filename = NULL;

    return ast;
}

frg_ast_t* frg_parse_string(const char* text, const char* filename) {
    if (text == NULL) {
        frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "cannot parse NULL string");
        return NULL;
    }

    size_t length = strlen(text);
    char* buffer = (char*)malloc(length + 2);
    memcpy(buffer, text, length);
    buffer[length] = buffer[length + 1] = 0;

    frg_ast_t* ast = frg_parse_buffer(buffer, length + 2, filename);

    free(buffer);

    return ast;
}
