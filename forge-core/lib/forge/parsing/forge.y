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
%parse-param { frg_ast_node_t** node }

%{
#include <forge/ast/node.h>
#include <forge/types.h>
#include <forge/lexical_casts.h>
#include <forge/messages/message_buffer.h>
#include <forge/parsing/unions.h>
#include <stdio.h>

#define YYERROR_VERBOSE 1

extern int yylineno;
extern frg_column_number_t yycolumnno;
extern int yylex();

extern const char* _frg_parsing_current_path;
extern size_t _frg_parsing_current_offset;
extern frg_message_buffer_t* _frg_parsing_current_message_buffer;

int yywrap() {
    return 1;
}

void yyerror(frg_ast_node_t**ast, const char* message) {
    // frg_parsing_range_t source_range = {
    //     .start = {
    //         .path = _frg_parsing_current_path,
    //         .offset = _frg_parsing_current_offset,
    //         .line_number = yylineno,
    //         .column_number = yycolumnno,
    //     },
    //     .length = 1
    // };

    // frg_log_error_at_source_character(_frg_parsing_current_path, yylineno, yycolumnno, "[%s]", message);
    printf("YYERROR: %s\n", message);
}
%}

%union {
    frg_parsing_range_t source_range;
    frg_parsing_union_symbol_t value_symbol;
    frg_parsing_union_uint_t value_int;
    frg_parsing_union_float_t value_float;
    // frg_character_t value_character;
    // GString* value_string;
    void* node;
    GList* list;
}

%token <source_range> KEYWORD_VOID "void"
%token <source_range> KEYWORD_BOOL "bool"
%token <source_range> KEYWORD_U8 "u8"
%token <source_range> KEYWORD_U16 "u16"
%token <source_range> KEYWORD_U32 "u32"
%token <source_range> KEYWORD_U64 "u64"
%token <source_range> KEYWORD_I8 "i8"
%token <source_range> KEYWORD_I16 "i16"
%token <source_range> KEYWORD_I32 "i32"
%token <source_range> KEYWORD_I64 "i64"
%token <source_range> KEYWORD_F32 "f32"
%token <source_range> KEYWORD_F64 "f64"
%token <source_range> KEYWORD_TY "ty"
%token <source_range> KEYWORD_PROP "prop"
%token <source_range> KEYWORD_IFACE "interface"
%token <source_range> KEYWORD_KW "kw"
%token <source_range> KEYWORD_FN "fn"
%token <source_range> KEYWORD_CONST "const"
%token <source_range> KEYWORD_MUT "mut"
%token <source_range> KEYWORD_OVERRIDE "override"
%token <source_range> KEYWORD_LET "let"
%token <source_range> KEYWORD_RETURN "return"
%token <source_range> KEYWORD_IF "if"
%token <source_range> KEYWORD_ELIF "elif"
%token <source_range> KEYWORD_ELSE "else"
%token <source_range> KEYWORD_WHILE "while"
%token <source_range> KEYWORD_TRUE "true"
%token <source_range> KEYWORD_FALSE "false"
%token <source_range> KEYWORD_AS "as"
%token <value_symbol> SYMBOL "symbol"
%token <source_range> CURLY_BRACE_LEFT "{"
%token <source_range> CURLY_BRACE_RIGHT "}"
%token <source_range> SQUARE_BRACKET_LEFT "["
%token <source_range> SQUARE_BRACKET_RIGHT "]"
%token <source_range> PARENTHESIS_LEFT "("
%token <source_range> PARENTHESIS_RIGHT ")"
%token <source_range> COMMA ","
%token <source_range> COLON ":"
%token <source_range> SEMICOLON ";"
%token <source_range> QUESTION_MARK "?"
%token <source_range> ARROW_RIGHT "->"
%token <source_range> ELIPSIS "..."
%token <value_int> INT "integer literal"
%token <value_float> FLOAT "float literal"
%token CHARACTER "char literal"
%token STRING "string literal"
%token <source_range> DOT "."
%token <source_range> BIT_NOT "~"
%token <source_range> BIT_AND "&"
%token <source_range> BIT_OR "|"
%token <source_range> BIT_XOR "^"
%token <source_range> BIT_SHIFT_LEFT "<<"
%token <source_range> BIT_SHIFT_RIGHT ">>"
%token <source_range> ADD "+"
%token <source_range> SUBTRACT "-"
%token <source_range> MULTIPLY "*"
%token <source_range> DIVIDE "/"
%token <source_range> DIVIDE_INT "//"
%token <source_range> MODULO "%"
%token <source_range> EXPONENTIATE "**"
%token <source_range> EQUALS "=="
%token <source_range> NOT_EQUALS "!="
%token <source_range> IS_LESS_THAN "<"
%token <source_range> IS_LESS_THAN_OR_EQUAL_TO "<="
%token <source_range> IS_GREATER_THAN ">"
%token <source_range> IS_GREATER_THAN_OR_EQUAL_TO ">="
%token <source_range> LOGICAL_NOT "!"
%token <source_range> LOGICAL_AND "&&"
%token <source_range> LOGICAL_OR "||"
%token <source_range> ASSIGN "="
%token <source_range> BIT_AND_ASSIGN "&="
%token <source_range> BIT_OR_ASSIGN "|="
%token <source_range> BIT_XOR_ASSIGN "^="
%token <source_range> BIT_SHIFT_LEFT_ASSIGN "<<="
%token <source_range> BIT_SHIFT_RIGHT_ASSIGN ">>="
%token <source_range> ADD_ASSIGN "+="
%token <source_range> INCREMENT "++"
%token <source_range> SUBTRACT_ASSIGN "-="
%token <source_range> DECREMENT "--"
%token <source_range> MULTIPLY_ASSIGN "*="
%token <source_range> DIVIDE_ASSIGN "/="
%token <source_range> DIVIDE_INT_ASSIGN "//="
%token <source_range> MODULO_ASSIGN "%="
%token <source_range> EXPONENTIATE_ASSIGN "**="
%token <source_range> LOGICAL_AND_ASSIGN "&&="
%token <source_range> LOGICAL_OR_ASSIGN "||="

