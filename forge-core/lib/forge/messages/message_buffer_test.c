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

#include <forge/messages/message_buffer.h>
#include <unity.h>

void setUp() {}

void tearDown() {}

void test_counts() {
  frg_message_buffer_t* message_buffer = frg_message_buffer_new();

  TEST_ASSERT_EQUAL(0, message_buffer->message_count);
  TEST_ASSERT_EQUAL(0, message_buffer->error_count);
  TEST_ASSERT_EQUAL(0, message_buffer->warning_count);

  frg_message_emit(message_buffer, FRG_MESSAGE_SEVERITY_DEBUG, NULL, "Hi");

  TEST_ASSERT_EQUAL(1, message_buffer->message_count);
  TEST_ASSERT_EQUAL(0, message_buffer->error_count);
  TEST_ASSERT_EQUAL(0, message_buffer->warning_count);

  frg_message_emit(message_buffer, FRG_MESSAGE_SEVERITY_NOTE, NULL, "Hi");

  TEST_ASSERT_EQUAL(2, message_buffer->message_count);
  TEST_ASSERT_EQUAL(0, message_buffer->error_count);
  TEST_ASSERT_EQUAL(0, message_buffer->warning_count);

  frg_message_emit(message_buffer, FRG_MESSAGE_SEVERITY_WARNING, NULL, "Hi");

  TEST_ASSERT_EQUAL(3, message_buffer->message_count);
  TEST_ASSERT_EQUAL(0, message_buffer->error_count);
  TEST_ASSERT_EQUAL(1, message_buffer->warning_count);

  frg_message_emit(message_buffer, FRG_MESSAGE_SEVERITY_ERROR, NULL, "Hi");

  TEST_ASSERT_EQUAL(4, message_buffer->message_count);
  TEST_ASSERT_EQUAL(1, message_buffer->error_count);
  TEST_ASSERT_EQUAL(1, message_buffer->warning_count);

  frg_message_emit(message_buffer, FRG_MESSAGE_SEVERITY_FATAL_ERROR, NULL, "Hi");

  TEST_ASSERT_EQUAL(5, message_buffer->message_count);
  TEST_ASSERT_EQUAL(2, message_buffer->error_count);
  TEST_ASSERT_EQUAL(1, message_buffer->warning_count);

  frg_message_emit(message_buffer, FRG_MESSAGE_SEVERITY_INTERNAL_ERROR, NULL, "Hi");

  TEST_ASSERT_EQUAL(6, message_buffer->message_count);
  TEST_ASSERT_EQUAL(3, message_buffer->error_count);
  TEST_ASSERT_EQUAL(1, message_buffer->warning_count);

  frg_message_buffer_destroy(message_buffer);
}

void test_query() {
  frg_message_buffer_t* message_buffer = frg_message_buffer_new();

  frg_message_emit(message_buffer, FRG_MESSAGE_SEVERITY_ERROR, NULL, "Hi");
  frg_message_emit(message_buffer, FRG_MESSAGE_SEVERITY_ERROR, NULL, "Bye");

  frg_message_query_t query_0 = frg_message_query_empty;

  frg_message_query_t query_1 = frg_message_query_empty;
  query_1.with_severity       = FRG_MESSAGE_SEVERITY_ERROR;
  query_1.with_text           = "Hi";

  frg_message_query_t query_2 = frg_message_query_empty;
  query_2.with_severity       = FRG_MESSAGE_SEVERITY_ERROR;

  TEST_ASSERT_EQUAL(0, frg_message_buffer_query_count(message_buffer, &query_0));
  TEST_ASSERT_EQUAL(1, frg_message_buffer_query_count(message_buffer, &query_1));
  TEST_ASSERT_EQUAL(2, frg_message_buffer_query_count(message_buffer, &query_2));

  frg_message_t* message = (frg_message_t*)1;

  TEST_ASSERT_EQUAL(
    FRG_MESSAGE_BUFFER_QUERY_RESULT_NONE,
    frg_message_buffer_query_single(&message, message_buffer, &query_0));
  TEST_ASSERT_NULL(message);

  TEST_ASSERT_EQUAL(
    FRG_MESSAGE_BUFFER_QUERY_RESULT_SINGLE,
    frg_message_buffer_query_single(&message, message_buffer, &query_1));
  TEST_ASSERT_NOT_NULL(message);
  TEST_ASSERT_EQUAL_STRING("Hi", message->text->str);

  message = (frg_message_t*)1;

  TEST_ASSERT_EQUAL(
    FRG_MESSAGE_BUFFER_QUERY_RESULT_MULTIPLE,
    frg_message_buffer_query_single(&message, message_buffer, &query_2));
  TEST_ASSERT_NULL(message);

  GList* results = frg_message_buffer_query(message_buffer, &query_0);
  TEST_ASSERT_NULL(results);

  results = frg_message_buffer_query(message_buffer, &query_1);
  TEST_ASSERT_EQUAL(1, g_list_length(results));
  g_list_free(results);

  results = frg_message_buffer_query(message_buffer, &query_2);
  TEST_ASSERT_EQUAL(2, g_list_length(results));
  g_list_free(results);

  frg_message_buffer_destroy(message_buffer);
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_counts);
  RUN_TEST(test_query);
  return UNITY_END();
}
