/*
Copyright (c) 2023-2024 Sophie Katz

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
#include <forge/ast/node.h>
#include <forge/enums.h>
#include <forge/lexical_casts.h>
#include <forge/types.h>
#include <forge/messages/message_buffer.h>
#include <forge/messages/codes.h>
#include <forge/parsing/unions.h>
#include <glib.h>
#include <stdbool.h>

#include "forge.tab.h"

#define YY_USER_ACTION yycolumnno += yyleng; \
    _frg_parsing_current_offset += yyleng;

extern const char* _frg_parsing_current_path;
extern size_t _frg_parsing_current_offset;
extern frg_message_buffer_t* _frg_parsing_current_message_buffer;

frg_column_number_t yycolumnno = 1;
%}

%%

\n {
    yycolumnno = 0;
}

[ \r\t\v]+ ;

#[^\n]*\n ;

void {
    return KEYWORD_VOID;
}

bool {
    return KEYWORD_BOOL;
}

u8 {
    return KEYWORD_U8;
}

u16 {
    return KEYWORD_U16;
}

u32 {
    return KEYWORD_U32;
}

u64 {
    return KEYWORD_U64;
}

i8 {
    return KEYWORD_I8;
}

i16 {
    return KEYWORD_I16;
}

i32 {
    return KEYWORD_I32;
}

i64 {
    return KEYWORD_I64;
}

f32 {
    return KEYWORD_F32;
}

f64 {
    return KEYWORD_F64;
}

ty {
    return KEYWORD_TY;
}

prop {
    return KEYWORD_PROP;
}

interface {
    return KEYWORD_IFACE;
}

kw {
    return KEYWORD_KW;
}

fn {
    return KEYWORD_FN;
}

const {
    return KEYWORD_CONST;
}

mut {
    return KEYWORD_MUT;
}

override {
    return KEYWORD_OVERRIDE;
}

return {
    return KEYWORD_RETURN;
}

let {
    return KEYWORD_LET;
}

if {
    return KEYWORD_IF;
}

elif {
    return KEYWORD_ELIF;
}

else {
    return KEYWORD_ELSE;
}

while {
    return KEYWORD_WHILE;
}

true {
    return KEYWORD_TRUE;
}

false {
    return KEYWORD_FALSE;
}

as {
    return KEYWORD_AS;
}

[a-zA-Z\x80-\xff_][a-zA-Z0-9\x80-\xff_]* {
    frg_parsing_location_t location = {
        .path = _frg_parsing_current_path,
        .offset = _frg_parsing_current_offset,
        .line_number = yylineno,
        .column_number = yycolumnno,
    };

    yylval.value_symbol.source_range.start = location;
    yylval.value_symbol.source_range.length = yyleng;
    yylval.value_symbol.value = g_string_new(yytext);

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
    return PARENTHESIS_LEFT;
}

")" {
    return PARENTHESIS_RIGHT;
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
        .line_number = yylineno,
        .column_number = yycolumnno,
    };

    frg_parsing_token_reader_t* reader = frg_parsing_token_reader_new(
        &location,
        yytext
    );

    yylval.value_int.source_range.start = location;
    yylval.value_int.source_range.length = yyleng;

    if (!frg_parse_uint(
        &yylval.value_int.value,
        _frg_parsing_current_message_buffer,
        reader
    )) {
        yylval.value_int.value.value = 0;
        yylval.value_int.value.is_signed = false;
        yylval.value_int.value.bit_width = 64;
    }

    frg_parsing_token_reader_destroy(reader);

    return INT;
}

[0-9][0-9a-zA-Z_]*\.[0-9a-zA-Z_]* {
    frg_parsing_location_t location = {
        .path = _frg_parsing_current_path,
        .offset = _frg_parsing_current_offset,
        .line_number = yylineno,
        .column_number = yycolumnno,
    };

    frg_parsing_token_reader_t* reader = frg_parsing_token_reader_new(
        &location,
        yytext
    );

    yylval.value_float.source_range.start = location;
    yylval.value_float.source_range.length = yyleng;

    if (!frg_parse_float(
        &yylval.value_float.value,
        _frg_parsing_current_message_buffer,
        reader
    )) {
        yylval.value_float.value.value = 0.0;
        yylval.value_float.value.bit_width = 64;
    }

    frg_parsing_token_reader_destroy(reader);

    return FLOAT;
}

([0-9][0-9a-zA-Z_]*)?\.[0-9][0-9a-zA-Z_]* {
    frg_parsing_location_t location = {
        .path = _frg_parsing_current_path,
        .offset = _frg_parsing_current_offset,
        .line_number = yylineno,
        .column_number = yycolumnno,
    };

    frg_parsing_token_reader_t* reader = frg_parsing_token_reader_new(
        &location,
        yytext
    );

    yylval.value_float.source_range.start = location;
    yylval.value_float.source_range.length = yyleng;

    if (!frg_parse_float(
        &yylval.value_float.value,
        _frg_parsing_current_message_buffer,
        reader
    )) {
        yylval.value_float.value.value = 0.0;
        yylval.value_float.value.bit_width = 64;
    }

    frg_parsing_token_reader_destroy(reader);

    return FLOAT;
}


\"(\\.|[^\\"\n])*\" {
    // Taken from https://gist.github.com/codebrainz/2933703

    frg_parsing_range_t range = {
        .start = {
            .path = _frg_parsing_current_path,
            .offset = _frg_parsing_current_offset,
            .line_number = yylineno,
            .column_number = yycolumnno
        },
        .length = yyleng
    };

    frg_message_emit_eft_1_unsupported_requirement(
        _frg_parsing_current_message_buffer,
        &range,
        4,
        "String literals"
    );
}

'(\\.|[^'\n])*' {
    // Taken from https://gist.github.com/codebrainz/2933703
    
    frg_parsing_range_t range = {
        .start = {
            .path = _frg_parsing_current_path,
            .offset = _frg_parsing_current_offset,
            .line_number = yylineno,
            .column_number = yycolumnno
        },
        .length = yyleng
    };

    frg_message_emit_eft_1_unsupported_requirement(
        _frg_parsing_current_message_buffer,
        &range,
        5,
        "Character literals"
    );
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
    return BIT_SHIFT_LEFT;
}

">>" {
    return BIT_SHIFT_RIGHT;
}

"+" {
    return ADD;
}

"-" {
    return SUBTRACT;
}

"*" {
    return MULTIPLY;
}

"/" {
    return DIVIDE;
}

"//" {
    return DIVIDE_INT;
}

"%" {
    return MODULO;
}

"**" {
    return EXPONENTIATE;
}

"==" {
    return EQUALS;
}

"!=" {
    return NOT_EQUALS;
}

"<" {
    return IS_LESS_THAN;
}

"<=" {
    return IS_LESS_THAN_OR_EQUAL_TO;
}

">" {
    return IS_GREATER_THAN;
}

">=" {
    return IS_GREATER_THAN_OR_EQUAL_TO;
}

"!" {
    return LOGICAL_NOT;
}

"&&" {
    return LOGICAL_AND;
}

"||" {
    return LOGICAL_OR;
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
    return BIT_SHIFT_LEFT_ASSIGN;
}

">>=" {
    return BIT_SHIFT_RIGHT_ASSIGN;
}

"+=" {
    return ADD_ASSIGN;
}

"-=" {
    return SUBTRACT_ASSIGN;
}

"*=" {
    return MULTIPLY_ASSIGN;
}

"/=" {
    return DIVIDE_ASSIGN;
}

"//=" {
    return DIVIDE_INT_ASSIGN;
}

"%=" {
    return MODULO_ASSIGN;
}

"**=" {
    return EXPONENTIATE_ASSIGN;
}

"&&=" {
    return LOGICAL_AND_ASSIGN;
}

"||=" {
    return LOGICAL_OR_ASSIGN;
}

"++" {
    return INCREMENT;
}

"--" {
    return DECREMENT;
}

. {
    frg_parsing_range_t source_range = {
        .start = {
            .path = _frg_parsing_current_path,
            .offset = _frg_parsing_current_offset,
            .line_number = yylineno,
            .column_number = yycolumnno,
        },
        .length = 1
    };

    if (isprint(yytext[0])) {
        frg_message_emit_es_1_unexpected_character(
            _frg_parsing_current_message_buffer,
            &source_range,
            yytext[0]
        );
    } else {
        frg_message_emit_es_2_unexpected_byte(
            _frg_parsing_current_message_buffer,
            &source_range,
            yytext[0]
        );
    }
}

%%
