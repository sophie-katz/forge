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

#include <forge/parsing/parse.h>
#include <unity.h>

void setUp() {}

void tearDown() {}

void test_buffer() {
    frg_message_buffer_t* message_buffer = frg_message_buffer_new();

    char buffer[] = "fn main();\0";

    frg_stream_input_t* stream = frg_stream_input_new_buffer(
        buffer,
        FRG_STREAM_INPUT_FLAG_EXTRA_NULL_BYTE
    );

    frg_parsing_source_t* source = frg_parsing_source_new(
        "--",
        stream
    );

    frg_ast_node_t* node = frg_parse(message_buffer, source);

    TEST_ASSERT_NOT_NULL(node);
    TEST_ASSERT_EQUAL(FRG_AST_NODE_KIND_DECLARATION_BLOCK, node->kind);

    frg_ast_node_destroy(node);
    frg_parsing_source_destroy(source);
    frg_message_buffer_destroy(message_buffer);
}

void test_file() {
    frg_message_buffer_t* message_buffer = frg_message_buffer_new();

    char buffer[] = "fn main();";

    FILE* file = fmemopen(buffer, 11, "r");

    frg_stream_input_t* stream = frg_stream_input_new_file(
        file,
        true
    );

    frg_parsing_source_t* source = frg_parsing_source_new(
        "--",
        stream
    );

    frg_ast_node_t* node = frg_parse(message_buffer, source);

    TEST_ASSERT_NOT_NULL(node);
    TEST_ASSERT_EQUAL(FRG_AST_NODE_KIND_DECLARATION_BLOCK, node->kind);

    frg_ast_node_destroy(node);
    frg_parsing_source_destroy(source);
    frg_message_buffer_destroy(message_buffer);
}

int main() {
    UNITY_BEGIN();
    RUN_TEST(test_buffer);
    RUN_TEST(test_file);
    return UNITY_END();
}
