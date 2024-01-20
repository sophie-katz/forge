// Copyright (c) 2023-2024 Sophie Katz
//
// This file is part of Forge.
//
// Forge is free software: you can redistribute it and/or modify it under the terms of
// the GNU General Public License as published by the Free Software Foundation, either
// version 3 of the License, or (at your option) any later version.
//
// Forge is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
// PURPOSE. See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with Forge.
// If not, see <https://www.gnu.org/licenses/>.

#include <forge/ast/ast.h>
#include <unity.h>

void setUp(void) {}

void tearDown(void) {}

void test_decl_get_name_union(void) {
    GList* props = g_list_append(NULL, frg_ast_new_decl_prop(
        FRG_AST_DECL_PROP_FLAG_NONE,
        g_string_new("asdf"),
        frg_ast_new_ty_primary(FRG_AST_ID_TY_I32)
    ));

    frg_ast_decl_union_t* ast = frg_ast_new_decl_union(g_string_new("Asdf"), props);

    TEST_ASSERT_EQUAL_STRING("Asdf", frg_ast_decl_get_name((const frg_ast_t*)ast));

    frg_ast_destroy((frg_ast_t**)&ast);
}

void test_decl_get_name_struct(void) {
    frg_ast_decl_struct_t* ast = frg_ast_new_decl_struct(g_string_new("Asdf"), NULL);

    TEST_ASSERT_EQUAL_STRING("Asdf", frg_ast_decl_get_name((const frg_ast_t*)ast));

    frg_ast_destroy((frg_ast_t**)&ast);
}

void test_decl_get_name_prop(void) {
    frg_ast_decl_prop_t* ast = frg_ast_new_decl_prop(
        FRG_AST_DECL_PROP_FLAG_NONE,
        g_string_new("asdf"),
        frg_ast_new_ty_primary(FRG_AST_ID_TY_I32)
    );

    TEST_ASSERT_EQUAL_STRING("asdf", frg_ast_decl_get_name((const frg_ast_t*)ast));

    frg_ast_destroy((frg_ast_t**)&ast);
}

void test_decl_get_name_iface(void) {
    frg_ast_decl_iface_t* ast = frg_ast_new_decl_iface(
        FRG_AST_DECL_IFACE_FLAG_NONE,
        g_string_new("Asdf"),
        NULL,
        NULL
    );

    TEST_ASSERT_EQUAL_STRING("Asdf", frg_ast_decl_get_name((const frg_ast_t*)ast));

    frg_ast_destroy((frg_ast_t**)&ast);
}

void test_decl_get_name_fn_arg(void) {
    frg_ast_decl_fn_arg_t* ast = frg_ast_new_decl_fn_arg(
        FRG_AST_DECL_FN_ARG_FLAG_NONE,
        frg_ast_new_decl_prop(
            FRG_AST_DECL_PROP_FLAG_NONE,
            g_string_new("asdf"),
            frg_ast_new_ty_primary(FRG_AST_ID_TY_I32)
        ),
        NULL
    );

    TEST_ASSERT_EQUAL_STRING("asdf", frg_ast_decl_get_name((const frg_ast_t*)ast));

    frg_ast_destroy((frg_ast_t**)&ast);
}

void test_decl_get_name_fn(void) {
    frg_ast_decl_fn_t* ast = frg_ast_new_decl_fn(
        FRG_AST_DECL_FN_FLAG_NONE,
        g_string_new("asdf"),
        frg_ast_new_ty_fn(
            NULL,
            NULL,
            NULL,
            NULL
        ),
        NULL
    );

    TEST_ASSERT_EQUAL_STRING("asdf", frg_ast_decl_get_name((const frg_ast_t*)ast));

    frg_ast_destroy((frg_ast_t**)&ast);
}

void test_decl_get_name_var(void) {
    frg_ast_decl_var_t* ast = frg_ast_new_decl_var(
        frg_ast_new_decl_prop(
            FRG_AST_DECL_PROP_FLAG_NONE,
            g_string_new("asdf"),
            frg_ast_new_ty_primary(FRG_AST_ID_TY_I32)
        ),
        NULL
    );

    TEST_ASSERT_EQUAL_STRING("asdf", frg_ast_decl_get_name((const frg_ast_t*)ast));

    frg_ast_destroy((frg_ast_t**)&ast);
}

void test_decl_get_name_non_decl(void) {
    frg_ast_ty_symbol_t* ast = frg_ast_new_ty_symbol(
        g_string_new("asdf")
    );

    TEST_ASSERT_NULL(frg_ast_decl_get_name((const frg_ast_t*)ast));

    frg_ast_destroy((frg_ast_t**)&ast);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_decl_get_name_union);
    RUN_TEST(test_decl_get_name_struct);
    RUN_TEST(test_decl_get_name_prop);
    RUN_TEST(test_decl_get_name_iface);
    RUN_TEST(test_decl_get_name_fn_arg);
    RUN_TEST(test_decl_get_name_fn);
    RUN_TEST(test_decl_get_name_var);
    RUN_TEST(test_decl_get_name_non_decl);
    return UNITY_END();
}
