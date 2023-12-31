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

#include <forge/common/error.h>
#include <forge/common/color.h>
#include <forge/common/log.h>
#include <forge/common/memory.h>
#include <forge/cli/option.h>

bool frg_cli_is_valid_short_name(char short_name) {
    if (short_name >= 'a' && short_name <= 'z') {
        return true;
    } else if (short_name >= 'A' && short_name <= 'Z') {
        return true;
    } else {
        return false;
    }
}

frg_cli_option_t* frg_cli_option_new_flag(
    const char* long_name,
    const char* help,
    frg_cli_option_callback_t callback
) {
    frg_assert_string_non_empty(long_name);
    frg_assert_string_non_empty(help);
    frg_assert_pointer_non_null(callback);

    frg_cli_option_t* option = frg_safe_malloc(sizeof(frg_cli_option_t));

    option->short_name = FRG_CLI_OPTION_SHORT_NAME_NULL;
    option->long_name = long_name;
    option->value_name = NULL;
    option->help = help;
    option->choices = NULL;
    option->callback = callback;

    return option;
}

frg_cli_option_t* frg_cli_option_new_flag_short(
    char short_name,
    const char* long_name,
    const char* help,
    frg_cli_option_callback_t callback
) {
    frg_assert(frg_cli_is_valid_short_name(short_name));
    frg_assert_string_non_empty(long_name);
    frg_assert_string_non_empty(help);
    frg_assert_pointer_non_null(callback);

    frg_cli_option_t* option = frg_safe_malloc(sizeof(frg_cli_option_t));

    option->short_name = short_name;
    option->long_name = long_name;
    option->value_name = NULL;
    option->help = help;
    option->choices = NULL;
    option->callback = callback;

    return option;
}

frg_cli_option_t* frg_cli_option_new_argument(
    const char* long_name,
    const char* value_name,
    const char* help,
    frg_cli_option_callback_t callback
) {
    frg_assert_string_non_empty(long_name);
    frg_assert_string_non_empty(value_name);
    frg_assert_string_non_empty(help);
    frg_assert_pointer_non_null(callback);

    frg_cli_option_t* option = frg_safe_malloc(sizeof(frg_cli_option_t));

    option->short_name = FRG_CLI_OPTION_SHORT_NAME_NULL;
    option->long_name = long_name;
    option->value_name = value_name;
    option->help = help;
    option->choices = NULL;
    option->callback = callback;

    return option;
}

frg_cli_option_t* frg_cli_option_new_argument_short(
    char short_name,
    const char* long_name,
    const char* value_name,
    const char* help,
    frg_cli_option_callback_t callback
) {
    frg_assert(frg_cli_is_valid_short_name(short_name));
    frg_assert_string_non_empty(long_name);
    frg_assert_string_non_empty(value_name);
    frg_assert_string_non_empty(help);
    frg_assert_pointer_non_null(callback);

    frg_cli_option_t* option = frg_safe_malloc(sizeof(frg_cli_option_t));

    option->short_name = short_name;
    option->long_name = long_name;
    option->value_name = value_name;
    option->help = help;
    option->choices = NULL;
    option->callback = callback;

    return option;
}

frg_cli_option_t* frg_cli_option_new_choice(
    const char* long_name,
    const char* value_name,
    const char* help,
    frg_cli_option_callback_t callback
) {
    frg_assert_string_non_empty(long_name);
    frg_assert_string_non_empty(value_name);
    frg_assert_string_non_empty(help);
    frg_assert_pointer_non_null(callback);

    frg_cli_option_t* option = frg_safe_malloc(sizeof(frg_cli_option_t));

    option->short_name = FRG_CLI_OPTION_SHORT_NAME_NULL;
    option->long_name = long_name;
    option->value_name = value_name;
    option->help = help;
    option->choices = NULL;
    option->callback = callback;

    return option;
}

frg_cli_option_t* frg_cli_option_new_choice_short(
    char short_name,
    const char* long_name,
    const char* value_name,
    const char* help,
    frg_cli_option_callback_t callback
) {
    frg_assert(frg_cli_is_valid_short_name(short_name));
    frg_assert_string_non_empty(long_name);
    frg_assert_string_non_empty(value_name);
    frg_assert_string_non_empty(help);
    frg_assert_pointer_non_null(callback);

    frg_cli_option_t* option = frg_safe_malloc(sizeof(frg_cli_option_t));

    option->short_name = short_name;
    option->long_name = long_name;
    option->value_name = value_name;
    option->help = help;
    option->choices = NULL;
    option->callback = callback;

    return option;
}

