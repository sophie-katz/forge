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
%start decl_block
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
    uint64_t value_int;
    frg_f64_t value_float;
    frg_char_t value_char;
    GString* value_str;
    void* ast;
    GList* list;
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
%token KW_KW
%token KW_FN
%token KW_CONST
%token KW_MUT
%token KW_OVERRIDE
%token KW_LET
%token KW_RETURN
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
%token COLON
%token SEMICOLON
%token QUESTION_MARK
%token ARROW_RIGHT
%token ELIPSIS
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
%type <ast> ty
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
%type <ast> decl_fn_body
%type <ast> decl_fn
%type <ast> decl_var
%type <ast> decl
%type <list> decl_list
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
%type <ast> value_access_call
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

%%
ty_bool : KW_BOOL
{
    frg_status_t result = frg_ast_new_ty_primary((frg_ast_t**)&$$, FRG_AST_ID_TY_BOOL);
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "unable to create ast: %s", frg_status_to_string(result));
    }
};

ty_u8 : KW_U8
{
    frg_status_t result = frg_ast_new_ty_primary((frg_ast_t**)&$$, FRG_AST_ID_TY_U8);
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "unable to create ast: %s", frg_status_to_string(result));
    }
};

ty_u16 : KW_U16
{
    frg_status_t result = frg_ast_new_ty_primary((frg_ast_t**)&$$, FRG_AST_ID_TY_U16);
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "unable to create ast: %s", frg_status_to_string(result));
    }
};

ty_u32 : KW_U32
{
    frg_status_t result = frg_ast_new_ty_primary((frg_ast_t**)&$$, FRG_AST_ID_TY_U32);
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "unable to create ast: %s", frg_status_to_string(result));
    }
};

ty_u64 : KW_U64
{
    frg_status_t result = frg_ast_new_ty_primary((frg_ast_t**)&$$, FRG_AST_ID_TY_U64);
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "unable to create ast: %s", frg_status_to_string(result));
    }
};

ty_i8 : KW_I8
{
    frg_status_t result = frg_ast_new_ty_primary((frg_ast_t**)&$$, FRG_AST_ID_TY_I8);
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "unable to create ast: %s", frg_status_to_string(result));
    }
};

ty_i16 : KW_I16
{
    frg_status_t result = frg_ast_new_ty_primary((frg_ast_t**)&$$, FRG_AST_ID_TY_I16);
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "unable to create ast: %s", frg_status_to_string(result));
    }
};

ty_i32 : KW_I32
{
    frg_status_t result = frg_ast_new_ty_primary((frg_ast_t**)&$$, FRG_AST_ID_TY_I32);
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "unable to create ast: %s", frg_status_to_string(result));
    }
};

ty_i64 : KW_I64
{
    frg_status_t result = frg_ast_new_ty_primary((frg_ast_t**)&$$, FRG_AST_ID_TY_I64);
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "unable to create ast: %s", frg_status_to_string(result));
    }
};

ty_f32 : KW_F32
{
    frg_status_t result = frg_ast_new_ty_primary((frg_ast_t**)&$$, FRG_AST_ID_TY_F32);
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "unable to create ast: %s", frg_status_to_string(result));
    }
};

ty_f64 : KW_F64
{
    frg_status_t result = frg_ast_new_ty_primary((frg_ast_t**)&$$, FRG_AST_ID_TY_F64);
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "unable to create ast: %s", frg_status_to_string(result));
    }
};

ty_symbol : SYMBOL
{
    frg_status_t result = frg_ast_new_ty_symbol((frg_ast_ty_symbol_t**)&$$, $1);
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "unable to create ast: %s", frg_status_to_string(result));
    }
};

ty : ty_bool
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
    frg_status_t result = frg_ast_new_decl_union((frg_ast_decl_union_t**)&$$, $2, $4);
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_ERROR, "unable to create ast: %s", frg_status_to_string(result));
    }
};

