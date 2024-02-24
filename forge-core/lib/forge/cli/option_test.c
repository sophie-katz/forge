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

#include <forge/assert.h>
#include <forge/cli/option.h>
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

frg_cli_option_t* create_option_flag() {
  return frg_cli_option_new_flag("long-flag", "A long-only flag.", callback_flag);
}

frg_cli_option_t* create_option_flag_short() {
  return frg_cli_option_new_flag_short(
    'f', "short-flag", "A long and short flag.", callback_flag_short);
}

frg_cli_option_t* create_option_argument() {
  return frg_cli_option_new_argument(
    "long-argument", "value", "A long-only argument.", callback_argument);
}

frg_cli_option_t* create_option_argument_short() {
  return frg_cli_option_new_argument_short('a',
                                           "short-argument",
                                           "value",
                                           "A long and short argument.",
                                           callback_argument_short);
}

frg_cli_option_t* create_option_choice() {
  frg_cli_option_t* option = frg_cli_option_new_choice(
    "long-choice", "value", "A long-only choice.", callback_choice);

  frg_cli_option_add_choice(option, frg_cli_choice_new("option-1", "A choice."));

  frg_cli_option_add_choice(option, frg_cli_choice_new("option-2", "Another choice."));

  return option;
}

frg_cli_option_t* create_option_choice_short() {
  frg_cli_option_t* option = frg_cli_option_new_choice_short(
    'c', "long-choice", "value", "A long-only choice.", callback_choice);

  frg_cli_option_add_choice(option, frg_cli_choice_new("option-1", "A choice."));

  frg_cli_option_add_choice(option, frg_cli_choice_new("option-2", "Another choice."));

  return option;
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

  frg_cli_option_t* option             = create_option_flag();

  int argi                             = 1;
  int argc                             = 2;
  const char* argv[]                   = { "forge", "--long-flag", NULL };

  config c;
  init_config(&c);

  bool result
    = frg_cli_option_parse_next(message_buffer, &argi, &c, option, argc, argv);

  TEST_ASSERT_TRUE(result);

  TEST_ASSERT_EQUAL(0, message_buffer->message_count);

  TEST_ASSERT_TRUE(c.flag);
  TEST_ASSERT_FALSE(c.flag_short);
  TEST_ASSERT_NULL(c.argument);
  TEST_ASSERT_NULL(c.argument_short);
  TEST_ASSERT_NULL(c.choice);
  TEST_ASSERT_NULL(c.choice_short);

  frg_cli_option_destroy(option);
  frg_message_buffer_destroy(message_buffer);
}

void test_flag_short_long() {
  frg_message_buffer_t* message_buffer = frg_message_buffer_new();

  frg_cli_option_t* option             = create_option_flag_short();

  int argi                             = 1;
  int argc                             = 2;
  const char* argv[]                   = { "forge", "--short-flag", NULL };

  config c;
  init_config(&c);

  bool result
    = frg_cli_option_parse_next(message_buffer, &argi, &c, option, argc, argv);

  TEST_ASSERT_TRUE(result);

  TEST_ASSERT_EQUAL(0, message_buffer->message_count);

  TEST_ASSERT_FALSE(c.flag);
  TEST_ASSERT_TRUE(c.flag_short);
  TEST_ASSERT_NULL(c.argument);
  TEST_ASSERT_NULL(c.argument_short);
  TEST_ASSERT_NULL(c.choice);
  TEST_ASSERT_NULL(c.choice_short);

  frg_cli_option_destroy(option);
  frg_message_buffer_destroy(message_buffer);
}

void test_flag_short_short() {
  frg_message_buffer_t* message_buffer = frg_message_buffer_new();

  frg_cli_option_t* option             = create_option_flag_short();

  int argi                             = 1;
  int argc                             = 2;
  const char* argv[]                   = { "forge", "-f", NULL };

  config c;
  init_config(&c);

  bool result
    = frg_cli_option_parse_next(message_buffer, &argi, &c, option, argc, argv);

  TEST_ASSERT_TRUE(result);

  TEST_ASSERT_EQUAL(0, message_buffer->message_count);

  TEST_ASSERT_FALSE(c.flag);
  TEST_ASSERT_TRUE(c.flag_short);
  TEST_ASSERT_NULL(c.argument);
  TEST_ASSERT_NULL(c.argument_short);
  TEST_ASSERT_NULL(c.choice);
  TEST_ASSERT_NULL(c.choice_short);

  frg_cli_option_destroy(option);
  frg_message_buffer_destroy(message_buffer);
}