void frg_cli_option_destroy(
    frg_cli_option_t** option
) {
    frg_assert_pointer_non_null(option);
    frg_assert_pointer_non_null(*option);

    for (GList* choice = (*option)->choices; choice != NULL; choice = choice->next) {
        frg_cli_choice_destroy((frg_cli_choice_t**)&choice->data);
    }

    g_list_free((*option)->choices);

    frg_safe_free((void**)option);
}

void frg_cli_option_add_choice(
    frg_cli_option_t* option,
    frg_cli_choice_t* choice
) {
    frg_assert_pointer_non_null(option);
    frg_assert_pointer_non_null(choice);

    option->choices = g_list_append(option->choices, choice);
}

void frg_cli_option_print_help(
    const frg_cli_option_t* option
) {
    frg_assert_pointer_non_null(option);

    printf("  ");

    if (option->short_name != FRG_CLI_OPTION_SHORT_NAME_NULL) {
        frg_color_set(stdout, FRG_COLOR_ID_BOLD);
        printf("-%c", option->short_name);
        frg_color_set(stdout, FRG_COLOR_ID_RESET);
        printf(", ");
    }

    frg_color_set(stdout, FRG_COLOR_ID_BOLD);
    printf("--%s", option->long_name);
    frg_color_set(stdout, FRG_COLOR_ID_RESET);

    if (option->value_name != NULL) {
        printf(" <%s>", option->value_name);
    }

    printf("\n    %s\n", option->help);

    if (option->choices != NULL) {
        printf("    ");
        frg_color_set(stdout, FRG_COLOR_ID_UNDERLINE);
        printf("Choices:");
        frg_color_set(stdout, FRG_COLOR_ID_RESET);
        printf("\n");

        frg_color_set(stdout, FRG_COLOR_ID_RESET);
        for (GList* choice = option->choices; choice != NULL; choice = choice->next) {
            frg_cli_choice_print_help((const frg_cli_choice_t*)choice->data);
        }
    }
}

bool _frg_cli_option_check_matches_argument(
    const frg_cli_option_t* option,
    const char* arg
) {
    frg_assert_pointer_non_null(option);
    frg_assert_string_non_empty(arg);
    frg_assert_int_eq(arg[0], '-');

    if (arg[1] == '-') {
        return strcmp(arg + 2, option->long_name) == 0;
    } else {
        return arg[1] == option->short_name;
    }
}

bool frg_cli_option_parse_next(
    const frg_cli_option_t* option,
    int* argi,
    int argc,
    const char** argv,
    void* user_data
) {
    // Error checking
    frg_assert_pointer_non_null(option);
    frg_assert_pointer_non_null(argi);
    frg_assert_pointer_non_null(argv);
    frg_assert_int_ge(*argi, 0);
    frg_assert_int_gt(argc, 0);
    frg_assert_int_lt(*argi, argc);
    frg_assert_string_non_empty(argv[*argi]);
    frg_assert_int_eq(argv[*argi][0], '-');
    frg_assert(_frg_cli_option_check_matches_argument(option, argv[*argi]));

    // Increment argument index
    (*argi)++;

    if (option->value_name == NULL) {
        // If the argument does not take a value, call the callback with NULL
        return option->callback(user_data, NULL);
    } else {
        // If the argument does take a value, make sure there is a value to parse
        if (*argi >= argc) {
            frg_log_fatal_error("argument '%s' expects a value", argv[*argi - 1]);
            return false;
        }

        frg_assert_string_non_empty(argv[*argi]);

        if (argv[*argi][0] == '-') {
            frg_log_fatal_error("argument '%s' expects a value, '%s' is an argument", argv[*argi - 1], argv[*argi]);
            return false;
        }

        // If the argument has a set of choices, make sure the value matches one of them
        if (option->choices != NULL) {
            bool found = false;

            for (GList* choice = option->choices; choice != NULL; choice = choice->next) {
                if (strcmp(argv[*argi], ((frg_cli_choice_t*)choice->data)->name) == 0) {
                    found = true;
                    break;
                }
            }

            if (!found) {
                frg_log_result_t result = frg_log_fatal_error("unexpected value for argument '%s', allowed values are:");

                for (GList* choice = option->choices; choice != NULL; choice = choice->next) {
                    const frg_cli_choice_t* choice_casted = (const frg_cli_choice_t*)choice->data;
                    frg_log_note(&result, "  '%s' - %s", choice_casted->name, choice_casted->help);
                }

                return false;
            }
        }

        // Call the callback with the value
        bool result = option->callback(user_data, argv[*argi]);

        // Increment argument index
        (*argi)++;

        return result;
    }
}
