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

#include <forge/ast/visitor.h>
#include <unity.h>

void setUp(void) {}

void tearDown(void) {}

frg_ast_t* create_ast(void) {
    GList* stmts = NULL;

    stmts = g_list_append(
        stmts,
        frg_ast_new_stmt_return(
            (frg_ast_t*)frg_ast_new_value_binary(
                FRG_AST_ID_VALUE_ADD,
                (frg_ast_t*)frg_ast_new_value_i32(
                    5
                ),
                (frg_ast_t*)frg_ast_new_value_i32(
                    6
                )
            )
        )
    );

    frg_ast_decl_fn_t* ast = frg_ast_new_decl_fn(
        FRG_AST_DECL_FN_FLAG_NONE,
        g_string_new("main"),
        frg_ast_new_ty_fn(
            NULL,
            NULL,
            NULL,
            NULL
        ),
        (frg_ast_t*)frg_ast_new_stmt_block(
            stmts
        )
    );

    return (frg_ast_t*)ast;
}

void test_empty(void) {
    frg_ast_t* ast = create_ast();

    frg_ast_visitor_t* visitor = frg_ast_visitor_new(NULL);

    frg_ast_visitor_status_t status = frg_ast_accept(visitor, &ast);

    TEST_ASSERT_EQUAL(FRG_AST_VISITOR_STATUS_OK, status);
    TEST_ASSERT_NOT_NULL(ast);
}

frg_ast_visitor_status_t callback_pre_and_post_pre(
    GList* parents,
    frg_ast_t** ast,
    void* user_data
) {
    TEST_ASSERT_NOT_NULL(parents);
    TEST_ASSERT_EQUAL(FRG_AST_ID_STMT_BLOCK, ((frg_ast_t*)parents->data)->id);
    TEST_ASSERT_NOT_NULL(parents->next);
    TEST_ASSERT_EQUAL(FRG_AST_ID_DECL_FN, ((frg_ast_t*)parents->next->data)->id);
    TEST_ASSERT_NULL(parents->next->next);

    int* user_data_int = (int*)user_data;

    TEST_ASSERT_EQUAL(0, *user_data_int);

    *user_data_int = 1;

    return FRG_AST_VISITOR_STATUS_OK;
}

frg_ast_visitor_status_t callback_pre_and_post_post(
    GList* parents,
    frg_ast_t** ast,
    void* user_data
) {
    TEST_ASSERT_NOT_NULL(parents);
    TEST_ASSERT_EQUAL(FRG_AST_ID_STMT_BLOCK, ((frg_ast_t*)parents->data)->id);
    TEST_ASSERT_NOT_NULL(parents->next);
    TEST_ASSERT_EQUAL(FRG_AST_ID_DECL_FN, ((frg_ast_t*)parents->next->data)->id);
    TEST_ASSERT_NULL(parents->next->next);

    int* user_data_int = (int*)user_data;

    TEST_ASSERT_EQUAL(1, *user_data_int);

    *user_data_int = 2;

    return FRG_AST_VISITOR_STATUS_OK;
}

void test_pre_and_post(void) {
    frg_ast_t* ast = create_ast();

    int user_data = 0;

    frg_ast_visitor_t* visitor = frg_ast_visitor_new(&user_data);

    frg_ast_visitor_add_entry(
        visitor,
        FRG_AST_ID_STMT_RETURN,
        callback_pre_and_post_pre,
        callback_pre_and_post_post
    );

    frg_ast_visitor_status_t status = frg_ast_accept(visitor, &ast);

    TEST_ASSERT_EQUAL(FRG_AST_VISITOR_STATUS_OK, status);
    TEST_ASSERT_NOT_NULL(ast);
    TEST_ASSERT_EQUAL(2, user_data);
}

frg_ast_visitor_status_t callback_skip_pre_stmt_return(
    GList* parents,
    frg_ast_t** ast,
    void* user_data
) {
    return FRG_AST_VISITOR_STATUS_SKIP;
}

frg_ast_visitor_status_t callback_skip_pre_value_add(
    GList* parents,
    frg_ast_t** ast,
    void* user_data
) {
    TEST_FAIL_MESSAGE("This should not be called");
}

frg_ast_visitor_status_t callback_skip_post_decl_fn(
    GList* parents,
    frg_ast_t** ast,
    void* user_data
) {
    int* user_data_int = (int*)user_data;

    TEST_ASSERT_EQUAL(0, *user_data_int);

    *user_data_int = 1;

    return FRG_AST_VISITOR_STATUS_OK;
}

void test_skip(void) {
    frg_ast_t* ast = create_ast();

    int user_data = 0;

    frg_ast_visitor_t* visitor = frg_ast_visitor_new(&user_data);

    frg_ast_visitor_add_entry(
        visitor,
        FRG_AST_ID_DECL_FN,
        NULL,
        callback_skip_post_decl_fn
    );

    frg_ast_visitor_add_entry(
        visitor,
        FRG_AST_ID_STMT_RETURN,
        callback_skip_pre_stmt_return,
        NULL
    );

    frg_ast_visitor_add_entry(
        visitor,
        FRG_AST_ID_VALUE_ADD,
        callback_skip_pre_value_add,
        NULL
    );

    frg_ast_visitor_status_t status = frg_ast_accept(visitor, &ast);

    TEST_ASSERT_EQUAL(FRG_AST_VISITOR_STATUS_OK, status);
    TEST_ASSERT_NOT_NULL(ast);
    TEST_ASSERT_EQUAL(1, user_data);
}

