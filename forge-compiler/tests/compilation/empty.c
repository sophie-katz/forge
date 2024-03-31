// This program is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software Foundation,
// either version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
// PARTICULAR PURPOSE. See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with this
// program. If not, see <https://www.gnu.org/licenses/>.
//
// Copyright 2023-2024 Sophie Katz

#include <forge-testing/compilation_test/test.h>
#include <forge/streams/output.h>
#include <unity.h>

void setUp() {
  frg_stream_output_init();
}

void tearDown() {
  frg_stream_output_cleanup();
}

void _callback_on_messages(const frg_message_buffer_t* message_buffer,
                           void* mut_user_data) {
  TEST_ASSERT_NULL(mut_user_data);
  TEST_ASSERT_EQUAL(0, message_buffer->warning_count);
  TEST_ASSERT_EQUAL(0, message_buffer->error_count);
}

void test_empty() {
  frg_testing_compilation_test_options_t* options
    = frg_testing_compilation_test_options_new_default();

  options->kind        = FRG_TESTING_COMPILATION_TEST_KIND_EXPECT_SUCCESS;
  options->name        = "empty";

  // clang-format off
  options->source_text =
    ""
  ;
  
  options->ast_debug = 
    "[declaration-block]"
  ;

  options->llvm_ir =
    "; ModuleID = 'forge'\n"
    "source_filename = \"forge\"\n"
  ;
  // clang-format on

  options->on_messages = _callback_on_messages;

  frg_testing_test_compilation(options);

  frg_testing_compilation_test_options_destroy(options);
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_empty);
  return UNITY_END();
}
