/*
This program is free software: you can redistribute it and/or modify it under the terms
of the GNU General Public License as published by the Free Software Foundation, either
version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this
program. If not, see <https://www.gnu.org/licenses/>.

Copyright 2023-2024 Sophie Katz
*/

%option noyywrap
%option nounput
%option noinput
%option yylineno

%{
#include <ctype.h>
#include <forge/ast/node.h>
#include <forge/enums.h>
#include <forge/lexical_casts/uint.h>
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
extern frg_parsing_location_t _frg_parsing_last_location;

frg_column_number_t yycolumnno = 1;

frg_parsing_location_t _frg_parsing_update_location() {
    frg_parsing_location_t last = _frg_parsing_last_location;

    _frg_parsing_last_location.offset = _frg_parsing_current_offset;
    _frg_parsing_last_location.line_number = yylineno;
    _frg_parsing_last_location.column_number = yycolumnno;

    return last;
}
%}

%%

\n {
    yycolumnno = 1;

    _frg_parsing_update_location();
}

[ \r\t\v]+ {
    _frg_parsing_update_location();
}

#[^\n]*\n {
    _frg_parsing_update_location();
}

void {
    yylval.source_range.start = _frg_parsing_update_location();
    yylval.source_range.length = yyleng;

    return KEYWORD_VOID;
}

bool {
    yylval.source_range.start = _frg_parsing_update_location();
    yylval.source_range.length = yyleng;

    return KEYWORD_BOOL;
}

u8 {
    yylval.source_range.start = _frg_parsing_update_location();
    yylval.source_range.length = yyleng;

    return KEYWORD_U8;
}

u16 {
    yylval.source_range.start = _frg_parsing_update_location();
    yylval.source_range.length = yyleng;

    return KEYWORD_U16;
}

u32 {
    yylval.source_range.start = _frg_parsing_update_location();
    yylval.source_range.length = yyleng;

    return KEYWORD_U32;
}

u64 {
    yylval.source_range.start = _frg_parsing_update_location();
    yylval.source_range.length = yyleng;

    return KEYWORD_U64;
}

i8 {
    yylval.source_range.start = _frg_parsing_update_location();
    yylval.source_range.length = yyleng;

    return KEYWORD_I8;
}

i16 {
    yylval.source_range.start = _frg_parsing_update_location();
    yylval.source_range.length = yyleng;

    return KEYWORD_I16;
}

i32 {
    yylval.source_range.start = _frg_parsing_update_location();
    yylval.source_range.length = yyleng;

    return KEYWORD_I32;
}

i64 {
    yylval.source_range.start = _frg_parsing_update_location();
    yylval.source_range.length = yyleng;

    return KEYWORD_I64;
}

f32 {
    yylval.source_range.start = _frg_parsing_update_location();
    yylval.source_range.length = yyleng;

    return KEYWORD_F32;
}

f64 {
    yylval.source_range.start = _frg_parsing_update_location();
    yylval.source_range.length = yyleng;

    return KEYWORD_F64;
}

ty {
    yylval.source_range.start = _frg_parsing_update_location();
    yylval.source_range.length = yyleng;

    return KEYWORD_TY;
}

prop {
    yylval.source_range.start = _frg_parsing_update_location();
    yylval.source_range.length = yyleng;

    return KEYWORD_PROP;
}

iface {
    yylval.source_range.start = _frg_parsing_update_location();
    yylval.source_range.length = yyleng;

    return KEYWORD_IFACE;
}

kw {
    yylval.source_range.start = _frg_parsing_update_location();
    yylval.source_range.length = yyleng;
    
    return KEYWORD_KW;
}

fn {
    yylval.source_range.start = _frg_parsing_update_location();
    yylval.source_range.length = yyleng;

    return KEYWORD_FN;
}

const {
    yylval.source_range.start = _frg_parsing_update_location();
    yylval.source_range.length = yyleng;

    return KEYWORD_CONST;
}

mut {
    yylval.source_range.start = _frg_parsing_update_location();
    yylval.source_range.length = yyleng;

    return KEYWORD_MUT;
}

override {
    yylval.source_range.start = _frg_parsing_update_location();
    yylval.source_range.length = yyleng;

    return KEYWORD_OVERRIDE;
}

return {
    yylval.source_range.start = _frg_parsing_update_location();
    yylval.source_range.length = yyleng;

    return KEYWORD_RETURN;
}

let {
    yylval.source_range.start = _frg_parsing_update_location();
    yylval.source_range.length = yyleng;

    return KEYWORD_LET;
}

if {
    yylval.source_range.start = _frg_parsing_update_location();
    yylval.source_range.length = yyleng;

    return KEYWORD_IF;
}

elif {
    yylval.source_range.start = _frg_parsing_update_location();
    yylval.source_range.length = yyleng;

    return KEYWORD_ELIF;
}

else {
    yylval.source_range.start = _frg_parsing_update_location();
    yylval.source_range.length = yyleng;

    return KEYWORD_ELSE;
}

