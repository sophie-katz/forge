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

#include <forge/cli/option_set.h>
#include <unity.h>

void setUp() {}

void tearDown() {}

typedef struct {
  bool flag;
  bool flag_short;
  const char* argument;
  const char* argument_short;
  const char* choice;
  const char* choice_short;
} config;

bool callback_flag(frg_message_buffer_t* message_buffer,
                   void* mut_user_data,
                   const char* value) {
  ((config*)mut_user_data)->flag = true;
  return true;
}

bool callback_flag_short(frg_message_buffer_t* message_buffer,
                         void* mut_user_data,
                         const char* value) {
  ((config*)mut_user_data)->flag_short = true;
  return true;
}

bool callback_argument(frg_message_buffer_t* message_buffer,
                       void* mut_user_data,
                       const char* value) {
  ((config*)mut_user_data)->argument = value;
  return true;
}

bool callback_argument_short(frg_message_buffer_t* message_buffer,
                             void* mut_user_data,
                             const char* value) {
  ((config*)mut_user_data)->argument_short = value;
  return true;
}

bool callback_choice(frg_message_buffer_t* message_buffer,
                     void* mut_user_data,
                     const char* value) {
  ((config*)mut_user_data)->choice = value;
  return true;
}

bool callback_choice_short(frg_message_buffer_t* message_buffer,
                           void* mut_user_data,
                           const char* value) {
  ((config*)mut_user_data)->choice_short = value;
  return true;
}

frg_cli_option_set_t* create_option_set() {
  frg_cli_option_set_t* option_set = frg_cli_option_set_new();

  frg_cli_option_set_add_option(
    option_set,
    frg_cli_option_new_flag("long-flag", "A long-only flag.", callback_flag));

  frg_cli_option_set_add_option(
    option_set,
    frg_cli_option_new_flag_short(
      'f', "short-flag", "A long and short flag.", callback_flag_short));

  frg_cli_option_set_add_option(
    option_set,
    frg_cli_option_new_argument(
      "long-argument", "value", "A long-only argument.", callback_argument));

  frg_cli_option_set_add_option(
    option_set,
    frg_cli_option_new_argument_short('a',
                                      "short-argument",
                                      "value",
                                      "A long and short argument.",
                                      callback_argument_short));

  frg_cli_option_t* option = frg_cli_option_new_choice(
    "long-choice", "value", "A long-only choice.", callback_choice);

  frg_cli_option_add_choice(option, frg_cli_choice_new("option-1", "A choice."));

  frg_cli_option_add_choice(option, frg_cli_choice_new("option-2", "Another choice."));

  frg_cli_option_set_add_option(option_set, option);

  option = frg_cli_option_new_choice_short(
    'g', "short-choice", "value", "A long and short choice.", callback_choice_short);

  frg_cli_option_add_choice(option, frg_cli_choice_new("option-1", "A choice."));

  frg_cli_option_add_choice(option, frg_cli_choice_new("option-2", "Another choice."));

  frg_cli_option_set_add_option(option_set, option);

  return option_set;
}

void init_config(config* c) {
  c->flag           = false;
  c->flag_short     = false;
  c->argument       = NULL;
  c->argument_short = NULL;
  c->choice         = NULL;
  c->choice_short   = NULL;
}

void test_flag_long() {
  frg_message_buffer_t* message_buffer = frg_message_buffer_new();

  frg_cli_option_set_t* option_set     = create_option_set();

  int argi                             = 1;
  int argc                             = 2;
  const char* argv[]                   = { "forge", "--long-flag", NULL };

  config c;
  init_config(&c);

  bool result
    = frg_cli_option_set_parse_next(message_buffer, &argi, &c, option_set, argc, argv);

  TEST_ASSERT_TRUE(result);
  TEST_ASSERT_EQUAL(2, argi);

  TEST_ASSERT_EQUAL(0, message_buffer->message_count);

  TEST_ASSERT_TRUE(c.flag);
  TEST_ASSERT_FALSE(c.flag_short);
  TEST_ASSERT_NULL(c.argument);
  TEST_ASSERT_NULL(c.argument_short);
  TEST_ASSERT_NULL(c.choice);
  TEST_ASSERT_NULL(c.choice_short);

  frg_cli_option_set_destroy(option_set);
  frg_message_buffer_destroy(message_buffer);
}

void test_argument_short() {
  frg_message_buffer_t* message_buffer = frg_message_buffer_new();

  frg_cli_option_set_t* option_set     = create_option_set();

  int argi                             = 1;
  int argc                             = 3;
  const char* argv[]                   = { "forge", "-a", "asdf", NULL };

  config c;
  init_config(&c);

  bool result
    = frg_cli_option_set_parse_next(message_buffer, &argi, &c, option_set, argc, argv);

  TEST_ASSERT_TRUE(result);
  TEST_ASSERT_EQUAL(3, argi);

  TEST_ASSERT_EQUAL(0, message_buffer->message_count);

  TEST_ASSERT_FALSE(c.flag);
  TEST_ASSERT_FALSE(c.flag_short);
  TEST_ASSERT_NULL(c.argument);
  TEST_ASSERT_EQUAL_STRING(c.argument_short, "asdf");
  TEST_ASSERT_NULL(c.choice);
  TEST_ASSERT_NULL(c.choice_short);

  frg_cli_option_set_destroy(option_set);
  frg_message_buffer_destroy(message_buffer);
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_flag_long);
  RUN_TEST(test_argument_short);
  return UNITY_END();
}
