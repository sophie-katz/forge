/*
Copyright (c) 2024 Sophie Katz

This file is part of Forge.

Forge is free software: you can redistribute it and/or modify it under the terms of the
GNU General Public License as published by the Free Software Foundation, either version
3 of the License, or (at your option) any later version.

Forge is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with Forge. If
not, see <https://www.gnu.org/licenses/>.
*/

%option noyywrap
%option nounput
%option noinput
%option yylineno

%{
#include <ctype.h>
#include <forge/ast/ast.h>
#include <forge/common/enums.h>
#include <forge/common/lexical_casts.h>
#include <forge/common/log.h>
#include <forge/common/types.h>
#include <glib.h>
#include <stdbool.h>

#include "forge.tab.h"

#define YY_USER_ACTION yycolumnno += yyleng; \
    _frg_parsing_current_offset += yyleng;

extern const char* _frg_parsing_current_path;
extern size_t _frg_parsing_current_offset;

frg_columnno_t yycolumnno = 1;
%}

%%

\n {
    yycolumnno = 0;
}

[ \r\t\v]+ ;

#[^\n]*\n ;

bool {
    return KW_BOOL;
}

u8 {
    return KW_U8;
}

u16 {
    return KW_U16;
}

u32 {
    return KW_U32;
}

u64 {
    return KW_U64;
}

i8 {
    return KW_I8;
}

i16 {
    return KW_I16;
}

i32 {
    return KW_I32;
}

i64 {
    return KW_I64;
}

f32 {
    return KW_F32;
}

f64 {
    return KW_F64;
}

ty {
    return KW_TY;
}

prop {
    return KW_PROP;
}

iface {
    return KW_IFACE;
}

kw {
    return KW_KW;
}

fn {
    return KW_FN;
}

const {
    return KW_CONST;
}

mut {
    return KW_MUT;
}

override {
    return KW_OVERRIDE;
}

return {
    return KW_RETURN;
}

let {
    return KW_LET;
}

if {
    return KW_IF;
}

else {
    return KW_ELSE;
}

while {
    return KW_WHILE;
}

true {
    return KW_TRUE;
}

false {
    return KW_FALSE;
}

[a-zA-Z\x80-\xff_][a-zA-Z0-9\x80-\xff_]* {
    yylval.value_str = g_string_new(yytext);
    return SYMBOL;
}

"{" {
    return CURLY_BRACE_LEFT;
}

"}" {
    return CURLY_BRACE_RIGHT;
}

"[" {
    return SQUARE_BRACKET_LEFT;
}

"]" {
    return SQUARE_BRACKET_RIGHT;
}

"(" {
    return PAREN_LEFT;
}

")" {
    return PAREN_RIGHT;
}

"," {
    return COMMA;
}

":" {
    return COLON;
}

";" {
    return SEMICOLON;
}

"?" {
    return QUESTION_MARK;
}

"->" {
    return ARROW_RIGHT;
}

"..." {
    return ELIPSIS;
}

[0-9][0-9a-zA-Z_]* {
    frg_parsing_location_t location = {
        .path = _frg_parsing_current_path,
        .offset = _frg_parsing_current_offset,
        .lineno = yylineno,
        .columnno = yycolumnno,
    };

    frg_parsing_token_reader_t* reader = frg_parsing_token_reader_new(
        location,
        yytext
    );

    frg_recoverable_status_t result = frg_parse_uint(&yylval.value_int, reader);
    if (result != FRG_RECOVERABLE_STATUS_OK) {
        yylval.value_int = 0;
    }

    frg_parsing_token_reader_destroy(&reader);

    return INT;
}

[0-9][0-9a-zA-Z_]*\.[0-9a-zA-Z_]* {
    // frg_recoverable_status_t result = frg_str_to_float(&yylval.value_float, yytext);
    // if (result != FRG_STATUS_OK) {
    //     frg_log_error_at_source_char(_frg_parsing_current_path, yylineno, yycolumnno, "invalid float literal \"%s\"", yytext);
    yylval.value_float = 0.0;
    // }

    return FLOAT;
}