%type <node> ty_void
%type <node> ty_bool
%type <node> ty_u8
%type <node> ty_u16
%type <node> ty_u32
%type <node> ty_u64
%type <node> ty_i8
%type <node> ty_i16
%type <node> ty_i32
%type <node> ty_i64
%type <node> ty_f32
%type <node> ty_f64
%type <node> ty_symbol
%type <node> ty_primary
%type <node> ty_pointer
%type <node> ty_array
%type <node> ty_function
%type <node> ty
%type <list> ty_list
%type <list> ty_list_optional
%type <list> decl_union_list
%type <node> decl_union
%type <node> decl_prop
%type <list> decl_structure_list
%type <node> decl_structure
%type <list> decl_interface_extends_list
%type <list> decl_interface_optional_extends
%type <node> decl_interface_list_element
%type <list> decl_interface_list
%type <node> decl_interface
%type <node> decl_function_argument_default_value
%type <node> decl_function_arg
%type <list> decl_function_argument_list
%type <list> decl_function_argument_list_optional
%type <node> decl_function_return_ty
%type <node> decl_function
%type <node> decl_var
%type <node> decl
%type <list> decl_list
%type <list> decl_list_optional
%type <node> decl_block
%type <node> statement_return
%type <node> statement_if
%type <node> statement_while
%type <node> statement
%type <list> statement_list
%type <node> statement_block
%type <node> value_true
%type <node> value_false
%type <node> value_int
%type <node> value_float
%type <node> value_char
%type <node> value_str
%type <node> value_array
%type <node> value_array_repeated
%type <node> value_structure
%type <node> value_symbol
%type <node> value_primary
%type <node> value_deref_getaddr
%type <node> value_access
%type <node> value_call
%type <node> value_inc_dec
%type <node> value_exp
%type <node> value_neg_bit_not
%type <node> value_mul
%type <node> value_add
%type <node> value_bit_shift
%type <node> value_bit_and
%type <node> value_bit_xor
%type <node> value_bit_or
%type <node> value_cmp
%type <node> value_log_not
%type <node> value_log_and
%type <node> value_log_or
%type <node> value_assign
%type <node> value_cast
%type <node> value
%type <list> value_list
%type <list> value_list_optional
%type <node> top_level

%%
ty_void : KEYWORD_VOID
{
    $$ = frg_ast_node_type_primary_new(&$1, FRG_AST_NODE_KIND_TYPE_VOID);
};

ty_bool : KEYWORD_BOOL
{
    $$ = frg_ast_node_type_primary_new(&$1, FRG_AST_NODE_KIND_TYPE_BOOL);
};

ty_u8 : KEYWORD_U8
{
    $$ = frg_ast_node_type_int_new(&$1, FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED, 8);
};

ty_u16 : KEYWORD_U16
{
    $$ = frg_ast_node_type_int_new(&$1, FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED, 16);
};

ty_u32 : KEYWORD_U32
{
    $$ = frg_ast_node_type_int_new(&$1, FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED, 32);
};

ty_u64 : KEYWORD_U64
{
    $$ = frg_ast_node_type_int_new(&$1, FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED, 64);
};

ty_i8 : KEYWORD_I8
{
    $$ = frg_ast_node_type_int_new(&$1, FRG_AST_NODE_TYPE_INT_FLAG_NONE, 8);
};

ty_i16 : KEYWORD_I16
{
    $$ = frg_ast_node_type_int_new(&$1, FRG_AST_NODE_TYPE_INT_FLAG_NONE, 16);
};

ty_i32 : KEYWORD_I32
{
    $$ = frg_ast_node_type_int_new(&$1, FRG_AST_NODE_TYPE_INT_FLAG_NONE, 32);
};

ty_i64 : KEYWORD_I64
{
    $$ = frg_ast_node_type_int_new(&$1, FRG_AST_NODE_TYPE_INT_FLAG_NONE, 64);
};

ty_f32 : KEYWORD_F32
{
    $$ = frg_ast_node_type_float_new(&$1, 32);
};

ty_f64 : KEYWORD_F64
{
    $$ = frg_ast_node_type_float_new(&$1, 64);
};

ty_symbol : SYMBOL
{
    $$ = frg_ast_node_type_symbol_new(&$1.source_range, $1.value);
};

ty_array: SQUARE_BRACKET_LEFT INT COLON ty SQUARE_BRACKET_RIGHT
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &$1,
        &$5
    );

    $$ = frg_ast_node_type_array_new(&source_range, $2.value.value, $4);
};

ty_primary : ty_void
{
    $$ = $1;
}
| ty_bool
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
}
| ty_array
{
    $$ = $1;
};

