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

%locations
%start top_level
%parse-param { frg_ast_t** ast }

%{
#include <forge/ast/ast.h>
#include <forge/common/types.h>
#include <forge/common/lexical_casts.h>
#include <forge/messages/message_buffer.h>
#include <forge/parsing/union.h>
#include <stdio.h>

#define YYERROR_VERBOSE 1

extern int yylineno;
extern frg_columnno_t yycolumnno;
extern int yylex();

extern const char* _frg_parsing_current_path;
extern size_t _frg_parsing_current_offset;
extern frg_message_buffer_t* _frg_parsing_current_message_buffer;

int yywrap() {
    return 1;
}

void yyerror(frg_ast_t**ast, const char* message) {
    // frg_parsing_range_t source_range = {
    //     .start = {
    //         .path = _frg_parsing_current_path,
    //         .offset = _frg_parsing_current_offset,
    //         .lineno = yylineno,
    //         .columnno = yycolumnno,
    //     },
    //     .length = 1
    // };

    // frg_log_error_at_source_char(_frg_parsing_current_path, yylineno, yycolumnno, "[%s]", message);
    printf("YYERROR: %s\n", message);
}
%}

%union {
    frg_parsing_range_t source_range;
    frg_parsing_union_symbol_t value_symbol;
    frg_parsing_union_uint_t value_int;
    frg_parsing_union_float_t value_float;
    // frg_char_t value_char;
    // GString* value_str;
    void* ast;
    GList* list;
}

%token <source_range> KW_BOOL "bool"
%token <source_range> KW_U8 "u8"
%token <source_range> KW_U16 "u16"
%token <source_range> KW_U32 "u32"
%token <source_range> KW_U64 "u64"
%token <source_range> KW_I8 "i8"
%token <source_range> KW_I16 "i16"
%token <source_range> KW_I32 "i32"
%token <source_range> KW_I64 "i64"
%token <source_range> KW_F32 "f32"
%token <source_range> KW_F64 "f64"
%token <source_range> KW_TY "ty"
%token <source_range> KW_PROP "prop"
%token <source_range> KW_IFACE "iface"
%token <source_range> KW_KW "kw"
%token <source_range> KW_FN "fn"
%token <source_range> KW_CONST "const"
%token <source_range> KW_MUT "mut"
%token <source_range> KW_OVERRIDE "override"
%token <source_range> KW_LET "let"
%token <source_range> KW_RETURN "return"
%token <source_range> KW_IF "if"
%token <source_range> KW_ELSE "else"
%token <source_range> KW_WHILE "while"
%token <source_range> KW_TRUE "true"
%token <source_range> KW_FALSE "false"
%token <value_symbol> SYMBOL "symbol"
%token <source_range> CURLY_BRACE_LEFT "{"
%token <source_range> CURLY_BRACE_RIGHT "}"
%token <source_range> SQUARE_BRACKET_LEFT "["
%token <source_range> SQUARE_BRACKET_RIGHT "]"
%token <source_range> PAREN_LEFT "("
%token <source_range> PAREN_RIGHT ")"
%token <source_range> COMMA ","
%token <source_range> COLON ":"
%token <source_range> SEMICOLON ";"
%token <source_range> QUESTION_MARK "?"
%token <source_range> ARROW_RIGHT "->"
%token <source_range> ELIPSIS "..."
%token <value_int> INT "integer literal"
%token <value_float> FLOAT "float literal"
%token CHAR "char literal"
%token STR "string literal"
%token <source_range> DOT "."
%token <source_range> BIT_NOT "~"
%token <source_range> BIT_AND "&"
%token <source_range> BIT_OR "|"
%token <source_range> BIT_XOR "^"
%token <source_range> BIT_SHL "<<"
%token <source_range> BIT_SHR ">>"
%token <source_range> ADD "+"
%token <source_range> SUB "-"
%token <source_range> MUL "*"
%token <source_range> DIV "/"
%token <source_range> DIV_INT "//"
%token <source_range> MOD "%"
%token <source_range> EXP "**"
%token <source_range> EQ "=="
%token <source_range> NE "!="
%token <source_range> LT "<"
%token <source_range> LE "<="
%token <source_range> GT ">"
%token <source_range> GE ">="
%token <source_range> LOG_NOT "!"
%token <source_range> LOG_AND "&&"
%token <source_range> LOG_OR "||"
%token <source_range> ASSIGN "="
%token <source_range> BIT_AND_ASSIGN "&="
%token <source_range> BIT_OR_ASSIGN "|="
%token <source_range> BIT_XOR_ASSIGN "^="
%token <source_range> BIT_SHL_ASSIGN "<<="
%token <source_range> BIT_SHR_ASSIGN ">>="
%token <source_range> ADD_ASSIGN "+="
%token <source_range> INC "++"
%token <source_range> SUB_ASSIGN "-="
%token <source_range> DEC "--"
%token <source_range> MUL_ASSIGN "*="
%token <source_range> DIV_ASSIGN "/="
%token <source_range> DIV_INT_ASSIGN "//="
%token <source_range> MOD_ASSIGN "%="
%token <source_range> EXP_ASSIGN "**="
%token <source_range> LOG_AND_ASSIGN "&&="
%token <source_range> LOG_OR_ASSIGN "||="