decl_prop : SYMBOL
{
    frg_status_t result = frg_ast_new_decl_prop((frg_ast_decl_prop_t**)&$$, FRG_AST_DECL_PROP_FLAG_NONE, $1, NULL);
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_ERROR, "unable to create ast: %s", frg_status_to_string(result));
    }
}
| SYMBOL QUESTION_MARK
{
    frg_status_t result = frg_ast_new_decl_prop((frg_ast_decl_prop_t**)&$$, FRG_AST_DECL_PROP_FLAG_OPTIONAL, $1, NULL);
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_ERROR, "unable to create ast: %s", frg_status_to_string(result));
    }
}
| SYMBOL LOG_NOT
{
    frg_status_t result = frg_ast_new_decl_prop((frg_ast_decl_prop_t**)&$$, FRG_AST_DECL_PROP_FLAG_NON_OPTIONAL, $1, NULL);
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_ERROR, "unable to create ast: %s", frg_status_to_string(result));
    }
}
| ELIPSIS SYMBOL
{
    frg_status_t result = frg_ast_new_decl_prop((frg_ast_decl_prop_t**)&$$, FRG_AST_DECL_PROP_FLAG_SPREAD, $2, NULL);
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_ERROR, "unable to create ast: %s", frg_status_to_string(result));
    }
}
| SYMBOL COLON ty
{
    frg_status_t result = frg_ast_new_decl_prop((frg_ast_decl_prop_t**)&$$, FRG_AST_DECL_PROP_FLAG_NONE, $1, $3);
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_ERROR, "unable to create ast: %s", frg_status_to_string(result));
    }
}
| SYMBOL QUESTION_MARK COLON ty
{
    frg_status_t result = frg_ast_new_decl_prop((frg_ast_decl_prop_t**)&$$, FRG_AST_DECL_PROP_FLAG_OPTIONAL, $1, $4);
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_ERROR, "unable to create ast: %s", frg_status_to_string(result));
    }
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
    frg_status_t result = frg_ast_new_decl_struct((frg_ast_decl_struct_t**)&$$, $2, $4);
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_ERROR, "unable to create ast: %s", frg_status_to_string(result));
    }
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
    frg_status_t result = frg_ast_new_decl_iface((frg_ast_decl_iface_t**)&$$, FRG_AST_DECL_IFACE_FLAG_NONE, $2, $3, $5);
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_ERROR, "unable to create ast: %s", frg_status_to_string(result));
    }
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
    frg_status_t result = frg_ast_new_decl_fn_arg((frg_ast_decl_fn_arg_t**)&$$, FRG_AST_DECL_FN_ARG_FLAG_NONE, $1, $2);
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_ERROR, "unable to create ast: %s", frg_status_to_string(result));
    }
}
| KW_KW decl_prop decl_fn_arg_default_value
{
    frg_status_t result = frg_ast_new_decl_fn_arg((frg_ast_decl_fn_arg_t**)&$$, FRG_AST_DECL_FN_ARG_FLAG_KW, $2, $3);
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_ERROR, "unable to create ast: %s", frg_status_to_string(result));
    }
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

decl_fn_body : stmt_block
{
    $$ = $1;
}
| SEMICOLON
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

decl_fn : KW_FN SYMBOL PAREN_LEFT decl_fn_arg_list_optional PAREN_RIGHT decl_fn_return_ty decl_fn_body
{
    frg_status_t result = frg_ast_new_decl_fn((frg_ast_decl_fn_t**)&$$, FRG_AST_DECL_FN_FLAG_NONE, $2, $4, NULL, NULL, $6, $7);
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_ERROR, "unable to create ast: %s", frg_status_to_string(result));
    }
}
| KW_MUT KW_FN SYMBOL PAREN_LEFT decl_fn_arg_list_optional PAREN_RIGHT decl_fn_return_ty decl_fn_body
{
    frg_status_t result = frg_ast_new_decl_fn((frg_ast_decl_fn_t**)&$$, FRG_AST_DECL_FN_FLAG_MUT, $3, $5, NULL, NULL, $7, $8);
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_ERROR, "unable to create ast: %s", frg_status_to_string(result));
    }
}
| KW_OVERRIDE KW_FN SYMBOL PAREN_LEFT decl_fn_arg_list_optional PAREN_RIGHT decl_fn_return_ty decl_fn_body
{
    frg_status_t result = frg_ast_new_decl_fn((frg_ast_decl_fn_t**)&$$, FRG_AST_DECL_FN_FLAG_OVERRIDE, $3, $5, NULL, NULL, $7, $8);
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_ERROR, "unable to create ast: %s", frg_status_to_string(result));
    }
};

decl_var : KW_LET decl_prop SEMICOLON
{
    frg_status_t result = frg_ast_new_decl_var((frg_ast_decl_var_t**)&$$, $2, NULL);
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_ERROR, "%s", frg_status_to_string(result));
    }
}
| KW_LET decl_prop ASSIGN value SEMICOLON
{
    frg_status_t result = frg_ast_new_decl_var((frg_ast_decl_var_t**)&$$, $2, $4);
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_ERROR, "%s", frg_status_to_string(result));
    }
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

