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

#include <unity.h>

#include "test.h"

void setUp(void) {}

void tearDown(void) {}

void _callback_return_true(
    const frg_message_buffer_t* message_buffer,
    void* shared_library
) {
    TEST_ASSERT_EQUAL(0, frg_message_buffer_get_warning_count(message_buffer));
    TEST_ASSERT_EQUAL(0, frg_message_buffer_get_error_count(message_buffer));

    bool (*return_true)(void) = frg_test_compilation_get_function(
        shared_library,
        "return_true"
    );
    
    TEST_ASSERT(return_true());
}

void test_return_true(void) {
    frg_test_compilation(
        "return_true",
        // Source
        "fn return_true() -> bool {\n"
        "  return true;\n"
        "}\n",
        // AST debug
        "[decl-block]\n"
        "  decls[0] = [decl-fn]\n"
        "    flags = none\n"
        "    name = \"return_true\"\n"
        "    ty = [ty-fn]\n"
        "      return-ty = [ty-bool]\n"
        "    body = [stmt-block]\n"
        "      stmts[0] = [stmt-return]\n"
        "        value = [value-true]",
        // LLVM IR
        "; ModuleID = 'forge'\n"
        "source_filename = \"forge\"\n"
        "\n"
        "define i1 @return_true() {\n"
        "entry:\n"
        "  ret i1 true\n"
        "}\n",
        // Callback
        _callback_return_true
    );
}

void _callback_return_false(
    const frg_message_buffer_t* message_buffer,
    void* shared_library
) {
    TEST_ASSERT_EQUAL(0, frg_message_buffer_get_warning_count(message_buffer));
    TEST_ASSERT_EQUAL(0, frg_message_buffer_get_error_count(message_buffer));

    bool (*return_false)(void) = frg_test_compilation_get_function(
        shared_library,
        "return_false"
    );
    
    TEST_ASSERT_FALSE(return_false());
}

void test_return_false(void) {
    frg_test_compilation(
        "return_false",
        // Source
        "fn return_false() -> bool {\n"
        "  return false;\n"
        "}\n",
        // AST debug
        "[decl-block]\n"
        "  decls[0] = [decl-fn]\n"
        "    flags = none\n"
        "    name = \"return_false\"\n"
        "    ty = [ty-fn]\n"
        "      return-ty = [ty-bool]\n"
        "    body = [stmt-block]\n"
        "      stmts[0] = [stmt-return]\n"
        "        value = [value-false]",
        // LLVM IR
        "; ModuleID = 'forge'\n"
        "source_filename = \"forge\"\n"
        "\n"
        "define i1 @return_false() {\n"
        "entry:\n"
        "  ret i1 false\n"
        "}\n",
        // Callback
        _callback_return_false
    );
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_return_true);
    RUN_TEST(test_return_false);
    return UNITY_END();
}