([0-9][0-9a-zA-Z_]*)?\.[0-9][0-9a-zA-Z_]* {
    // frg_status_t result = frg_str_to_float(&yylval.value_float, yytext);
    // if (result != FRG_STATUS_OK) {
    //     frg_log_error_at_source_char(_frg_parsing_current_path, yylineno, yycolumnno, "invalid float literal \"%s\"", yytext);
    yylval.value_float = 0.0;
    // }

    return FLOAT;
}


\"(\\.|[^\\"\n])*\" {
    // Taken from https://gist.github.com/codebrainz/2933703

    frg_parsing_location_t location = {
        .path = _frg_parsing_current_path,
        .offset = _frg_parsing_current_offset,
        .lineno = yylineno,
        .columnno = yycolumnno,
    };

    frg_parsing_token_reader_t* reader = frg_parsing_token_reader_new(
        location,
        yytext
    );

    frg_recoverable_status_t result = frg_parse_str(&yylval.value_str, reader);
    if (result != FRG_RECOVERABLE_STATUS_OK) {
        yylval.value_str = g_string_new(NULL);
    }

    frg_parsing_token_reader_destroy(&reader);

    return STR;
}

'(\\.|[^'\n])*' {
    // Taken from https://gist.github.com/codebrainz/2933703
    
    frg_parsing_location_t location = {
        .path = _frg_parsing_current_path,
        .offset = _frg_parsing_current_offset,
        .lineno = yylineno,
        .columnno = yycolumnno,
    };

    frg_parsing_token_reader_t* reader = frg_parsing_token_reader_new(
        location,
        yytext
    );

    frg_recoverable_status_t result = frg_parse_char(&yylval.value_char, reader);
    if (result != FRG_RECOVERABLE_STATUS_OK) {
        yylval.value_char = 0;
    }

    frg_parsing_token_reader_destroy(&reader);

    return CHAR;
}

"." {
    return DOT;
}

"~" {
    return BIT_NOT;
}

"&" {
    return BIT_AND;
}

"|" {
    return BIT_OR;
}

"^" {
    return BIT_XOR;
}

"<<" {
    return BIT_SHL;
}

">>" {
    return BIT_SHR;
}

"+" {
    return ADD;
}

"-" {
    return SUB;
}

"*" {
    return MUL;
}

"/" {
    return DIV;
}

"//" {
    return DIV_INT;
}

"%" {
    return MOD;
}

"**" {
    return EXP;
}

"==" {
    return EQ;
}

"!=" {
    return NE;
}

"<" {
    return LT;
}

"<=" {
    return LE;
}

">" {
    return GT;
}

">=" {
    return GE;
}

"!" {
    return LOG_NOT;
}

"&&" {
    return LOG_AND;
}

"||" {
    return LOG_OR;
}

"=" {
    return ASSIGN;
}

"&=" {
    return BIT_AND_ASSIGN;
}

"|=" {
    return BIT_OR_ASSIGN;
}

"^=" {
    return BIT_XOR_ASSIGN;
}

"<<=" {
    return BIT_SHL_ASSIGN;
}

">>=" {
    return BIT_SHR_ASSIGN;
}

"+=" {
    return ADD_ASSIGN;
}

"-=" {
    return SUB_ASSIGN;
}

"*=" {
    return MUL_ASSIGN;
}

"/=" {
    return DIV_ASSIGN;
}

"//=" {
    return DIV_INT_ASSIGN;
}

"%=" {
    return MOD_ASSIGN;
}

"**=" {
    return EXP_ASSIGN;
}

"&&=" {
    return LOG_AND_ASSIGN;
}

"||=" {
    return LOG_OR_ASSIGN;
}

"++" {
    return INC;
}

"--" {
    return DEC;
}

. {
    char as_char = yytext[0];
    int as_int = ((int)as_char) & 0xff;
    if (isprint(yytext[0])) {
        frg_log_error_at_source_char(_frg_parsing_current_path, yylineno, yycolumnno, "unexpected character '%c' (0x%2x)", as_char, as_int);
    } else {
        frg_log_error_at_source_char(_frg_parsing_current_path, yylineno, yycolumnno, "unexpected byte 0x%2x", as_int);
    }
}

%%