%type <ast> ty_bool
%type <ast> ty_u8
%type <ast> ty_u16
%type <ast> ty_u32
%type <ast> ty_u64
%type <ast> ty_i8
%type <ast> ty_i16
%type <ast> ty_i32
%type <ast> ty_i64
%type <ast> ty_f32
%type <ast> ty_f64
%type <ast> ty_symbol
%type <ast> ty_primary
%type <ast> ty_pointer
%type <ast> ty_fn
%type <ast> ty
%type <list> ty_list
%type <list> ty_list_optional
%type <list> decl_union_list
%type <ast> decl_union
%type <ast> decl_prop
%type <list> decl_struct_list
%type <ast> decl_struct
%type <list> decl_iface_extends_list
%type <list> decl_iface_optional_extends
%type <ast> decl_iface_list_element
%type <list> decl_iface_list
%type <ast> decl_iface
%type <ast> decl_fn_arg_default_value
%type <ast> decl_fn_arg
%type <list> decl_fn_arg_list
%type <list> decl_fn_arg_list_optional
%type <ast> decl_fn_return_ty
%type <ast> decl_fn
%type <ast> decl_var
%type <ast> decl
%type <list> decl_list
%type <list> decl_list_optional
%type <ast> decl_block
%type <ast> stmt_return
%type <ast> stmt_if
%type <ast> stmt_while
%type <ast> stmt
%type <list> stmt_list
%type <ast> stmt_block
%type <ast> value_true
%type <ast> value_false
%type <ast> value_int
%type <ast> value_float
%type <ast> value_char
%type <ast> value_str
%type <ast> value_symbol
%type <ast> value_primary
%type <ast> value_deref_getaddr
%type <ast> value_access
%type <ast> value_call
%type <ast> value_inc_dec
%type <ast> value_exp
%type <ast> value_neg_bit_not
%type <ast> value_mul
%type <ast> value_add
%type <ast> value_bit_shift
%type <ast> value_bit_and
%type <ast> value_bit_xor
%type <ast> value_bit_or
%type <ast> value_cmp
%type <ast> value_log_not
%type <ast> value_log_and
%type <ast> value_log_or
%type <ast> value_assign
%type <ast> value
%type <list> value_list
%type <list> value_list_optional
%type <ast> top_level

%%
ty_bool : KW_BOOL
{
    $$ = frg_ast_new_ty_primary(&$1, FRG_AST_KIND_TY_BOOL);
};

ty_u8 : KW_U8
{
    $$ = frg_ast_new_ty_primary(&$1, FRG_AST_KIND_TY_U8);
};

ty_u16 : KW_U16
{
    $$ = frg_ast_new_ty_primary(&$1, FRG_AST_KIND_TY_U16);
};

ty_u32 : KW_U32
{
    $$ = frg_ast_new_ty_primary(&$1, FRG_AST_KIND_TY_U32);
};

ty_u64 : KW_U64
{
    $$ = frg_ast_new_ty_primary(&$1, FRG_AST_KIND_TY_U64);
};

ty_i8 : KW_I8
{
    $$ = frg_ast_new_ty_primary(&$1, FRG_AST_KIND_TY_I8);
};

ty_i16 : KW_I16
{
    $$ = frg_ast_new_ty_primary(&$1, FRG_AST_KIND_TY_I16);
};

ty_i32 : KW_I32
{
    $$ = frg_ast_new_ty_primary(&$1, FRG_AST_KIND_TY_I32);
};

ty_i64 : KW_I64
{
    $$ = frg_ast_new_ty_primary(&$1, FRG_AST_KIND_TY_I64);
};

ty_f32 : KW_F32
{
    $$ = frg_ast_new_ty_primary(&$1, FRG_AST_KIND_TY_F32);
};

ty_f64 : KW_F64
{
    $$ = frg_ast_new_ty_primary(&$1, FRG_AST_KIND_TY_F64);
};

ty_symbol : SYMBOL
{
    $$ = frg_ast_new_ty_symbol(&$1.source_range, $1.value);
};

ty_primary : ty_bool
{
    $$ = $1;
}
| ty_u8
{
    $$ = $1;
}
| ty_u16
{
    $$ = $1;
}
| ty_u32
{
    $$ = $1;
}
| ty_u64
{
    $$ = $1;
}
| ty_i8
{
    $$ = $1;
}
| ty_i16
{
    $$ = $1;
}
| ty_i32
{
    $$ = $1;
}
| ty_i64
{
    $$ = $1;
}
| ty_f32
{
    $$ = $1;
}
| ty_f64
{
    $$ = $1;
}
| ty_symbol
{
    $$ = $1;
};

