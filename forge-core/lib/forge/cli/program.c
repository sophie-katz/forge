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
#include <forge/common/color.h>
#include <forge/common/log.h>
#include <forge/common/memory.h>
#include <forge/cli/program.h>

frg_status_t frg_cli_program_new(
    frg_cli_program_t** program,
    const char* name,
    const char* binary_name,
    const char* pos_args_name,
    int version_major,
    int version_minor,
    int version_patch,
    const char* version_label,
    GString* version_details,
    frg_cli_program_callback_t callback
) {
    if (program == NULL || name == NULL || binary_name == NULL
        || version_label == NULL || callback == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    } else if (*name == 0 || *binary_name == 0 || *version_label == 0) {
        return FRG_STATUS_ERROR_EMPTY_STRING;
    } else if (*program != NULL || version_major < 0 || version_minor < 0
        || version_patch < 0) {
        return FRG_STATUS_ERROR_UNEXPECTED_ARGUMENT_VALUE;
    }

    frg_check(
        frg_safe_malloc((void**)program, sizeof(frg_cli_program_t))
    );

    (*program)->name = name;
    (*program)->binary_name = binary_name;
    (*program)->pos_args_name = pos_args_name;
    (*program)->version_major = version_major;
    (*program)->version_minor = version_minor;
    (*program)->version_patch = version_patch;
    (*program)->version_label = version_label;
    (*program)->version_details = version_details;
    (*program)->callback = callback;
    (*program)->commands = NULL;
    (*program)->commands_by_name = g_hash_table_new(g_str_hash, g_str_equal);

    (*program)->global_options = NULL;
    frg_check(
        frg_cli_option_set_new(
            &(*program)->global_options
        )
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
        frg_check(
            frg_cli_command_destroy((frg_cli_command_t**)&command->data)
        );
    }

    g_list_free((*program)->commands);

    g_hash_table_destroy((*program)->commands_by_name);

    frg_check(
        frg_cli_option_set_destroy(
            &(*program)->global_options
        )
    );

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

frg_status_t frg_cli_program_get_command_by_name(
    frg_cli_command_t** command,
    frg_cli_program_t* program,
    const char* name
) {
    if (command == NULL || program == NULL || name == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    } else if (*name == 0) {
        return FRG_STATUS_ERROR_EMPTY_STRING;
    }

    *command = (frg_cli_command_t*)g_hash_table_lookup(program->commands_by_name, name);
    if (*command == NULL) {
        return FRG_STATUS_ERROR_KEY_NOT_FOUND;
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

    frg_check(
        frg_cli_program_print_version_long(program)
    );

    printf("\n");

    if (command_name == NULL) {
        if (program->commands == NULL) {
            printf("Usage: ");
            frg_set_color(stdout, FRG_COLOR_ID_BOLD);
            printf(
                "%s [options]%s%s%s\n",
                program->binary_name,
                program->pos_args_name == NULL
                    ? ""
                    : " <",
                program->pos_args_name,
                program->pos_args_name == NULL
                    ? ""
                    : ">"
            );
            frg_set_color(stdout, FRG_COLOR_ID_RESET);
            printf("\n");
            frg_set_color(stdout, FRG_COLOR_ID_UNDERLINE);
            printf("Options:");
            frg_set_color(stdout, FRG_COLOR_ID_RESET);
            printf("\n");
        } else {
            printf("Usage: ");
            frg_set_color(stdout, FRG_COLOR_ID_BOLD);
            printf("%s [global options] <command> [command options]\n", program->binary_name);
            frg_set_color(stdout, FRG_COLOR_ID_RESET);
            printf("\n");
            frg_set_color(stdout, FRG_COLOR_ID_UNDERLINE);
            printf("Global options:");
            frg_set_color(stdout, FRG_COLOR_ID_RESET);
            printf("\n");
        }

        frg_check(
            frg_cli_option_set_print_help(
                program->global_options
            )
        );

        if (program->commands != NULL) {
            printf("\n");
            frg_set_color(stdout, FRG_COLOR_ID_UNDERLINE);
            printf("Commands:");
            frg_set_color(stdout, FRG_COLOR_ID_RESET);
            printf("\n");

            for (GList* command = program->commands; command != NULL; command = command->next) {
                frg_set_color(stdout, FRG_COLOR_ID_BOLD);
                printf("  %s\n", ((frg_cli_command_t*)command->data)->name);
                frg_set_color(stdout, FRG_COLOR_ID_RESET);
            }

            printf("\n");
            printf("To see more info about a given command, run:\n");
            frg_set_color(stdout, FRG_COLOR_ID_BRIGHT_BLACK);
            printf("$ ");
            frg_set_color(stdout, FRG_COLOR_ID_RESET);
            printf("%s help <command>\n", program->binary_name);
        }
    } else {
        frg_cli_command_t* command = (frg_cli_command_t*)g_hash_table_lookup(program->commands_by_name, command_name);
        if (command == NULL) {
            printf("Unknown command '%s'\n", command_name);
            return FRG_STATUS_ERROR_UNEXPECTED_ARGUMENT_VALUE;
        }

        printf("Usage: ");
        frg_set_color(stdout, FRG_COLOR_ID_BOLD);
        printf(
            "%s [global options] %s [command options]%s%s%s\n\n",
            program->binary_name,
            command_name,
            command->pos_args_name == NULL
                ? ""
                : " <",
            command->pos_args_name,
            command->pos_args_name == NULL
                ? ""
                : ">"
        );
        frg_set_color(stdout, FRG_COLOR_ID_RESET);
        
        frg_cli_command_print_help(command);

        printf("\n");
        frg_set_color(stdout, FRG_COLOR_ID_UNDERLINE);
        printf("Global options:");
        frg_set_color(stdout, FRG_COLOR_ID_RESET);
        printf("\n");

        frg_check(
            frg_cli_option_set_print_help(
                program->global_options
            )
        );

        if (command->option_set->options != NULL) {
            printf("\n");
            frg_set_color(stdout, FRG_COLOR_ID_UNDERLINE);
            printf("Command options:");
            frg_set_color(stdout, FRG_COLOR_ID_RESET);
            printf("\n");

            frg_check(
                frg_cli_option_set_print_help(command->option_set)
            );
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

frg_status_t frg_cli_program_parse(
    int* exit_status,
    frg_cli_program_t* program,
    int argc,
    const char** argv,
    void* user_data
) {
    if (exit_status == NULL || program == NULL || argv == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    } else if (argc < 1) {
        return FRG_STATUS_ERROR_UNEXPECTED_ARGUMENT_VALUE;
    }

    GList* pos_args = NULL;

    int argi = 1;

    while (argi < argc) {
        if (argv[argi] == NULL) {
            return FRG_STATUS_ERROR_UNEXPECTED_ARGUMENT_VALUE;
        } else if (argv[argi][0] == 0) {
            return FRG_STATUS_ERROR_EMPTY_STRING;
        } else if (argv[argi][0] == '-') {
            frg_status_t result = frg_cli_option_set_parse_next(
                program->global_options,
                &argi,
                argc,
                argv,
                user_data
            );
            if (result == FRG_STATUS_CLI_ERROR) {
                return FRG_STATUS_CLI_ERROR;
            } else {
                frg_check(result);
            }
        } else {
            if (program->commands == NULL) {
                pos_args = g_list_append(pos_args, (void*)argv[argi]);
                argi++;
            } else {
                break;
            }
        }
    }

    if (program->commands == NULL || argi >= argc) {
        if (program->callback == NULL) {
            return FRG_STATUS_ERROR_UNEXPECTED_ARGUMENT_VALUE;
        }

        return ((frg_cli_program_callback_t)(program->callback))(
            exit_status,
            program,
            user_data,
            pos_args
        );
    } else {
        frg_cli_command_t* command = NULL;
        frg_status_t result = frg_cli_program_get_command_by_name(
            &command,
            program,
            argv[argi]
        );
        if (result == FRG_STATUS_ERROR_KEY_NOT_FOUND) {
            frg_log_fatal_error("unknown command '%s'", argv[argi]);
            return FRG_STATUS_CLI_ERROR;
        } else {
            frg_check(result);
        }

        argi++;

        while (argi < argc) {
            if (argv[argi] == NULL) {
                return FRG_STATUS_ERROR_UNEXPECTED_ARGUMENT_VALUE;
            } else if (argv[argi][0] == 0) {
                return FRG_STATUS_ERROR_EMPTY_STRING;
            } else if (argv[argi][0] == '-') {
                result = frg_cli_option_set_parse_next(
                    command->option_set,
                    &argi,
                    argc,
                    argv,
                    user_data
                );
                if (result == FRG_STATUS_CLI_ERROR) {
                    return FRG_STATUS_CLI_ERROR;
                } else {
                    frg_check(result);
                }
            } else {
                pos_args = g_list_append(pos_args, (void*)argv[argi]);
                argi++;
            }
        }

        if (command->callback == NULL) {
            return FRG_STATUS_ERROR_UNEXPECTED_ARGUMENT_VALUE;
        }

        return ((frg_cli_command_callback_t)(command->callback))(
            exit_status,
            (const struct frg_cli_program_t*)program,
            user_data,
            pos_args
        );
    }

    return FRG_STATUS_OK;
}