void test_argument_long() {
  frg_message_buffer_t* message_buffer = frg_message_buffer_new();

  frg_cli_option_t* option             = create_option_argument();

  int argi                             = 1;
  int argc                             = 3;
  const char* argv[]                   = { "forge", "--long-argument", "asdf", NULL };

  config c;
  init_config(&c);

  bool result
    = frg_cli_option_parse_next(message_buffer, &argi, &c, option, argc, argv);

  TEST_ASSERT_TRUE(result);

  TEST_ASSERT_EQUAL(0, message_buffer->message_count);

  TEST_ASSERT_FALSE(c.flag);
  TEST_ASSERT_FALSE(c.flag_short);
  TEST_ASSERT_EQUAL_STRING("asdf", c.argument);
  TEST_ASSERT_NULL(c.argument_short);
  TEST_ASSERT_NULL(c.choice);
  TEST_ASSERT_NULL(c.choice_short);

  frg_cli_option_destroy(option);
  frg_message_buffer_destroy(message_buffer);
}

void test_argument_long_without_value() {
  frg_message_buffer_t* message_buffer = frg_message_buffer_new();

  frg_cli_option_t* option             = create_option_argument();

  int argi                             = 1;
  int argc                             = 2;
  const char* argv[]                   = { "forge", "--long-argument", NULL };

  config c;
  init_config(&c);

  bool result
    = frg_cli_option_parse_next(message_buffer, &argi, &c, option, argc, argv);

  TEST_ASSERT_FALSE(result);

  TEST_ASSERT_GREATER_THAN(0, message_buffer->error_count);

  TEST_ASSERT(frg_message_buffer_has_message_with_text_equal_to(
    message_buffer, "Argument '--long-argument <value>' must have a value passed"));

  frg_cli_option_destroy(option);
  frg_message_buffer_destroy(message_buffer);
}

void test_argument_long_with_argument_instead_of_value() {
  frg_message_buffer_t* message_buffer = frg_message_buffer_new();

  frg_cli_option_t* option             = create_option_argument();

  int argi                             = 1;
  int argc                             = 3;
  const char* argv[]                   = { "forge", "--long-argument", "-a", NULL };

  config c;
  init_config(&c);

  bool result
    = frg_cli_option_parse_next(message_buffer, &argi, &c, option, argc, argv);

  TEST_ASSERT_FALSE(result);

  TEST_ASSERT_GREATER_THAN(0, message_buffer->error_count);

  TEST_ASSERT(frg_message_buffer_has_message_with_text_equal_to(
    message_buffer,
    "Argument '----long-argument <value>' expects to have a value passed, but '-a' is "
    "an argument not a value"));

  frg_cli_option_destroy(option);
  frg_message_buffer_destroy(message_buffer);
}

void test_argument_short_long() {
  frg_message_buffer_t* message_buffer = frg_message_buffer_new();

  frg_cli_option_t* option             = create_option_argument_short();

  int argi                             = 1;
  int argc                             = 3;
  const char* argv[]                   = { "forge", "--short-argument", "asdf", NULL };

  config c;
  init_config(&c);

  bool result
    = frg_cli_option_parse_next(message_buffer, &argi, &c, option, argc, argv);

  TEST_ASSERT_TRUE(result);

  TEST_ASSERT_EQUAL(0, message_buffer->message_count);

  TEST_ASSERT_FALSE(c.flag);
  TEST_ASSERT_FALSE(c.flag_short);
  TEST_ASSERT_NULL(c.argument);
  TEST_ASSERT_EQUAL_STRING("asdf", c.argument_short);
  TEST_ASSERT_NULL(c.choice);
  TEST_ASSERT_NULL(c.choice_short);

  frg_cli_option_destroy(option);
  frg_message_buffer_destroy(message_buffer);
}

void test_argument_short_short() {
  frg_message_buffer_t* message_buffer = frg_message_buffer_new();

  frg_cli_option_t* option             = create_option_argument_short();

  int argi                             = 1;
  int argc                             = 3;
  const char* argv[]                   = { "forge", "-a", "asdf", NULL };

  config c;
  init_config(&c);

  bool result
    = frg_cli_option_parse_next(message_buffer, &argi, &c, option, argc, argv);

  TEST_ASSERT_TRUE(result);

  TEST_ASSERT_EQUAL(0, message_buffer->message_count);

  TEST_ASSERT_FALSE(c.flag);
  TEST_ASSERT_FALSE(c.flag_short);
  TEST_ASSERT_NULL(c.argument);
  TEST_ASSERT_EQUAL_STRING("asdf", c.argument_short);
  TEST_ASSERT_NULL(c.choice);
  TEST_ASSERT_NULL(c.choice_short);

  frg_cli_option_destroy(option);
  frg_message_buffer_destroy(message_buffer);
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_flag_long);
  RUN_TEST(test_flag_short_long);
  RUN_TEST(test_flag_short_short);
  RUN_TEST(test_argument_long);
  RUN_TEST(test_argument_long_without_value);
  RUN_TEST(test_argument_long_with_argument_instead_of_value);
  RUN_TEST(test_argument_short_long);
  RUN_TEST(test_argument_short_short);
  return UNITY_END();
}
