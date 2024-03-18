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

#include <forge-testing/string_comparison.h>
#include <unity.h>

void setUp() {}

void tearDown() {}

void test_print_entry_matching() {
  frg_testing_string_comparison_entry_t* entry
    = frg_testing_string_comparison_entry_new_matching(g_string_new("hello"));

  frg_stream_output_t* stream
    = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);

  frg_testing_string_comparison_entry_print(stream, entry);

  GString* buffer = frg_stream_output_destroy_take_buffer(stream);

  TEST_ASSERT_EQUAL_STRING("         | hello\n", buffer->str);

  g_string_free(buffer, TRUE);

  frg_testing_string_comparison_entry_destroy(entry);
}

void test_print_entry_only_expected() {
  frg_testing_string_comparison_entry_t* entry
    = frg_testing_string_comparison_entry_new_only_expected(g_string_new("hello"));

  frg_stream_output_t* stream
    = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);

  frg_testing_string_comparison_entry_print(stream, entry);

  GString* buffer = frg_stream_output_destroy_take_buffer(stream);

  TEST_ASSERT_EQUAL_STRING("Expected | hello\n", buffer->str);

  g_string_free(buffer, TRUE);

  frg_testing_string_comparison_entry_destroy(entry);
}

void test_print_entry_only_actual() {
  frg_testing_string_comparison_entry_t* entry
    = frg_testing_string_comparison_entry_new_only_actual(g_string_new("hello"));

  frg_stream_output_t* stream
    = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);

  frg_testing_string_comparison_entry_print(stream, entry);

  GString* buffer = frg_stream_output_destroy_take_buffer(stream);

  TEST_ASSERT_EQUAL_STRING("Actual   | hello\n", buffer->str);

  g_string_free(buffer, TRUE);

  frg_testing_string_comparison_entry_destroy(entry);
}

void test_print_entry_different_0() {
  frg_testing_string_comparison_entry_t* entry
    = frg_testing_string_comparison_entry_new_different(g_string_new("hello"),
                                                        g_string_new("goodbye"));

  frg_stream_output_t* stream
    = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);

  frg_testing_string_comparison_entry_print(stream, entry);

  GString* buffer = frg_stream_output_destroy_take_buffer(stream);

  TEST_ASSERT_EQUAL_STRING("Expected | hello\nActual   | goodbye\n", buffer->str);

  g_string_free(buffer, TRUE);

  frg_testing_string_comparison_entry_destroy(entry);
}

void test_print_entry_different_1() {
  frg_testing_string_comparison_entry_t* entry
    = frg_testing_string_comparison_entry_new_different(
      g_string_new("  value = truesdfsdf"), g_string_new("  value = true"));

  frg_stream_output_t* stream
    = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);

  frg_testing_string_comparison_entry_print(stream, entry);

  GString* buffer = frg_stream_output_destroy_take_buffer(stream);

  TEST_ASSERT_EQUAL_STRING(
    "Expected |   value = truesdfsdf\nActual   |   value = true\n", buffer->str);

  g_string_free(buffer, TRUE);

  frg_testing_string_comparison_entry_destroy(entry);
}

void test_print_result_matching() {
  frg_testing_string_comparison_result_t* result
    = frg_testing_string_comparison_result_new(
      FRG_TESTING_STRING_COMPARISON_RESULT_KIND_MATCHING, NULL);

  frg_stream_output_t* stream
    = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);

  frg_testing_string_comparison_result_print(stream, result);

  GString* buffer = frg_stream_output_destroy_take_buffer(stream);

  TEST_ASSERT_EQUAL_STRING("", buffer->str);

  g_string_free(buffer, TRUE);

  frg_testing_string_comparison_result_destroy(result);
}