ty_pointer: MUL ty_primary
{
    $$ = frg_ast_new_ty_pointer(&$1, $2);
}
| ty_primary
{
    $$ = $1;
};

ty_fn: PAREN_LEFT ty_list_optional PAREN_RIGHT ARROW_RIGHT ty_pointer
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &$1,
        &((frg_ast_t*)$5)->source_range
    );

    $$ = frg_ast_new_ty_fn(
        &source_range,
        $2,
        NULL,
        NULL,
        $5
    );
}
| ty_pointer
{
    $$ = $1;
};

ty : ty_fn
{
    $$ = $1;
};

ty_list: ty
{
    $$ = g_list_append(NULL, $1);
}
| ty_list COMMA ty
{
    $$ = g_list_append($1, $3);
};

ty_list_optional: ty_list
{
    $$ = $1;
}
|
{
    $$ = NULL;
};

decl_union_list : ty
{
    $$ = g_list_append(NULL, $1);
}
| decl_union_list BIT_OR ty
{
    $$ = g_list_append($1, $3);
};

decl_union : KW_TY SYMBOL ASSIGN decl_union_list SEMICOLON
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &$1,
        &$5
    );

    $$ = frg_ast_new_decl_union(&source_range, $2.value, $4);
};

decl_prop : SYMBOL
{
    $$ = frg_ast_new_decl_prop(&$1.source_range, FRG_AST_DECL_PROP_FLAG_NONE, $1.value, NULL);
}
| SYMBOL QUESTION_MARK
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &$1.source_range,
        &$2
    );

    $$ = frg_ast_new_decl_prop(&source_range, FRG_AST_DECL_PROP_FLAG_OPTIONAL, $1.value, NULL);
}
| SYMBOL LOG_NOT
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &$1.source_range,
        &$2
    );

    $$ = frg_ast_new_decl_prop(&source_range, FRG_AST_DECL_PROP_FLAG_NON_OPTIONAL, $1.value, NULL);
}
| ELIPSIS SYMBOL
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &$1,
        &$2.source_range
    );

    $$ = frg_ast_new_decl_prop(&source_range, FRG_AST_DECL_PROP_FLAG_SPREAD, $2.value, NULL);
}
| SYMBOL COLON ty
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &$1.source_range,
        &((frg_ast_t*)$3)->source_range
    );

    $$ = frg_ast_new_decl_prop(&source_range, FRG_AST_DECL_PROP_FLAG_NONE, $1.value, $3);
}
| SYMBOL QUESTION_MARK COLON ty
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &$1.source_range,
        &((frg_ast_t*)$4)->source_range
    );

    $$ = frg_ast_new_decl_prop(&source_range, FRG_AST_DECL_PROP_FLAG_OPTIONAL, $1.value, $4);
};

decl_struct_list : decl_prop SEMICOLON
{
    $$ = g_list_append(NULL, $1);
}
| decl_struct_list decl_prop SEMICOLON
{
    $$ = g_list_append($1, $2);
};

decl_struct : KW_TY SYMBOL CURLY_BRACE_LEFT decl_struct_list CURLY_BRACE_RIGHT
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &$1,
        &$5
    );

    $$ = frg_ast_new_decl_struct(&source_range, $2.value, $4);
};

decl_iface_extends_list : ty
{
    $$ = g_list_append(NULL, $1);
}
| decl_iface_extends_list COMMA ty
{
    $$ = g_list_append($1, $3);
};

decl_iface_optional_extends : 
{
    $$ = NULL;
}
| COLON decl_iface_extends_list
{
    $$ = $2;
};

decl_iface_list_element : KW_PROP decl_prop {
    $$ = NULL;
};

decl_iface_list : decl_iface_list_element
{
    $$ = g_list_append(NULL, $1);
}
| decl_iface_list decl_iface_list_element
{
    $$ = g_list_append($1, $2);
};

decl_iface : KW_IFACE SYMBOL decl_iface_optional_extends CURLY_BRACE_LEFT decl_iface_list CURLY_BRACE_RIGHT
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &$1,
        &$6
    );

    $$ = frg_ast_new_decl_iface(&source_range, FRG_AST_DECL_IFACE_FLAG_NONE, $2.value, $3, $5);
};

decl_fn_arg_default_value: ASSIGN value
{
    $$ = $2;
}
|
{
    $$ = NULL;
};

