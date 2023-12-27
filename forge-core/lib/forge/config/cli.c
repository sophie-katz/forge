#include <forge/common/color.h>
#include <forge/common/log.h>
#include <forge/common/memory.h>
#include <forge/config/cli.h>

bool _frg_cli_is_valid_short_name(char short_name) {
    if (short_name >= 'a' && short_name <= 'z') {
        return true;
    } else if (short_name >= 'A' && short_name <= 'Z') {
        return true;
    } else {
        return false;
    }
}

frg_status_t _frg_cli_get_short_name_index(int* index, char short_name) {
    if (index == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    } else if (short_name >= 'a' && short_name <= 'z') {
        *index = short_name - 'a';
        return FRG_STATUS_OK;
    } else if (short_name >= 'A' && short_name <= 'Z') {
        *index = short_name - 'A' + 26;
        return FRG_STATUS_OK;
    } else {
        return FRG_STATUS_ERROR_UNEXPECTED_ARGUMENT_VALUE;
    }
}

frg_status_t frg_cli_option_choice_new(
    frg_cli_option_choice_t** choice,
    const char* name,
    const char* help
) {
    if (choice == NULL || name == NULL || help == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    } else if (*choice != NULL) {
        return FRG_STATUS_ERROR_UNEXPECTED_ARGUMENT_VALUE;
    } else if (*name == 0 || *help == 0) {
        return FRG_STATUS_ERROR_EMPTY_STRING;
    }

    frg_status_t result = frg_safe_malloc(
        (void**)choice,
        sizeof(frg_cli_option_choice_t)
    );
    if (result != FRG_STATUS_OK) {
        return result;
    }

    (*choice)->name = name;
    (*choice)->help = help;

    return FRG_STATUS_OK;
}

frg_status_t frg_cli_option_choice_destroy(
    frg_cli_option_choice_t** choice
) {
    return frg_safe_free((void**)choice);
}

frg_status_t frg_cli_option_choice_print_help(
    const frg_cli_option_choice_t* choice
) {
    if (choice == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    }

    printf("      • ");
    frg_set_color(stdout, FRG_COLOR_ID_BOLD);
    printf("%s", choice->name);
    frg_set_color(stdout, FRG_COLOR_ID_RESET);
    printf(" (%s)\n", choice->help);

    return FRG_STATUS_OK;
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

    frg_status_t result = frg_safe_malloc((void**)option, sizeof(frg_cli_option_t));
    if (result != FRG_STATUS_OK) {
        return result;
    }

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
    } else if (*option != NULL || !_frg_cli_is_valid_short_name(short_name)) {
        return FRG_STATUS_ERROR_UNEXPECTED_ARGUMENT_VALUE;
    }

    frg_status_t result = frg_safe_malloc((void**)option, sizeof(frg_cli_option_t));
    if (result != FRG_STATUS_OK) {
        return result;
    }

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

    frg_status_t result = frg_safe_malloc((void**)option, sizeof(frg_cli_option_t));
    if (result != FRG_STATUS_OK) {
        return result;
    }

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
    } else if (*option != NULL || !_frg_cli_is_valid_short_name(short_name)) {
        return FRG_STATUS_ERROR_UNEXPECTED_ARGUMENT_VALUE;
    }

    frg_status_t result = frg_safe_malloc((void**)option, sizeof(frg_cli_option_t));
    if (result != FRG_STATUS_OK) {
        return result;
    }

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
    GList* choices,
    frg_cli_option_callback_t callback
) {
    if (option == NULL || long_name == NULL || value_name == NULL || help == NULL
        || choices == NULL || callback == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    } else if (*long_name == 0 || *value_name == 0 || *help == 0) {
        return FRG_STATUS_ERROR_EMPTY_STRING;
    } else if (*option != NULL || choices->next == NULL) {
        return FRG_STATUS_ERROR_UNEXPECTED_ARGUMENT_VALUE;
    }

    frg_status_t result = frg_safe_malloc((void**)option, sizeof(frg_cli_option_t));
    if (result != FRG_STATUS_OK) {
        return result;
    }

    (*option)->short_name = FRG_CLI_OPTION_SHORT_NAME_NULL;
    (*option)->long_name = long_name;
    (*option)->value_name = value_name;
    (*option)->help = help;
    (*option)->choices = choices;
    (*option)->callback = callback;

    return FRG_STATUS_OK;
}

