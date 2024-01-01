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

#include <forge/common/check.h>
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

frg_status_t frg_cli_option_new_flag(
    frg_cli_option_t** option,
    const char* long_name,
    const char* help,
    frg_cli_option_callback_t callback
) {
    if (option == NULL || long_name == NULL || help == NULL || callback == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    } else if (*long_name == 0 || *help == 0) {
        return FRG_STATUS_ERROR_EMPTY_STRING;
    } else if (*option != NULL) {
        return FRG_STATUS_ERROR_UNEXPECTED_ARGUMENT_VALUE;
    }

    frg_check(
        frg_safe_malloc((void**)option, sizeof(frg_cli_option_t))
    );

    (*option)->short_name = FRG_CLI_OPTION_SHORT_NAME_NULL;
    (*option)->long_name = long_name;
    (*option)->value_name = NULL;
    (*option)->help = help;
    (*option)->choices = NULL;
    (*option)->callback = callback;

    return FRG_STATUS_OK;
}

frg_status_t frg_cli_option_new_flag_short(
    frg_cli_option_t** option,
    char short_name,
    const char* long_name,
    const char* help,
    frg_cli_option_callback_t callback
) {
    if (option == NULL || long_name == NULL || help == NULL || callback == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    } else if (*long_name == 0 || *help == 0) {
        return FRG_STATUS_ERROR_EMPTY_STRING;
    } else if (*option != NULL || !frg_cli_is_valid_short_name(short_name)) {
        return FRG_STATUS_ERROR_UNEXPECTED_ARGUMENT_VALUE;
    }

    frg_check(
        frg_safe_malloc((void**)option, sizeof(frg_cli_option_t))
    );

    (*option)->short_name = short_name;
    (*option)->long_name = long_name;
    (*option)->value_name = NULL;
    (*option)->help = help;
    (*option)->choices = NULL;
    (*option)->callback = callback;

    return FRG_STATUS_OK;
}

frg_status_t frg_cli_option_new_argument(
    frg_cli_option_t** option,
    const char* long_name,
    const char* value_name,
    const char* help,
    frg_cli_option_callback_t callback
) {
    if (option == NULL || long_name == NULL || value_name == NULL || help == NULL
        || callback == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    } else if (*long_name == 0 || *value_name == 0 || *help == 0) {
        return FRG_STATUS_ERROR_EMPTY_STRING;
    } else if (*option != NULL) {
        return FRG_STATUS_ERROR_UNEXPECTED_ARGUMENT_VALUE;
    }

    frg_check(
        frg_safe_malloc((void**)option, sizeof(frg_cli_option_t))
    );

    (*option)->short_name = FRG_CLI_OPTION_SHORT_NAME_NULL;
    (*option)->long_name = long_name;
    (*option)->value_name = value_name;
    (*option)->help = help;
    (*option)->choices = NULL;
    (*option)->callback = callback;

    return FRG_STATUS_OK;
}

frg_status_t frg_cli_option_new_argument_short(
    frg_cli_option_t** option,
    char short_name,
    const char* long_name,
    const char* value_name,
    const char* help,
    frg_cli_option_callback_t callback
) {
    if (option == NULL || long_name == NULL || value_name == NULL || help == NULL
        || callback == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    } else if (*long_name == 0 || *value_name == 0 || *help == 0) {
        return FRG_STATUS_ERROR_EMPTY_STRING;
    } else if (*option != NULL || !frg_cli_is_valid_short_name(short_name)) {
        return FRG_STATUS_ERROR_UNEXPECTED_ARGUMENT_VALUE;
    }

    frg_check(
        frg_safe_malloc((void**)option, sizeof(frg_cli_option_t))
    );

    (*option)->short_name = short_name;
    (*option)->long_name = long_name;
    (*option)->value_name = value_name;
    (*option)->help = help;
    (*option)->choices = NULL;
    (*option)->callback = callback;

    return FRG_STATUS_OK;
}

frg_status_t frg_cli_option_new_choice(
    frg_cli_option_t** option,
    const char* long_name,
    const char* value_name,
    const char* help,
    frg_cli_option_callback_t callback
) {
    if (option == NULL || long_name == NULL || value_name == NULL || help == NULL
        || callback == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    } else if (*long_name == 0 || *value_name == 0 || *help == 0) {
        return FRG_STATUS_ERROR_EMPTY_STRING;
    } else if (*option != NULL) {
        return FRG_STATUS_ERROR_UNEXPECTED_ARGUMENT_VALUE;
    }

    frg_check(
        frg_safe_malloc((void**)option, sizeof(frg_cli_option_t))
    );

    (*option)->short_name = FRG_CLI_OPTION_SHORT_NAME_NULL;
    (*option)->long_name = long_name;
    (*option)->value_name = value_name;
    (*option)->help = help;
    (*option)->choices = NULL;
    (*option)->callback = callback;

    return FRG_STATUS_OK;
}

