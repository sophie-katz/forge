/*
Copyright (c) 2023 Sophie Katz

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

%locations
%start value
%parse-param { frg_ast_t** ast }

%{
#include <forge/ast/ast.h>
#include <forge/common/log.h>
#include <forge/common/types.h>
#include <stdio.h>

extern int yylineno;
extern frg_columnno_t yycolumnno;
extern int yylex();

extern const char* _frg_current_filename;

int yywrap() {
    return 1;
}

void yyerror(frg_ast_t**ast, const char* message) {
    frg_log_prefix_source_char(_frg_current_filename, yylineno, yycolumnno);
    frg_log(FRG_LOG_SEVERITY_ERROR, "%s\n", message);
}
%}

%union {
    int64_t value_int;
    frg_f64_t value_float;
    frg_char_t value_char;
    GString* value_str;
    void* ast;
}

%token KW_BOOL
%token KW_U8
%token KW_U16
%token KW_U32
%token KW_U64
%token KW_I8
%token KW_I16
%token KW_I32
%token KW_I64
%token KW_F32
%token KW_F64
%token KW_TY
%token KW_PROP
%token KW_IFACE
%token KW_FN
%token KW_CONST
%token KW_MUT
%token KW_LET
%token KW_IF
%token KW_ELSE
%token KW_WHILE
%token KW_TRUE
%token KW_FALSE
%token <value_str> SYMBOL
%token CURLY_BRACE_LEFT
%token CURLY_BRACE_RIGHT
%token SQUARE_BRACKET_LEFT
%token SQUARE_BRACKET_RIGHT
%token PAREN_LEFT
%token PAREN_RIGHT
%token COMMA
%token SEMICOLON
%token <value_int> INT
%token <value_float> FLOAT
%token <value_char> CHAR
%token <value_str> STR
%token DOT
%token BIT_NOT
%token BIT_AND
%token BIT_OR
%token BIT_XOR
%token BIT_SHL
%token BIT_SHR
%token ADD
%token SUB
%token MUL
%token DIV
%token DIV_INT
%token MOD
%token EXP
%token EQ
%token NE
%token LT
%token LE
%token GT
%token GE
%token LOG_NOT
%token LOG_AND
%token LOG_OR
%token ASSIGN
%token BIT_AND_ASSIGN
%token BIT_OR_ASSIGN
%token BIT_XOR_ASSIGN
%token BIT_SHL_ASSIGN
%token BIT_SHR_ASSIGN
%token ADD_ASSIGN
%token INC
%token SUB_ASSIGN
%token DEC
%token MUL_ASSIGN
%token DIV_ASSIGN
%token DIV_INT_ASSIGN
%token MOD_ASSIGN
%token EXP_ASSIGN
%token LOG_AND_ASSIGN
%token LOG_OR_ASSIGN

%type <ast> value_true
%type <ast> value_false
%type <ast> value_int
%type <ast> value_float
%type <ast> value_symbol
%type <ast> value_primary
%type <ast> value_log_not
%type <ast> value_log_and
%type <ast> value_log_or
%type <ast> value

%%
value_true : KW_TRUE
    {
        frg_status_t result = frg_ast_new_value_primary((frg_ast_t**)&$$, FRG_AST_ID_VALUE_TRUE);
        if (result != FRG_STATUS_OK) {
            frg_log_prefix_internal();
            frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "unable to create ast: %s", frg_status_to_string(result));
        }
    }
    ;

value_false : KW_FALSE
    {
        frg_status_t result = frg_ast_new_value_primary((frg_ast_t**)&$$, FRG_AST_ID_VALUE_FALSE);
        if (result != FRG_STATUS_OK) {
            frg_log_prefix_internal();
            frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "unable to create ast: %s", frg_status_to_string(result));
        }
    }
    ;

value_int : INT
    {
        frg_status_t result = frg_ast_new_value_i64((frg_ast_value_int_t**)&$$, $1);
        if (result != FRG_STATUS_OK) {
            frg_log_prefix_internal();
            frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "unable to create ast: %s", frg_status_to_string(result));
        }
    }
    ;

value_float : FLOAT
    {
        frg_status_t result = frg_ast_new_value_f64((frg_ast_value_float_t**)&$$, $1);
        if (result != FRG_STATUS_OK) {
            frg_log_prefix_internal();
            frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "unable to create ast: %s", frg_status_to_string(result));
        }
    }
    ;

value_symbol : SYMBOL
    {
        frg_status_t result = frg_ast_new_value_symbol((frg_ast_value_symbol_t**)&$$, $1);
        if (result != FRG_STATUS_OK) {
            frg_log_prefix_internal();
            frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "unable to create ast: %s", frg_status_to_string(result));
        }
    }
    ;

value_primary : value_true
    {
        $$ = $1;
    }
    | value_false
    {
        $$ = $1;
    }
    | value_int
    {
        $$ = $1;
    }
    | value_float
    {
        $$ = $1;
    }
    | value_symbol
    {
        $$ = $1;
    }
    | PAREN_LEFT value PAREN_RIGHT
    {
        $$ = $2;
    }
    ;

value_log_not : LOG_NOT value_primary
    {
        frg_status_t result = frg_ast_new_value_unary((frg_ast_value_unary_t**)&$$, FRG_AST_ID_VALUE_LOG_NOT, $2);
        if (result != FRG_STATUS_OK) {
            frg_log_prefix_internal();
            frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "unable to create ast: %s", frg_status_to_string(result));
        }
    }
    | value_primary
    {
        $$ = $1;
    }
    ;

value_log_and : value_log_not LOG_AND value_log_and
    {
        frg_status_t result = frg_ast_new_value_binary((frg_ast_value_binary_t**)&$$, FRG_AST_ID_VALUE_LOG_AND, $1, $3);
        if (result != FRG_STATUS_OK) {
            frg_log_prefix_internal();
            frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "unable to create ast: %s", frg_status_to_string(result));
        }
    }
    | value_log_not
    {
        $$ = $1;
    }
    ;

value_log_or : value_log_and LOG_OR value_log_or
    {
        frg_status_t result = frg_ast_new_value_binary((frg_ast_value_binary_t**)&$$, FRG_AST_ID_VALUE_LOG_OR, $1, $3);
        if (result != FRG_STATUS_OK) {
            frg_log_prefix_internal();
            frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "unable to create ast: %s", frg_status_to_string(result));
        }
    }
    | value_log_and
    {
        $$ = $1;
    }
    ;

value : value_log_or
    {
        *ast = $1;
    }
    ;
%%
