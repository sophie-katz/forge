// Copyright (c) 2023 Sophie Katz
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

// void test_simple_expression(void) {
//     frg_ast_t* ast = frg_ast_new_value_log_and(
//         frg_ast_new_value_log_not(
//             frg_ast_new_value_false()
//         ),
//         frg_ast_new_value_true()
//     );

//     TEST_ASSERT_NOT_NULL(frg_ast_as_value_binary(ast)->rhs);

//     frg_ast_destroy(&ast);
// }

int main(void) {
    UNITY_BEGIN();
    // RUN_TEST(test_simple_expression);
    return UNITY_END();
}
