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

#include <forge/ast/scope.h>
#include <unity.h>

void setUp() {}

void tearDown() {}

void test_frame() {
  frg_ast_scope_frame_t* frame = frg_ast_scope_frame_new();

  frg_ast_node_declaration_property_t* ast
    = frg_ast_node_declaration_property_new(&frg_global_parsing_range_null,
                                            FRG_AST_NODE_DECLARATION_PROPERTY_FLAG_NONE,
                                            g_string_new("Asdf"),
                                            NULL);

  int ir = 5;

  frg_ast_scope_frame_add_ast(frame, (frg_ast_node_t*)ast);

  TEST_ASSERT_EQUAL_PTR(ast, frg_ast_scope_frame_get_ast(frame, "Asdf"));

  TEST_ASSERT_NULL(frg_ast_scope_frame_get_ast(frame, "Asd"));

  TEST_ASSERT_NULL(frg_ast_scope_frame_get_ast(frame, "Asdf2"));

  TEST_ASSERT_NULL(frg_ast_scope_frame_get_ast(frame, "Fdsa"));

  frg_ast_scope_frame_add_ir(frame, "Asdf", (frg_codegen_type_t*)&ir);

  TEST_ASSERT_EQUAL_PTR(&ir, frg_ast_scope_frame_get_ir(frame, "Asdf"));

  TEST_ASSERT_NULL(frg_ast_scope_frame_get_ir(frame, "Asd"));

  TEST_ASSERT_NULL(frg_ast_scope_frame_get_ir(frame, "Asdf2"));

  TEST_ASSERT_NULL(frg_ast_scope_frame_get_ir(frame, "Fdsa"));

  frg_ast_node_destroy((frg_ast_node_t*)ast);

  frg_ast_scope_frame_destroy(frame);
}

void test_frame_load_declaration_block() {
  frg_ast_scope_frame_t* frame = frg_ast_scope_frame_new();

  GList* decls                 = NULL;

  decls                        = g_list_append(
    decls,
    frg_ast_node_declaration_assignment_new(
      &frg_global_parsing_range_null,
      frg_ast_node_declaration_property_new(&frg_global_parsing_range_null,
                                            FRG_AST_NODE_DECLARATION_PROPERTY_FLAG_NONE,
                                            g_string_new("x"),
                                            NULL),
      NULL));

  decls = g_list_append(decls,
                        frg_ast_node_declaration_function_new(
                          &frg_global_parsing_range_null,
                          FRG_AST_NODE_DECLARATION_FUNCTION_FLAG_NONE,
                          g_string_new("f"),
                          frg_ast_node_type_function_new(
                            &frg_global_parsing_range_null, NULL, NULL, NULL, NULL),
                          NULL));

  frg_ast_node_declaration_block_t* decl_block
    = frg_ast_node_declaration_block_new(&frg_global_parsing_range_null, decls);

  frg_ast_scope_frame_load_declaration_block(frame, decl_block);

  TEST_ASSERT_NOT_NULL(frg_ast_scope_frame_get_ast(frame, "x"));

  TEST_ASSERT_NOT_NULL(frg_ast_scope_frame_get_ast(frame, "f"));

  TEST_ASSERT_NULL(frg_ast_scope_frame_get_ast(frame, "a"));

  frg_ast_scope_frame_destroy(frame);
  frg_ast_node_destroy((frg_ast_node_t*)decl_block);
}

