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

#include <forge/enum_printing.h>
#include <unity.h>

void setUp() {}

void tearDown() {}

void test_message_severity() {
    for (frg_message_severity_t value = FRG_MESSAGE_SEVERITY_FIRST; value <= FRG_MESSAGE_SEVERITY_LAST; value++) {
        frg_stream_output_t* stream = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
        frg_message_severity_print(stream, value);
        GString* buffer = frg_stream_output_destroy_take_buffer(stream);
        TEST_ASSERT_GREATER_THAN(0, buffer->len);
        g_string_free(buffer, TRUE);
    }
}

void test_ast_node_type_int_flags() {
    frg_stream_output_t* stream = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
    frg_ast_node_type_int_flags_print(stream, FRG_AST_NODE_TYPE_INT_FLAG_NONE);
    GString* buffer = frg_stream_output_destroy_take_buffer(stream);
    TEST_ASSERT_EQUAL_STRING("none", buffer->str);
    g_string_free(buffer, TRUE);

    for (frg_ast_node_type_int_flags_t value = 1; value <= FRG_AST_NODE_TYPE_INT_FLAG_MAX; value <<= 1) {
        stream = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
        frg_ast_node_type_int_flags_print(stream, value);
        buffer = frg_stream_output_destroy_take_buffer(stream);
        TEST_ASSERT_GREATER_THAN(0, buffer->len);
        g_string_free(buffer, TRUE);
    }
}

void test_ast_node_declaration_property_flags() {
    frg_stream_output_t* stream = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
    frg_ast_node_declaration_property_flags_print(stream, FRG_AST_NODE_DECLARATION_PROPERTY_FLAG_NONE);
    GString* buffer = frg_stream_output_destroy_take_buffer(stream);
    TEST_ASSERT_EQUAL_STRING("none", buffer->str);
    g_string_free(buffer, TRUE);

    for (frg_ast_node_declaration_property_flags_t value = 1; value <= FRG_AST_NODE_DECLARATION_PROPERTY_FLAG_MAX; value <<= 1) {
        stream = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
        frg_ast_node_declaration_property_flags_print(stream, value);
        buffer = frg_stream_output_destroy_take_buffer(stream);
        TEST_ASSERT_GREATER_THAN(0, buffer->len);
        g_string_free(buffer, TRUE);
    }

    stream = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
    frg_ast_node_declaration_property_flags_print(stream, 3);
    buffer = frg_stream_output_destroy_take_buffer(stream);
    TEST_ASSERT_NOT_NULL(strchr(buffer->str, ','));
    g_string_free(buffer, TRUE);
}

void test_ast_node_declaration_interface_flags() {
    frg_stream_output_t* stream = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
    frg_ast_node_declaration_interface_flags_print(stream, FRG_AST_NODE_DECLARATION_INTERFACE_FLAG_NONE);
    GString* buffer = frg_stream_output_destroy_take_buffer(stream);
    TEST_ASSERT_EQUAL_STRING("none", buffer->str);
    g_string_free(buffer, TRUE);

    for (frg_ast_node_declaration_interface_flags_t value = 1; value <= FRG_AST_NODE_DECLARATION_INTERFACE_FLAG_MAX; value <<= 1) {
        stream = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
        frg_ast_node_declaration_interface_flags_print(stream, value);
        buffer = frg_stream_output_destroy_take_buffer(stream);
        TEST_ASSERT_GREATER_THAN(0, buffer->len);
        g_string_free(buffer, TRUE);
    }
}

void test_ast_node_declaration_function_argument_flags() {
    frg_stream_output_t* stream = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
    frg_ast_node_declaration_function_argument_flags_print(stream, FRG_AST_NODE_DECLARATION_FUNCTION_ARGUMENT_FLAG_NONE);
    GString* buffer = frg_stream_output_destroy_take_buffer(stream);
    TEST_ASSERT_EQUAL_STRING("none", buffer->str);
    g_string_free(buffer, TRUE);

    for (frg_ast_node_declaration_function_argument_flags_t value = 1; value <= FRG_AST_NODE_DECLARATION_FUNCTION_ARGUMENT_FLAG_MAX; value <<= 1) {
        stream = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
        frg_ast_node_declaration_function_argument_flags_print(stream, value);
        buffer = frg_stream_output_destroy_take_buffer(stream);
        TEST_ASSERT_GREATER_THAN(0, buffer->len);
        g_string_free(buffer, TRUE);
    }
}

void test_ast_node_declaration_function_flags() {
    frg_stream_output_t* stream = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
    frg_ast_node_declaration_function_flags_print(stream, FRG_AST_NODE_DECLARATION_FUNCTION_FLAG_NONE);
    GString* buffer = frg_stream_output_destroy_take_buffer(stream);
    TEST_ASSERT_EQUAL_STRING("none", buffer->str);
    g_string_free(buffer, TRUE);

    for (frg_ast_node_declaration_function_flags_t value = 1; value <= FRG_AST_NODE_DECLARATION_FUNCTION_FLAG_MAX; value <<= 1) {
        stream = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
        frg_ast_node_declaration_function_flags_print(stream, value);
        buffer = frg_stream_output_destroy_take_buffer(stream);
        TEST_ASSERT_GREATER_THAN(0, buffer->len);
        g_string_free(buffer, TRUE);
    }

    stream = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
    frg_ast_node_declaration_function_flags_print(stream, 3);
    buffer = frg_stream_output_destroy_take_buffer(stream);
    TEST_ASSERT_NOT_NULL(strchr(buffer->str, ','));
    g_string_free(buffer, TRUE);
}

int main() {
    UNITY_BEGIN();
    RUN_TEST(test_message_severity);
    RUN_TEST(test_ast_node_type_int_flags);
    RUN_TEST(test_ast_node_declaration_property_flags);
    RUN_TEST(test_ast_node_declaration_interface_flags);
    RUN_TEST(test_ast_node_declaration_function_argument_flags);
    RUN_TEST(test_ast_node_declaration_function_flags);
    return UNITY_END();
}