while {
    yylval.source_range.start = _frg_parsing_update_location();
    yylval.source_range.length = yyleng;

    return KEYWORD_WHILE;
}

true {
    yylval.source_range.start = _frg_parsing_update_location();
    yylval.source_range.length = yyleng;

    return KEYWORD_TRUE;
}

false {
    yylval.source_range.start = _frg_parsing_update_location();
    yylval.source_range.length = yyleng;

    return KEYWORD_FALSE;
}

as {
    yylval.source_range.start = _frg_parsing_update_location();
    yylval.source_range.length = yyleng;

    return KEYWORD_AS;
}

[a-zA-Z\x80-\xff_][a-zA-Z0-9\x80-\xff_]* {
    yylval.value_symbol.source_range.start = _frg_parsing_update_location();
    yylval.value_symbol.source_range.length = yyleng;
    yylval.value_symbol.value = g_string_new(yytext);

    return SYMBOL;
}

"{" {
    yylval.source_range.start = _frg_parsing_update_location();
    yylval.source_range.length = yyleng;

    return CURLY_BRACE_LEFT;
}

"}" {
    yylval.source_range.start = _frg_parsing_update_location();
    yylval.source_range.length = yyleng;

    return CURLY_BRACE_RIGHT;
}

"[" {
    yylval.source_range.start = _frg_parsing_update_location();
    yylval.source_range.length = yyleng;

    return SQUARE_BRACKET_LEFT;
}

"]" {
    yylval.source_range.start = _frg_parsing_update_location();
    yylval.source_range.length = yyleng;

    return SQUARE_BRACKET_RIGHT;
}

"(" {
    yylval.source_range.start = _frg_parsing_update_location();
    yylval.source_range.length = yyleng;

    return PARENTHESIS_LEFT;
}

")" {
    yylval.source_range.start = _frg_parsing_update_location();
    yylval.source_range.length = yyleng;

    return PARENTHESIS_RIGHT;
}

"," {
    yylval.source_range.start = _frg_parsing_update_location();
    yylval.source_range.length = yyleng;

    return COMMA;
}

":" {
    yylval.source_range.start = _frg_parsing_update_location();
    yylval.source_range.length = yyleng;

    return COLON;
}

";" {
    yylval.source_range.start = _frg_parsing_update_location();
    yylval.source_range.length = yyleng;

    return SEMICOLON;
}

"?" {
    yylval.source_range.start = _frg_parsing_update_location();
    yylval.source_range.length = yyleng;

    return QUESTION_MARK;
}

"->" {
    yylval.source_range.start = _frg_parsing_update_location();
    yylval.source_range.length = yyleng;

    return ARROW_RIGHT;
}

"..." {
    yylval.source_range.start = _frg_parsing_update_location();
    yylval.source_range.length = yyleng;

    return ELIPSIS;
}

[0-9][0-9a-zA-Z_]* {
    yylval.value_int.source_range.start = _frg_parsing_update_location();
    yylval.value_int.source_range.length = yyleng;

    frg_parsing_token_reader_t* reader = frg_parsing_token_reader_new(
        &yylval.value_int.source_range.start,
        yytext
    );

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
    yylval.value_float.source_range.start = _frg_parsing_update_location();
    yylval.value_float.source_range.length = yyleng;

    frg_parsing_token_reader_t* reader = frg_parsing_token_reader_new(
        &yylval.value_float.source_range.start,
        yytext
    );

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
    frg_parsing_token_reader_t* reader = frg_parsing_token_reader_new(
        &yylval.value_float.source_range.start,
        yytext
    );

    yylval.value_float.source_range.start = _frg_parsing_update_location();
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
        .start = _frg_parsing_update_location(),
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
        .start = _frg_parsing_update_location(),
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
    yylval.source_range.start = _frg_parsing_update_location();
    yylval.source_range.length = yyleng;

    return DOT;
}

"~" {
    yylval.source_range.start = _frg_parsing_update_location();
    yylval.source_range.length = yyleng;

    return BIT_NOT;
}

"&" {
    yylval.source_range.start = _frg_parsing_update_location();
    yylval.source_range.length = yyleng;

    return BIT_AND;
}

"|" {
    yylval.source_range.start = _frg_parsing_update_location();
    yylval.source_range.length = yyleng;

    return BIT_OR;
}

"^" {
    yylval.source_range.start = _frg_parsing_update_location();
    yylval.source_range.length = yyleng;

    return BIT_XOR;
}

"<<" {
    yylval.source_range.start = _frg_parsing_update_location();
    yylval.source_range.length = yyleng;

    return BIT_SHIFT_LEFT;
}

">>" {
    yylval.source_range.start = _frg_parsing_update_location();
    yylval.source_range.length = yyleng;

    return BIT_SHIFT_RIGHT;
}

"+" {
    yylval.source_range.start = _frg_parsing_update_location();
    yylval.source_range.length = yyleng;

    return ADD;
}

"-" {
    yylval.source_range.start = _frg_parsing_update_location();
    yylval.source_range.length = yyleng;

    return SUBTRACT;
}