frg_status_t frg_cli_option_new_choice_short(
    frg_cli_option_t** option,
    char short_name,
    const char* long_name,
    const char* value_name,
    const char* help,
    GList* choices,
    frg_cli_option_callback_t callback
) {
    if (option == NULL || long_name == NULL || value_name == NULL || help == NULL
        || choices == NULL || callback == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    } else if (*long_name == 0 || *value_name == 0 || *help == 0) {
        return FRG_STATUS_ERROR_EMPTY_STRING;
    } else if (*option != NULL || choices->next == NULL
        || !_frg_cli_is_valid_short_name(short_name)) {
        return FRG_STATUS_ERROR_UNEXPECTED_ARGUMENT_VALUE;
    }

    frg_status_t result = frg_safe_malloc((void**)option, sizeof(frg_cli_option_t));
    if (result != FRG_STATUS_OK) {
        return result;
    }

    (*option)->short_name = short_name;
    (*option)->long_name = long_name;
    (*option)->value_name = value_name;
    (*option)->help = help;
    (*option)->choices = choices;
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
        frg_cli_option_choice_destroy((frg_cli_option_choice_t**)&choice->data);
    }

    g_list_free((*option)->choices);

    return frg_safe_free((void**)option);
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
            frg_cli_option_choice_print_help((const frg_cli_option_choice_t*)choice->data);
        }
    }

    return FRG_STATUS_OK;
}

frg_status_t frg_cli_option_set_new(
    frg_cli_option_set_t** option_set
) {
    if (option_set == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    } else if (*option_set != NULL) {
        return FRG_STATUS_ERROR_UNEXPECTED_ARGUMENT_VALUE;
    }

    frg_status_t result = frg_safe_malloc((void**)option_set, sizeof(frg_cli_option_set_t));
    if (result != FRG_STATUS_OK) {
        return result;
    }

    (*option_set)->options = NULL;
    memset((*option_set)->options_by_short_name, 0, sizeof((*option_set)->options_by_short_name));
    (*option_set)->options_by_long_name = g_hash_table_new(g_str_hash, g_str_equal);

    return FRG_STATUS_OK;
}

frg_status_t frg_cli_option_set_destroy(
    frg_cli_option_set_t** option_set
) {
    if (option_set == NULL || *option_set == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    }

    for (GList* option = (*option_set)->options; option != NULL; option = option->next) {
        frg_cli_option_destroy((frg_cli_option_t**)&option->data);
    }

    g_list_free((*option_set)->options);

    g_hash_table_destroy((*option_set)->options_by_long_name);

    return frg_safe_free((void**)option_set);
}

frg_status_t frg_cli_option_set_add(
    frg_cli_option_set_t* option_set,
    frg_cli_option_t* option
) {
    if (option_set == NULL || option == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    }

    option_set->options = g_list_append(option_set->options, option);

    if (option->short_name != FRG_CLI_OPTION_SHORT_NAME_NULL) {
        int index = 0;
        frg_status_t result = _frg_cli_get_short_name_index(&index, option->short_name);
        if (result != FRG_STATUS_OK) {
            return result;
        }

        if (option_set->options_by_short_name[index] != NULL) {
            return FRG_STATUS_ERROR_DUPLICATE;
        }

        option_set->options_by_short_name[index] = option;
    }

    if (g_hash_table_contains(option_set->options_by_long_name, option->long_name)) {
        return FRG_STATUS_ERROR_DUPLICATE;
    } else {
        g_hash_table_insert(option_set->options_by_long_name, (void*)option->long_name, option);
    }

    return FRG_STATUS_OK;
}