decl_block: decl_list
{
    frg_status_t result = frg_ast_new_decl_block((frg_ast_decl_block_t**)&$$, $1);
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_ERROR, "unable to create ast: %s", frg_status_to_string(result));
    }
};

stmt_return : KW_RETURN value SEMICOLON
{
    frg_status_t result = frg_ast_new_stmt_return((frg_ast_stmt_return_t**)&$$, $2);
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_ERROR, "unable to create ast: %s", frg_status_to_string(result));
    }
};

stmt_if : KW_IF PAREN_LEFT value PAREN_RIGHT SEMICOLON
{
    frg_status_t result = frg_ast_new_stmt_if((frg_ast_stmt_if_t**)&$$, $3, NULL, NULL);
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_ERROR, "unable to create ast: %s", frg_status_to_string(result));
    }
}
| KW_IF PAREN_LEFT value PAREN_RIGHT stmt_block
{
    frg_status_t result = frg_ast_new_stmt_if((frg_ast_stmt_if_t**)&$$, $3, NULL, NULL);
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_ERROR, "unable to create ast: %s", frg_status_to_string(result));
    }
}
| KW_IF PAREN_LEFT value PAREN_RIGHT stmt_block KW_ELSE stmt_if
{
    frg_status_t result = frg_ast_new_stmt_if((frg_ast_stmt_if_t**)&$$, $3, $5, $7);
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_ERROR, "unable to create ast: %s", frg_status_to_string(result));
    }
}
| KW_IF PAREN_LEFT value PAREN_RIGHT stmt_block KW_ELSE stmt_block
{
    frg_status_t result = frg_ast_new_stmt_if((frg_ast_stmt_if_t**)&$$, $3, $5, $7);
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_ERROR, "unable to create ast: %s", frg_status_to_string(result));
    }
};

stmt_while : KW_WHILE PAREN_LEFT value PAREN_RIGHT SEMICOLON
{
    frg_status_t result = frg_ast_new_stmt_while((frg_ast_stmt_while_t**)&$$, $3, NULL);
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_ERROR, "unable to create ast: %s", frg_status_to_string(result));
    }
}
| KW_WHILE PAREN_LEFT value PAREN_RIGHT stmt_block
{
    frg_status_t result = frg_ast_new_stmt_while((frg_ast_stmt_while_t**)&$$, $3, $5);
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_ERROR, "unable to create ast: %s", frg_status_to_string(result));
    }
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
    $$ = NULL;
}
| CURLY_BRACE_LEFT stmt_list CURLY_BRACE_RIGHT
{
    frg_status_t result = frg_ast_new_stmt_block((frg_ast_stmt_block_t**)&$$, $2);
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "unable to create ast: %s", frg_status_to_string(result));
    }
};

value_true : KW_TRUE
{
    frg_status_t result = frg_ast_new_value_primary((frg_ast_t**)&$$, FRG_AST_ID_VALUE_TRUE);
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "unable to create ast: %s", frg_status_to_string(result));
    }
};

value_false : KW_FALSE
{
    frg_status_t result = frg_ast_new_value_primary((frg_ast_t**)&$$, FRG_AST_ID_VALUE_FALSE);
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "unable to create ast: %s", frg_status_to_string(result));
    }
};

value_int : INT
{
    frg_status_t result = frg_ast_new_value_i64((frg_ast_value_int_t**)&$$, $1);
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "unable to create ast: %s", frg_status_to_string(result));
    }
};

value_float : FLOAT
{
    frg_status_t result = frg_ast_new_value_f64((frg_ast_value_float_t**)&$$, $1);
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "unable to create ast: %s", frg_status_to_string(result));
    }
};

value_char : CHAR
{
    frg_status_t result = frg_ast_new_value_char((frg_ast_value_char_t**)&$$, $1);
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "unable to create ast: %s", frg_status_to_string(result));
    }
};

value_str : STR
{
    frg_status_t result = frg_ast_new_value_str((frg_ast_value_str_t**)&$$, $1);
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "unable to create ast: %s", frg_status_to_string(result));
    }
};

value_symbol : SYMBOL
{
    frg_status_t result = frg_ast_new_value_symbol((frg_ast_value_symbol_t**)&$$, $1);
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "unable to create ast: %s", frg_status_to_string(result));
    }
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

