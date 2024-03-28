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

#include <forge/lexical_casts/uint.h>
#include <unity.h>

void setUp() {
  frg_stream_output_init();
}

void tearDown() {
  frg_stream_output_cleanup();
}

void test_format_uint_0_base2() {
  frg_stream_output_t* stream
    = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
  frg_print_uint(stream, 0, 2, 0);
  GString* str = frg_stream_output_destroy_take_buffer(stream);
  TEST_ASSERT_EQUAL_STRING("0b0", str->str);
  g_string_free(str, TRUE);
}

void test_format_uint_1_base2() {
  frg_stream_output_t* stream
    = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
  frg_print_uint(stream, 1, 2, 0);
  GString* str = frg_stream_output_destroy_take_buffer(stream);
  TEST_ASSERT_EQUAL_STRING("0b1", str->str);
  g_string_free(str, TRUE);
}

void test_format_uint_10_base2() {
  frg_stream_output_t* stream
    = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
  frg_print_uint(stream, 2, 2, 0);
  GString* str = frg_stream_output_destroy_take_buffer(stream);
  TEST_ASSERT_EQUAL_STRING("0b10", str->str);
  g_string_free(str, TRUE);
}

void test_format_uint_11_base2() {
  frg_stream_output_t* stream
    = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
  frg_print_uint(stream, 3, 2, 0);
  GString* str = frg_stream_output_destroy_take_buffer(stream);
  TEST_ASSERT_EQUAL_STRING("0b11", str->str);
  g_string_free(str, TRUE);
}

void test_format_uint_100_base2() {
  frg_stream_output_t* stream
    = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
  frg_print_uint(stream, 4, 2, 0);
  GString* str = frg_stream_output_destroy_take_buffer(stream);
  TEST_ASSERT_EQUAL_STRING("0b100", str->str);
  g_string_free(str, TRUE);
}

void test_format_uint_111111111_base2() {
  frg_stream_output_t* stream
    = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
  frg_print_uint(stream, 0xff, 2, 0);
  GString* str = frg_stream_output_destroy_take_buffer(stream);
  TEST_ASSERT_EQUAL_STRING("0b11111111", str->str);
  g_string_free(str, TRUE);
}

void test_format_uint_0_base8() {
  frg_stream_output_t* stream
    = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
  frg_print_uint(stream, 0, 8, 0);
  GString* str = frg_stream_output_destroy_take_buffer(stream);
  TEST_ASSERT_EQUAL_STRING("0o0", str->str);
  g_string_free(str, TRUE);
}

void test_format_uint_7_base8() {
  frg_stream_output_t* stream
    = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
  frg_print_uint(stream, 7, 8, 0);
  GString* str = frg_stream_output_destroy_take_buffer(stream);
  TEST_ASSERT_EQUAL_STRING("0o7", str->str);
  g_string_free(str, TRUE);
}

void test_format_uint_10_base8() {
  frg_stream_output_t* stream
    = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
  frg_print_uint(stream, 010, 8, 0);
  GString* str = frg_stream_output_destroy_take_buffer(stream);
  TEST_ASSERT_EQUAL_STRING("0o10", str->str);
  g_string_free(str, TRUE);
}

void test_format_uint_77_base8() {
  frg_stream_output_t* stream
    = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
  frg_print_uint(stream, 077, 8, 0);
  GString* str = frg_stream_output_destroy_take_buffer(stream);
  TEST_ASSERT_EQUAL_STRING("0o77", str->str);
  g_string_free(str, TRUE);
}

void test_format_uint_100_base8() {
  frg_stream_output_t* stream
    = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
  frg_print_uint(stream, 0100, 8, 0);
  GString* str = frg_stream_output_destroy_take_buffer(stream);
  TEST_ASSERT_EQUAL_STRING("0o100", str->str);
  g_string_free(str, TRUE);
}

void test_format_uint_7777777_base8() {
  frg_stream_output_t* stream
    = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
  frg_print_uint(stream, 07777777, 8, 0);
  GString* str = frg_stream_output_destroy_take_buffer(stream);
  TEST_ASSERT_EQUAL_STRING("0o7777777", str->str);
  g_string_free(str, TRUE);
}

