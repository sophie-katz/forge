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
#include <forge/verification/visitor/well_formed.h>
#include <unity.h>

void setUp() {}

void tearDown() {}

void test_node_kind_handler_coverage() {
  frg_ast_visitor_t* visitor = frg_ast_visitor_new(NULL);

  frg_verification_well_formed_add_handlers(visitor);

  for (frg_ast_node_kind_t kind = 0; kind <= FRG_AST_NODE_KIND_LAST; kind++) {
    TEST_ASSERT(frg_ast_visitor_has_handler_for_node_kind(visitor, kind));

    printf("Node kind %s passed\n", frg_ast_node_kind_info_get(kind)->name);
  }
}

int main() {
  UNITY_BEGIN();
  // TODO: Complete coverage
  // RUN_TEST(test_node_kind_handler_coverage);
  return UNITY_END();
}
