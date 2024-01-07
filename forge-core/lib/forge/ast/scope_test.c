// Copyright (c) 2024 Sophie Katz
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

#include <forge/ast/scope.h>
#include <unity.h>

void setUp(void) {}

void tearDown(void) {}

void test_frame(void) {
    frg_ast_scope_frame_t* frame = frg_ast_scope_frame_new();

    frg_ast_decl_prop_t* ast = frg_ast_new_decl_prop(
        FRG_AST_DECL_PROP_FLAG_NONE,
        g_string_new("Asdf"),
        NULL
    );

    int ir = 5;

    frg_ast_scope_frame_add_ast(
        frame,
        (frg_ast_t*)ast
    );

    TEST_ASSERT_EQUAL_PTR(
        ast,
        frg_ast_scope_frame_get_ast(
            frame,
            "Asdf"
        )
    );

    TEST_ASSERT_NULL(
        frg_ast_scope_frame_get_ast(
            frame,
            "Asd"
        )
    );

    TEST_ASSERT_NULL(
        frg_ast_scope_frame_get_ast(
            frame,
            "Asdf2"
        )
    );

    TEST_ASSERT_NULL(
        frg_ast_scope_frame_get_ast(
            frame,
            "Fdsa"
        )
    );

    frg_ast_scope_frame_add_ir(
        frame,
        "Asdf",
        &ir
    );

    TEST_ASSERT_EQUAL_PTR(
        &ir,
        frg_ast_scope_frame_get_ir(
            frame,
            "Asdf"
        )
    );

    TEST_ASSERT_NULL(
        frg_ast_scope_frame_get_ir(
            frame,
            "Asd"
        )
    );

    TEST_ASSERT_NULL(
        frg_ast_scope_frame_get_ir(
            frame,
            "Asdf2"
        )
    );

    TEST_ASSERT_NULL(
        frg_ast_scope_frame_get_ir(
            frame,
            "Fdsa"
        )
    );

    frg_ast_destroy((frg_ast_t**)&ast);

    frg_ast_scope_frame_destroy(&frame);
}

void test_frame_load_decl_block(void) {
    frg_ast_scope_frame_t* frame = frg_ast_scope_frame_new();

    GList* decls = NULL;

    decls = g_list_append(
        decls,
        frg_ast_new_decl_var(
            frg_ast_new_decl_prop(
                FRG_AST_DECL_PROP_FLAG_NONE,
                g_string_new("x"),
                NULL
            ),
            NULL
        )
    );

    decls = g_list_append(
        decls,
        frg_ast_new_decl_fn(
            FRG_AST_DECL_FN_FLAG_NONE,
            g_string_new("f"),
            frg_ast_new_ty_fn(
                NULL,
                NULL,
                NULL,
                NULL
            ),
            NULL
        )
    );

    frg_ast_decl_block_t* decl_block = frg_ast_new_decl_block(decls);

    frg_ast_scope_frame_load_decl_block(
        frame,
        decl_block
    );

    TEST_ASSERT_NOT_NULL(
        frg_ast_scope_frame_get_ast(
            frame,
            "x"
        )
    );

    TEST_ASSERT_NOT_NULL(
        frg_ast_scope_frame_get_ast(
            frame,
            "f"
        )
    );

    TEST_ASSERT_NULL(
        frg_ast_scope_frame_get_ast(
            frame,
            "a"
        )
    );

    frg_ast_scope_frame_destroy(&frame);
    frg_ast_destroy((frg_ast_t**)&decl_block);
}

