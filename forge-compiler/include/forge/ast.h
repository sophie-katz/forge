#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <glib.h>
#include <forge/enums.h>

typedef struct {
    frg_ast_id_t id;
    void* data;
} frg_ast_t;

void frg_ast_destroy(frg_ast_t** ast);
frg_ast_t* frg_ast_new_value_true(void);
frg_ast_t* frg_ast_new_value_false(void);

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

typedef struct {
    frg_ast_iface_flags_t flags;
    GString* name;
    GList* extends;
    GList* props;
    GList* fns;
} frg_ast_iface_t;

typedef struct {
    frg_ast_fn_arg_flags_t flags;
    frg_ast_t* prop;
    frg_ast_t* default_value;
} frg_ast_fn_arg_t;

typedef struct {
    frg_ast_fn_flags_t flags;
    GString* name;
    GList* args;
    frg_ast_t* var_pos_args;
    frg_ast_t* var_kw_args;
    frg_ast_t* body;
} frg_ast_fn_t;

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