void test_format_uint_0_base10() {
  frg_stream_output_t* stream
    = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
  frg_print_uint(stream, 0, 10, 0);
  GString* str = frg_stream_output_destroy_take_buffer(stream);
  TEST_ASSERT_EQUAL_STRING("0", str->str);
  g_string_free(str, TRUE);
}

void test_format_uint_9_base10() {
  frg_stream_output_t* stream
    = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
  frg_print_uint(stream, 9, 10, 0);
  GString* str = frg_stream_output_destroy_take_buffer(stream);
  TEST_ASSERT_EQUAL_STRING("9", str->str);
  g_string_free(str, TRUE);
}

void test_format_uint_10_base10() {
  frg_stream_output_t* stream
    = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
  frg_print_uint(stream, 10, 10, 0);
  GString* str = frg_stream_output_destroy_take_buffer(stream);
  TEST_ASSERT_EQUAL_STRING("10", str->str);
  g_string_free(str, TRUE);
}

void test_format_uint_99_base10() {
  frg_stream_output_t* stream
    = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
  frg_print_uint(stream, 99, 10, 0);
  GString* str = frg_stream_output_destroy_take_buffer(stream);
  TEST_ASSERT_EQUAL_STRING("99", str->str);
  g_string_free(str, TRUE);
}

void test_format_uint_100_base10() {
  frg_stream_output_t* stream
    = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
  frg_print_uint(stream, 100, 10, 0);
  GString* str = frg_stream_output_destroy_take_buffer(stream);
  TEST_ASSERT_EQUAL_STRING("100", str->str);
  g_string_free(str, TRUE);
}

void test_format_uint_123456789_base10() {
  frg_stream_output_t* stream
    = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
  frg_print_uint(stream, 123456789, 10, 0);
  GString* str = frg_stream_output_destroy_take_buffer(stream);
  TEST_ASSERT_EQUAL_STRING("123456789", str->str);
  g_string_free(str, TRUE);
}

void test_format_uint_0_base16() {
  frg_stream_output_t* stream
    = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
  frg_print_uint(stream, 0x0, 16, 0);
  GString* str = frg_stream_output_destroy_take_buffer(stream);
  TEST_ASSERT_EQUAL_STRING("0x0", str->str);
  g_string_free(str, TRUE);
}

void test_format_uint_9_base16() {
  frg_stream_output_t* stream
    = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
  frg_print_uint(stream, 0x9, 16, 0);
  GString* str = frg_stream_output_destroy_take_buffer(stream);
  TEST_ASSERT_EQUAL_STRING("0x9", str->str);
  g_string_free(str, TRUE);
}

void test_format_uint_a_base16() {
  frg_stream_output_t* stream
    = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
  frg_print_uint(stream, 0xa, 16, 0);
  GString* str = frg_stream_output_destroy_take_buffer(stream);
  TEST_ASSERT_EQUAL_STRING("0xa", str->str);
  g_string_free(str, TRUE);
}

void test_format_uint_f_base16() {
  frg_stream_output_t* stream
    = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
  frg_print_uint(stream, 0xf, 16, 0);
  GString* str = frg_stream_output_destroy_take_buffer(stream);
  TEST_ASSERT_EQUAL_STRING("0xf", str->str);
  g_string_free(str, TRUE);
}

void test_format_uint_10_base16() {
  frg_stream_output_t* stream
    = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
  frg_print_uint(stream, 0x10, 16, 0);
  GString* str = frg_stream_output_destroy_take_buffer(stream);
  TEST_ASSERT_EQUAL_STRING("0x10", str->str);
  g_string_free(str, TRUE);
}

void test_format_uint_99_base16() {
  frg_stream_output_t* stream
    = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
  frg_print_uint(stream, 0x99, 16, 0);
  GString* str = frg_stream_output_destroy_take_buffer(stream);
  TEST_ASSERT_EQUAL_STRING("0x99", str->str);
  g_string_free(str, TRUE);
}