decl_fn_arg: decl_prop decl_fn_arg_default_value
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &((frg_ast_t*)$1)->source_range,
        &((frg_ast_t*)$2)->source_range
    );

    $$ = frg_ast_new_decl_fn_arg(&source_range, FRG_AST_DECL_FN_ARG_FLAG_NONE, $1, $2);
}
| KW_KW decl_prop decl_fn_arg_default_value
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &$1,
        &((frg_ast_t*)$3)->source_range
    );

    $$ = frg_ast_new_decl_fn_arg(&source_range, FRG_AST_DECL_FN_ARG_FLAG_KW, $2, $3);
};

decl_fn_arg_list: decl_fn_arg
{
    $$ = g_list_append(NULL, $1);
}
| decl_fn_arg_list COMMA decl_fn_arg
{
    $$ = g_list_append($1, $3);
};

decl_fn_arg_list_optional: decl_fn_arg_list
{
    $$ = $1;
}
|
{
    $$ = NULL;
};

decl_fn_return_ty : ARROW_RIGHT ty
{
    $$ = $2;
}
|
{
    $$ = NULL;
};

decl_fn : KW_FN SYMBOL PAREN_LEFT decl_fn_arg_list_optional PAREN_RIGHT decl_fn_return_ty SEMICOLON
{
    frg_parsing_range_t source_range_ty = frg_parsing_range_get_span(
        &$3,
        $6 == NULL
            ? &$5
            : &((frg_ast_t*)$6)->source_range
    );

    frg_ast_ty_fn_t* ty_fn = (frg_ast_ty_fn_t*)frg_ast_new_ty_fn(
        &source_range_ty,
        $4,
        NULL,
        NULL,
        $6
    );

    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &$1,
        &$7
    );

    $$ = frg_ast_new_decl_fn(&source_range, FRG_AST_DECL_FN_FLAG_NONE, $2.value, ty_fn, NULL);
}
| KW_FN SYMBOL PAREN_LEFT decl_fn_arg_list_optional PAREN_RIGHT decl_fn_return_ty stmt_block
{
    frg_parsing_range_t source_range_ty = frg_parsing_range_get_span(
        &$3,
        $6 == NULL
            ? &$5
            : &((frg_ast_t*)$6)->source_range
    );

    frg_ast_ty_fn_t* ty_fn = (frg_ast_ty_fn_t*)frg_ast_new_ty_fn(
        &source_range_ty,
        $4,
        NULL,
        NULL,
        $6
    );

    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &$1,
        &((frg_ast_t*)$7)->source_range
    );

    $$ = frg_ast_new_decl_fn(&source_range, FRG_AST_DECL_FN_FLAG_NONE, $2.value, ty_fn, $7);
}
| KW_MUT KW_FN SYMBOL PAREN_LEFT decl_fn_arg_list_optional PAREN_RIGHT decl_fn_return_ty SEMICOLON
{
    frg_parsing_range_t source_range_ty = frg_parsing_range_get_span(
        &$4,
        $7 == NULL
            ? &$6
            : &((frg_ast_t*)$7)->source_range
    );

    frg_ast_ty_fn_t* ty_fn = (frg_ast_ty_fn_t*)frg_ast_new_ty_fn(
        &source_range_ty,
        $5,
        NULL,
        NULL,
        $7
    );

    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &$1,
        &$8
    );

    $$ = frg_ast_new_decl_fn(&source_range, FRG_AST_DECL_FN_FLAG_MUT, $3.value, ty_fn, NULL);
}
| KW_MUT KW_FN SYMBOL PAREN_LEFT decl_fn_arg_list_optional PAREN_RIGHT decl_fn_return_ty stmt_block
{
    frg_parsing_range_t source_range_ty = frg_parsing_range_get_span(
        &$4,
        $7 == NULL
            ? &$6
            : &((frg_ast_t*)$7)->source_range
    );

    frg_ast_ty_fn_t* ty_fn = (frg_ast_ty_fn_t*)frg_ast_new_ty_fn(
        &source_range_ty,
        $5,
        NULL,
        NULL,
        $7
    );

    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &$1,
        &((frg_ast_t*)$8)->source_range
    );

    $$ = frg_ast_new_decl_fn(&source_range, FRG_AST_DECL_FN_FLAG_MUT, $3.value, ty_fn, $8);
}
| KW_OVERRIDE KW_FN SYMBOL PAREN_LEFT decl_fn_arg_list_optional PAREN_RIGHT decl_fn_return_ty SEMICOLON
{
    frg_parsing_range_t source_range_ty = frg_parsing_range_get_span(
        &$4,
        $7 == NULL
            ? &$6
            : &((frg_ast_t*)$7)->source_range
    );

    frg_ast_ty_fn_t* ty_fn = (frg_ast_ty_fn_t*)frg_ast_new_ty_fn(
        &source_range_ty,
        $5,
        NULL,
        NULL,
        $7
    );

    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &$1,
        &$8
    );

    $$ = frg_ast_new_decl_fn(&source_range, FRG_AST_DECL_FN_FLAG_OVERRIDE, $3.value, ty_fn, NULL);
}
| KW_OVERRIDE KW_FN SYMBOL PAREN_LEFT decl_fn_arg_list_optional PAREN_RIGHT decl_fn_return_ty stmt_block
{
    frg_parsing_range_t source_range_ty = frg_parsing_range_get_span(
        &$4,
        $7 == NULL
            ? &$6
            : &((frg_ast_t*)$7)->source_range
    );

    frg_ast_ty_fn_t* ty_fn = (frg_ast_ty_fn_t*)frg_ast_new_ty_fn(
        &source_range_ty,
        $5,
        NULL,
        NULL,
        $7
    );

    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &$1,
        &((frg_ast_t*)$8)->source_range
    );

    $$ = frg_ast_new_decl_fn(&source_range, FRG_AST_DECL_FN_FLAG_OVERRIDE, $3.value, ty_fn, $8);
};

