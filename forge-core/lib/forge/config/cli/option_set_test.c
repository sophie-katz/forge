// Copyright (c) 2023 Sophie Katz
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

#include <forge/config/cli/option_set.h>
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

frg_status_t create_option_set(frg_cli_option_set_t** option_set) {
    frg_status_t result = frg_cli_option_set_new(option_set);
    if (result != FRG_STATUS_OK) {
        return result;
    }

    frg_cli_option_t* option = NULL;
    result = frg_cli_option_new_flag(
        &option,
        "long-flag",
        "A long-only flag.",
        callback_flag
    );
    if (result != FRG_STATUS_OK) {
        return result;
    }

    result = frg_cli_option_set_add_option(
        *option_set,
        option
    );
    if (result != FRG_STATUS_OK) {
        return result;
    }

    option = NULL;
    result = frg_cli_option_new_flag_short(
        &option,
        'f',
        "short-flag",
        "A long and short flag.",
        callback_flag_short
    );
    if (result != FRG_STATUS_OK) {
        return result;
    }

    result = frg_cli_option_set_add_option(
        *option_set,
        option
    );
    if (result != FRG_STATUS_OK) {
        return result;
    }

    option = NULL;
    result = frg_cli_option_new_argument(
        &option,
        "long-argument",
        "value",
        "A long-only argument.",
        callback_argument
    );
    if (result != FRG_STATUS_OK) {
        return result;
    }

    result = frg_cli_option_set_add_option(
        *option_set,
        option
    );
    if (result != FRG_STATUS_OK) {
        return result;
    }

    option = NULL;
    result = frg_cli_option_new_argument_short(
        &option,
        'a',
        "short-argument",
        "value",
        "A long and short argument.",
        callback_argument_short
    );
    if (result != FRG_STATUS_OK) {
        return result;
    }

    result = frg_cli_option_set_add_option(
        *option_set,
        option
    );
    if (result != FRG_STATUS_OK) {
        return result;
    }

    option = NULL;
    result = frg_cli_option_new_choice(
        &option,
        "long-choice",
        "value",
        "A long-only choice.",
        callback_choice
    );
    if (result != FRG_STATUS_OK) {
        return result;
    }

    frg_cli_choice_t* choice = NULL;
    result = frg_cli_choice_new(
        &choice,
        "option-1",
        "A choice."
    );
    if (result != FRG_STATUS_OK) {
        return result;
    }

    result = frg_cli_option_add_choice(
        option,
        choice
    );
    if (result != FRG_STATUS_OK) {
        return result;
    }

    choice = NULL;
    result = frg_cli_choice_new(
        &choice,
        "option-2",
        "Another choice."
    );
    if (result != FRG_STATUS_OK) {
        return result;
    }

    result = frg_cli_option_add_choice(
        option,
        choice
    );
    if (result != FRG_STATUS_OK) {
        return result;
    }

    result = frg_cli_option_set_add_option(
        *option_set,
        option
    );
    if (result != FRG_STATUS_OK) {
        return result;
    }

    option = NULL;
    result = frg_cli_option_new_choice_short(
        &option,
        'g',
        "short-choice",
        "value",
        "A long and short choice.",
        callback_choice_short
    );
    if (result != FRG_STATUS_OK) {
        return result;
    }

    choice = NULL;
    result = frg_cli_choice_new(
        &choice,
        "option-1",
        "A choice."
    );
    if (result != FRG_STATUS_OK) {
        return result;
    }

    result = frg_cli_option_add_choice(
        option,
        choice
    );
    if (result != FRG_STATUS_OK) {
        return result;
    }

    choice = NULL;
    result = frg_cli_choice_new(
        &choice,
        "option-2",
        "Another choice."
    );
    if (result != FRG_STATUS_OK) {
        return result;
    }

    result = frg_cli_option_add_choice(
        option,
        choice
    );
    if (result != FRG_STATUS_OK) {
        return result;
    }

    result = frg_cli_option_set_add_option(
        *option_set,
        option
    );
    if (result != FRG_STATUS_OK) {
        return result;
    }

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

void test_no_more_args(void) {
    frg_cli_option_set_t* option_set = NULL;
    frg_status_t result = create_option_set(&option_set);
    TEST_ASSERT_EQUAL(FRG_STATUS_OK, result);
    TEST_ASSERT_NOT_NULL(option_set);

    int argi = 1;
    int argc = 1;
    const char* argv[] = { "forge", NULL };

    result = frg_cli_option_set_parse_next(
        option_set,
        &argi,
        argc,
        argv,
        NULL
    );

    TEST_ASSERT_EQUAL(FRG_STATUS_CLI_NO_MORE_ARGUMENTS, result);
    TEST_ASSERT_EQUAL(1, argi);
}

void test_flag_long(void) {
    frg_cli_option_set_t* option_set = NULL;
    frg_status_t result = create_option_set(&option_set);
    TEST_ASSERT_EQUAL(FRG_STATUS_OK, result);
    TEST_ASSERT_NOT_NULL(option_set);

    int argi = 1;
    int argc = 2;
    const char* argv[] = { "forge", "--long-flag", NULL };

    config c;
    init_config(&c);

    result = frg_cli_option_set_parse_next(
        option_set,
        &argi,
        argc,
        argv,
        &c
    );

    TEST_ASSERT_EQUAL(FRG_STATUS_OK, result);
    TEST_ASSERT_EQUAL(2, argi);

    TEST_ASSERT_TRUE(c.flag);
    TEST_ASSERT_FALSE(c.flag_short);
    TEST_ASSERT_NULL(c.argument);
    TEST_ASSERT_NULL(c.argument_short);
    TEST_ASSERT_NULL(c.choice);
    TEST_ASSERT_NULL(c.choice_short);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_no_more_args);
    RUN_TEST(test_flag_long);
    return UNITY_END();
}
