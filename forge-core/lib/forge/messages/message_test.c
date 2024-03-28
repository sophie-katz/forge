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

#include <forge/messages/message.h>
#include <unity.h>

void setUp() {}

void tearDown() {}

void test_message_matches_query() {
  // Create message
  frg_parsing_range_t source_range = {
    .start  = {.line_number = 2, .column_number = 3, .offset = 4, .path = "--"},
    .length = 5,
  };

  frg_message_t* message                           = frg_message_new("log_path",
                                           1,
                                           &source_range,
                                           FRG_MESSAGE_SEVERITY_ERROR,
                                           "CODE",
                                           g_string_new("Text"));

  // Test queries
  frg_message_query_t query_only_severity_matching = frg_message_query_empty;
  query_only_severity_matching.with_severity       = FRG_MESSAGE_SEVERITY_ERROR;

  TEST_ASSERT_TRUE(frg_message_matches_query(message, &query_only_severity_matching));

  frg_message_query_t query_only_severity_mismatched = frg_message_query_empty;
  query_only_severity_mismatched.with_severity       = FRG_MESSAGE_SEVERITY_WARNING;

  TEST_ASSERT_FALSE(
    frg_message_matches_query(message, &query_only_severity_mismatched));

  frg_message_query_t query_with_code_matching = frg_message_query_empty;
  query_with_code_matching.with_severity       = FRG_MESSAGE_SEVERITY_ERROR;
  query_with_code_matching.with_code           = "CODE";

  TEST_ASSERT_TRUE(frg_message_matches_query(message, &query_with_code_matching));

  frg_message_query_t query_with_code_mismatched = frg_message_query_empty;
  query_with_code_mismatched.with_severity       = FRG_MESSAGE_SEVERITY_ERROR;
  query_with_code_mismatched.with_code           = "asdf";

  TEST_ASSERT_FALSE(frg_message_matches_query(message, &query_with_code_mismatched));

  frg_message_query_t query_with_text_matching = frg_message_query_empty;
  query_with_text_matching.with_severity       = FRG_MESSAGE_SEVERITY_ERROR;
  query_with_text_matching.with_text           = "Text";

  TEST_ASSERT_TRUE(frg_message_matches_query(message, &query_with_text_matching));

  frg_message_query_t query_with_text_mismatched = frg_message_query_empty;
  query_with_text_mismatched.with_severity       = FRG_MESSAGE_SEVERITY_ERROR;
  query_with_text_mismatched.with_text           = "asdf";

  TEST_ASSERT_FALSE(frg_message_matches_query(message, &query_with_text_mismatched));

  frg_message_query_t query_on_line_matching = frg_message_query_empty;
  query_on_line_matching.with_severity       = FRG_MESSAGE_SEVERITY_ERROR;
  query_on_line_matching.on_line             = 2;

  TEST_ASSERT_TRUE(frg_message_matches_query(message, &query_on_line_matching));

  frg_message_query_t query_on_line_mismatched = frg_message_query_empty;
  query_on_line_mismatched.with_severity       = FRG_MESSAGE_SEVERITY_ERROR;
  query_on_line_mismatched.on_line             = 3;

  TEST_ASSERT_FALSE(frg_message_matches_query(message, &query_on_line_mismatched));

  frg_message_query_t query_from_path_matching = frg_message_query_empty;
  query_from_path_matching.with_severity       = FRG_MESSAGE_SEVERITY_ERROR;
  query_from_path_matching.from_path           = "--";

  TEST_ASSERT_TRUE(frg_message_matches_query(message, &query_from_path_matching));

  frg_message_query_t query_from_path_mismatched = frg_message_query_empty;
  query_from_path_mismatched.with_severity       = FRG_MESSAGE_SEVERITY_ERROR;
  query_from_path_mismatched.from_path           = "asdf";

  TEST_ASSERT_FALSE(frg_message_matches_query(message, &query_from_path_mismatched));

  frg_message_destroy(message);
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_message_matches_query);
  return UNITY_END();
}