decl_var : KW_LET decl_prop SEMICOLON
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &$1,
        &$3
    );

    $$ = frg_ast_new_decl_var(&source_range, $2, NULL);
}
| KW_LET decl_prop ASSIGN value SEMICOLON
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &$1,
        &$5
    );

    $$ = frg_ast_new_decl_var(&source_range, $2, $4);
};

decl : decl_union
{
    $$ = $1;
}
| decl_struct
{
    $$ = $1;
}
| decl_iface
{
    $$ = $1;
}
| decl_fn
{
    $$ = $1;
}
| decl_var
{
    $$ = $1;
};

decl_list: decl
{
    $$ = g_list_append(NULL, $1);
}
| decl_list decl
{
    $$ = g_list_append($1, $2);
};

decl_list_optional: decl_list
{
    $$ = $1;
}
|
{
    $$ = NULL;
};

decl_block: decl_list_optional
{
    $$ = frg_ast_new_decl_block(&frg_parsing_range_null, $1);
};

stmt_return : KW_RETURN value SEMICOLON
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &$1,
        &$3
    );

    $$ = frg_ast_new_stmt_return(&source_range, $2);
};

stmt_if : KW_IF PAREN_LEFT value PAREN_RIGHT stmt_block
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &$1,
        &((frg_ast_t*)$5)->source_range
    );

    $$ = frg_ast_new_stmt_if(&source_range, $3, $5, NULL);
}
| KW_IF PAREN_LEFT value PAREN_RIGHT stmt_block KW_ELSE stmt_if
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &$1,
        &((frg_ast_t*)$7)->source_range
    );

    $$ = frg_ast_new_stmt_if(&source_range, $3, $5, $7);
}
| KW_IF PAREN_LEFT value PAREN_RIGHT stmt_block KW_ELSE stmt_block
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &$1,
        &((frg_ast_t*)$7)->source_range
    );

    $$ = frg_ast_new_stmt_if(&source_range, $3, $5, $7);
};

stmt_while : KW_WHILE PAREN_LEFT value PAREN_RIGHT SEMICOLON
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &$1,
        &$5
    );

    $$ = frg_ast_new_stmt_while(&source_range, $3, NULL);
}
| KW_WHILE PAREN_LEFT value PAREN_RIGHT stmt_block
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &$1,
        &((frg_ast_t*)$5)->source_range
    );

    $$ = frg_ast_new_stmt_while(&source_range, $3, $5);
};

stmt : value SEMICOLON
{
    $$ = $1;
}
| stmt_return
{
    $$ = $1;
}
| stmt_if
{
    $$ = $1;
}
| stmt_while
{
    $$ = $1;
}
| stmt_block
{
    $$ = $1;
}
| decl
{
    $$ = $1;
}
| SEMICOLON
{
    $$ = NULL;
};

stmt_list : stmt
{
    $$ = g_list_append(NULL, $1);
}
| stmt_list stmt
{
    $$ = g_list_append($1, $2);
};

stmt_block : CURLY_BRACE_LEFT CURLY_BRACE_RIGHT
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &$1,
        &$2
    );

    $$ = frg_ast_new_stmt_block(&source_range, NULL);
}
| CURLY_BRACE_LEFT stmt_list CURLY_BRACE_RIGHT
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &$1,
        &$3
    );

    $$ = frg_ast_new_stmt_block(&source_range, $2);
};

value_true : KW_TRUE
{
    $$ = frg_ast_new_value_primary(&$1, FRG_AST_KIND_VALUE_TRUE);
};

value_false : KW_FALSE
{
    $$ = frg_ast_new_value_primary(&$1, FRG_AST_KIND_VALUE_FALSE);
};

value_int : INT
{
    $$ = frg_ast_new_value_int_from_lexical_cast_result(&$1.source_range, &$1.value);
};

value_float : FLOAT
{
    $$ = frg_ast_new_value_float_from_lexical_cast_result(&$1.source_range, &$1.value);
};

value_char : CHAR
{
    // Lexer already emits an error for this
    $$ = NULL;
};