void test_format_uint_aa_base16() {
  frg_stream_output_t* stream
    = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
  frg_print_uint(stream, 0xaa, 16, 0);
  GString* str = frg_stream_output_destroy_take_buffer(stream);
  TEST_ASSERT_EQUAL_STRING("0xaa", str->str);
  g_string_free(str, TRUE);
}

void test_format_uint_ff_base16() {
  frg_stream_output_t* stream
    = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
  frg_print_uint(stream, 0xff, 16, 0);
  GString* str = frg_stream_output_destroy_take_buffer(stream);
  TEST_ASSERT_EQUAL_STRING("0xff", str->str);
  g_string_free(str, TRUE);
}

void test_format_uint_100_base16() {
  frg_stream_output_t* stream
    = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
  frg_print_uint(stream, 0x100, 16, 0);
  GString* str = frg_stream_output_destroy_take_buffer(stream);
  TEST_ASSERT_EQUAL_STRING("0x100", str->str);
  g_string_free(str, TRUE);
}

void test_format_uint_ffffffff_base16() {
  frg_stream_output_t* stream
    = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
  frg_print_uint(stream, 0xffffffff, 16, 0);
  GString* str = frg_stream_output_destroy_take_buffer(stream);
  TEST_ASSERT_EQUAL_STRING("0xffffffff", str->str);
  g_string_free(str, TRUE);
}

void test_format_uint_suffix_signed_32() {
  frg_stream_output_t* stream
    = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
  frg_print_uint_suffix(stream, true, 32);
  GString* str = frg_stream_output_destroy_take_buffer(stream);
  TEST_ASSERT_EQUAL_STRING("i32", str->str);
  g_string_free(str, TRUE);
}

void test_format_uint_suffix_unsigned_32() {
  frg_stream_output_t* stream
    = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
  frg_print_uint_suffix(stream, false, 32);
  GString* str = frg_stream_output_destroy_take_buffer(stream);
  TEST_ASSERT_EQUAL_STRING("u32", str->str);
  g_string_free(str, TRUE);
}

void _test_parse_uint(uint64_t expected_value,
                      bool expected_is_signed,
                      frg_bit_width_t expected_bit_width,
                      const char* formatted) {
  frg_message_buffer_t* message_buffer = frg_message_buffer_new();

  frg_parse_uint_result_t actual;
  frg_parsing_location_t start
    = { .path = "--", .line_number = 1, .column_number = 1, .offset = 0 };

  frg_parsing_token_reader_t* reader = frg_parsing_token_reader_new(&start, formatted);

  bool result                        = frg_parse_uint(&actual, message_buffer, reader);

  frg_message_buffer_print(frg_stream_output_get_stderr(),
                           NULL,
                           message_buffer,
                           FRG_MESSAGE_SEVERITY_DEBUG,
                           0);

  TEST_ASSERT(result);

  frg_parsing_token_reader_destroy(reader);

  TEST_ASSERT_EQUAL(expected_is_signed, actual.is_signed);
  TEST_ASSERT_EQUAL(expected_bit_width, actual.bit_width);
  TEST_ASSERT_EQUAL(expected_value, actual.value);

  TEST_ASSERT_EQUAL(0, message_buffer->message_count);

  frg_message_buffer_destroy(message_buffer);
}

void test_parse_uint_0() {
  _test_parse_uint(0, false, 64, "0");
}

void test_parse_uint_1() {
  _test_parse_uint(1, false, 64, "1");
}

void test_parse_uint_9() {
  _test_parse_uint(9, false, 64, "9");
}

void test_parse_uint_10() {
  _test_parse_uint(10, false, 64, "10");
}

void test_parse_uint_99() {
  _test_parse_uint(99, false, 64, "99");
}

void test_parse_uint_100() {
  _test_parse_uint(100, false, 64, "100");
}

void test_parse_uint_999() {
  _test_parse_uint(999, false, 64, "999");
}