ty_pointer: MULTIPLY ty_pointer
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &$1,
        &((frg_ast_node_t*)$2)->source_range
    );

    $$ = frg_ast_node_type_pointer_new(
        &source_range,
        FRG_AST_NODE_TYPE_POINTER_FLAG_NONE,
        $2
    );
}
| BIT_AND ty_pointer
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &$1,
        &((frg_ast_node_t*)$2)->source_range
    );

    $$ = frg_ast_node_type_pointer_new(
        &source_range,
        FRG_AST_NODE_TYPE_POINTER_FLAG_IMPLICIT_DEREFERENCE,
        $2
    );
}
| MULTIPLY KEYWORD_CONST ty_pointer
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &$1,
        &((frg_ast_node_t*)$3)->source_range
    );

    $$ = frg_ast_node_type_pointer_new(
        &source_range,
        FRG_AST_NODE_TYPE_POINTER_FLAG_CONSTANT,
        $3
    );
}
| BIT_AND KEYWORD_CONST ty_pointer
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &$1,
        &((frg_ast_node_t*)$3)->source_range
    );

    $$ = frg_ast_node_type_pointer_new(
        &source_range,
        FRG_AST_NODE_TYPE_POINTER_FLAG_CONSTANT
            | FRG_AST_NODE_TYPE_POINTER_FLAG_IMPLICIT_DEREFERENCE,
        $3
    );
}
| ty_primary
{
    $$ = $1;
};

ty_function: PARENTHESIS_LEFT ty_list_optional PARENTHESIS_RIGHT ARROW_RIGHT ty_pointer
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &$1,
        &((frg_ast_node_t*)$5)->source_range
    );

    $$ = frg_ast_node_type_function_new(
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

ty : ty_function
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

decl_union : KEYWORD_TY SYMBOL ASSIGN decl_union_list SEMICOLON
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &$1,
        &$5
    );

    $$ = frg_ast_node_declaration_union_new(&source_range, $2.value, $4);
};

decl_prop : SYMBOL
{
    $$ = frg_ast_node_declaration_property_new(&$1.source_range, FRG_AST_NODE_DECLARATION_PROPERTY_FLAG_NONE, $1.value, NULL);
}
| SYMBOL QUESTION_MARK
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &$1.source_range,
        &$2
    );

    $$ = frg_ast_node_declaration_property_new(&source_range, FRG_AST_NODE_DECLARATION_PROPERTY_FLAG_OPTIONAL, $1.value, NULL);
}
| SYMBOL LOGICAL_NOT
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &$1.source_range,
        &$2
    );

    $$ = frg_ast_node_declaration_property_new(&source_range, FRG_AST_NODE_DECLARATION_PROPERTY_FLAG_NON_OPTIONAL, $1.value, NULL);
}
| ELIPSIS SYMBOL
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &$1,
        &$2.source_range
    );

    $$ = frg_ast_node_declaration_property_new(&source_range, FRG_AST_NODE_DECLARATION_PROPERTY_FLAG_SPREAD, $2.value, NULL);
}
| SYMBOL COLON ty
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &$1.source_range,
        &((frg_ast_node_t*)$3)->source_range
    );

    $$ = frg_ast_node_declaration_property_new(&source_range, FRG_AST_NODE_DECLARATION_PROPERTY_FLAG_NONE, $1.value, $3);
}
| SYMBOL QUESTION_MARK COLON ty
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &$1.source_range,
        &((frg_ast_node_t*)$4)->source_range
    );

    $$ = frg_ast_node_declaration_property_new(&source_range, FRG_AST_NODE_DECLARATION_PROPERTY_FLAG_OPTIONAL, $1.value, $4);
};

decl_structure_list : decl_prop SEMICOLON
{
    $$ = g_list_append(NULL, $1);
}
| decl_structure_list decl_prop SEMICOLON
{
    $$ = g_list_append($1, $2);
};

decl_structure : KEYWORD_TY SYMBOL CURLY_BRACE_LEFT decl_structure_list CURLY_BRACE_RIGHT
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &$1,
        &$5
    );

    $$ = frg_ast_node_declaration_structure_new(&source_range, $2.value, $4);
};

decl_interface_extends_list : ty
{
    $$ = g_list_append(NULL, $1);
}
| decl_interface_extends_list COMMA ty
{
    $$ = g_list_append($1, $3);
};

decl_interface_optional_extends : 
{
    $$ = NULL;
}
| COLON decl_interface_extends_list
{
    $$ = $2;
};

decl_interface_list_element : KEYWORD_PROP decl_prop {
    $$ = NULL;
};

decl_interface_list : decl_interface_list_element
{
    $$ = g_list_append(NULL, $1);
}
| decl_interface_list decl_interface_list_element
{
    $$ = g_list_append($1, $2);
};

decl_interface : KEYWORD_IFACE SYMBOL decl_interface_optional_extends CURLY_BRACE_LEFT decl_interface_list CURLY_BRACE_RIGHT
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &$1,
        &$6
    );

    $$ = frg_ast_node_declaration_interface_new(&source_range, FRG_AST_NODE_DECLARATION_INTERFACE_FLAG_NONE, $2.value, $3, $5);
};

decl_function_argument_default_value: ASSIGN value
{
    $$ = $2;
}
|
{
    $$ = NULL;
};