value_str : STR
{
    // Lexer already emits an error for this
    $$ = NULL;
};

value_symbol : SYMBOL
{
    $$ = frg_ast_new_value_symbol(&$1.source_range, $1.value);
};

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
| value_char
{
    $$ = $1;
}
| value_str
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
};

value_call : value_symbol PAREN_LEFT value_list_optional PAREN_RIGHT
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &((frg_ast_t*)$1)->source_range,
        &$4
    );

    $$ = frg_ast_new_value_call(&source_range, $1, $3, NULL);
}
| value_primary
{
    $$ = $1;
};

value_access : value_access DOT value_call
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &((frg_ast_t*)$1)->source_range,
        &((frg_ast_t*)$3)->source_range
    );

    $$ = frg_ast_new_value_binary(&source_range, FRG_AST_KIND_VALUE_ACCESS, $1, $3);
}
| value_call
{
    $$ = $1;
};

value_deref_getaddr: BIT_AND value_deref_getaddr
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &$1,
        &((frg_ast_t*)$2)->source_range
    );

    $$ = frg_ast_new_value_unary(&source_range, FRG_AST_KIND_VALUE_GETADDR, $2);
}
| MUL value_deref_getaddr
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &$1,
        &((frg_ast_t*)$2)->source_range
    );

    $$ = frg_ast_new_value_unary(&source_range, FRG_AST_KIND_VALUE_DEREF, $2);
}
| value_access
{
    $$ = $1;
};

value_inc_dec : value_deref_getaddr INC
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &((frg_ast_t*)$1)->source_range,
        &$2
    );

    $$ = frg_ast_new_value_unary(&source_range, FRG_AST_KIND_VALUE_INC, $1);
}
| value_deref_getaddr DEC
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &((frg_ast_t*)$1)->source_range,
        &$2
    );

    $$ = frg_ast_new_value_unary(&source_range, FRG_AST_KIND_VALUE_DEC, $1);
}
| value_deref_getaddr
{
    $$ = $1;
};

value_exp : value_inc_dec EXP value_exp
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &((frg_ast_t*)$1)->source_range,
        &((frg_ast_t*)$3)->source_range
    );

    $$ = frg_ast_new_value_binary(&source_range, FRG_AST_KIND_VALUE_EXP, $1, $3);
}
| value_inc_dec
{
    $$ = $1;
};

value_neg_bit_not : BIT_NOT value_neg_bit_not
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &$1,
        &((frg_ast_t*)$2)->source_range
    );

    $$ = frg_ast_new_value_unary(&source_range, FRG_AST_KIND_VALUE_BIT_NOT, $2);
}
| SUB value_neg_bit_not
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &$1,
        &((frg_ast_t*)$2)->source_range
    );

    $$ = frg_ast_new_value_unary(&source_range, FRG_AST_KIND_VALUE_NEG, $2);
}
| value_exp
{
    $$ = $1;
};

value_mul : value_neg_bit_not MUL value_mul
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &((frg_ast_t*)$1)->source_range,
        &((frg_ast_t*)$3)->source_range
    );

    $$ = frg_ast_new_value_binary(&source_range, FRG_AST_KIND_VALUE_MUL, $1, $3);
}
| value_neg_bit_not DIV value_mul
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &((frg_ast_t*)$1)->source_range,
        &((frg_ast_t*)$3)->source_range
    );

    $$ = frg_ast_new_value_binary(&source_range, FRG_AST_KIND_VALUE_DIV, $1, $3);
}
| value_neg_bit_not DIV_INT value_mul
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &((frg_ast_t*)$1)->source_range,
        &((frg_ast_t*)$3)->source_range
    );

    $$ = frg_ast_new_value_binary(&source_range, FRG_AST_KIND_VALUE_DIV_INT, $1, $3);
}
| value_neg_bit_not MOD value_mul
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &((frg_ast_t*)$1)->source_range,
        &((frg_ast_t*)$3)->source_range
    );

    $$ = frg_ast_new_value_binary(&source_range, FRG_AST_KIND_VALUE_MOD, $1, $3);
}
| value_neg_bit_not
{
    $$ = $1;
};

value_add : value_mul ADD value_add
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &((frg_ast_t*)$1)->source_range,
        &((frg_ast_t*)$3)->source_range
    );

    $$ = frg_ast_new_value_binary(&source_range, FRG_AST_KIND_VALUE_ADD, $1, $3);
}
| value_mul SUB value_add
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &((frg_ast_t*)$1)->source_range,
        &((frg_ast_t*)$3)->source_range
    );

    $$ = frg_ast_new_value_binary(&source_range, FRG_AST_KIND_VALUE_SUB, $1, $3);
}
| value_mul
{
    $$ = $1;
};