void test_parse_uint_123456789() {
  _test_parse_uint(123456789, false, 64, "123456789");
}

void test_parse_uint_0b0() {
  _test_parse_uint(0, false, 64, "0b0");
}

void test_parse_uint_0b1() {
  // Hello there
  _test_parse_uint(1, false, 64, "0b1");
}

void test_parse_uint_0b10() {
  _test_parse_uint(2, false, 64, "0b10");
}

void test_parse_uint_0b11() {
  _test_parse_uint(3, false, 64, "0b11");
}

void test_parse_uint_0b11111111() {
  _test_parse_uint(0xff, false, 64, "0b11111111");
}

void test_parse_uint_0o0() {
  _test_parse_uint(0, false, 64, "0o0");
}

void test_parse_uint_0o7() {
  _test_parse_uint(07, false, 64, "0o7");
}

void test_parse_uint_0o10() {
  _test_parse_uint(010, false, 64, "0o10");
}

void test_parse_uint_0o77777777() {
  _test_parse_uint(077777777, false, 64, "0o77777777");
}

void test_parse_uint_0i8() {
  _test_parse_uint(0, true, 8, "0i8");
}

void test_parse_uint_0u32() {
  _test_parse_uint(0, false, 32, "0u32");
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_format_uint_0_base2);
  RUN_TEST(test_format_uint_1_base2);
  RUN_TEST(test_format_uint_10_base2);
  RUN_TEST(test_format_uint_11_base2);
  RUN_TEST(test_format_uint_100_base2);
  RUN_TEST(test_format_uint_111111111_base2);
  RUN_TEST(test_format_uint_0_base8);
  RUN_TEST(test_format_uint_7_base8);
  RUN_TEST(test_format_uint_10_base8);
  RUN_TEST(test_format_uint_77_base8);
  RUN_TEST(test_format_uint_100_base8);
  RUN_TEST(test_format_uint_7777777_base8);
  RUN_TEST(test_format_uint_0_base10);
  RUN_TEST(test_format_uint_9_base10);
  RUN_TEST(test_format_uint_10_base10);
  RUN_TEST(test_format_uint_99_base10);
  RUN_TEST(test_format_uint_100_base10);
  RUN_TEST(test_format_uint_123456789_base10);
  RUN_TEST(test_format_uint_0_base16);
  RUN_TEST(test_format_uint_9_base16);
  RUN_TEST(test_format_uint_a_base16);
  RUN_TEST(test_format_uint_f_base16);
  RUN_TEST(test_format_uint_10_base16);
  RUN_TEST(test_format_uint_99_base16);
  RUN_TEST(test_format_uint_aa_base16);
  RUN_TEST(test_format_uint_ff_base16);
  RUN_TEST(test_format_uint_100_base16);
  RUN_TEST(test_format_uint_ffffffff_base16);
  RUN_TEST(test_format_uint_suffix_signed_32);
  RUN_TEST(test_format_uint_suffix_unsigned_32);
  RUN_TEST(test_parse_uint_0);
  RUN_TEST(test_parse_uint_1);
  RUN_TEST(test_parse_uint_9);
  RUN_TEST(test_parse_uint_10);
  RUN_TEST(test_parse_uint_99);
  RUN_TEST(test_parse_uint_100);
  RUN_TEST(test_parse_uint_999);
  RUN_TEST(test_parse_uint_123456789);
  RUN_TEST(test_parse_uint_0b0);
  RUN_TEST(test_parse_uint_0b1);
  RUN_TEST(test_parse_uint_0b10);
  RUN_TEST(test_parse_uint_0b11);
  RUN_TEST(test_parse_uint_0b11111111);
  RUN_TEST(test_parse_uint_0o0);
  RUN_TEST(test_parse_uint_0o7);
  RUN_TEST(test_parse_uint_0o10);
  RUN_TEST(test_parse_uint_0o77777777);
  RUN_TEST(test_parse_uint_0i8);
  RUN_TEST(test_parse_uint_0u32);
  return UNITY_END();
}