decl_function_arg: decl_prop decl_function_argument_default_value
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &((frg_ast_node_t*)$1)->source_range,
        &((frg_ast_node_t*)$2)->source_range
    );

    $$ = frg_ast_node_declaration_function_argument_new(&source_range, FRG_AST_NODE_DECLARATION_FUNCTION_ARGUMENT_FLAG_NONE, $1, $2);
}
| KEYWORD_KW decl_prop decl_function_argument_default_value
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &$1,
        &((frg_ast_node_t*)$3)->source_range
    );

    $$ = frg_ast_node_declaration_function_argument_new(&source_range, FRG_AST_NODE_DECLARATION_FUNCTION_ARGUMENT_FLAG_KEYWORD, $2, $3);
};

decl_function_argument_list: decl_function_arg
{
    $$ = g_list_append(NULL, $1);
}
| decl_function_argument_list COMMA decl_function_arg
{
    $$ = g_list_append($1, $3);
};

decl_function_argument_list_optional: decl_function_argument_list
{
    $$ = $1;
}
|
{
    $$ = NULL;
};

decl_function_return_ty : ARROW_RIGHT ty
{
    $$ = $2;
}
|
{
    $$ = NULL;
};

decl_function : KEYWORD_FN SYMBOL PARENTHESIS_LEFT decl_function_argument_list_optional PARENTHESIS_RIGHT decl_function_return_ty SEMICOLON
{
    frg_parsing_range_t source_range_ty = frg_parsing_range_get_span(
        &$3,
        $6 == NULL
            ? &$5
            : &((frg_ast_node_t*)$6)->source_range
    );

    frg_ast_node_type_function_t* ty_function = (frg_ast_node_type_function_t*)frg_ast_node_type_function_new(
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

    $$ = frg_ast_node_declaration_function_new(&source_range, FRG_AST_NODE_DECLARATION_FUNCTION_FLAG_NONE, $2.value, ty_function, NULL);
}
| KEYWORD_FN SYMBOL PARENTHESIS_LEFT decl_function_argument_list_optional PARENTHESIS_RIGHT decl_function_return_ty statement_block
{
    frg_parsing_range_t source_range_ty = frg_parsing_range_get_span(
        &$3,
        $6 == NULL
            ? &$5
            : &((frg_ast_node_t*)$6)->source_range
    );

    frg_ast_node_type_function_t* ty_function = (frg_ast_node_type_function_t*)frg_ast_node_type_function_new(
        &source_range_ty,
        $4,
        NULL,
        NULL,
        $6
    );

    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &$1,
        &((frg_ast_node_t*)$7)->source_range
    );

    $$ = frg_ast_node_declaration_function_new(&source_range, FRG_AST_NODE_DECLARATION_FUNCTION_FLAG_NONE, $2.value, ty_function, $7);
}
| KEYWORD_MUT KEYWORD_FN SYMBOL PARENTHESIS_LEFT decl_function_argument_list_optional PARENTHESIS_RIGHT decl_function_return_ty SEMICOLON
{
    frg_parsing_range_t source_range_ty = frg_parsing_range_get_span(
        &$4,
        $7 == NULL
            ? &$6
            : &((frg_ast_node_t*)$7)->source_range
    );

    frg_ast_node_type_function_t* ty_function = (frg_ast_node_type_function_t*)frg_ast_node_type_function_new(
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

    $$ = frg_ast_node_declaration_function_new(&source_range, FRG_AST_NODE_DECLARATION_FUNCTION_FLAG_MUTABLE, $3.value, ty_function, NULL);
}
| KEYWORD_MUT KEYWORD_FN SYMBOL PARENTHESIS_LEFT decl_function_argument_list_optional PARENTHESIS_RIGHT decl_function_return_ty statement_block
{
    frg_parsing_range_t source_range_ty = frg_parsing_range_get_span(
        &$4,
        $7 == NULL
            ? &$6
            : &((frg_ast_node_t*)$7)->source_range
    );

    frg_ast_node_type_function_t* ty_function = (frg_ast_node_type_function_t*)frg_ast_node_type_function_new(
        &source_range_ty,
        $5,
        NULL,
        NULL,
        $7
    );

    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &$1,
        &((frg_ast_node_t*)$8)->source_range
    );

    $$ = frg_ast_node_declaration_function_new(&source_range, FRG_AST_NODE_DECLARATION_FUNCTION_FLAG_MUTABLE, $3.value, ty_function, $8);
}
| KEYWORD_OVERRIDE KEYWORD_FN SYMBOL PARENTHESIS_LEFT decl_function_argument_list_optional PARENTHESIS_RIGHT decl_function_return_ty SEMICOLON
{
    frg_parsing_range_t source_range_ty = frg_parsing_range_get_span(
        &$4,
        $7 == NULL
            ? &$6
            : &((frg_ast_node_t*)$7)->source_range
    );

    frg_ast_node_type_function_t* ty_function = (frg_ast_node_type_function_t*)frg_ast_node_type_function_new(
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

    $$ = frg_ast_node_declaration_function_new(&source_range, FRG_AST_NODE_DECLARATION_FUNCTION_FLAG_OVERRIDE, $3.value, ty_function, NULL);
}
| KEYWORD_OVERRIDE KEYWORD_FN SYMBOL PARENTHESIS_LEFT decl_function_argument_list_optional PARENTHESIS_RIGHT decl_function_return_ty statement_block
{
    frg_parsing_range_t source_range_ty = frg_parsing_range_get_span(
        &$4,
        $7 == NULL
            ? &$6
            : &((frg_ast_node_t*)$7)->source_range
    );

    frg_ast_node_type_function_t* ty_function = (frg_ast_node_type_function_t*)frg_ast_node_type_function_new(
        &source_range_ty,
        $5,
        NULL,
        NULL,
        $7
    );

    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &$1,
        &((frg_ast_node_t*)$8)->source_range
    );

    $$ = frg_ast_node_declaration_function_new(&source_range, FRG_AST_NODE_DECLARATION_FUNCTION_FLAG_OVERRIDE, $3.value, ty_function, $8);
};