value_bit_shift : value_add BIT_SHL value_bit_shift
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &((frg_ast_t*)$1)->source_range,
        &((frg_ast_t*)$3)->source_range
    );

    $$ = frg_ast_new_value_binary(&source_range, FRG_AST_KIND_VALUE_BIT_SHL, $1, $3);
}
| value_add BIT_SHR value_bit_shift
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &((frg_ast_t*)$1)->source_range,
        &((frg_ast_t*)$3)->source_range
    );

    $$ = frg_ast_new_value_binary(&source_range, FRG_AST_KIND_VALUE_BIT_SHR, $1, $3);
}
| value_add
{
    $$ = $1;
};

value_bit_and : value_bit_shift BIT_AND value_bit_and
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &((frg_ast_t*)$1)->source_range,
        &((frg_ast_t*)$3)->source_range
    );

    $$ = frg_ast_new_value_binary(&source_range, FRG_AST_KIND_VALUE_BIT_AND, $1, $3);
}
| value_bit_shift
{
    $$ = $1;
};

value_bit_xor : value_bit_and BIT_XOR value_bit_xor
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &((frg_ast_t*)$1)->source_range,
        &((frg_ast_t*)$3)->source_range
    );

    $$ = frg_ast_new_value_binary(&source_range, FRG_AST_KIND_VALUE_BIT_XOR, $1, $3);
}
| value_bit_and
{
    $$ = $1;
};

value_bit_or : value_bit_xor BIT_OR value_bit_or
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &((frg_ast_t*)$1)->source_range,
        &((frg_ast_t*)$3)->source_range
    );

    $$ = frg_ast_new_value_binary(&source_range, FRG_AST_KIND_VALUE_BIT_OR, $1, $3);
}
| value_bit_xor
{
    $$ = $1;
};

value_cmp : value_bit_or EQ value_cmp
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &((frg_ast_t*)$1)->source_range,
        &((frg_ast_t*)$3)->source_range
    );

    $$ = frg_ast_new_value_binary(&source_range, FRG_AST_KIND_VALUE_EQ, $1, $3);
}
| value_bit_or NE value_cmp
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &((frg_ast_t*)$1)->source_range,
        &((frg_ast_t*)$3)->source_range
    );

    $$ = frg_ast_new_value_binary(&source_range, FRG_AST_KIND_VALUE_NE, $1, $3);
}
| value_bit_or LT value_cmp
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &((frg_ast_t*)$1)->source_range,
        &((frg_ast_t*)$3)->source_range
    );

    $$ = frg_ast_new_value_binary(&source_range, FRG_AST_KIND_VALUE_LT, $1, $3);
}
| value_bit_or LE value_cmp
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &((frg_ast_t*)$1)->source_range,
        &((frg_ast_t*)$3)->source_range
    );

    $$ = frg_ast_new_value_binary(&source_range, FRG_AST_KIND_VALUE_LE, $1, $3);
}
| value_bit_or GT value_cmp
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &((frg_ast_t*)$1)->source_range,
        &((frg_ast_t*)$3)->source_range
    );

    $$ = frg_ast_new_value_binary(&source_range, FRG_AST_KIND_VALUE_GT, $1, $3);
}
| value_bit_or GE value_cmp
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &((frg_ast_t*)$1)->source_range,
        &((frg_ast_t*)$3)->source_range
    );

    $$ = frg_ast_new_value_binary(&source_range, FRG_AST_KIND_VALUE_GE, $1, $3);
}
| value_bit_or
{
    $$ = $1;
};

value_log_not : LOG_NOT value_cmp
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &$1,
        &((frg_ast_t*)$2)->source_range
    );

    $$ = frg_ast_new_value_unary(&source_range, FRG_AST_KIND_VALUE_LOG_NOT, $2);
}
| value_cmp
{
    $$ = $1;
};

value_log_and : value_log_not LOG_AND value_log_and
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &((frg_ast_t*)$1)->source_range,
        &((frg_ast_t*)$3)->source_range
    );

    $$ = frg_ast_new_value_binary(&source_range, FRG_AST_KIND_VALUE_LOG_AND, $1, $3);
}
| value_log_not
{
    $$ = $1;
};

value_log_or : value_log_and LOG_OR value_log_or
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &((frg_ast_t*)$1)->source_range,
        &((frg_ast_t*)$3)->source_range
    );

    $$ = frg_ast_new_value_binary(&source_range, FRG_AST_KIND_VALUE_LOG_OR, $1, $3);
}
| value_log_and
{
    $$ = $1;
};

