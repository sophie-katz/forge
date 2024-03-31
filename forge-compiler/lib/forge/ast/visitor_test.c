// This program is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software Foundation,
// either version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
// PARTICULAR PURPOSE. See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with this
// program. If not, see <https://www.gnu.org/licenses/>.
//
// Copyright 2023-2024 Sophie Katz

#include <forge/ast/utilities.h>
#include <forge/ast/visitor.h>
#include <forge/log.h>
#include <unity.h>

void setUp() {
  frg_stream_output_init();
}

void tearDown() {
  frg_stream_output_cleanup();
}

frg_ast_node_t* create_ast() {
  GList* statements = NULL;

  statements        = g_list_append(
    statements,
    frg_ast_node_statement_return_new(&frg_global_parsing_range_null,
                                      (frg_ast_node_t*)frg_ast_node_value_binary_new(
                                        &frg_global_parsing_range_null,
                                        FRG_AST_NODE_KIND_VALUE_ADD,
                                        (frg_ast_node_t*)frg_ast_node_value_int_new_i32(
                                          &frg_global_parsing_range_null, 5),
                                        (frg_ast_node_t*)frg_ast_node_value_int_new_i32(
                                          &frg_global_parsing_range_null, 6))));

  frg_ast_node_declaration_function_t* ast = frg_ast_node_declaration_function_new(
    &frg_global_parsing_range_null,
    FRG_AST_NODE_DECLARATION_FUNCTION_FLAG_NONE,
    g_string_new("main"),
    frg_ast_node_type_function_new(
      &frg_global_parsing_range_null, NULL, NULL, NULL, NULL),
    (frg_ast_node_t*)frg_ast_node_statement_block_new(&frg_global_parsing_range_null,
                                                      statements));

  return (frg_ast_node_t*)ast;
}

void test_empty() {
  frg_ast_node_t* node            = create_ast();

  frg_ast_visitor_t* visitor      = frg_ast_visitor_new(NULL);

  frg_ast_visitor_status_t status = frg_ast_accept(&node, visitor);

  TEST_ASSERT_EQUAL(FRG_AST_VISITOR_STATUS_OK, status);
  TEST_ASSERT_NOT_NULL(node);

  frg_ast_visitor_destroy(visitor);
  frg_ast_node_destroy(node);
}

frg_ast_visitor_status_t callback_enter_and_leave_enter(frg_ast_node_t** mut_node,
                                                        void* mut_user_data,
                                                        const GList* parents) {
  TEST_ASSERT_NOT_NULL(parents);
  TEST_ASSERT_EQUAL(FRG_AST_NODE_KIND_STATEMENT_BLOCK,
                    ((frg_ast_node_t*)parents->data)->kind);
  TEST_ASSERT_NOT_NULL(parents->next);
  TEST_ASSERT_EQUAL(FRG_AST_NODE_KIND_DECLARATION_FUNCTION,
                    ((frg_ast_node_t*)parents->next->data)->kind);
  TEST_ASSERT_NULL(parents->next->next);

  int* user_data_int = (int*)mut_user_data;

  TEST_ASSERT_EQUAL(0, *user_data_int);

  *user_data_int = 1;

  return FRG_AST_VISITOR_STATUS_OK;
}

frg_ast_visitor_status_t callback_enter_and_leave_leave(frg_ast_node_t** mut_node,
                                                        void* mut_user_data,
                                                        const GList* parents) {
  TEST_ASSERT_NOT_NULL(parents);
  TEST_ASSERT_EQUAL(FRG_AST_NODE_KIND_STATEMENT_BLOCK,
                    ((frg_ast_node_t*)parents->data)->kind);
  TEST_ASSERT_NOT_NULL(parents->next);
  TEST_ASSERT_EQUAL(FRG_AST_NODE_KIND_DECLARATION_FUNCTION,
                    ((frg_ast_node_t*)parents->next->data)->kind);
  TEST_ASSERT_NULL(parents->next->next);

  int* user_data_int = (int*)mut_user_data;

  TEST_ASSERT_EQUAL(1, *user_data_int);

  *user_data_int = 2;

  return FRG_AST_VISITOR_STATUS_OK;
}

void test_enter_and_leave() {
  frg_ast_node_t* node       = create_ast();

  int user_data              = 0;

  frg_ast_visitor_t* visitor = frg_ast_visitor_new(&user_data);

  frg_ast_visitor_handler_t* handler
    = frg_ast_visitor_add_handler(visitor, FRG_AST_NODE_KIND_STATEMENT_RETURN);
  handler->event_callbacks[FRG_AST_VISITOR_EVENT_ENTER]
    = callback_enter_and_leave_enter;
  handler->event_callbacks[FRG_AST_VISITOR_EVENT_LEAVE]
    = callback_enter_and_leave_leave;

  frg_ast_visitor_status_t status = frg_ast_accept(&node, visitor);

  TEST_ASSERT_EQUAL(FRG_AST_VISITOR_STATUS_OK, status);
  TEST_ASSERT_NOT_NULL(node);
  TEST_ASSERT_EQUAL(2, user_data);

  frg_ast_visitor_destroy(visitor);
  frg_ast_node_destroy(node);
}