decl_var : KEYWORD_LET decl_prop SEMICOLON
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &$1,
        &$3
    );

    $$ = frg_ast_node_declaration_assignment_new(&source_range, $2, NULL);
}
| KEYWORD_LET decl_prop ASSIGN value SEMICOLON
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &$1,
        &$5
    );

    $$ = frg_ast_node_declaration_assignment_new(&source_range, $2, $4);
};

decl : decl_union
{
    $$ = $1;
}
| decl_structure
{
    $$ = $1;
}
| decl_interface
{
    $$ = $1;
}
| decl_function
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
    $$ = frg_ast_node_declaration_block_new(&frg_global_parsing_range_null, $1);
};

statement_return : KEYWORD_RETURN value SEMICOLON
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &$1,
        &$3
    );

    $$ = frg_ast_node_statement_return_new(&source_range, $2);
};

statement_if : KEYWORD_IF PARENTHESIS_LEFT value PARENTHESIS_RIGHT statement_block
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &$1,
        &((frg_ast_node_t*)$5)->source_range
    );

    GList* conditional_clauses = g_list_append(
        NULL,
        frg_ast_node_statement_if_conditional_clause_new(
            &source_range,
            $3,
            $5
        )
    );

    $$ = frg_ast_node_statement_if_new(&source_range, conditional_clauses, NULL);
}
| statement_if KEYWORD_ELIF PARENTHESIS_LEFT value PARENTHESIS_RIGHT statement_block
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &$2,
        &((frg_ast_node_t*)$6)->source_range
    );

    ((frg_ast_node_t*)$1)->source_range = frg_parsing_range_get_span(
        &((frg_ast_node_t*)$1)->source_range,
        &source_range
    );

    ((frg_ast_node_statement_if_t*)$1)->conditional_clauses = g_list_append(
        ((frg_ast_node_statement_if_t*)$1)->conditional_clauses,
        frg_ast_node_statement_if_conditional_clause_new(
            &source_range,
            $4,
            $6
        )
    );

    $$ = $1;
}
| statement_if KEYWORD_ELSE statement_block
{
    ((frg_ast_node_t*)$1)->source_range = frg_parsing_range_get_span(
        &((frg_ast_node_t*)$1)->source_range,
        &((frg_ast_node_t*)$3)->source_range
    );

    ((frg_ast_node_statement_if_t*)$1)->else_clause = $3;

    $$ = $1;
};

statement_while : KEYWORD_WHILE PARENTHESIS_LEFT value PARENTHESIS_RIGHT SEMICOLON
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &$1,
        &$5
    );

    $$ = frg_ast_node_statement_while_new(&source_range, $3, NULL);
}
| KEYWORD_WHILE PARENTHESIS_LEFT value PARENTHESIS_RIGHT statement_block
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &$1,
        &((frg_ast_node_t*)$5)->source_range
    );

    $$ = frg_ast_node_statement_while_new(&source_range, $3, $5);
};

statement : value SEMICOLON
{
    $$ = $1;
}
| statement_return
{
    $$ = $1;
}
| statement_if
{
    $$ = $1;
}
| statement_while
{
    $$ = $1;
}
| statement_block
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

statement_list : statement
{
    $$ = g_list_append(NULL, $1);
}
| statement_list statement
{
    $$ = g_list_append($1, $2);
};

statement_block : CURLY_BRACE_LEFT CURLY_BRACE_RIGHT
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &$1,
        &$2
    );

    $$ = frg_ast_node_statement_block_new(&source_range, NULL);
}
| CURLY_BRACE_LEFT statement_list CURLY_BRACE_RIGHT
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &$1,
        &$3
    );

    $$ = frg_ast_node_statement_block_new(&source_range, $2);
};

value_true : KEYWORD_TRUE
{
    $$ = frg_ast_node_value_bool_new(&$1, true);
};

value_false : KEYWORD_FALSE
{
    $$ = frg_ast_node_value_bool_new(&$1, false);
};

value_int : INT
{
    $$ = frg_ast_node_value_int_new_from_lexical_cast_result(&$1.source_range, &$1.value);
};

value_float : FLOAT
{
    $$ = frg_ast_node_value_float_new_from_lexical_cast_result(&$1.source_range, &$1.value);
};

value_char : CHARACTER
{
    // Lexer already emits an error for this
    $$ = NULL;
};

value_str : STRING
{
    // Lexer already emits an error for this
    $$ = NULL;
};

value_array : SQUARE_BRACKET_LEFT value_list_optional SQUARE_BRACKET_RIGHT
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &$1,
        &$3
    );

    $$ = frg_ast_node_value_array_new(&source_range, $2);
};

value_array_repeated : SQUARE_BRACKET_LEFT INT COLON value SQUARE_BRACKET_RIGHT
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &$1,
        &$5
    );

    $$ = frg_ast_node_value_array_repeated_new(&source_range, $2.value.value, $4);
};

/* value_structure : CURLY_BRACE_LEFT  */