value_assign : value_log_or ASSIGN value_assign
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &((frg_ast_t*)$1)->source_range,
        &((frg_ast_t*)$3)->source_range
    );

    $$ = frg_ast_new_value_binary(&source_range, FRG_AST_KIND_VALUE_ASSIGN, $1, $3);
}
| value_log_or ADD_ASSIGN value_assign
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &((frg_ast_t*)$1)->source_range,
        &((frg_ast_t*)$3)->source_range
    );

    $$ = frg_ast_new_value_binary(&source_range, FRG_AST_KIND_VALUE_ADD_ASSIGN, $1, $3);
}
| value_log_or SUB_ASSIGN value_assign
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &((frg_ast_t*)$1)->source_range,
        &((frg_ast_t*)$3)->source_range
    );

    $$ = frg_ast_new_value_binary(&source_range, FRG_AST_KIND_VALUE_SUB_ASSIGN, $1, $3);
}
| value_log_or MUL_ASSIGN value_assign
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &((frg_ast_t*)$1)->source_range,
        &((frg_ast_t*)$3)->source_range
    );

    $$ = frg_ast_new_value_binary(&source_range, FRG_AST_KIND_VALUE_MUL_ASSIGN, $1, $3);
}
| value_log_or DIV_ASSIGN value_assign
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &((frg_ast_t*)$1)->source_range,
        &((frg_ast_t*)$3)->source_range
    );

    $$ = frg_ast_new_value_binary(&source_range, FRG_AST_KIND_VALUE_DIV_ASSIGN, $1, $3);
}
| value_log_or DIV_INT_ASSIGN value_assign
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &((frg_ast_t*)$1)->source_range,
        &((frg_ast_t*)$3)->source_range
    );

    $$ = frg_ast_new_value_binary(&source_range, FRG_AST_KIND_VALUE_DIV_INT_ASSIGN, $1, $3);
}
| value_log_or MOD_ASSIGN value_assign
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &((frg_ast_t*)$1)->source_range,
        &((frg_ast_t*)$3)->source_range
    );

    $$ = frg_ast_new_value_binary(&source_range, FRG_AST_KIND_VALUE_MOD_ASSIGN, $1, $3);
}
| value_log_or BIT_SHL_ASSIGN value_assign
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &((frg_ast_t*)$1)->source_range,
        &((frg_ast_t*)$3)->source_range
    );

    $$ = frg_ast_new_value_binary(&source_range, FRG_AST_KIND_VALUE_BIT_SHL_ASSIGN, $1, $3);
}
| value_log_or BIT_SHR_ASSIGN value_assign
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &((frg_ast_t*)$1)->source_range,
        &((frg_ast_t*)$3)->source_range
    );

    $$ = frg_ast_new_value_binary(&source_range, FRG_AST_KIND_VALUE_BIT_SHR_ASSIGN, $1, $3);
}
| value_log_or BIT_AND_ASSIGN value_assign
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &((frg_ast_t*)$1)->source_range,
        &((frg_ast_t*)$3)->source_range
    );

    $$ = frg_ast_new_value_binary(&source_range, FRG_AST_KIND_VALUE_BIT_AND_ASSIGN, $1, $3);
}
| value_log_or BIT_XOR_ASSIGN value_assign
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &((frg_ast_t*)$1)->source_range,
        &((frg_ast_t*)$3)->source_range
    );

    $$ = frg_ast_new_value_binary(&source_range, FRG_AST_KIND_VALUE_BIT_XOR_ASSIGN, $1, $3);
}
| value_log_or BIT_OR_ASSIGN value_assign
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &((frg_ast_t*)$1)->source_range,
        &((frg_ast_t*)$3)->source_range
    );

    $$ = frg_ast_new_value_binary(&source_range, FRG_AST_KIND_VALUE_BIT_OR_ASSIGN, $1, $3);
}
| value_log_or EXP_ASSIGN value_assign
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &((frg_ast_t*)$1)->source_range,
        &((frg_ast_t*)$3)->source_range
    );

    $$ = frg_ast_new_value_binary(&source_range, FRG_AST_KIND_VALUE_EXP_ASSIGN, $1, $3);
}
| value_log_or LOG_AND_ASSIGN value_assign
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &((frg_ast_t*)$1)->source_range,
        &((frg_ast_t*)$3)->source_range
    );

    $$ = frg_ast_new_value_binary(&source_range, FRG_AST_KIND_VALUE_LOG_AND_ASSIGN, $1, $3);
}
| value_log_or LOG_OR_ASSIGN value_assign
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &((frg_ast_t*)$1)->source_range,
        &((frg_ast_t*)$3)->source_range
    );

    $$ = frg_ast_new_value_binary(&source_range, FRG_AST_KIND_VALUE_LOG_OR_ASSIGN, $1, $3);
}
| value_log_or
{
    $$ = $1;
};

value : value_assign
{
    $$ = $1;
};

value_list : value
{
    $$ = g_list_append(NULL, $1);
}
| value_list COMMA value
{
    $$ = g_list_append($1, $3);
};

value_list_optional : value_list
{
    $$ = $1;
}
|
{
    $$ = NULL;
};

top_level : decl_block
{
    $$ = *ast = $1;
};
%%