frg_ast_visitor_status_t callback_skip_enter_statement_return(frg_ast_node_t** mut_node,
                                                              void* mut_user_data,
                                                              const GList* parents) {
  return FRG_AST_VISITOR_STATUS_SKIP;
}

frg_ast_visitor_status_t callback_skip_enter_value_add(frg_ast_node_t** mut_node,
                                                       void* mut_user_data,
                                                       const GList* parents) {
  TEST_FAIL_MESSAGE("This should not be called");
}

frg_ast_visitor_status_t callback_skip_leave_declaration_function(
  frg_ast_node_t** mut_node, void* mut_user_data, const GList* parents) {
  int* user_data_int = (int*)mut_user_data;

  TEST_ASSERT_EQUAL(0, *user_data_int);

  *user_data_int = 1;

  return FRG_AST_VISITOR_STATUS_OK;
}

void test_skip() {
  frg_ast_node_t* node       = create_ast();

  int user_data              = 0;

  frg_ast_visitor_t* visitor = frg_ast_visitor_new(&user_data);

  frg_ast_visitor_handler_t* handler
    = frg_ast_visitor_add_handler(visitor, FRG_AST_NODE_KIND_DECLARATION_FUNCTION);
  handler->event_callbacks[FRG_AST_VISITOR_EVENT_LEAVE]
    = callback_skip_leave_declaration_function;

  handler = frg_ast_visitor_add_handler(visitor, FRG_AST_NODE_KIND_STATEMENT_RETURN);
  handler->event_callbacks[FRG_AST_VISITOR_EVENT_ENTER]
    = callback_skip_enter_statement_return;

  handler = frg_ast_visitor_add_handler(visitor, FRG_AST_NODE_KIND_VALUE_ADD);
  handler->event_callbacks[FRG_AST_VISITOR_EVENT_ENTER] = callback_skip_enter_value_add;

  frg_ast_visitor_status_t status = frg_ast_accept(&node, visitor);

  TEST_ASSERT_EQUAL(FRG_AST_VISITOR_STATUS_OK, status);
  TEST_ASSERT_NOT_NULL(node);
  TEST_ASSERT_EQUAL(1, user_data);

  frg_ast_visitor_destroy(visitor);
  frg_ast_node_destroy(node);
}

frg_ast_visitor_status_t callback_stop_enter_statement_return(frg_ast_node_t** mut_node,
                                                              void* mut_user_data,
                                                              const GList* parents) {
  return FRG_AST_VISITOR_STATUS_STOP;
}

frg_ast_visitor_status_t callback_stop_enter_value_add(frg_ast_node_t** mut_node,
                                                       void* mut_user_data,
                                                       const GList* parents) {
  TEST_FAIL_MESSAGE("This should not be called");
}

frg_ast_visitor_status_t callback_stop_leave_declaration_function(
  frg_ast_node_t** mut_node, void* mut_user_data, const GList* parents) {
  TEST_FAIL_MESSAGE("This should not be called");
}

void test_stop() {
  frg_ast_node_t* node       = create_ast();

  frg_ast_visitor_t* visitor = frg_ast_visitor_new(NULL);

  frg_ast_visitor_handler_t* handler
    = frg_ast_visitor_add_handler(visitor, FRG_AST_NODE_KIND_DECLARATION_FUNCTION);
  handler->event_callbacks[FRG_AST_VISITOR_EVENT_LEAVE]
    = callback_stop_leave_declaration_function;

  handler = frg_ast_visitor_add_handler(visitor, FRG_AST_NODE_KIND_STATEMENT_RETURN);
  handler->event_callbacks[FRG_AST_VISITOR_EVENT_ENTER]
    = callback_stop_enter_statement_return;

  handler = frg_ast_visitor_add_handler(visitor, FRG_AST_NODE_KIND_VALUE_ADD);
  handler->event_callbacks[FRG_AST_VISITOR_EVENT_ENTER] = callback_stop_enter_value_add;

  frg_ast_visitor_status_t status = frg_ast_accept(&node, visitor);

  TEST_ASSERT_EQUAL(FRG_AST_VISITOR_STATUS_STOP, status);
  TEST_ASSERT_NOT_NULL(node);

  frg_ast_visitor_destroy(visitor);
  frg_ast_node_destroy(node);
}

frg_ast_visitor_status_t callback_replace_node_enter_value_add(
  frg_ast_node_t** mut_node, void* mut_user_data, const GList* parents) {
  frg_ast_node_destroy(*mut_node);

  *mut_node = (frg_ast_node_t*)frg_ast_node_value_int_new_i32(
    &frg_global_parsing_range_null, 11);

  return FRG_AST_VISITOR_STATUS_OK;
}