value_access_call : value_access_call DOT value_symbol
{
    frg_status_t result = frg_ast_new_value_binary((frg_ast_value_binary_t**)&$$, FRG_AST_ID_VALUE_ACCESS, $1, $3);
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_ERROR, "unable to create ast: %s", frg_status_to_string(result));
    }
}
| value_access_call PAREN_LEFT value_list_optional PAREN_RIGHT
{
    frg_status_t result = frg_ast_new_value_call((frg_ast_value_call_t**)&$$, $1, $3, NULL);
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_ERROR, "unable to create ast: %s", frg_status_to_string(result));
    }
}
| value_primary
{
    $$ = $1;
};

value_inc_dec : value_access_call INC
{
    frg_status_t result = frg_ast_new_value_unary((frg_ast_value_unary_t**)&$$, FRG_AST_ID_VALUE_INC, $1);
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_ERROR, "%s", frg_status_to_string(result));
    }
}
| value_access_call DEC
{
    frg_status_t result = frg_ast_new_value_unary((frg_ast_value_unary_t**)&$$, FRG_AST_ID_VALUE_DEC, $1);
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_ERROR, "%s", frg_status_to_string(result));
    }
};

value_exp : value_inc_dec EXP value_exp
{
    frg_status_t result = frg_ast_new_value_binary((frg_ast_value_binary_t**)&$$, FRG_AST_ID_VALUE_EXP, $1, $3);
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_ERROR, "%s", frg_status_to_string(result));
    }
}
| value_inc_dec
{
    $$ = $1;
};

value_neg_bit_not : BIT_NOT value_neg_bit_not
{
    frg_status_t result = frg_ast_new_value_unary((frg_ast_value_unary_t**)&$$, FRG_AST_ID_VALUE_BIT_NOT, $2);
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_ERROR, "%s", frg_status_to_string(result));
    }
}
| SUB value_neg_bit_not
{
    frg_status_t result = frg_ast_new_value_unary((frg_ast_value_unary_t**)&$$, FRG_AST_ID_VALUE_NEG, $2);
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_ERROR, "%s", frg_status_to_string(result));
    }
}
| value_exp
{
    $$ = $1;
};

value_mul : value_neg_bit_not MUL value_mul
{
    frg_status_t result = frg_ast_new_value_binary((frg_ast_value_binary_t**)&$$, FRG_AST_ID_VALUE_MUL, $1, $3);
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_ERROR, "%s", frg_status_to_string(result));
    }
}
| value_neg_bit_not DIV value_mul
{
    frg_status_t result = frg_ast_new_value_binary((frg_ast_value_binary_t**)&$$, FRG_AST_ID_VALUE_DIV, $1, $3);
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_ERROR, "%s", frg_status_to_string(result));
    }
}
| value_neg_bit_not DIV_INT value_mul
{
    frg_status_t result = frg_ast_new_value_binary((frg_ast_value_binary_t**)&$$, FRG_AST_ID_VALUE_DIV_INT, $1, $3);
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_ERROR, "%s", frg_status_to_string(result));
    }
}
| value_neg_bit_not MOD value_mul
{
    frg_status_t result = frg_ast_new_value_binary((frg_ast_value_binary_t**)&$$, FRG_AST_ID_VALUE_MOD, $1, $3);
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_ERROR, "%s", frg_status_to_string(result));
    }
}
| value_neg_bit_not
{
    $$ = $1;
};

value_add : value_mul ADD value_add
{
    frg_status_t result = frg_ast_new_value_binary((frg_ast_value_binary_t**)&$$, FRG_AST_ID_VALUE_ADD, $1, $3);
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_ERROR, "%s", frg_status_to_string(result));
    }
}
| value_mul SUB value_add
{
    frg_status_t result = frg_ast_new_value_binary((frg_ast_value_binary_t**)&$$, FRG_AST_ID_VALUE_SUB, $1, $3);
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_ERROR, "%s", frg_status_to_string(result));
    }
}
| value_exp
{
    $$ = $1;
};

value_bit_shift : value_add BIT_SHL value_bit_shift
{
    frg_status_t result = frg_ast_new_value_binary((frg_ast_value_binary_t**)&$$, FRG_AST_ID_VALUE_BIT_SHL, $1, $3);
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_ERROR, "%s", frg_status_to_string(result));
    }
}
| value_add BIT_SHR value_bit_shift
{
    frg_status_t result = frg_ast_new_value_binary((frg_ast_value_binary_t**)&$$, FRG_AST_ID_VALUE_BIT_SHR, $1, $3);
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_ERROR, "%s", frg_status_to_string(result));
    }
}
| value_exp
{
    $$ = $1;
};

