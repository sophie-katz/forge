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

void _callback_return_i64(
    const frg_message_buffer_t* message_buffer,
    void* shared_library
) {
    TEST_ASSERT_EQUAL(0, frg_message_buffer_get_warning_count(message_buffer));
    TEST_ASSERT_EQUAL(0, frg_message_buffer_get_error_count(message_buffer));

    int64_t (*return_i64)(void) = frg_test_compilation_get_function(
        shared_library,
        "return_i64"
    );
    
    TEST_ASSERT_EQUAL(0, return_i64());
}

void test_return_i64(void) {
    frg_test_compilation(
        "return_i64",
        // Source
        "fn return_i64() -> i64 {\n"
        "  return 0i64;\n"
        "}\n",
        // AST debug
        "[decl-block]\n"
        "  decls[0] = [decl-fn]\n"
        "    flags = none\n"
        "    name = \"return_i64\"\n"
        "    ty = [ty-fn]\n"
        "      return-ty = [ty-i64]\n"
        "    body = [stmt-block]\n"
        "      stmts[0] = [stmt-return]\n"
        "        value = [value-int]\n"
        "          ty = [ty-i64]\n"
        "          value = 0",
        // LLVM IR
        "; ModuleID = 'forge'\n"
        "source_filename = \"forge\"\n"
        "\n"
        "define i64 @return_i64() {\n"
        "entry:\n"
        "  ret i64 0\n"
        "}\n",
        // Callback
        _callback_return_i64
    );
}

void _callback_return_i32(
    const frg_message_buffer_t* message_buffer,
    void* shared_library
) {
    TEST_ASSERT_EQUAL(0, frg_message_buffer_get_warning_count(message_buffer));
    TEST_ASSERT_EQUAL(0, frg_message_buffer_get_error_count(message_buffer));

    int32_t (*return_i32)(void) = frg_test_compilation_get_function(
        shared_library,
        "return_i32"
    );
    
    TEST_ASSERT_EQUAL(0, return_i32());
}

void test_return_i32(void) {
    frg_test_compilation(
        "return_i32",
        // Source
        "fn return_i32() -> i32 {\n"
        "  return 0i32;\n"
        "}\n",
        // AST debug
        "[decl-block]\n"
        "  decls[0] = [decl-fn]\n"
        "    flags = none\n"
        "    name = \"return_i32\"\n"
        "    ty = [ty-fn]\n"
        "      return-ty = [ty-i32]\n"
        "    body = [stmt-block]\n"
        "      stmts[0] = [stmt-return]\n"
        "        value = [value-int]\n"
        "          ty = [ty-i32]\n"
        "          value = 0",
        // LLVM IR
        "; ModuleID = 'forge'\n"
        "source_filename = \"forge\"\n"
        "\n"
        "define i32 @return_i32() {\n"
        "entry:\n"
        "  ret i32 0\n"
        "}\n",
        // Callback
        _callback_return_i32
    );
}

void _callback_return_u8(
    const frg_message_buffer_t* message_buffer,
    void* shared_library
) {
    TEST_ASSERT_EQUAL(0, frg_message_buffer_get_warning_count(message_buffer));
    TEST_ASSERT_EQUAL(0, frg_message_buffer_get_error_count(message_buffer));

    int32_t (*return_u8)(void) = frg_test_compilation_get_function(
        shared_library,
        "return_u8"
    );
    
    TEST_ASSERT_EQUAL(0, return_u8());
}

void test_return_u8(void) {
    frg_test_compilation(
        "return_u8",
        // Source
        "fn return_u8() -> u8 {\n"
        "  return 0u8;\n"
        "}\n",
        // AST debug
        "[decl-block]\n"
        "  decls[0] = [decl-fn]\n"
        "    flags = none\n"
        "    name = \"return_u8\"\n"
        "    ty = [ty-fn]\n"
        "      return-ty = [ty-u8]\n"
        "    body = [stmt-block]\n"
        "      stmts[0] = [stmt-return]\n"
        "        value = [value-int]\n"
        "          ty = [ty-u8]\n"
        "          value = 0",
        // LLVM IR
        "; ModuleID = 'forge'\n"
        "source_filename = \"forge\"\n"
        "\n"
        "define i8 @return_u8() {\n"
        "entry:\n"
        "  ret i8 0\n"
        "}\n",
        // Callback
        _callback_return_u8
    );
}

void _callback_return_implicit(
    const frg_message_buffer_t* message_buffer,
    void* shared_library
) {
    TEST_ASSERT_EQUAL(0, frg_message_buffer_get_warning_count(message_buffer));
    TEST_ASSERT_EQUAL(0, frg_message_buffer_get_error_count(message_buffer));

    int32_t (*return_implicit)(void) = frg_test_compilation_get_function(
        shared_library,
        "return_implicit"
    );
    
    TEST_ASSERT_EQUAL(0, return_implicit());
}

void test_return_implicit(void) {
    frg_test_compilation(
        "return_implicit",
        // Source
        "fn return_implicit() -> u64 {\n"
        "  return 0;\n"
        "}\n",
        // AST debug
        "[decl-block]\n"
        "  decls[0] = [decl-fn]\n"
        "    flags = none\n"
        "    name = \"return_implicit\"\n"
        "    ty = [ty-fn]\n"
        "      return-ty = [ty-u64]\n"
        "    body = [stmt-block]\n"
        "      stmts[0] = [stmt-return]\n"
        "        value = [value-int]\n"
        "          ty = [ty-u64]\n"
        "          value = 0",
        // LLVM IR
        "; ModuleID = 'forge'\n"
        "source_filename = \"forge\"\n"
        "\n"
        "define i64 @return_implicit() {\n"
        "entry:\n"
        "  ret i64 0\n"
        "}\n",
        // Callback
        _callback_return_implicit
    );
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_return_i64);
    RUN_TEST(test_return_i32);
    RUN_TEST(test_return_u8);
    RUN_TEST(test_return_implicit);
    return UNITY_END();
}