frg_status_t frg_cli_option_set_print_help(
    const frg_cli_option_set_t* option_set
) {
    if (option_set == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    }

    bool first = true;

    for (GList* option = option_set->options; option != NULL; option = option->next) {
        if (first) {
            first = false;
        } else {
            printf("\n");
        }

        frg_status_t result = frg_cli_option_print_help((const frg_cli_option_t*)option->data);
        if (result != FRG_STATUS_OK) {
            return result;
        }
    }

    return FRG_STATUS_OK;
}

bool frg_cli_option_set_parse_next(
    frg_cli_option_set_t* option_set,
    int* argi,
    int argc,
    const char** argv,
    void* user_data
) {
    // Error checking
    if (option_set == NULL) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "option_set is NULL");
        return false;
    } else if (argi == NULL) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "argi is NULL");
        return false;
    } else if (*argi < 0) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "argi (%i) is negative", *argi);
        return false;
    } else if (argc <= 0) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "argc (%i) is non-positive", argc);
        return false;
    } else if (*argi >= argc) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "argi (%i) is not less than argc (%i)", *argi, argc);
        return false;
    } else if (argv == NULL) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "argv is NULL");
        return false;
    } else if (user_data == NULL) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "user_data is NULL");
        return false;
    }

    // Try to parse the next argument
    if (argv[*argi] == NULL) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "argv[%d] is NULL", *argi);
        return false;
    } else if (argv[*argi][0] == 0) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "argv[%d] is an empty string", *argi);
        return false;
    } else if (argv[*argi][0] == '-') {
        if (argv[*argi][1] == '-') {
            if (argv[*argi][2] == 0) {
                frg_log(FRG_LOG_SEVERITY_FATAL_ERROR, "invalid argument '%s'", argv[*argi]);
                return false;
            } else {
                const char* long_name = argv[*argi] + 2;
                frg_cli_option_t* option = (frg_cli_option_t*)g_hash_table_lookup(option_set->options_by_long_name, long_name);
                if (option == NULL) {
                    frg_log(FRG_LOG_SEVERITY_FATAL_ERROR, "unknown argument '%s'", argv[*argi]);
                    return false;
                } else {
                    const char* value = NULL;
                    if (option->value_name != NULL) {
                        (*argi)++;

                        if (*argi >= argc) {
                            frg_log(FRG_LOG_SEVERITY_FATAL_ERROR, "missing value for argument '%s'", argv[*argi - 1]);
                            return false;
                        } else if (argv[*argi][0] == 0) {
                            frg_log_prefix_internal();
                            frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "argv[%d] is an empty string", *argi);
                            return false;
                        } else if (argv[*argi][0] == '-') {
                            frg_log(FRG_LOG_SEVERITY_FATAL_ERROR, "missing value for argument '%s'", argv[*argi - 1]);
                            return false;
                        } else {
                            value = argv[*argi];
                        }
                    }

                    if (option->callback == NULL) {
                        frg_log_prefix_internal();
                        frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "option '%s' does not have a callback", option->long_name);
                        return false;
                    } else {
                        frg_status_t result = option->callback(user_data, value);
                        if (result != FRG_STATUS_OK) {
                            frg_log_prefix_internal();
                            frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "error while loading option '%s': %s", option->long_name, frg_status_to_string(result));
                            return false;
                        } else {
                            (*argi)++;
                            return true;
                        }
                    }
                }
            }
        } else if (_frg_cli_is_valid_short_name(argv[*argi][1])) {
            int index = -1;
            frg_status_t result = _frg_cli_get_short_name_index(&index, argv[*argi][1]);
            if (result != FRG_STATUS_OK) {
                frg_log_prefix_internal();
                frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "unable to get option by short name '%c'", argv[*argi][1]);
                return false;
            }

            if (index < 0 || index >= FRG_CLI_OPTION_SHORT_NAME_MAX_COUNT) {
                frg_log_prefix_internal();
                frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "invalid index %d", index);
                return false;
            }

            frg_cli_option_t* option = option_set->options_by_short_name[index];

            if (option == NULL) {
                frg_log(FRG_LOG_SEVERITY_FATAL_ERROR, "unknown argument '%s'", argv[*argi]);
                return false;
            } else {
                const char* value = NULL;
                if (option->value_name != NULL) {
                    (*argi)++;

                    if (*argi >= argc) {
                        frg_log(FRG_LOG_SEVERITY_FATAL_ERROR, "missing value for argument '%s'", argv[*argi - 1]);
                        return false;
                    } else if (argv[*argi][0] == 0) {
                        frg_log_prefix_internal();
                        frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "argv[%d] is an empty string", *argi);
                        return false;
                    } else if (argv[*argi][0] == '-') {
                        frg_log(FRG_LOG_SEVERITY_FATAL_ERROR, "missing value for argument '%s'", argv[*argi - 1]);
                        return false;
                    } else {
                        value = argv[*argi];
                    }
                }

                if (option->callback == NULL) {
                    frg_log_prefix_internal();
                    frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "option '%s' does not have a callback", option->long_name);
                    return false;
                } else {
                    frg_status_t result = option->callback(user_data, value);
                    if (result != FRG_STATUS_OK) {
                        frg_log_prefix_internal();
                        frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "error while loading option '%s': %s", option->long_name, frg_status_to_string(result));
                        return false;
                    } else {
                        (*argi)++;
                        return true;
                    }
                }
            }
        } else {
            frg_log(FRG_LOG_SEVERITY_FATAL_ERROR, "invalid argument '%s'", argv[*argi]);
            return false;
        }
    } else {
        // The next argument is not an option, so do nothing
        return true;
    }
}