value_bit_and : value_bit_shift BIT_AND value_bit_and
{
    frg_status_t result = frg_ast_new_value_binary((frg_ast_value_binary_t**)&$$, FRG_AST_ID_VALUE_BIT_AND, $1, $3);
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_ERROR, "%s", frg_status_to_string(result)); 
    }
}
| value_bit_shift
{
    $$ = $1;
};

value_bit_xor : value_bit_and BIT_XOR value_bit_xor
{
    frg_status_t result = frg_ast_new_value_binary((frg_ast_value_binary_t**)&$$, FRG_AST_ID_VALUE_BIT_XOR, $1, $3);
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_ERROR, "%s", frg_status_to_string(result)); 
    }
}
| value_bit_and
{
    $$ = $1;
};

value_bit_or : value_bit_xor BIT_OR value_bit_or
{
    frg_status_t result = frg_ast_new_value_binary((frg_ast_value_binary_t**)&$$, FRG_AST_ID_VALUE_BIT_OR, $1, $3);
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_ERROR, "%s", frg_status_to_string(result)); 
    }
}
| value_bit_xor
{
    $$ = $1;
};

value_cmp : value_bit_or EQ value_cmp
{
    frg_status_t result = frg_ast_new_value_binary((frg_ast_value_binary_t**)&$$, FRG_AST_ID_VALUE_EQ, $1, $3);
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_ERROR,"%s", frg_status_to_string(result));
    }
}
| value_bit_or NE value_cmp
{
    frg_status_t result = frg_ast_new_value_binary((frg_ast_value_binary_t**)&$$, FRG_AST_ID_VALUE_NE, $1, $3);
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_ERROR,"%s", frg_status_to_string(result));
    }
}
| value_bit_or LT value_cmp
{
    frg_status_t result = frg_ast_new_value_binary((frg_ast_value_binary_t**)&$$, FRG_AST_ID_VALUE_LT, $1, $3);
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_ERROR,"%s", frg_status_to_string(result));
    }
}
| value_bit_or LE value_cmp
{
    frg_status_t result = frg_ast_new_value_binary((frg_ast_value_binary_t**)&$$, FRG_AST_ID_VALUE_LE, $1, $3);
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_ERROR,"%s", frg_status_to_string(result));
    }
}
| value_bit_or GT value_cmp
{
    frg_status_t result = frg_ast_new_value_binary((frg_ast_value_binary_t**)&$$, FRG_AST_ID_VALUE_GT, $1, $3);
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_ERROR,"%s", frg_status_to_string(result));
    }
}
| value_bit_or GE value_cmp
{
    frg_status_t result = frg_ast_new_value_binary((frg_ast_value_binary_t**)&$$, FRG_AST_ID_VALUE_GE, $1, $3);
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_ERROR,"%s", frg_status_to_string(result));
    }
}
| value_bit_or
{
    $$ = $1;
};

value_log_not : LOG_NOT value_cmp
{
    frg_status_t result = frg_ast_new_value_unary((frg_ast_value_unary_t**)&$$, FRG_AST_ID_VALUE_LOG_NOT, $2);
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_ERROR,"%s", frg_status_to_string(result));
    }
}
| value_cmp
{
    $$ = $1;
};

value_log_and : value_log_not LOG_AND value_log_and
{
    frg_status_t result = frg_ast_new_value_binary((frg_ast_value_binary_t**)&$$, FRG_AST_ID_VALUE_LOG_AND, $1, $3);
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_ERROR,"%s", frg_status_to_string(result));
    }
}
| value_log_not
{
    $$ = $1;
};

value_log_or : value_log_and LOG_OR value_log_or
{
    frg_status_t result = frg_ast_new_value_binary((frg_ast_value_binary_t**)&$$, FRG_AST_ID_VALUE_LOG_OR, $1, $3);
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_ERROR,"%s", frg_status_to_string(result));
    }
}
| value_log_and
{
    $$ = $1;
};