void test_print_result_different_incomplete() {
  GList* entries = NULL;

  entries        = g_list_append(entries,
                          frg_testing_string_comparison_entry_new_different(
                            g_string_new("hello"), g_string_new("goodbye")));

  frg_testing_string_comparison_result_t* result
    = frg_testing_string_comparison_result_new(
      FRG_TESTING_STRING_COMPARISON_RESULT_KIND_DIFFERENT_INCOMPLETE, entries);

  frg_stream_output_t* stream
    = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);

  frg_testing_string_comparison_result_print(stream, result);

  GString* buffer = frg_stream_output_destroy_take_buffer(stream);

  TEST_ASSERT_EQUAL_STRING("Expected | hello\nActual   | goodbye\n...\n", buffer->str);

  g_string_free(buffer, TRUE);

  frg_testing_string_comparison_result_destroy(result);
}

void test_print_result_different_complete() {
  GList* entries = NULL;

  entries        = g_list_append(entries,
                          frg_testing_string_comparison_entry_new_different(
                            g_string_new("hello"), g_string_new("goodbye")));

  frg_testing_string_comparison_result_t* result
    = frg_testing_string_comparison_result_new(
      FRG_TESTING_STRING_COMPARISON_RESULT_KIND_DIFFERENT_COMPLETE, entries);

  frg_stream_output_t* stream
    = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);

  frg_testing_string_comparison_result_print(stream, result);

  GString* buffer = frg_stream_output_destroy_take_buffer(stream);

  TEST_ASSERT_EQUAL_STRING("Expected | hello\nActual   | goodbye\n", buffer->str);

  g_string_free(buffer, TRUE);

  frg_testing_string_comparison_result_destroy(result);
}

void test_compare_strings_empty_and_empty() {
  frg_testing_string_comparison_result_t* result
    = frg_testing_compare_strings("", "", 1);

  TEST_ASSERT_EQUAL(FRG_TESTING_STRING_COMPARISON_RESULT_KIND_MATCHING, result->kind);

  TEST_ASSERT_NULL(result->entries);

  frg_testing_string_comparison_result_destroy(result);
}

void test_compare_strings_empty_and_one_line() {
  frg_testing_string_comparison_result_t* result
    = frg_testing_compare_strings("", "hello", 1);

  TEST_ASSERT_EQUAL(FRG_TESTING_STRING_COMPARISON_RESULT_KIND_DIFFERENT_COMPLETE,
                    result->kind);

  TEST_ASSERT_NOT_NULL(result->entries);
  TEST_ASSERT_EQUAL(
    FRG_TESTING_STRING_COMPARISON_ENTRY_KIND_ONLY_ACTUAL,
    ((frg_testing_string_comparison_entry_t*)result->entries->data)->kind);
  TEST_ASSERT_NULL(
    ((frg_testing_string_comparison_entry_t*)result->entries->data)->expected);
  TEST_ASSERT_EQUAL_STRING(
    "hello",
    ((frg_testing_string_comparison_entry_t*)result->entries->data)->actual->str);

  TEST_ASSERT_NULL(result->entries->next);

  frg_testing_string_comparison_result_destroy(result);
}

void test_compare_strings_one_line_and_empty() {
  frg_testing_string_comparison_result_t* result
    = frg_testing_compare_strings("hello", "", 1);

  TEST_ASSERT_EQUAL(FRG_TESTING_STRING_COMPARISON_RESULT_KIND_DIFFERENT_COMPLETE,
                    result->kind);

  TEST_ASSERT_NOT_NULL(result->entries);
  TEST_ASSERT_EQUAL(
    FRG_TESTING_STRING_COMPARISON_ENTRY_KIND_ONLY_EXPECTED,
    ((frg_testing_string_comparison_entry_t*)result->entries->data)->kind);
  TEST_ASSERT_EQUAL_STRING(
    "hello",
    ((frg_testing_string_comparison_entry_t*)result->entries->data)->expected->str);
  TEST_ASSERT_NULL(
    ((frg_testing_string_comparison_entry_t*)result->entries->data)->actual);

  TEST_ASSERT_NULL(result->entries->next);

  frg_testing_string_comparison_result_destroy(result);
}