value_symbol : SYMBOL
{
    $$ = frg_ast_node_value_symbol_new(&$1.source_range, $1.value);
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
| value_array
{
    $$ = $1;
}
| value_array_repeated
{
    $$ = $1;
}
| PARENTHESIS_LEFT value PARENTHESIS_RIGHT
{
    $$ = $2;
};

value_call : value_symbol PARENTHESIS_LEFT value_list_optional PARENTHESIS_RIGHT
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &((frg_ast_node_t*)$1)->source_range,
        &$4
    );

    $$ = frg_ast_node_value_call_new(&source_range, $1, $3, NULL);
}
| value_primary
{
    $$ = $1;
};

value_access : value_access DOT value_call
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &((frg_ast_node_t*)$1)->source_range,
        &((frg_ast_node_t*)$3)->source_range
    );

    $$ = frg_ast_node_value_binary_new(&source_range, FRG_AST_NODE_KIND_VALUE_ACCESS, $1, $3);
}
| value_call
{
    $$ = $1;
};

value_deref_getaddr: BIT_AND value_deref_getaddr
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &$1,
        &((frg_ast_node_t*)$2)->source_range
    );

    $$ = frg_ast_node_value_unary_new(&source_range, FRG_AST_NODE_KIND_VALUE_GET_ADDRESS, $2);
}
| MULTIPLY value_deref_getaddr
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &$1,
        &((frg_ast_node_t*)$2)->source_range
    );

    $$ = frg_ast_node_value_unary_new(&source_range, FRG_AST_NODE_KIND_VALUE_DEREFERENCE, $2);
}
| value_access
{
    $$ = $1;
};

value_inc_dec : value_deref_getaddr INCREMENT
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &((frg_ast_node_t*)$1)->source_range,
        &$2
    );

    $$ = frg_ast_node_value_unary_new(&source_range, FRG_AST_NODE_KIND_VALUE_INCREMENT, $1);
}
| value_deref_getaddr DECREMENT
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &((frg_ast_node_t*)$1)->source_range,
        &$2
    );

    $$ = frg_ast_node_value_unary_new(&source_range, FRG_AST_NODE_KIND_VALUE_DECREMENT, $1);
}
| value_deref_getaddr
{
    $$ = $1;
};

value_exp : value_inc_dec EXPONENTIATE value_exp
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &((frg_ast_node_t*)$1)->source_range,
        &((frg_ast_node_t*)$3)->source_range
    );

    $$ = frg_ast_node_value_binary_new(&source_range, FRG_AST_NODE_KIND_VALUE_EXPONENTIATE, $1, $3);
}
| value_inc_dec
{
    $$ = $1;
};

value_neg_bit_not : BIT_NOT value_neg_bit_not
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &$1,
        &((frg_ast_node_t*)$2)->source_range
    );

    $$ = frg_ast_node_value_unary_new(&source_range, FRG_AST_NODE_KIND_VALUE_BIT_NOT, $2);
}
| SUBTRACT value_neg_bit_not
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &$1,
        &((frg_ast_node_t*)$2)->source_range
    );

    $$ = frg_ast_node_value_unary_new(&source_range, FRG_AST_NODE_KIND_VALUE_NEGATE, $2);
}
| value_exp
{
    $$ = $1;
};

value_mul : value_neg_bit_not MULTIPLY value_mul
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &((frg_ast_node_t*)$1)->source_range,
        &((frg_ast_node_t*)$3)->source_range
    );

    $$ = frg_ast_node_value_binary_new(&source_range, FRG_AST_NODE_KIND_VALUE_MULTIPLY, $1, $3);
}
| value_neg_bit_not DIVIDE value_mul
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &((frg_ast_node_t*)$1)->source_range,
        &((frg_ast_node_t*)$3)->source_range
    );

    $$ = frg_ast_node_value_binary_new(&source_range, FRG_AST_NODE_KIND_VALUE_DIVIDE, $1, $3);
}
| value_neg_bit_not DIVIDE_INT value_mul
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &((frg_ast_node_t*)$1)->source_range,
        &((frg_ast_node_t*)$3)->source_range
    );

    $$ = frg_ast_node_value_binary_new(&source_range, FRG_AST_NODE_KIND_VALUE_DIVIDE_INT, $1, $3);
}
| value_neg_bit_not MODULO value_mul
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &((frg_ast_node_t*)$1)->source_range,
        &((frg_ast_node_t*)$3)->source_range
    );

    $$ = frg_ast_node_value_binary_new(&source_range, FRG_AST_NODE_KIND_VALUE_MODULO, $1, $3);
}
| value_neg_bit_not
{
    $$ = $1;
};

value_add : value_mul ADD value_add
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &((frg_ast_node_t*)$1)->source_range,
        &((frg_ast_node_t*)$3)->source_range
    );

    $$ = frg_ast_node_value_binary_new(&source_range, FRG_AST_NODE_KIND_VALUE_ADD, $1, $3);
}
| value_mul SUBTRACT value_add
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &((frg_ast_node_t*)$1)->source_range,
        &((frg_ast_node_t*)$3)->source_range
    );

    $$ = frg_ast_node_value_binary_new(&source_range, FRG_AST_NODE_KIND_VALUE_SUBTRACT, $1, $3);
}
| value_mul
{
    $$ = $1;
};

