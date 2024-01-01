// Copyright (c) 2024 Sophie Katz
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

#include <forge/common/check.h>
#include <forge/cli/option.h>
#include <unity.h>

void setUp(void) {}

void tearDown(void) {}

typedef struct {
    bool flag;
    bool flag_short;
    const char* argument;
    const char* argument_short;
    const char* choice;
    const char* choice_short;
} config;

frg_status_t callback_flag(void* user_data, const char* value) {
    ((config*)user_data)->flag = true;
    return FRG_STATUS_OK;
}

frg_status_t callback_flag_short(void* user_data, const char* value) {
    ((config*)user_data)->flag_short = true;
    return FRG_STATUS_OK;
}

frg_status_t callback_argument(void* user_data, const char* value) {
    ((config*)user_data)->argument = value;
    return FRG_STATUS_OK;
}

frg_status_t callback_argument_short(void* user_data, const char* value) {
    ((config*)user_data)->argument_short = value;
    return FRG_STATUS_OK;
}

frg_status_t callback_choice(void* user_data, const char* value) {
    ((config*)user_data)->choice = value;
    return FRG_STATUS_OK;
}

frg_status_t callback_choice_short(void* user_data, const char* value) {
    ((config*)user_data)->choice_short = value;
    return FRG_STATUS_OK;
}

frg_status_t create_option_flag(frg_cli_option_t** option) {
    return frg_cli_option_new_flag(
        option,
        "long-flag",
        "A long-only flag.",
        callback_flag
    );
}

frg_status_t create_option_flag_short(frg_cli_option_t** option) {
    return frg_cli_option_new_flag_short(
        option,
        'f',
        "short-flag",
        "A long and short flag.",
        callback_flag_short
    );
}

frg_status_t create_option_argument(frg_cli_option_t** option) {
    return frg_cli_option_new_argument(
        option,
        "long-argument",
        "value",
        "A long-only argument.",
        callback_argument
    );
}

frg_status_t create_option_argument_short(frg_cli_option_t** option) {
    return frg_cli_option_new_argument_short(
        option,
        'a',
        "short-argument",
        "value",
        "A long and short argument.",
        callback_argument_short
    );
}

frg_status_t create_option_choice(frg_cli_option_t** option) {
    frg_check(
        frg_cli_option_new_choice(
            option,
            "long-choice",
            "value",
            "A long-only choice.",
            callback_choice
        )
    );

    frg_cli_choice_t* choice = NULL;
    frg_check(
        frg_cli_choice_new(
            &choice,
            "option-1",
            "A choice."
        )
    );

    frg_check(
        frg_cli_option_add_choice(
            *option,
            choice
        )
    );

    choice = NULL;
    frg_check(
        frg_cli_choice_new(
            &choice,
            "option-2",
            "Another choice."
        )
    );

    frg_check(
        frg_cli_option_add_choice(
            *option,
            choice
        )
    );

    return FRG_STATUS_OK;
}

frg_status_t create_option_choice_short(frg_cli_option_t** option) {
    frg_check(
        frg_cli_option_new_choice_short(
            option,
            'c',
            "long-choice",
            "value",
            "A long-only choice.",
            callback_choice
        )
    );

    frg_cli_choice_t* choice = NULL;
    frg_check(
        frg_cli_choice_new(
            &choice,
            "option-1",
            "A choice."
        )
    );

    frg_check(
        frg_cli_option_add_choice(
            *option,
            choice
        )
    );

    choice = NULL;
    frg_check(
        frg_cli_choice_new(
            &choice,
            "option-2",
            "Another choice."
        )
    );

    frg_check(
        frg_cli_option_add_choice(
            *option,
            choice
        )
    );

    return FRG_STATUS_OK;
}

void init_config(config* c) {
    c->flag = false;
    c->flag_short = false;
    c->argument = NULL;
    c->argument_short = NULL;
    c->choice = NULL;
    c->choice_short = NULL;
}

void test_flag_long(void) {
    frg_cli_option_t* option = NULL;
    TEST_ASSERT_EQUAL(FRG_STATUS_OK, create_option_flag(&option));
    TEST_ASSERT_NOT_NULL(option);

    int argi = 1;
    int argc = 2;
    const char* argv[] = { "forge", "--long-flag", NULL };

    config c;
    init_config(&c);

    frg_status_t result = frg_cli_option_parse_next(
        option,
        &argi,
        argc,
        argv,
        &c
    );

    TEST_ASSERT_EQUAL(FRG_STATUS_OK, result);

    TEST_ASSERT_TRUE(c.flag);
    TEST_ASSERT_FALSE(c.flag_short);
    TEST_ASSERT_NULL(c.argument);
    TEST_ASSERT_NULL(c.argument_short);
    TEST_ASSERT_NULL(c.choice);
    TEST_ASSERT_NULL(c.choice_short);
}