void test_compare_strings_one_line_different() {
  frg_testing_string_comparison_result_t* result
    = frg_testing_compare_strings("hello", "goodbye", 1);

  TEST_ASSERT_EQUAL(FRG_TESTING_STRING_COMPARISON_RESULT_KIND_DIFFERENT_COMPLETE,
                    result->kind);

  TEST_ASSERT_NOT_NULL(result->entries);
  TEST_ASSERT_EQUAL(
    FRG_TESTING_STRING_COMPARISON_ENTRY_KIND_DIFFERENT,
    ((frg_testing_string_comparison_entry_t*)result->entries->data)->kind);
  TEST_ASSERT_EQUAL_STRING(
    "hello",
    ((frg_testing_string_comparison_entry_t*)result->entries->data)->expected->str);
  TEST_ASSERT_EQUAL_STRING(
    "goodbye",
    ((frg_testing_string_comparison_entry_t*)result->entries->data)->actual->str);

  TEST_ASSERT_NULL(result->entries->next);

  frg_testing_string_comparison_result_destroy(result);
}

void test_compare_strings_newline_and_empty() {
  frg_testing_string_comparison_result_t* result
    = frg_testing_compare_strings("\n", "", 1);

  TEST_ASSERT_EQUAL(FRG_TESTING_STRING_COMPARISON_RESULT_KIND_DIFFERENT_COMPLETE,
                    result->kind);

  TEST_ASSERT_NOT_NULL(result->entries);
  TEST_ASSERT_EQUAL(
    FRG_TESTING_STRING_COMPARISON_ENTRY_KIND_ONLY_EXPECTED,
    ((frg_testing_string_comparison_entry_t*)result->entries->data)->kind);
  TEST_ASSERT_EQUAL_STRING(
    "", ((frg_testing_string_comparison_entry_t*)result->entries->data)->expected->str);
  TEST_ASSERT_NULL(
    ((frg_testing_string_comparison_entry_t*)result->entries->data)->actual);

  TEST_ASSERT_NOT_NULL(result->entries->next);
  TEST_ASSERT_EQUAL(
    FRG_TESTING_STRING_COMPARISON_ENTRY_KIND_ONLY_EXPECTED,
    ((frg_testing_string_comparison_entry_t*)result->entries->next->data)->kind);
  TEST_ASSERT_EQUAL_STRING(
    "",
    ((frg_testing_string_comparison_entry_t*)result->entries->next->data)
      ->expected->str);
  TEST_ASSERT_NULL(
    ((frg_testing_string_comparison_entry_t*)result->entries->next->data)->actual);

  TEST_ASSERT_NULL(result->entries->next->next);

  frg_testing_string_comparison_result_destroy(result);
}

void test_compare_strings_matching_then_not() {
  frg_testing_string_comparison_result_t* result
    = frg_testing_compare_strings("hello\nhello", "hello\ngoodbye", 1);

  TEST_ASSERT_EQUAL(FRG_TESTING_STRING_COMPARISON_RESULT_KIND_DIFFERENT_COMPLETE,
                    result->kind);

  TEST_ASSERT_NOT_NULL(result->entries);
  TEST_ASSERT_EQUAL(
    FRG_TESTING_STRING_COMPARISON_ENTRY_KIND_MATCHING,
    ((frg_testing_string_comparison_entry_t*)result->entries->data)->kind);
  TEST_ASSERT_NULL(
    ((frg_testing_string_comparison_entry_t*)result->entries->data)->expected);
  TEST_ASSERT_EQUAL_STRING(
    "hello",
    ((frg_testing_string_comparison_entry_t*)result->entries->data)->actual->str);

  TEST_ASSERT_NOT_NULL(result->entries->next);
  TEST_ASSERT_EQUAL(
    FRG_TESTING_STRING_COMPARISON_ENTRY_KIND_DIFFERENT,
    ((frg_testing_string_comparison_entry_t*)result->entries->next->data)->kind);
  TEST_ASSERT_EQUAL_STRING(
    "hello",
    ((frg_testing_string_comparison_entry_t*)result->entries->next->data)
      ->expected->str);
  TEST_ASSERT_EQUAL_STRING(
    "goodbye",
    ((frg_testing_string_comparison_entry_t*)result->entries->next->data)->actual->str);

  TEST_ASSERT_NULL(result->entries->next->next);

  frg_testing_string_comparison_result_destroy(result);
}