void test_frame_load_fn_args(void) {
    frg_ast_scope_frame_t* frame = frg_ast_scope_frame_new();

    GList* args = NULL;

    args = g_list_append(
        args,
        frg_ast_new_decl_fn_arg(
            FRG_AST_DECL_FN_ARG_FLAG_NONE,
            frg_ast_new_decl_prop(
                FRG_AST_DECL_PROP_FLAG_NONE,
                g_string_new("x"),
                NULL
            ),
            NULL
        )
    );

    args = g_list_append(
        args,
        frg_ast_new_decl_fn_arg(
            FRG_AST_DECL_FN_ARG_FLAG_NONE,
            frg_ast_new_decl_prop(
                FRG_AST_DECL_PROP_FLAG_NONE,
                g_string_new("y"),
                NULL
            ),
            NULL
        )
    );

    frg_ast_decl_fn_t* decl_fn = frg_ast_new_decl_fn(
        FRG_AST_DECL_FN_FLAG_NONE,
        g_string_new("f"),
        frg_ast_new_ty_fn(
            args,
            NULL,
            NULL,
            NULL
        ),
        NULL
    );

    frg_ast_scope_frame_load_decl_fn_args(
        frame,
        decl_fn
    );

    TEST_ASSERT_NOT_NULL(
        frg_ast_scope_frame_get_ast(
            frame,
            "x"
        )
    );

    TEST_ASSERT_NOT_NULL(
        frg_ast_scope_frame_get_ast(
            frame,
            "y"
        )
    );

    TEST_ASSERT_NULL(
        frg_ast_scope_frame_get_ast(
            frame,
            "a"
        )
    );

    frg_ast_scope_frame_destroy(&frame);
    frg_ast_destroy((frg_ast_t**)&decl_fn);
}

void test_scope(void) {
    frg_ast_scope_t* scope = frg_ast_scope_new();

    frg_ast_decl_prop_t* x = frg_ast_new_decl_prop(
        FRG_AST_DECL_PROP_FLAG_NONE,
        g_string_new("x"),
        NULL
    );

    int ir = 5;

    frg_ast_scope_add_ast(
        scope,
        (frg_ast_t*)x
    );

    TEST_ASSERT_EQUAL_PTR(
        x,
        frg_ast_scope_get_ast(
            scope,
            "x"
        )
    );

    TEST_ASSERT_NULL(
        frg_ast_scope_get_ast(
            scope,
            "y"
        )
    );

    frg_ast_scope_add_ir(
        scope,
        "x",
        &ir
    );

    TEST_ASSERT_EQUAL_PTR(
        &ir,
        frg_ast_scope_get_ir(
            scope,
            "x"
        )
    );

    TEST_ASSERT_NULL(
        frg_ast_scope_get_ir(
            scope,
            "y"
        )
    );

    frg_ast_scope_push_frame(scope);

    frg_ast_decl_prop_t* y = frg_ast_new_decl_prop(
        FRG_AST_DECL_PROP_FLAG_NONE,
        g_string_new("y"),
        NULL
    );

    frg_ast_scope_add_ast(
        scope,
        (frg_ast_t*)y
    );

    TEST_ASSERT_EQUAL_PTR(
        x,
        frg_ast_scope_get_ast(
            scope,
            "x"
        )
    );

    TEST_ASSERT_EQUAL_PTR(
        y,
        frg_ast_scope_get_ast(
            scope,
            "y"
        )
    );

    TEST_ASSERT_NULL(
        frg_ast_scope_get_ast(
            scope,
            "z"
        )
    );

    frg_ast_scope_add_ir(
        scope,
        "y",
        &ir
    );

    TEST_ASSERT_EQUAL_PTR(
        &ir,
        frg_ast_scope_get_ir(
            scope,
            "x"
        )
    );

    TEST_ASSERT_EQUAL_PTR(
        &ir,
        frg_ast_scope_get_ir(
            scope,
            "y"
        )
    );

    TEST_ASSERT_NULL(
        frg_ast_scope_get_ir(
            scope,
            "z"
        )
    );

    frg_ast_scope_pop_frame(scope);

    TEST_ASSERT_EQUAL_PTR(
        x,
        frg_ast_scope_get_ast(
            scope,
            "x"
        )
    );

    TEST_ASSERT_NULL(
        frg_ast_scope_get_ast(
            scope,
            "y"
        )
    );

    TEST_ASSERT_EQUAL_PTR(
        &ir,
        frg_ast_scope_get_ir(
            scope,
            "x"
        )
    );

    TEST_ASSERT_NULL(
        frg_ast_scope_get_ir(
            scope,
            "y"
        )
    );

    frg_ast_destroy((frg_ast_t**)&x);
    frg_ast_destroy((frg_ast_t**)&y);
    frg_ast_scope_destroy(&scope);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_frame);
    RUN_TEST(test_frame_load_decl_block);
    RUN_TEST(test_scope);
    return UNITY_END();
}