frg_status_t frg_cli_option_new_choice_short(
    frg_cli_option_t** option,
    char short_name,
    const char* long_name,
    const char* value_name,
    const char* help,
    frg_cli_option_callback_t callback
) {
    if (option == NULL || long_name == NULL || value_name == NULL || help == NULL
        || callback == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    } else if (*long_name == 0 || *value_name == 0 || *help == 0) {
        return FRG_STATUS_ERROR_EMPTY_STRING;
    } else if (*option != NULL || !frg_cli_is_valid_short_name(short_name)) {
        return FRG_STATUS_ERROR_UNEXPECTED_ARGUMENT_VALUE;
    }

    frg_check(
        frg_safe_malloc((void**)option, sizeof(frg_cli_option_t))
    );

    (*option)->short_name = short_name;
    (*option)->long_name = long_name;
    (*option)->value_name = value_name;
    (*option)->help = help;
    (*option)->choices = NULL;
    (*option)->callback = callback;

    return FRG_STATUS_OK;
}

frg_status_t frg_cli_option_destroy(
    frg_cli_option_t** option
) {
    if (option == NULL || *option == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    }

    for (GList* choice = (*option)->choices; choice != NULL; choice = choice->next) {
        frg_check(
            frg_cli_choice_destroy((frg_cli_choice_t**)&choice->data)
        );
    }

    g_list_free((*option)->choices);

    return frg_safe_free((void**)option);
}

frg_status_t frg_cli_option_add_choice(
    frg_cli_option_t* option,
    frg_cli_choice_t* choice
) {
    if (option == NULL || choice == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    }

    option->choices = g_list_append(option->choices, choice);

    return FRG_STATUS_OK;
}

frg_status_t frg_cli_option_print_help(
    const frg_cli_option_t* option
) {
    if (option == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    }

    printf("  ");

    if (option->short_name != FRG_CLI_OPTION_SHORT_NAME_NULL) {
        frg_set_color(stdout, FRG_COLOR_ID_BOLD);
        printf("-%c", option->short_name);
        frg_set_color(stdout, FRG_COLOR_ID_RESET);
        printf(", ");
    }

    frg_set_color(stdout, FRG_COLOR_ID_BOLD);
    printf("--%s", option->long_name);
    frg_set_color(stdout, FRG_COLOR_ID_RESET);

    if (option->value_name != NULL) {
        printf(" <%s>", option->value_name);
    }

    printf("\n    %s\n", option->help);

    if (option->choices != NULL) {
        printf("    ");
        frg_set_color(stdout, FRG_COLOR_ID_UNDERLINE);
        printf("Choices:");
        frg_set_color(stdout, FRG_COLOR_ID_RESET);
        printf("\n");

        frg_set_color(stdout, FRG_COLOR_ID_RESET);
        for (GList* choice = option->choices; choice != NULL; choice = choice->next) {
            frg_check(
                frg_cli_choice_print_help((const frg_cli_choice_t*)choice->data)
            );
        }
    }

    return FRG_STATUS_OK;
}

frg_status_t _frg_cli_option_ensure_matches_argument(
    const frg_cli_option_t* option,
    const char* arg
) {
    if (option == NULL || arg == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    } else if (arg[0] != '-') {
        return FRG_STATUS_ERROR_UNEXPECTED_ARGUMENT_VALUE;
    }

    if (arg[1] == '-') {
        // It is a long argument

        // Confirm that the current argument in 'argv' matches 'option'
        if (strcmp(arg + 2, option->long_name) != 0) {
            return FRG_STATUS_ERROR_UNEXPECTED_ARGUMENT_VALUE;
        }
    } else {
        // It is a short argument
        if (arg[1] != option->short_name) {
            return FRG_STATUS_ERROR_UNEXPECTED_ARGUMENT_VALUE;
        }

        // Confirm that the current argument in 'argv' matches 'option'
        if (arg[1] != option->short_name) {
            return FRG_STATUS_ERROR_UNEXPECTED_ARGUMENT_VALUE;
        }
    }

    return FRG_STATUS_OK;
}

frg_status_t frg_cli_option_parse_next(
    const frg_cli_option_t* option,
    int* argi,
    int argc,
    const char** argv,
    void* user_data
) {
    // Error checking
    if (option == NULL || argi == NULL || argv == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    } else if (*argi < 0 || argc <= 0 || *argi >= argc || argv[*argi] == NULL || argv[*argi][0] != '-') {
        return FRG_STATUS_ERROR_UNEXPECTED_ARGUMENT_VALUE;
    }

    // Make sure the argument being parsed actually matches 'option'
    frg_check(
        _frg_cli_option_ensure_matches_argument(
            option,
            argv[*argi]
        )
    );

    // Increment argument index
    (*argi)++;

    if (option->value_name == NULL) {
        // If the argument does not take a value, call the callback with NULL
        return option->callback(user_data, NULL);
    } else {
        // If the argument does take a value, make sure there is a value to parse

        if (*argi >= argc) {
            frg_log_fatal_error("argument '%s' expects a value", argv[*argi - 1]);
            return FRG_STATUS_CLI_ERROR;
        } else if (argv[*argi] == NULL) {
            return FRG_STATUS_ERROR_UNEXPECTED_ARGUMENT_VALUE;
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

                return FRG_STATUS_CLI_ERROR;
            }
        }

        // Call the callback with the value
        frg_check(
            option->callback(user_data, argv[*argi])
        );

        // Increment argument index
        (*argi)++;

        return FRG_STATUS_OK;
    }
}