void test_frame_load_function_args() {
  frg_ast_scope_frame_t* frame = frg_ast_scope_frame_new();

  GList* args                  = NULL;

  args                         = g_list_append(
    args,
    frg_ast_node_declaration_function_argument_new(
      &frg_global_parsing_range_null,
      FRG_AST_NODE_DECLARATION_FUNCTION_ARGUMENT_FLAG_NONE,
      frg_ast_node_declaration_property_new(&frg_global_parsing_range_null,
                                            FRG_AST_NODE_DECLARATION_PROPERTY_FLAG_NONE,
                                            g_string_new("x"),
                                            NULL),
      NULL));

  args = g_list_append(
    args,
    frg_ast_node_declaration_function_argument_new(
      &frg_global_parsing_range_null,
      FRG_AST_NODE_DECLARATION_FUNCTION_ARGUMENT_FLAG_NONE,
      frg_ast_node_declaration_property_new(&frg_global_parsing_range_null,
                                            FRG_AST_NODE_DECLARATION_PROPERTY_FLAG_NONE,
                                            g_string_new("y"),
                                            NULL),
      NULL));

  frg_ast_node_declaration_function_t* decl_function
    = frg_ast_node_declaration_function_new(
      &frg_global_parsing_range_null,
      FRG_AST_NODE_DECLARATION_FUNCTION_FLAG_NONE,
      g_string_new("f"),
      frg_ast_node_type_function_new(
        &frg_global_parsing_range_null, args, NULL, NULL, NULL),
      NULL);

  frg_ast_scope_frame_load_declaration_function_args(frame, decl_function);

  TEST_ASSERT_NOT_NULL(frg_ast_scope_frame_get_ast(frame, "x"));

  TEST_ASSERT_NOT_NULL(frg_ast_scope_frame_get_ast(frame, "y"));

  TEST_ASSERT_NULL(frg_ast_scope_frame_get_ast(frame, "a"));

  frg_ast_scope_frame_destroy(frame);
  frg_ast_node_destroy((frg_ast_node_t*)decl_function);
}

void test_scope() {
  frg_ast_scope_t* scope = frg_ast_scope_new();

  frg_ast_node_declaration_property_t* x
    = frg_ast_node_declaration_property_new(&frg_global_parsing_range_null,
                                            FRG_AST_NODE_DECLARATION_PROPERTY_FLAG_NONE,
                                            g_string_new("x"),
                                            NULL);

  int ir = 5;

  frg_ast_scope_add_ast(scope, (frg_ast_node_t*)x);

  TEST_ASSERT_EQUAL_PTR(x, frg_ast_scope_get_ast(scope, "x"));

  TEST_ASSERT_NULL(frg_ast_scope_get_ast(scope, "y"));

  frg_ast_scope_add_ir(scope, "x", (frg_codegen_type_t*)&ir);

  TEST_ASSERT_EQUAL_PTR(&ir, frg_ast_scope_get_ir(scope, "x"));

  TEST_ASSERT_NULL(frg_ast_scope_get_ir(scope, "y"));

  frg_ast_scope_push_frame(scope);

  frg_ast_node_declaration_property_t* y
    = frg_ast_node_declaration_property_new(&frg_global_parsing_range_null,
                                            FRG_AST_NODE_DECLARATION_PROPERTY_FLAG_NONE,
                                            g_string_new("y"),
                                            NULL);

  frg_ast_scope_add_ast(scope, (frg_ast_node_t*)y);

  TEST_ASSERT_EQUAL_PTR(x, frg_ast_scope_get_ast(scope, "x"));

  TEST_ASSERT_EQUAL_PTR(y, frg_ast_scope_get_ast(scope, "y"));

  TEST_ASSERT_NULL(frg_ast_scope_get_ast(scope, "z"));

  frg_ast_scope_add_ir(scope, "y", (frg_codegen_type_t*)&ir);

  TEST_ASSERT_EQUAL_PTR(&ir, frg_ast_scope_get_ir(scope, "x"));

  TEST_ASSERT_EQUAL_PTR(&ir, frg_ast_scope_get_ir(scope, "y"));

  TEST_ASSERT_NULL(frg_ast_scope_get_ir(scope, "z"));

  frg_ast_scope_pop_frame(scope);

  TEST_ASSERT_EQUAL_PTR(x, frg_ast_scope_get_ast(scope, "x"));

  TEST_ASSERT_NULL(frg_ast_scope_get_ast(scope, "y"));

  TEST_ASSERT_EQUAL_PTR(&ir, frg_ast_scope_get_ir(scope, "x"));

  TEST_ASSERT_NULL(frg_ast_scope_get_ir(scope, "y"));

  frg_ast_node_destroy((frg_ast_node_t*)x);
  frg_ast_node_destroy((frg_ast_node_t*)y);
  frg_ast_scope_destroy(scope);
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_frame);
  RUN_TEST(test_frame_load_declaration_block);
  RUN_TEST(test_frame_load_function_args);
  RUN_TEST(test_scope);
  return UNITY_END();
}
