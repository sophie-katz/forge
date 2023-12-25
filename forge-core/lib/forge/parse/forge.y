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
%token KW_FN
%token KW_CONST
%token KW_MUT
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
%type <ast> decl_prop_non_const
%type <ast> decl_prop
%type <list> decl_struct_list
%type <ast> decl_struct
%type <list> decl_iface_extends_list
%type <list> decl_iface_optional_extends
%type <ast> decl_iface_list_element
%type <list> decl_iface_list
%type <ast> decl_iface
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
%type <ast> value_access
%type <ast> value_bit_not
%type <ast> value_bit_and
%type <ast> value_bit_or
%type <ast> value_log_not
%type <ast> value_log_and
%type <ast> value_log_or
%type <ast> value

%%
ty_bool : KW_BOOL
{
    frg_status_t result = frg_ast_new_ty_primary((frg_ast_ty_t**)&$$, FRG_AST_ID_TY_BOOL);
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "unable to create ast: %s", frg_status_to_string(result));
    }
};

ty_u8 : KW_U8
{
    frg_status_t result = frg_ast_new_ty_primary((frg_ast_ty_t**)&$$, FRG_AST_ID_TY_U8);
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "unable to create ast: %s", frg_status_to_string(result));
    }
};

ty_u16 : KW_U16
{
    frg_status_t result = frg_ast_new_ty_primary((frg_ast_ty_t**)&$$, FRG_AST_ID_TY_U16);
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "unable to create ast: %s", frg_status_to_string(result));
    }
};

ty_u32 : KW_U32
{
    frg_status_t result = frg_ast_new_ty_primary((frg_ast_ty_t**)&$$, FRG_AST_ID_TY_U32);
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "unable to create ast: %s", frg_status_to_string(result));
    }
};

ty_u64 : KW_U64
{
    frg_status_t result = frg_ast_new_ty_primary((frg_ast_ty_t**)&$$, FRG_AST_ID_TY_U64);
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "unable to create ast: %s", frg_status_to_string(result));
    }
};

ty_i8 : KW_I8
{
    frg_status_t result = frg_ast_new_ty_primary((frg_ast_ty_t**)&$$, FRG_AST_ID_TY_I8);
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "unable to create ast: %s", frg_status_to_string(result));
    }
};

ty_i16 : KW_I16
{
    frg_status_t result = frg_ast_new_ty_primary((frg_ast_ty_t**)&$$, FRG_AST_ID_TY_I16);
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "unable to create ast: %s", frg_status_to_string(result));
    }
};

ty_i32 : KW_I32
{
    frg_status_t result = frg_ast_new_ty_primary((frg_ast_ty_t**)&$$, FRG_AST_ID_TY_I32);
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "unable to create ast: %s", frg_status_to_string(result));
    }
};

ty_i64 : KW_I64
{
    frg_status_t result = frg_ast_new_ty_primary((frg_ast_ty_t**)&$$, FRG_AST_ID_TY_I64);
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "unable to create ast: %s", frg_status_to_string(result));
    }
};

ty_f32 : KW_F32
{
    frg_status_t result = frg_ast_new_ty_primary((frg_ast_ty_t**)&$$, FRG_AST_ID_TY_F32);
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "unable to create ast: %s", frg_status_to_string(result));
    }
};

ty_f64 : KW_F64
{
    frg_status_t result = frg_ast_new_ty_primary((frg_ast_ty_t**)&$$, FRG_AST_ID_TY_F64);
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
    $$ = NULL;
    g_list_append($$, $1);
}
| decl_union_list BIT_OR ty
{
    g_list_append($1, $3);
    $$ = $1;
};

decl_union : KW_TY SYMBOL ASSIGN decl_union_list SEMICOLON
{
    frg_status_t result = frg_ast_new_decl_union((frg_ast_decl_union_t**)&$$, $2, $4);
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_ERROR, "unable to create ast: %s", frg_status_to_string(result));
    }
};

decl_prop_non_const : SYMBOL
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

decl_prop : decl_prop_non_const
{
    $$ = $1;
}
| KW_CONST decl_prop_non_const
{
    $2->flags |= FRG_AST_DECL_PROP_FLAG_CONST;
    $$ = prop;
}

