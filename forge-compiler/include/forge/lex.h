#pragma once

#include <forge/types.h>
#include <forge/str.h>

typedef uint8_t frg_lex_token_id_t;

#define FRG_LEX_TOKEN_KW_TRUE 0
#define FRG_LEX_TOKEN_KW_FALSE 1
#define FRG_LEX_TOKEN_KW_NOT 2
#define FRG_LEX_TOKEN_KW_AND 3
#define FRG_LEX_TOKEN_KW_OR 4
#define FRG_LEX_TOKEN_LPAREN 5
#define FRG_LEX_TOKEN_RPAREN 6

typedef struct {
    frg_str_t* filename;
    frg_size_t line;
    frg_size_t column;
    frg_lex_token_id_t id;
    frg_str_t* text;
} frg_lex_token_t;