value_assign : value_log_or ASSIGN value_assign
{
    frg_status_t result = frg_ast_new_value_binary((frg_ast_value_binary_t**)&$$, FRG_AST_ID_VALUE_ASSIGN, $1, $3);
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_ERROR,"%s", frg_status_to_string(result));
    }
}
| value_log_or ADD_ASSIGN value_assign
{
    frg_status_t result = frg_ast_new_value_binary((frg_ast_value_binary_t**)&$$, FRG_AST_ID_VALUE_ADD_ASSIGN, $1, $3);
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_ERROR,"%s", frg_status_to_string(result));
    }
}
| value_log_or SUB_ASSIGN value_assign
{
    frg_status_t result = frg_ast_new_value_binary((frg_ast_value_binary_t**)&$$, FRG_AST_ID_VALUE_SUB_ASSIGN, $1, $3);
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_ERROR,"%s", frg_status_to_string(result));
    }
}
| value_log_or MUL_ASSIGN value_assign
{
    frg_status_t result = frg_ast_new_value_binary((frg_ast_value_binary_t**)&$$, FRG_AST_ID_VALUE_MUL_ASSIGN, $1, $3);
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_ERROR,"%s", frg_status_to_string(result));
    }
}
| value_log_or DIV_ASSIGN value_assign
{
    frg_status_t result = frg_ast_new_value_binary((frg_ast_value_binary_t**)&$$, FRG_AST_ID_VALUE_DIV_ASSIGN, $1, $3);
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_ERROR,"%s", frg_status_to_string(result)); 
    }
}
| value_log_or DIV_INT_ASSIGN value_assign
{
    frg_status_t result = frg_ast_new_value_binary((frg_ast_value_binary_t**)&$$, FRG_AST_ID_VALUE_DIV_INT_ASSIGN, $1, $3);
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_ERROR,"%s", frg_status_to_string(result)); 
    }
}
| value_log_or MOD_ASSIGN value_assign
{
    frg_status_t result = frg_ast_new_value_binary((frg_ast_value_binary_t**)&$$, FRG_AST_ID_VALUE_MOD_ASSIGN, $1, $3);
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_ERROR,"%s", frg_status_to_string(result)); 
    }
}
| value_log_or BIT_SHL_ASSIGN value_assign
{
    frg_status_t result = frg_ast_new_value_binary((frg_ast_value_binary_t**)&$$, FRG_AST_ID_VALUE_BIT_SHL_ASSIGN, $1, $3);
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_ERROR,"%s", frg_status_to_string(result)); 
    }
}
| value_log_or BIT_SHR_ASSIGN value_assign
{
    frg_status_t result = frg_ast_new_value_binary((frg_ast_value_binary_t**)&$$, FRG_AST_ID_VALUE_BIT_SHR_ASSIGN, $1, $3);
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_ERROR,"%s", frg_status_to_string(result)); 
    }
}
| value_log_or BIT_AND_ASSIGN value_assign
{
    frg_status_t result = frg_ast_new_value_binary((frg_ast_value_binary_t**)&$$, FRG_AST_ID_VALUE_BIT_AND_ASSIGN, $1, $3);
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_ERROR, "%s", frg_status_to_string(result)); 
    }
}
| value_log_or BIT_XOR_ASSIGN value_assign
{
    frg_status_t result = frg_ast_new_value_binary((frg_ast_value_binary_t**)&$$, FRG_AST_ID_VALUE_BIT_XOR_ASSIGN, $1, $3);
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "%s", frg_status_to_string(result)); 
    }
}
| value_log_or BIT_OR_ASSIGN value_assign
{
    frg_status_t result = frg_ast_new_value_binary((frg_ast_value_binary_t**)&$$, FRG_AST_ID_VALUE_BIT_OR_ASSIGN, $1, $3);
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "%s", frg_status_to_string(result)); 
    }
}
| value_log_or EXP_ASSIGN value_assign
{
    frg_status_t result = frg_ast_new_value_binary((frg_ast_value_binary_t**)&$$, FRG_AST_ID_VALUE_EXP_ASSIGN, $1, $3);
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "%s", frg_status_to_string(result)); 
    }
}
| value_log_or LOG_AND_ASSIGN value_assign
{
    frg_status_t result = frg_ast_new_value_binary((frg_ast_value_binary_t**)&$$, FRG_AST_ID_VALUE_LOG_AND_ASSIGN, $1, $3);
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "%s", frg_status_to_string(result)); 
    }
}
| value_log_or LOG_OR_ASSIGN value_assign
{
    frg_status_t result = frg_ast_new_value_binary((frg_ast_value_binary_t**)&$$, FRG_AST_ID_VALUE_LOG_OR_ASSIGN, $1, $3);
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "%s", frg_status_to_string(result)); 
    }
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
%%