value_bit_shift : value_add BIT_SHIFT_LEFT value_bit_shift
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &((frg_ast_node_t*)$1)->source_range,
        &((frg_ast_node_t*)$3)->source_range
    );

    $$ = frg_ast_node_value_binary_new(&source_range, FRG_AST_NODE_KIND_VALUE_BIT_SHIFT_LEFT, $1, $3);
}
| value_add BIT_SHIFT_RIGHT value_bit_shift
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &((frg_ast_node_t*)$1)->source_range,
        &((frg_ast_node_t*)$3)->source_range
    );

    $$ = frg_ast_node_value_binary_new(&source_range, FRG_AST_NODE_KIND_VALUE_BIT_SHIFT_RIGHT, $1, $3);
}
| value_add
{
    $$ = $1;
};

value_bit_and : value_bit_shift BIT_AND value_bit_and
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &((frg_ast_node_t*)$1)->source_range,
        &((frg_ast_node_t*)$3)->source_range
    );

    $$ = frg_ast_node_value_binary_new(&source_range, FRG_AST_NODE_KIND_VALUE_BIT_AND, $1, $3);
}
| value_bit_shift
{
    $$ = $1;
};

value_bit_xor : value_bit_and BIT_XOR value_bit_xor
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &((frg_ast_node_t*)$1)->source_range,
        &((frg_ast_node_t*)$3)->source_range
    );

    $$ = frg_ast_node_value_binary_new(&source_range, FRG_AST_NODE_KIND_VALUE_BIT_XOR, $1, $3);
}
| value_bit_and
{
    $$ = $1;
};

value_bit_or : value_bit_xor BIT_OR value_bit_or
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &((frg_ast_node_t*)$1)->source_range,
        &((frg_ast_node_t*)$3)->source_range
    );

    $$ = frg_ast_node_value_binary_new(&source_range, FRG_AST_NODE_KIND_VALUE_BIT_OR, $1, $3);
}
| value_bit_xor
{
    $$ = $1;
};

value_cmp : value_bit_or EQUALS value_cmp
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &((frg_ast_node_t*)$1)->source_range,
        &((frg_ast_node_t*)$3)->source_range
    );

    $$ = frg_ast_node_value_binary_new(&source_range, FRG_AST_NODE_KIND_VALUE_EQUALS, $1, $3);
}
| value_bit_or NOT_EQUALS value_cmp
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &((frg_ast_node_t*)$1)->source_range,
        &((frg_ast_node_t*)$3)->source_range
    );

    $$ = frg_ast_node_value_binary_new(&source_range, FRG_AST_NODE_KIND_VALUE_NOT_EQUALS, $1, $3);
}
| value_bit_or IS_LESS_THAN value_cmp
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &((frg_ast_node_t*)$1)->source_range,
        &((frg_ast_node_t*)$3)->source_range
    );

    $$ = frg_ast_node_value_binary_new(&source_range, FRG_AST_NODE_KIND_VALUE_IS_LESS_THAN, $1, $3);
}
| value_bit_or IS_LESS_THAN_OR_EQUAL_TO value_cmp
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &((frg_ast_node_t*)$1)->source_range,
        &((frg_ast_node_t*)$3)->source_range
    );

    $$ = frg_ast_node_value_binary_new(&source_range, FRG_AST_NODE_KIND_VALUE_IS_LESS_THAN_OR_EQUAL_TO, $1, $3);
}
| value_bit_or IS_GREATER_THAN value_cmp
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &((frg_ast_node_t*)$1)->source_range,
        &((frg_ast_node_t*)$3)->source_range
    );

    $$ = frg_ast_node_value_binary_new(&source_range, FRG_AST_NODE_KIND_VALUE_IS_GREATER_THAN, $1, $3);
}
| value_bit_or IS_GREATER_THAN_OR_EQUAL_TO value_cmp
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &((frg_ast_node_t*)$1)->source_range,
        &((frg_ast_node_t*)$3)->source_range
    );

    $$ = frg_ast_node_value_binary_new(&source_range, FRG_AST_NODE_KIND_VALUE_IS_GREATER_THAN_OR_EQUAL_TO, $1, $3);
}
| value_bit_or
{
    $$ = $1;
};

value_log_not : LOGICAL_NOT value_cmp
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &$1,
        &((frg_ast_node_t*)$2)->source_range
    );

    $$ = frg_ast_node_value_unary_new(&source_range, FRG_AST_NODE_KIND_VALUE_LOGICAL_NOT, $2);
}
| value_cmp
{
    $$ = $1;
};

value_log_and : value_log_not LOGICAL_AND value_log_and
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &((frg_ast_node_t*)$1)->source_range,
        &((frg_ast_node_t*)$3)->source_range
    );

    $$ = frg_ast_node_value_binary_new(&source_range, FRG_AST_NODE_KIND_VALUE_LOGICAL_AND, $1, $3);
}
| value_log_not
{
    $$ = $1;
};

value_log_or : value_log_and LOGICAL_OR value_log_or
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &((frg_ast_node_t*)$1)->source_range,
        &((frg_ast_node_t*)$3)->source_range
    );

    $$ = frg_ast_node_value_binary_new(&source_range, FRG_AST_NODE_KIND_VALUE_LOGICAL_OR, $1, $3);
}
| value_log_and
{
    $$ = $1;
};