frg_status_t frg_cli_command_new(
    frg_cli_command_t** command,
    const char* name,
    const char* help,
    frg_cli_command_callback_t callback
) {
    if (command == NULL || name == NULL || help == NULL || callback == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    } else if (*name == 0 || *help == 0) {
        return FRG_STATUS_ERROR_EMPTY_STRING;
    } else if (*command != NULL) {
        return FRG_STATUS_ERROR_UNEXPECTED_ARGUMENT_VALUE;
    }

    frg_status_t result = frg_safe_malloc((void**)command, sizeof(frg_cli_command_t));
    if (result != FRG_STATUS_OK) {
        return result;
    }

    (*command)->name = name;
    (*command)->help = help;
    (*command)->option_set = NULL;
    (*command)->callback = callback;

    result = frg_cli_option_set_new(
        &(*command)->option_set
    );
    if (result != FRG_STATUS_OK) {
        return result;
    }

    return FRG_STATUS_OK;
}

frg_status_t frg_cli_command_destroy(
    frg_cli_command_t** command
) {
    if (command == NULL || *command == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    }

    frg_status_t result = frg_cli_option_set_destroy(
        &(*command)->option_set
    );
    if (result != FRG_STATUS_OK) {
        return result;
    }

    return frg_safe_free((void**)command);
}

frg_status_t frg_cli_command_print_help(
    const frg_cli_command_t* command
) {
    printf("%s\n", command->help);

    if (command->option_set->options != NULL) {
        printf("\n");
        frg_set_color(stdout, FRG_COLOR_ID_UNDERLINE);
        printf("Command options:");
        frg_set_color(stdout, FRG_COLOR_ID_RESET);
        printf("\n");

        frg_status_t result = frg_cli_option_set_print_help(
            command->option_set
        );
        if (result != FRG_STATUS_OK) {
            return result;
        }
    }

    return FRG_STATUS_OK;
}