frg_ast_visitor_status_t callback_replace_node_leave_value_add(
  frg_ast_node_t** mut_node, void* mut_user_data, const GList* parents) {
  TEST_FAIL_MESSAGE("This should not be called");
}

void test_replace_node() {
  frg_ast_node_t* node       = create_ast();

  frg_ast_visitor_t* visitor = frg_ast_visitor_new(NULL);

  frg_ast_visitor_handler_t* handler
    = frg_ast_visitor_add_handler(visitor, FRG_AST_NODE_KIND_VALUE_ADD);
  handler->event_callbacks[FRG_AST_VISITOR_EVENT_ENTER]
    = callback_replace_node_enter_value_add;
  handler->event_callbacks[FRG_AST_VISITOR_EVENT_LEAVE]
    = callback_replace_node_leave_value_add;

  frg_ast_visitor_status_t status = frg_ast_accept(&node, visitor);

  TEST_ASSERT_EQUAL(FRG_AST_VISITOR_STATUS_OK, status);
  TEST_ASSERT_NOT_NULL(node);

  TEST_ASSERT_EQUAL(
    FRG_AST_NODE_KIND_VALUE_INT,
    ((frg_ast_node_statement_return_t*)((frg_ast_node_statement_block_t*)((frg_ast_node_declaration_function_t*)
                                                                            node)
                                          ->body)
       ->statements->data)
      ->value->kind);

  frg_ast_visitor_destroy(visitor);
  frg_ast_node_destroy(node);
}

frg_ast_visitor_status_t callback_replace_node_in_list_enter_statement_return(
  frg_ast_node_t** mut_node, void* mut_user_data, const GList* parents) {
  frg_ast_node_destroy(*mut_node);

  *mut_node = (frg_ast_node_t*)frg_ast_node_value_int_new_i32(
    &frg_global_parsing_range_null, 11);

  return FRG_AST_VISITOR_STATUS_OK;
}

frg_ast_visitor_status_t callback_replace_node_in_list_leave_statement_return(
  frg_ast_node_t** mut_node, void* mut_user_data, const GList* parents) {
  TEST_FAIL_MESSAGE("This should not be called");
}

void test_replace_node_in_list() {
  frg_ast_node_t* node       = create_ast();

  frg_ast_visitor_t* visitor = frg_ast_visitor_new(NULL);

  frg_ast_visitor_handler_t* handler
    = frg_ast_visitor_add_handler(visitor, FRG_AST_NODE_KIND_STATEMENT_RETURN);
  handler->event_callbacks[FRG_AST_VISITOR_EVENT_ENTER]
    = callback_replace_node_in_list_enter_statement_return;
  handler->event_callbacks[FRG_AST_VISITOR_EVENT_LEAVE]
    = callback_replace_node_in_list_leave_statement_return;

  frg_ast_visitor_status_t status = frg_ast_accept(&node, visitor);

  TEST_ASSERT_EQUAL(FRG_AST_VISITOR_STATUS_OK, status);
  TEST_ASSERT_NOT_NULL(node);

  TEST_ASSERT_EQUAL(
    FRG_AST_NODE_KIND_VALUE_INT,
    ((frg_ast_node_t*)((frg_ast_node_statement_block_t*)((frg_ast_node_declaration_function_t*)
                                                           node)
                         ->body)
       ->statements->data)
      ->kind);

  frg_ast_visitor_destroy(visitor);
  frg_ast_node_destroy(node);
}

frg_ast_visitor_status_t callback_remove_node_in_list_enter_statement_return(
  frg_ast_node_t** mut_node, void* mut_user_data, const GList* parents) {
  frg_ast_node_destroy(*mut_node);
  *mut_node = NULL;

  return FRG_AST_VISITOR_STATUS_OK;
}

void test_remove_node_in_list() {
  frg_ast_node_t* node       = create_ast();

  frg_ast_visitor_t* visitor = frg_ast_visitor_new(NULL);

  frg_ast_visitor_handler_t* handler
    = frg_ast_visitor_add_handler(visitor, FRG_AST_NODE_KIND_STATEMENT_RETURN);
  handler->event_callbacks[FRG_AST_VISITOR_EVENT_ENTER]
    = callback_remove_node_in_list_enter_statement_return;

  frg_ast_visitor_status_t status = frg_ast_accept(&node, visitor);

  TEST_ASSERT_EQUAL(FRG_AST_VISITOR_STATUS_OK, status);
  TEST_ASSERT_NOT_NULL(node);

  TEST_ASSERT_NULL(
    ((frg_ast_node_statement_block_t*)((frg_ast_node_declaration_function_t*)node)
       ->body)
      ->statements);

  frg_ast_visitor_destroy(visitor);
  frg_ast_node_destroy(node);
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_empty);
  RUN_TEST(test_enter_and_leave);
  RUN_TEST(test_skip);
  RUN_TEST(test_stop);
  RUN_TEST(test_replace_node);
  RUN_TEST(test_replace_node_in_list);
  RUN_TEST(test_remove_node_in_list);
  return UNITY_END();
}
