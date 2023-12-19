#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <glib.h>

typedef uint8_t frg_ast_id_t;

#define FRG_AST_ID_PROP 0
#define FRG_AST_ID_TY_BOOL 1
#define FRG_AST_ID_TY_U8 2
#define FRG_AST_ID_TY_U16 3
#define FRG_AST_ID_TY_U32 4
#define FRG_AST_ID_TY_U64 5
#define FRG_AST_ID_TY_I8 6
#define FRG_AST_ID_TY_I16 7
#define FRG_AST_ID_TY_I32 8
#define FRG_AST_ID_TY_I64 9
#define FRG_AST_ID_TY_F32 10
#define FRG_AST_ID_TY_F64 11
#define FRG_AST_ID_TY_ALIAS 12
#define FRG_AST_ID_TY_UNION 13
#define FRG_AST_ID_TY_STRUCT 14
#define FRG_AST_ID_IFACE 15
#define FRG_AST_ID_FN_ARG 16
#define FRG_AST_ID_FN 17
#define FRG_AST_ID_VAR 18
#define FRG_AST_ID_RETURN 19
#define FRG_AST_ID_WHILE 20
#define FRG_AST_ID_VALUE_TRUE 21
#define FRG_AST_ID_VALUE_FALSE 22
#define FRG_AST_ID_VALUE_INT 23
#define FRG_AST_ID_VALUE_FLOAT 24
#define FRG_AST_ID_VALUE_CHAR 25
#define FRG_AST_ID_VALUE_STR 26
#define FRG_AST_ID_VALUE_BIT_NEG 27
#define FRG_AST_ID_VALUE_BIT_AND 28
#define FRG_AST_ID_VALUE_BIT_OR 29
#define FRG_AST_ID_VALUE_BIT_XOR 30
#define FRG_AST_ID_VALUE_BIT_SHL 31
#define FRG_AST_ID_VALUE_BIT_SHR 32
#define FRG_AST_ID_VALUE_NEG 33
#define FRG_AST_ID_VALUE_ADD 34
#define FRG_AST_ID_VALUE_SUB 35
#define FRG_AST_ID_VALUE_MUL 36
#define FRG_AST_ID_VALUE_DIV 37
#define FRG_AST_ID_VALUE_DIV_INT 38
#define FRG_AST_ID_VALUE_MOD 39
#define FRG_AST_ID_VALUE_EXP 40
#define FRG_AST_ID_VALUE_EQ 41
#define FRG_AST_ID_VALUE_NE 42
#define FRG_AST_ID_VALUE_LT 43
#define FRG_AST_ID_VALUE_LE 44
#define FRG_AST_ID_VALUE_GT 45
#define FRG_AST_ID_VALUE_GE 46
#define FRG_AST_ID_VALUE_LOG_NOT 47
#define FRG_AST_ID_VALUE_LOG_AND 48
#define FRG_AST_ID_VALUE_LOG_OR 49
#define FRG_AST_ID_VALUE_FOR 50
#define FRG_AST_ID_VALUE_IF 51
#define FRG_AST_ID_VALUE_MATCH 52

typedef struct {
    frg_ast_id_t id;
    void* data;
} frg_ast_t;

void frg_ast_destroy(frg_ast_t** ast);
frg_ast_t* frg_ast_new_value_true(void);
frg_ast_t* frg_ast_new_value_false(void);

typedef uint8_t frg_ast_prop_flags_t;

#define FRG_AST_PROP_FLAG_NONE 0x0
#define FRG_AST_PROP_FLAG_OPTIONAL 0x1
#define FRG_AST_PROP_FLAG_NON_OPTIONAL 0x2
#define FRG_AST_PROP_FLAG_SPREAD 0x4
#define FRG_AST_PROP_FLAG_CONST 0x8

typedef struct {
    frg_ast_prop_flags_t flags;
    GString* name;
    frg_ast_t* type;
} frg_ast_prop_t;

typedef struct {
    GString* name;
    frg_ast_t* target;
} frg_ast_ty_alias_t;

typedef struct {
    GString* name;
    GList* props;
} frg_ast_ty_union_t;

typedef struct {
    GString* name;
    GList* props;
    GList* fns;
} frg_ast_ty_struct_t;

typedef uint8_t frg_ast_iface_flags_t;

#define FRG_AST_IFACE_FLAG_NONE 0x0
#define FRG_AST_IFACE_FLAG_ABSTRACT 0x1

typedef struct {
    frg_ast_iface_flags_t flags;
    GString* name;
    GList* extends;
    GList* props;
    GList* fns;
} frg_ast_iface_t;

typedef uint8_t frg_ast_fn_arg_flags_t;

#define FRG_AST_FN_ARG_FLAG_NONE 0x0
#define FRG_AST_FN_ARG_FLAG_CONST 0x1
#define FRG_AST_FN_ARG_FLAG_MUT 0x2
#define FRG_AST_FN_ARG_FLAG_KW 0x4

typedef struct {
    frg_ast_fn_arg_flags_t flags;
    frg_ast_t* prop;
    frg_ast_t* default_value;
} frg_ast_fn_arg_t;

typedef uint8_t frg_ast_fn_flags_t;

#define FRG_AST_FN_FLAG_NONE 0x0
#define FRG_AST_FN_FLAG_CONST 0x1
#define FRG_AST_FN_FLAG_MUT 0x2
#define FRG_AST_FN_FLAG_OVERRIDE 0x4

typedef struct {
    frg_ast_fn_flags_t flags;
    GString* name;
    GList* args;
    frg_ast_t* var_pos_args;
    frg_ast_t* var_kw_args;
    frg_ast_t* body;
} frg_ast_fn_t;

typedef uint8_t frg_ast_var_flags_t;

#define FRG_AST_VAR_FLAG_NONE 0x0
#define FRG_AST_VAR_FLAG_CONST 0x1
#define FRG_AST_VAR_FLAG_MUT 0x2

typedef struct {
    frg_ast_var_flags_t flags;
    frg_ast_t* prop;
    frg_ast_t* initial_value;
} frg_ast_var_t;

typedef struct {
    frg_ast_t* operand;
} frg_ast_value_unary_t;

frg_ast_t* frg_ast_new_value_log_not(frg_ast_t* operand);
void frg_ast_value_unary_destroy(frg_ast_value_unary_t** ast_value_unary);
frg_ast_value_unary_t* frg_ast_as_value_unary(frg_ast_t* ast);

typedef struct {
    frg_ast_t* lhs;
    frg_ast_t* rhs;
} frg_ast_value_binary_t;

frg_ast_t* frg_ast_new_value_log_and(frg_ast_t* lhs, frg_ast_t* rhs);
frg_ast_t* frg_ast_new_value_log_or(frg_ast_t* lhs, frg_ast_t* rhs);
void frg_ast_value_binary_destroy(frg_ast_value_binary_t** ast_value_binary);
frg_ast_value_binary_t* frg_ast_as_value_binary(frg_ast_t* ast);