frg_status_t frg_cli_program_new(
    frg_cli_program_t** program,
    const char* name,
    const char* binary_name,
    const char* pos_args_name,
    int version_major,
    int version_minor,
    int version_patch,
    const char* version_label,
    GString* version_details
) {
    if (program == NULL || name == NULL || binary_name == NULL
        || version_label == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    } else if (*name == 0 || *binary_name == 0 || *version_label == 0) {
        return FRG_STATUS_ERROR_EMPTY_STRING;
    } else if (*program != NULL || version_major < 0 || version_minor < 0
        || version_patch < 0) {
        return FRG_STATUS_ERROR_UNEXPECTED_ARGUMENT_VALUE;
    }

    frg_status_t result = frg_safe_malloc((void**)program, sizeof(frg_cli_program_t));
    if (result != FRG_STATUS_OK) {
        return result;
    }

    (*program)->name = name;
    (*program)->binary_name = binary_name;
    (*program)->pos_args_name = pos_args_name;
    (*program)->version_major = version_major;
    (*program)->version_minor = version_minor;
    (*program)->version_patch = version_patch;
    (*program)->version_label = version_label;
    (*program)->version_details = version_details;
    (*program)->commands = NULL;
    (*program)->commands_by_name = g_hash_table_new(g_str_hash, g_str_equal);

    (*program)->global_options = NULL;
    result = frg_cli_option_set_new(
        &(*program)->global_options
    );

    return FRG_STATUS_OK;

}

frg_status_t frg_cli_program_destroy(
    frg_cli_program_t** program
) {
    if (program == NULL || *program == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    }

    for (GList* command = (*program)->commands; command != NULL; command = command->next) {
        frg_cli_command_destroy((frg_cli_command_t**)&command->data);
    }

    g_list_free((*program)->commands);

    g_hash_table_destroy((*program)->commands_by_name);

    frg_status_t result = frg_cli_option_set_destroy(
        &(*program)->global_options
    );
    if (result != FRG_STATUS_OK) {
        return result;
    }

    return frg_safe_free((void**)program);
}

frg_status_t frg_cli_program_add_command(
    frg_cli_program_t* program,
    frg_cli_command_t* command
) {
    if (program == NULL || command == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    } else if (program->pos_args_name != NULL) {
        return FRG_STATUS_ERROR_DUPLICATE;
    }

    program->commands = g_list_append(program->commands, command);

    if (g_hash_table_contains(program->commands_by_name, command->name)) {
        return FRG_STATUS_ERROR_DUPLICATE;
    } else {
        g_hash_table_insert(program->commands_by_name, (void*)command->name, command);
    }

    return FRG_STATUS_OK;
}

frg_status_t frg_cli_program_print_help(
    const frg_cli_program_t* program,
    const char* command_name
) {
    if (program == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    }

    frg_status_t result = frg_cli_program_print_version_long(program);
    if (result != FRG_STATUS_OK) {
        return result;
    }

    printf("\n");

    if (command_name == NULL) {
        if (program->commands == NULL) {
            printf("Usage: ");
            frg_set_color(stdout, FRG_COLOR_ID_BOLD);
            printf("%s [options]%s%s\n", program->binary_name, program->pos_args_name == NULL ? "" : " ", program->pos_args_name);
            frg_set_color(stdout, FRG_COLOR_ID_RESET);
            printf("\n");
            printf("Options:\n");
        } else {
            printf("Usage: ");
            frg_set_color(stdout, FRG_COLOR_ID_BOLD);
            printf("%s [global options] <command> [command options]\n", program->binary_name);
            frg_set_color(stdout, FRG_COLOR_ID_RESET);
            printf("\n");
            printf("Global options:\n");
        }

        result = frg_cli_option_set_print_help(
            program->global_options
        );
        if (result != FRG_STATUS_OK) {
            return result;
        }

        printf("\n");
        printf("Commands:\n");

        for (GList* command = program->commands; command != NULL; command = command->next) {
            frg_set_color(stdout, FRG_COLOR_ID_BOLD);
            printf("  %s\n    ", ((frg_cli_command_t*)command->data)->name);
            frg_set_color(stdout, FRG_COLOR_ID_RESET);
            frg_cli_command_print_help((const frg_cli_command_t*)command->data);
        }
    } else {
        printf("Usage: ");
        frg_set_color(stdout, FRG_COLOR_ID_BOLD);
        printf("%s [global options] %s [command options]\n\n", program->binary_name, command_name);
        frg_set_color(stdout, FRG_COLOR_ID_RESET);
        
        frg_cli_command_t* command = (frg_cli_command_t*)g_hash_table_lookup(program->commands_by_name, command_name);
        if (command == NULL) {
            printf("Unknown command '%s'\n", command_name);
            return FRG_STATUS_ERROR_UNEXPECTED_ARGUMENT_VALUE;
        } else {
            frg_cli_command_print_help(command);
        }

        printf("\n");
        printf("Global options:\n");

        frg_status_t result = frg_cli_option_set_print_help(
            program->global_options
        );
        if (result != FRG_STATUS_OK) {
            return result;
        }

        if (command->option_set->options != NULL) {
            printf("\n");
            printf("Command options:\n");

            result = frg_cli_option_set_print_help(command->option_set);
            if (result != FRG_STATUS_OK) {
                return result;
            }
        }
    }

    return FRG_STATUS_OK;
}