value_assign : value_log_or ASSIGN value_assign
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &((frg_ast_node_t*)$1)->source_range,
        &((frg_ast_node_t*)$3)->source_range
    );

    $$ = frg_ast_node_value_binary_new(&source_range, FRG_AST_NODE_KIND_VALUE_ASSIGN, $1, $3);
}
| value_log_or ADD_ASSIGN value_assign
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &((frg_ast_node_t*)$1)->source_range,
        &((frg_ast_node_t*)$3)->source_range
    );

    $$ = frg_ast_node_value_binary_new(&source_range, FRG_AST_NODE_KIND_VALUE_ADD_ASSIGN, $1, $3);
}
| value_log_or SUBTRACT_ASSIGN value_assign
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &((frg_ast_node_t*)$1)->source_range,
        &((frg_ast_node_t*)$3)->source_range
    );

    $$ = frg_ast_node_value_binary_new(&source_range, FRG_AST_NODE_KIND_VALUE_SUBTRACT_ASSIGN, $1, $3);
}
| value_log_or MULTIPLY_ASSIGN value_assign
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &((frg_ast_node_t*)$1)->source_range,
        &((frg_ast_node_t*)$3)->source_range
    );

    $$ = frg_ast_node_value_binary_new(&source_range, FRG_AST_NODE_KIND_VALUE_MULTIPLY_ASSIGN, $1, $3);
}
| value_log_or DIVIDE_ASSIGN value_assign
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &((frg_ast_node_t*)$1)->source_range,
        &((frg_ast_node_t*)$3)->source_range
    );

    $$ = frg_ast_node_value_binary_new(&source_range, FRG_AST_NODE_KIND_VALUE_DIVIDE_ASSIGN, $1, $3);
}
| value_log_or DIVIDE_INT_ASSIGN value_assign
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &((frg_ast_node_t*)$1)->source_range,
        &((frg_ast_node_t*)$3)->source_range
    );

    $$ = frg_ast_node_value_binary_new(&source_range, FRG_AST_NODE_KIND_VALUE_DIVIDE_INT_ASSIGN, $1, $3);
}
| value_log_or MODULO_ASSIGN value_assign
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &((frg_ast_node_t*)$1)->source_range,
        &((frg_ast_node_t*)$3)->source_range
    );

    $$ = frg_ast_node_value_binary_new(&source_range, FRG_AST_NODE_KIND_VALUE_MODULO_ASSIGN, $1, $3);
}
| value_log_or BIT_SHIFT_LEFT_ASSIGN value_assign
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &((frg_ast_node_t*)$1)->source_range,
        &((frg_ast_node_t*)$3)->source_range
    );

    $$ = frg_ast_node_value_binary_new(&source_range, FRG_AST_NODE_KIND_VALUE_BIT_SHIFT_LEFT_ASSIGN, $1, $3);
}
| value_log_or BIT_SHIFT_RIGHT_ASSIGN value_assign
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &((frg_ast_node_t*)$1)->source_range,
        &((frg_ast_node_t*)$3)->source_range
    );

    $$ = frg_ast_node_value_binary_new(&source_range, FRG_AST_NODE_KIND_VALUE_BIT_SHIFT_RIGHT_ASSIGN, $1, $3);
}
| value_log_or BIT_AND_ASSIGN value_assign
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &((frg_ast_node_t*)$1)->source_range,
        &((frg_ast_node_t*)$3)->source_range
    );

    $$ = frg_ast_node_value_binary_new(&source_range, FRG_AST_NODE_KIND_VALUE_BIT_AND_ASSIGN, $1, $3);
}
| value_log_or BIT_XOR_ASSIGN value_assign
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &((frg_ast_node_t*)$1)->source_range,
        &((frg_ast_node_t*)$3)->source_range
    );

    $$ = frg_ast_node_value_binary_new(&source_range, FRG_AST_NODE_KIND_VALUE_BIT_XOR_ASSIGN, $1, $3);
}
| value_log_or BIT_OR_ASSIGN value_assign
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &((frg_ast_node_t*)$1)->source_range,
        &((frg_ast_node_t*)$3)->source_range
    );

    $$ = frg_ast_node_value_binary_new(&source_range, FRG_AST_NODE_KIND_VALUE_BIT_OR_ASSIGN, $1, $3);
}
| value_log_or EXPONENTIATE_ASSIGN value_assign
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &((frg_ast_node_t*)$1)->source_range,
        &((frg_ast_node_t*)$3)->source_range
    );

    $$ = frg_ast_node_value_binary_new(&source_range, FRG_AST_NODE_KIND_VALUE_EXPONENTIATE_ASSIGN, $1, $3);
}
| value_log_or LOGICAL_AND_ASSIGN value_assign
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &((frg_ast_node_t*)$1)->source_range,
        &((frg_ast_node_t*)$3)->source_range
    );

    $$ = frg_ast_node_value_binary_new(&source_range, FRG_AST_NODE_KIND_VALUE_LOGICAL_AND_ASSIGN, $1, $3);
}
| value_log_or LOGICAL_OR_ASSIGN value_assign
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &((frg_ast_node_t*)$1)->source_range,
        &((frg_ast_node_t*)$3)->source_range
    );

    $$ = frg_ast_node_value_binary_new(&source_range, FRG_AST_NODE_KIND_VALUE_LOGICAL_OR_ASSIGN, $1, $3);
}
| value_log_or
{
    $$ = $1;
};

value_cast : value_assign KEYWORD_AS ty
{
    frg_parsing_range_t source_range = frg_parsing_range_get_span(
        &((frg_ast_node_t*)$1)->source_range,
        &((frg_ast_node_t*)$3)->source_range
    );

    $$ = frg_ast_node_value_cast_new(&source_range, $1, $3);
}
| value_assign
{
    $$ = $1;
};

value : value_cast
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
    $$ = *node = $1;
};
%%