frg_ast_visitor_status_t callback_stop_pre_stmt_return(
    GList* parents,
    frg_ast_t** ast,
    void* user_data
) {
    return FRG_AST_VISITOR_STATUS_STOP;
}

frg_ast_visitor_status_t callback_stop_pre_value_add(
    GList* parents,
    frg_ast_t** ast,
    void* user_data
) {
    TEST_FAIL_MESSAGE("This should not be called");
}

frg_ast_visitor_status_t callback_stop_post_decl_fn(
    GList* parents,
    frg_ast_t** ast,
    void* user_data
) {
    TEST_FAIL_MESSAGE("This should not be called");
}

void test_stop(void) {
    frg_ast_t* ast = create_ast();

    frg_ast_visitor_t* visitor = frg_ast_visitor_new(NULL);

    frg_ast_visitor_add_entry(
        visitor,
        FRG_AST_ID_DECL_FN,
        NULL,
        callback_stop_post_decl_fn
    );

    frg_ast_visitor_add_entry(
        visitor,
        FRG_AST_ID_STMT_RETURN,
        callback_stop_pre_stmt_return,
        NULL
    );

    frg_ast_visitor_add_entry(
        visitor,
        FRG_AST_ID_VALUE_ADD,
        callback_stop_pre_value_add,
        NULL
    );

    frg_ast_visitor_status_t status = frg_ast_accept(visitor, &ast);

    TEST_ASSERT_EQUAL(FRG_AST_VISITOR_STATUS_STOP, status);
    TEST_ASSERT_NOT_NULL(ast);
}

frg_ast_visitor_status_t callback_replace_node_pre_value_add(
    GList* parents,
    frg_ast_t** ast,
    void* user_data
) {
    *ast = (frg_ast_t*)frg_ast_new_value_i32(11);

    return FRG_AST_VISITOR_STATUS_OK;
}

frg_ast_visitor_status_t callback_replace_node_post_value_add(
    GList* parents,
    frg_ast_t** ast,
    void* user_data
) {
    TEST_FAIL_MESSAGE("This should not be called");
}

void test_replace_node(void) {
    frg_ast_t* ast = create_ast();

    frg_ast_visitor_t* visitor = frg_ast_visitor_new(NULL);

    frg_ast_visitor_add_entry(
        visitor,
        FRG_AST_ID_VALUE_ADD,
        callback_replace_node_pre_value_add,
        callback_replace_node_post_value_add
    );

    frg_ast_visitor_status_t status = frg_ast_accept(visitor, &ast);

    TEST_ASSERT_EQUAL(FRG_AST_VISITOR_STATUS_OK, status);
    TEST_ASSERT_NOT_NULL(ast);

    TEST_ASSERT_EQUAL(
        FRG_AST_ID_VALUE_INT,
        ((frg_ast_stmt_return_t*)((frg_ast_stmt_block_t*)((frg_ast_decl_fn_t*)ast)->body)->stmts->data)->value->id
    );
}

frg_ast_visitor_status_t callback_replace_node_in_list_pre_stmt_return(
    GList* parents,
    frg_ast_t** ast,
    void* user_data
) {
    *ast = (frg_ast_t*)frg_ast_new_value_i32(11);

    return FRG_AST_VISITOR_STATUS_OK;
}

frg_ast_visitor_status_t callback_replace_node_in_list_post_stmt_return(
    GList* parents,
    frg_ast_t** ast,
    void* user_data
) {
    TEST_FAIL_MESSAGE("This should not be called");
}

void test_replace_node_in_list(void) {
    frg_ast_t* ast = create_ast();

    frg_ast_visitor_t* visitor = frg_ast_visitor_new(NULL);

    frg_ast_visitor_add_entry(
        visitor,
        FRG_AST_ID_STMT_RETURN,
        callback_replace_node_in_list_pre_stmt_return,
        callback_replace_node_in_list_post_stmt_return
    );

    frg_ast_visitor_status_t status = frg_ast_accept(visitor, &ast);

    TEST_ASSERT_EQUAL(FRG_AST_VISITOR_STATUS_OK, status);
    TEST_ASSERT_NOT_NULL(ast);

    TEST_ASSERT_EQUAL(
        FRG_AST_ID_VALUE_INT,
        ((frg_ast_t*)((frg_ast_stmt_block_t*)((frg_ast_decl_fn_t*)ast)->body)->stmts->data)->id
    );
}

frg_ast_visitor_status_t callback_remove_node_in_list_pre_stmt_return(
    GList* parents,
    frg_ast_t** ast,
    void* user_data
) {
    *ast = NULL;

    return FRG_AST_VISITOR_STATUS_OK;
}

void test_remove_node_in_list(void) {
    frg_ast_t* ast = create_ast();

    frg_ast_visitor_t* visitor = frg_ast_visitor_new(NULL);

    frg_ast_visitor_add_entry(
        visitor,
        FRG_AST_ID_STMT_RETURN,
        callback_remove_node_in_list_pre_stmt_return,
        NULL
    );

    frg_ast_visitor_status_t status = frg_ast_accept(visitor, &ast);

    TEST_ASSERT_EQUAL(FRG_AST_VISITOR_STATUS_OK, status);
    TEST_ASSERT_NOT_NULL(ast);

    TEST_ASSERT_NULL(
        ((frg_ast_stmt_block_t*)((frg_ast_decl_fn_t*)ast)->body)->stmts
    );
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_empty);
    RUN_TEST(test_pre_and_post);
    RUN_TEST(test_skip);
    RUN_TEST(test_stop);
    RUN_TEST(test_replace_node);
    RUN_TEST(test_replace_node_in_list);
    RUN_TEST(test_remove_node_in_list);
    return UNITY_END();
}
