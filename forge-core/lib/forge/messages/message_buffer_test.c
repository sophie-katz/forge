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

void test_has_message_with_text() {
  frg_message_buffer_t* message_buffer = frg_message_buffer_new();

  frg_message_emit(message_buffer, FRG_MESSAGE_SEVERITY_ERROR, NULL, "Hi");

  TEST_ASSERT(frg_message_buffer_has_message_with_text_equal_to(message_buffer, "Hi"));
  TEST_ASSERT_FALSE(
    frg_message_buffer_has_message_with_text_equal_to(message_buffer, "Bye"));
  TEST_ASSERT_FALSE(
    frg_message_buffer_has_message_with_text_equal_to(message_buffer, "Hii"));
  TEST_ASSERT_FALSE(
    frg_message_buffer_has_message_with_text_equal_to(message_buffer, "H"));

  frg_message_buffer_destroy(message_buffer);
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_counts);
  RUN_TEST(test_has_message_with_text);
  return UNITY_END();
}