void test_compare_strings_context_1_after_1() {
  frg_testing_string_comparison_result_t* result
    = frg_testing_compare_strings("a\nb\nc", "a\nB\nc", 1);

  TEST_ASSERT_EQUAL(FRG_TESTING_STRING_COMPARISON_RESULT_KIND_DIFFERENT_COMPLETE,
                    result->kind);

  frg_testing_string_comparison_result_destroy(result);
}

void test_compare_strings_context_1_after_2() {
  frg_testing_string_comparison_result_t* result
    = frg_testing_compare_strings("a\nb\nc\nd", "a\nB\nc\nd", 1);

  TEST_ASSERT_EQUAL(FRG_TESTING_STRING_COMPARISON_RESULT_KIND_DIFFERENT_INCOMPLETE,
                    result->kind);

  frg_testing_string_comparison_result_destroy(result);
}

void test_compare_strings_context_2_after_1() {
  frg_testing_string_comparison_result_t* result
    = frg_testing_compare_strings("a\nb\nc", "a\nB\nc", 2);

  TEST_ASSERT_EQUAL(FRG_TESTING_STRING_COMPARISON_RESULT_KIND_DIFFERENT_COMPLETE,
                    result->kind);

  frg_testing_string_comparison_result_destroy(result);
}

void test_compare_strings_context_2_after_2() {
  frg_testing_string_comparison_result_t* result
    = frg_testing_compare_strings("a\nb\nc\nd", "a\nB\nc\nd", 2);

  TEST_ASSERT_EQUAL(FRG_TESTING_STRING_COMPARISON_RESULT_KIND_DIFFERENT_COMPLETE,
                    result->kind);

  frg_testing_string_comparison_result_destroy(result);
}

void test_compare_strings_context_2_after_3() {
  frg_testing_string_comparison_result_t* result
    = frg_testing_compare_strings("a\nb\nc\nd\ne", "a\nB\nc\nd\ne", 2);

  TEST_ASSERT_EQUAL(FRG_TESTING_STRING_COMPARISON_RESULT_KIND_DIFFERENT_INCOMPLETE,
                    result->kind);

  frg_testing_string_comparison_result_destroy(result);
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_print_entry_matching);
  RUN_TEST(test_print_entry_only_expected);
  RUN_TEST(test_print_entry_only_actual);
  RUN_TEST(test_print_entry_different_0);
  RUN_TEST(test_print_entry_different_1);
  RUN_TEST(test_print_result_matching);
  RUN_TEST(test_print_result_different_incomplete);
  RUN_TEST(test_print_result_different_complete);
  RUN_TEST(test_compare_strings_empty_and_empty);
  RUN_TEST(test_compare_strings_empty_and_one_line);
  RUN_TEST(test_compare_strings_one_line_and_empty);
  RUN_TEST(test_compare_strings_one_line_different);
  RUN_TEST(test_compare_strings_newline_and_empty);
  RUN_TEST(test_compare_strings_matching_then_not);
  RUN_TEST(test_compare_strings_context_1_after_1);
  RUN_TEST(test_compare_strings_context_1_after_2);
  RUN_TEST(test_compare_strings_context_2_after_1);
  RUN_TEST(test_compare_strings_context_2_after_2);
  RUN_TEST(test_compare_strings_context_2_after_3);
  return UNITY_END();
}