decl_struct_list : decl_prop SEMICOLON
{
    $$ = NULL;
    g_list_append($$, $1);
}
| decl_struct_list decl_prop SEMICOLON
{
    g_list_append($1, $2);
    $$ = $1;
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
    $$ = NULL;
    g_list_append($$, $1);
}
| decl_iface_extends_list COMMA ty
{
    g_list_append($1, $3);
    $$ = $1;
};

decl_iface_optional_extends : 
{
    $$ = NULL;
}
| COLON decl_iface_extends_list
{
    $$ = $2;
};

decl_iface_list_element : KW_PROP decl_prop_non_const {
    $$ = NULL;
};

decl_iface_list : decl_iface_list_element
{
    $$ = NULL;
    g_list_append($$, $1);
}
| decl_iface_list decl_iface_list_element
{
    g_list_append($1, $2);
    $$ = $1;
};

decl_iface : KW_IFACE SYMBOL decl_iface_optional_extends CURLY_BRACE_LEFT decl_iface_list CURLY_BRACE_RIGHT
{
    frg_status_t result = frg_ast_new_decl_iface((frg_ast_decl_iface_t**)&$$, $2, $3, $5);
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_ERROR, "unable to create ast: %s", frg_status_to_string(result));
    }
};

/* decl_fn : KW_FN SYMBOL PAREN_LEFT decl_fn_arg_list PAREN_RIGHT stmt_block
{
    frg_status_t result = frg_ast_new_decl_fn((frg_ast_decl_fn_t**)&$$, $2, NULL, NULL);
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_ERROR, "unable to create ast: %s", frg_status_to_string(result));
    }
}
| KW_FN SYMBOL PAREN_LEFT decl_fn_arg_list PAREN_RIGHT SEMICOLON
{
    frg_status_t result = frg_ast_new_decl_fn((frg_ast_decl_fn_t**)&$$, $2, NULL, NULL);
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_ERROR, "unable to create ast: %s", frg_status_to_string(result));
    }
} */

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
| SEMICOLON
{
    $$ = NULL;
};

stmt_list : stmt
{
    $$ = NULL;
    g_list_append($$, $1);
}
| stmt_list stmt
{
    g_list_append($1, $2);
    $$ = $1;
};

stmt_block : CURLY_BRACE_LEFT CURLY_BRACE_RIGHT
{
    $$ = NULL;
}
| CURLY_BRACE_LEFT stmt_list CURLY_BRACE_RIGHT
{
    frg_status_t result = frg_ast_new_stmt_block((frg_ast_t**)&$$, $2);
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

value_access : value_primary DOT value_symbol
{
    frg_status_t result = frg_ast_new_value_access((frg_ast_value_access_t**)&$$, $1, $3);
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_ERROR, "unable to create ast: %s", frg_status_to_string(result));
    }
}
| value_primary
{
    $$ = $1;
};

value_bit_not : BIT_NOT value_access
{
    frg_status_t result = frg_ast_new_value_unary((frg_ast_value_unary_t**)&$$, FRG_AST_ID_VALUE_BIT_NOT, $2);
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_ERROR, "unable to create ast: %s", frg_status_to_string(result));
    }
}
| value_access
{
    $$ = $1;
};

/* value_bit_shift : value_bit_not BIT_SHL value_bit_shift
{
    frg_status_t result = frg_ast_new_value_binary((frg_ast_value_binary_t**)&$$, FRG_AST_ID_VALUE_BIT_OR, $1, $3);
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_ERROR, "unable to create ast: %s", frg_status_to_string(result));
    }
}
| value_bit_and
{
    $$ = $1;
}; */

value_bit_and : value_bit_not BIT_AND value_bit_and
{
    frg_status_t result = frg_ast_new_value_binary((frg_ast_value_binary_t**)&$$, FRG_AST_ID_VALUE_BIT_AND, $1, $3);
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_ERROR, "unable to create ast: %s", frg_status_to_string(result));
    }
}
| value_bit_not
{
    $$ = $1;
};

value_bit_or : value_bit_and BIT_OR value_bit_or
{
    frg_status_t result = frg_ast_new_value_binary((frg_ast_value_binary_t**)&$$, FRG_AST_ID_VALUE_BIT_OR, $1, $3);
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_ERROR, "unable to create ast: %s", frg_status_to_string(result));
    }
}
| value_bit_and
{
    $$ = $1;
};

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
};

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
};

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
};

value : value_log_or
{
    *ast = $1;
};
%%