frg_status_t frg_cli_program_print_version_long(
    const frg_cli_program_t* program
) {
    if (program == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    }

    printf(
        "%s %d.%d.%d%s%s\n", program->name, program->version_major,
        program->version_minor, program->version_patch,
        program->version_label == NULL ? "" : "-", program->version_label);

    if (program->version_details != NULL) {
        frg_set_color(stdout, FRG_COLOR_ID_BRIGHT_BLACK);
        printf("%s\n", program->version_details->str);
        frg_set_color(stdout, FRG_COLOR_ID_RESET);
    }

    return FRG_STATUS_OK;
}

frg_status_t frg_cli_program_print_version_short(
    const frg_cli_program_t* program
) {
    if (program == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    }

    printf(
        "%d.%d.%d%s%s\n", program->version_major,
        program->version_minor, program->version_patch,
        program->version_label == NULL ? "" : "-", program->version_label);

    return FRG_STATUS_OK;
}

bool frg_cli_program_parse(
    frg_cli_program_t* program,
    int argc,
    const char** argv,
    void* user_data
) {
    if (program == NULL || argv == NULL) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "program or argv is NULL");
        return false;
    } else if (argc < 1) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "argc less than 1");
        return false;
    }

    int argi = 1;

    while (argi < argc) {
        if (argv[argi] == NULL) {
            frg_log_prefix_internal();
            frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "argv[%i] is NULL", argi);
            return false;
        } else if (argv[argi][0] == 0) {
            frg_log_prefix_internal();
            frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "argv[%i] is an empty string", argi);
            return false;
        } else if (argv[argi][0] == '-') {
            int argi_last = argi;
            if (!frg_cli_option_set_parse_next(
                program->global_options,
                &argi,
                argc,
                argv,
                user_data
            )) {
                return false;
            }

            if (argi_last == argi) {
                frg_log_prefix_internal();
                frg_log(FRG_LOG_SEVERITY_FATAL_ERROR, "argi not incremented after parsing '%s'", argv[argi]);
                return false;
            }
        } else {
            break;
        }
    }

    if (program->commands != NULL) {
        if (argi >= argc) {
            frg_log(FRG_LOG_SEVERITY_FATAL_ERROR, "no command provided");
            printf("\n");
            frg_cli_program_print_help(program, NULL);
            return false;
        }

        frg_cli_command_t* command = (frg_cli_command_t*)g_hash_table_lookup(program->commands_by_name, argv[argi]);

        if (command == NULL) {
            frg_log(FRG_LOG_SEVERITY_FATAL_ERROR, "no such command: %s", argv[argi]);
            printf("\n");
            frg_cli_program_print_help(program, NULL);
            return false;
        }
    }
}
