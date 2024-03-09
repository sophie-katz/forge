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

#include <forge/ast/node_kind_info.h>
#include <unity.h>

void setUp() {}

void tearDown() {}

void test_node_kind_info() {
  for (frg_ast_node_kind_t kind = 0; kind < FRG_AST_NODE_KIND_LAST; kind++) {
    const frg_ast_node_kind_info_t* info = frg_ast_node_kind_info_get(kind);

    TEST_ASSERT_NOT_NULL(info);

    TEST_ASSERT_NOT_NULL(info->name);
    TEST_ASSERT_NOT_EQUAL(0, info->name[0]);

    TEST_ASSERT_NOT_EQUAL_MESSAGE(0, info->flags, "at least one flag must be set");

    if (info->flags & FRG_AST_NODE_KIND_FLAG_TYPE) {
      TEST_ASSERT_EQUAL_MESSAGE(0,
                                info->flags & FRG_AST_NODE_KIND_FLAG_DECLARATION,
                                "type and declaration flags cannot be used together");

      TEST_ASSERT_EQUAL_MESSAGE(0,
                                info->flags & FRG_AST_NODE_KIND_FLAG_STATEMENT,
                                "type and statement flags cannot be used together");

      TEST_ASSERT_EQUAL_MESSAGE(0,
                                info->flags & FRG_AST_NODE_KIND_FLAG_VALUE,
                                "type and value flags cannot be used together");
    }

    if (info->flags & FRG_AST_NODE_KIND_FLAG_DECLARATION) {
      TEST_ASSERT_EQUAL_MESSAGE(
        0,
        info->flags & FRG_AST_NODE_KIND_FLAG_STATEMENT,
        "declaration and statement flags cannot be used together");

      TEST_ASSERT_EQUAL_MESSAGE(0,
                                info->flags & FRG_AST_NODE_KIND_FLAG_VALUE,
                                "declaration and value flags cannot be used together");

      TEST_ASSERT_NOT_EQUAL_MESSAGE(0,
                                    info->flags & FRG_AST_NODE_KIND_FLAG_HAS_CHILDREN,
                                    "all declaration nodes must have children");
    }

    if (info->flags & FRG_AST_NODE_KIND_FLAG_STATEMENT) {
      TEST_ASSERT_EQUAL_MESSAGE(0,
                                info->flags & FRG_AST_NODE_KIND_FLAG_VALUE,
                                "statement and value flags cannot be used together");

      TEST_ASSERT_NOT_EQUAL_MESSAGE(0,
                                    info->flags & FRG_AST_NODE_KIND_FLAG_HAS_CHILDREN,
                                    "all statement nodes must have children");
    }

    if (info->flags & FRG_AST_NODE_KIND_FLAG_VALUE) {
      if (info->flags & FRG_AST_NODE_KIND_FLAG_VALUE_UNARY) {
        TEST_ASSERT_NOT_EQUAL_MESSAGE(0,
                                      info->flags & FRG_AST_NODE_KIND_FLAG_HAS_CHILDREN,
                                      "all value-unary nodes must have children");

        TEST_ASSERT_EQUAL_MESSAGE(
          0,
          info->flags & FRG_AST_NODE_KIND_FLAG_VALUE_BINARY,
          "value-unary and value-binary flags cannot be used together");
      }

      if (info->flags & FRG_AST_NODE_KIND_FLAG_VALUE_BINARY) {
        TEST_ASSERT_NOT_EQUAL_MESSAGE(0,
                                      info->flags & FRG_AST_NODE_KIND_FLAG_HAS_CHILDREN,
                                      "all value-binary nodes must have children");
      }

    } else {
      TEST_ASSERT_EQUAL_MESSAGE(0,
                                info->flags & FRG_AST_NODE_KIND_FLAG_VALUE_UNARY,
                                "only value nodes can have value-unary flag set");

      TEST_ASSERT_EQUAL_MESSAGE(0,
                                info->flags & FRG_AST_NODE_KIND_FLAG_VALUE_BINARY,
                                "only value nodes can have value-binary flag set");
    }

    if (kind == FRG_AST_NODE_KIND_TYPE_SYMBOL || kind == FRG_AST_NODE_KIND_VALUE_STRING
        || kind == FRG_AST_NODE_KIND_VALUE_SYMBOL) {
      TEST_ASSERT_NOT_NULL_MESSAGE(
        info->_destructor,
        "node kinds without child nodes but with heap memory must have a destructor");
    } else if (info->flags & FRG_AST_NODE_KIND_FLAG_HAS_CHILDREN) {
      TEST_ASSERT_NOT_NULL_MESSAGE(
        info->_destructor, "all node kinds with children must have a destructor");
    } else {
      TEST_ASSERT_NULL_MESSAGE(
        info->_destructor,
        "node kinds without children or heap memory must not have destructors");
    }

    TEST_ASSERT_NOT_NULL_MESSAGE(info->_cloner, "all node kinds must have a cloner");

    if (info->flags & FRG_AST_NODE_KIND_FLAG_DECLARATION) {
      TEST_ASSERT_NOT_NULL_MESSAGE(info->_declaration_name_getter,
                                   "all declarations must have a name getter");
    } else {
      TEST_ASSERT_NULL_MESSAGE(info->_declaration_name_getter,
                               "only declarations can have a name getter");
    }

    if (info->flags & FRG_AST_NODE_KIND_FLAG_TYPE_PRIMARY) {
      TEST_ASSERT_NULL_MESSAGE(info->_debug_printer,
                               "primary types do not have debug printers");
    } else {
      TEST_ASSERT_NOT_NULL_MESSAGE(
        info->_debug_printer,
        "all node kinds must have a debug printer (except special cases)");
    }

    // TODO: Add formatted printer for all node kinds
    // TEST_ASSERT_NOT_NULL_MESSAGE(info->_formatted_printer, "all node kinds must have
    // a formatted printer");

    if (info->flags & FRG_AST_NODE_KIND_FLAG_HAS_CHILDREN) {
      TEST_ASSERT_NOT_NULL_MESSAGE(
        info->_visitor_acceptor,
        "all node kinds with children must have a visitor acceptor");
    } else {
      TEST_ASSERT_NULL_MESSAGE(
        info->_visitor_acceptor,
        "node kinds without children do not have visitor acceptorss");
    }

    if (kind == FRG_AST_NODE_KIND_DECLARATION_FUNCTION_ARGUMENT) {
      TEST_ASSERT_NOT_NULL_MESSAGE(
        info->_type_resolver,
        "declaration-function-argument must have a type resolver");
    } else if (kind == FRG_AST_NODE_KIND_VALUE_STRUCTURE) {
      TEST_ASSERT_NULL_MESSAGE(info->_type_resolver,
                               "value-structure must not have a type resolver");
    } else if ((info->flags & FRG_AST_NODE_KIND_FLAG_DECLARATION)
               || (info->flags & FRG_AST_NODE_KIND_FLAG_VALUE)) {
      TEST_ASSERT_NOT_NULL_MESSAGE(
        info->_type_resolver, "all declarations and values must have a type resolver");
    } else {
      TEST_ASSERT_NULL_MESSAGE(info->_type_resolver,
                               "only declarations and values can have a type resolver");
    }

    printf("Node kind %s passed\n", info->name);
  }
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_node_kind_info);
  return UNITY_END();
}