void test_flag_short_long(void) {
    frg_cli_option_t* option = NULL;
    TEST_ASSERT_EQUAL(FRG_STATUS_OK, create_option_flag_short(&option));
    TEST_ASSERT_NOT_NULL(option);

    int argi = 1;
    int argc = 2;
    const char* argv[] = { "forge", "--short-flag", NULL };

    config c;
    init_config(&c);

    frg_status_t result = frg_cli_option_parse_next(
        option,
        &argi,
        argc,
        argv,
        &c
    );

    TEST_ASSERT_EQUAL(FRG_STATUS_OK, result);

    TEST_ASSERT_FALSE(c.flag);
    TEST_ASSERT_TRUE(c.flag_short);
    TEST_ASSERT_NULL(c.argument);
    TEST_ASSERT_NULL(c.argument_short);
    TEST_ASSERT_NULL(c.choice);
    TEST_ASSERT_NULL(c.choice_short);
}

void test_flag_short_short(void) {
    frg_cli_option_t* option = NULL;
    TEST_ASSERT_EQUAL(FRG_STATUS_OK, create_option_flag_short(&option));
    TEST_ASSERT_NOT_NULL(option);

    int argi = 1;
    int argc = 2;
    const char* argv[] = { "forge", "-f", NULL };

    config c;
    init_config(&c);

    frg_status_t result = frg_cli_option_parse_next(
        option,
        &argi,
        argc,
        argv,
        &c
    );

    TEST_ASSERT_EQUAL(FRG_STATUS_OK, result);

    TEST_ASSERT_FALSE(c.flag);
    TEST_ASSERT_TRUE(c.flag_short);
    TEST_ASSERT_NULL(c.argument);
    TEST_ASSERT_NULL(c.argument_short);
    TEST_ASSERT_NULL(c.choice);
    TEST_ASSERT_NULL(c.choice_short);
}

void test_argument_long(void) {
    frg_cli_option_t* option = NULL;
    TEST_ASSERT_EQUAL(FRG_STATUS_OK, create_option_argument(&option));
    TEST_ASSERT_NOT_NULL(option);

    int argi = 1;
    int argc = 3;
    const char* argv[] = { "forge", "--long-argument", "asdf", NULL };

    config c;
    init_config(&c);

    frg_status_t result = frg_cli_option_parse_next(
        option,
        &argi,
        argc,
        argv,
        &c
    );

    TEST_ASSERT_EQUAL(FRG_STATUS_OK, result);

    TEST_ASSERT_FALSE(c.flag);
    TEST_ASSERT_FALSE(c.flag_short);
    TEST_ASSERT_EQUAL_STRING("asdf", c.argument);
    TEST_ASSERT_NULL(c.argument_short);
    TEST_ASSERT_NULL(c.choice);
    TEST_ASSERT_NULL(c.choice_short);
}

void test_argument_short_long(void) {
    frg_cli_option_t* option = NULL;
    TEST_ASSERT_EQUAL(FRG_STATUS_OK, create_option_argument_short(&option));
    TEST_ASSERT_NOT_NULL(option);

    int argi = 1;
    int argc = 3;
    const char* argv[] = { "forge", "--short-argument", "asdf", NULL };

    config c;
    init_config(&c);

    frg_status_t result = frg_cli_option_parse_next(
        option,
        &argi,
        argc,
        argv,
        &c
    );

    TEST_ASSERT_EQUAL(FRG_STATUS_OK, result);

    TEST_ASSERT_FALSE(c.flag);
    TEST_ASSERT_FALSE(c.flag_short);
    TEST_ASSERT_NULL(c.argument);
    TEST_ASSERT_EQUAL_STRING("asdf", c.argument_short);
    TEST_ASSERT_NULL(c.choice);
    TEST_ASSERT_NULL(c.choice_short);
}

void test_argument_short_short(void) {
    frg_cli_option_t* option = NULL;
    TEST_ASSERT_EQUAL(FRG_STATUS_OK, create_option_argument_short(&option));
    TEST_ASSERT_NOT_NULL(option);

    int argi = 1;
    int argc = 3;
    const char* argv[] = { "forge", "-a", "asdf", NULL };

    config c;
    init_config(&c);

    frg_status_t result = frg_cli_option_parse_next(
        option,
        &argi,
        argc,
        argv,
        &c
    );

    TEST_ASSERT_EQUAL(FRG_STATUS_OK, result);

    TEST_ASSERT_FALSE(c.flag);
    TEST_ASSERT_FALSE(c.flag_short);
    TEST_ASSERT_NULL(c.argument);
    TEST_ASSERT_EQUAL_STRING("asdf", c.argument_short);
    TEST_ASSERT_NULL(c.choice);
    TEST_ASSERT_NULL(c.choice_short);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_flag_long);
    RUN_TEST(test_flag_short_long);
    RUN_TEST(test_flag_short_short);
    RUN_TEST(test_argument_long);
    RUN_TEST(test_argument_short_long);
    RUN_TEST(test_argument_short_short);
    return UNITY_END();
}