"*" {
    yylval.source_range.start = _frg_parsing_update_location();
    yylval.source_range.length = yyleng;

    return MULTIPLY;
}

"/" {
    yylval.source_range.start = _frg_parsing_update_location();
    yylval.source_range.length = yyleng;

    return DIVIDE;
}

"//" {
    yylval.source_range.start = _frg_parsing_update_location();
    yylval.source_range.length = yyleng;

    return DIVIDE_INT;
}

"%" {
    yylval.source_range.start = _frg_parsing_update_location();
    yylval.source_range.length = yyleng;

    return MODULO;
}

"**" {
    yylval.source_range.start = _frg_parsing_update_location();
    yylval.source_range.length = yyleng;

    return EXPONENTIATE;
}

"==" {
    yylval.source_range.start = _frg_parsing_update_location();
    yylval.source_range.length = yyleng;

    return EQUALS;
}

"!=" {
    yylval.source_range.start = _frg_parsing_update_location();
    yylval.source_range.length = yyleng;

    return NOT_EQUALS;
}

"<" {
    yylval.source_range.start = _frg_parsing_update_location();
    yylval.source_range.length = yyleng;

    return IS_LESS_THAN;
}

"<=" {
    yylval.source_range.start = _frg_parsing_update_location();
    yylval.source_range.length = yyleng;

    return IS_LESS_THAN_OR_EQUAL_TO;
}

">" {
    yylval.source_range.start = _frg_parsing_update_location();
    yylval.source_range.length = yyleng;

    return IS_GREATER_THAN;
}

">=" {
    yylval.source_range.start = _frg_parsing_update_location();
    yylval.source_range.length = yyleng;

    return IS_GREATER_THAN_OR_EQUAL_TO;
}

"!" {
    yylval.source_range.start = _frg_parsing_update_location();
    yylval.source_range.length = yyleng;

    return LOGICAL_NOT;
}

"&&" {
    yylval.source_range.start = _frg_parsing_update_location();
    yylval.source_range.length = yyleng;

    return LOGICAL_AND;
}

"||" {
    yylval.source_range.start = _frg_parsing_update_location();
    yylval.source_range.length = yyleng;

    return LOGICAL_OR;
}

"=" {
    yylval.source_range.start = _frg_parsing_update_location();
    yylval.source_range.length = yyleng;

    return ASSIGN;
}

"&=" {
    yylval.source_range.start = _frg_parsing_update_location();
    yylval.source_range.length = yyleng;

    return BIT_AND_ASSIGN;
}

"|=" {
    yylval.source_range.start = _frg_parsing_update_location();
    yylval.source_range.length = yyleng;

    return BIT_OR_ASSIGN;
}

"^=" {
    yylval.source_range.start = _frg_parsing_update_location();
    yylval.source_range.length = yyleng;

    return BIT_XOR_ASSIGN;
}

"<<=" {
    yylval.source_range.start = _frg_parsing_update_location();
    yylval.source_range.length = yyleng;

    return BIT_SHIFT_LEFT_ASSIGN;
}

">>=" {
    yylval.source_range.start = _frg_parsing_update_location();
    yylval.source_range.length = yyleng;

    return BIT_SHIFT_RIGHT_ASSIGN;
}

"+=" {
    yylval.source_range.start = _frg_parsing_update_location();
    yylval.source_range.length = yyleng;

    return ADD_ASSIGN;
}

"-=" {
    yylval.source_range.start = _frg_parsing_update_location();
    yylval.source_range.length = yyleng;

    return SUBTRACT_ASSIGN;
}

"*=" {
    yylval.source_range.start = _frg_parsing_update_location();
    yylval.source_range.length = yyleng;

    return MULTIPLY_ASSIGN;
}

"/=" {
    yylval.source_range.start = _frg_parsing_update_location();
    yylval.source_range.length = yyleng;

    return DIVIDE_ASSIGN;
}

"//=" {
    yylval.source_range.start = _frg_parsing_update_location();
    yylval.source_range.length = yyleng;

    return DIVIDE_INT_ASSIGN;
}

"%=" {
    yylval.source_range.start = _frg_parsing_update_location();
    yylval.source_range.length = yyleng;

    return MODULO_ASSIGN;
}

"**=" {
    yylval.source_range.start = _frg_parsing_update_location();
    yylval.source_range.length = yyleng;

    return EXPONENTIATE_ASSIGN;
}

"&&=" {
    yylval.source_range.start = _frg_parsing_update_location();
    yylval.source_range.length = yyleng;

    return LOGICAL_AND_ASSIGN;
}

"||=" {
    yylval.source_range.start = _frg_parsing_update_location();
    yylval.source_range.length = yyleng;

    return LOGICAL_OR_ASSIGN;
}

"++" {
    yylval.source_range.start = _frg_parsing_update_location();
    yylval.source_range.length = yyleng;

    return INCREMENT;
}

"--" {
    yylval.source_range.start = _frg_parsing_update_location();
    yylval.source_range.length = yyleng;

    return DECREMENT;
}

. {
    frg_parsing_range_t source_range = {
        .start = _frg_parsing_update_location(),
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
