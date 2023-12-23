#pragma once

#include <forge/ast/ast.h>
#include <forge/common/enums.h>
#include <stdio.h>

typedef union {
    bool as_bool;
} frg_value_data_t;

typedef struct {
    frg_exec_type_id_t type_id;
    frg_value_data_t data;
} frg_value_t;

frg_value_t* frg_value_new_bool(bool data);
void frg_value_destroy(frg_value_t** value);

bool frg_value_is_bool(const frg_value_t* value);
bool frg_value_get_bool(const frg_value_t* value);
void frg_value_set_bool(frg_value_t* value, bool data);

void frg_value_print(FILE* file, const frg_value_t* value);

frg_value_t* frg_exec(const frg_ast_t* ast);
