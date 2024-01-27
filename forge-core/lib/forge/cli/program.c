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

#include <forge/common/error.h>
#include <forge/common/memory.h>
#include <forge/cli/program.h>
#include <forge/messages/codes.h>

frg_cli_program_t* frg_cli_program_new(
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
    frg_assert_string_non_empty(name);
    frg_assert_string_non_empty(binary_name);

    if (pos_args_name != NULL) {
        frg_assert_string_non_empty(pos_args_name);
    }

    frg_assert_int_ge(version_major, 0);
    frg_assert_int_ge(version_minor, 0);
    frg_assert_int_ge(version_patch, 0);

    if (version_label != NULL) {
        frg_assert_string_non_empty(version_label);
    }

    if (version_details != NULL) {
        frg_assert_gstring_non_empty(version_details);
    }

    frg_cli_program_t* program = frg_safe_malloc(sizeof(frg_cli_program_t));

    program->name = name;
    program->binary_name = binary_name;
    program->pos_args_name = pos_args_name;
    program->version_major = version_major;
    program->version_minor = version_minor;
    program->version_patch = version_patch;
    program->version_label = version_label;
    program->version_details = version_details;
    program->callback = callback;
    program->commands = NULL;
    program->commands_by_name = g_hash_table_new(g_str_hash, g_str_equal);
    program->global_options = frg_cli_option_set_new();

    return program;

}

void frg_cli_program_destroy(
    frg_cli_program_t** program
) {
    frg_assert_pointer_non_null(program);
    frg_assert_pointer_non_null(*program);

    for (GList* command = (*program)->commands; command != NULL; command = command->next) {
        frg_cli_command_destroy((frg_cli_command_t**)&command->data);
    }

    g_list_free((*program)->commands);

    g_hash_table_destroy((*program)->commands_by_name);

    frg_cli_option_set_destroy(
        &(*program)->global_options
    );

    frg_safe_free((void**)program);
}

void frg_cli_program_add_command(
    frg_cli_program_t* program,
    frg_cli_command_t* command
) {
    frg_assert_pointer_non_null(program);
    frg_assert_pointer_non_null(command);

    program->commands = g_list_append(program->commands, command);

    if (g_hash_table_contains(program->commands_by_name, command->name)) {
        frg_die("command '%s' already exists", command->name);
    } else {
        g_hash_table_insert(program->commands_by_name, (void*)command->name, command);
    }
}

frg_cli_command_t* frg_cli_program_get_command_by_name(
    frg_cli_program_t* program,
    const char* name
) {
    frg_assert_pointer_non_null(program);
    frg_assert_string_non_empty(name);

    return (frg_cli_command_t*)g_hash_table_lookup(program->commands_by_name, name);
}

bool frg_cli_program_try_print_help(
    frg_stream_output_t* stream,
    frg_message_buffer_t* message_buffer,
    const frg_cli_program_t* program,
    const char* command_name
) {
    frg_assert_pointer_non_null(program);

    if (command_name != NULL) {
        frg_assert_string_non_empty(command_name);
    }

    frg_cli_program_print_version_long(stream, program);

    frg_stream_output_write_printf(stream, "\n");

    if (command_name == NULL) {
        if (program->commands == NULL) {
            frg_stream_output_write_printf(stream, "Usage: ");
            frg_stream_output_set_color(stream, FRG_STREAM_OUTPUT_COLOR_BOLD);
            frg_stream_output_write_printf(stream, 
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
            frg_stream_output_set_color(stream, FRG_STREAM_OUTPUT_COLOR_RESET);
            frg_stream_output_write_printf(stream, "\n");
            frg_stream_output_set_color(stream, FRG_STREAM_OUTPUT_COLOR_UNDERLINE);
            frg_stream_output_write_printf(stream, "Options:");
            frg_stream_output_set_color(stream, FRG_STREAM_OUTPUT_COLOR_RESET);
            frg_stream_output_write_printf(stream, "\n");
        } else {
            frg_stream_output_write_printf(stream, "Usage: ");
            frg_stream_output_set_color(stream, FRG_STREAM_OUTPUT_COLOR_BOLD);
            frg_stream_output_write_printf(stream, "%s [global options] <command> [command options]\n", program->binary_name);
            frg_stream_output_set_color(stream, FRG_STREAM_OUTPUT_COLOR_RESET);
            frg_stream_output_write_printf(stream, "\n");
            frg_stream_output_set_color(stream, FRG_STREAM_OUTPUT_COLOR_UNDERLINE);
            frg_stream_output_write_printf(stream, "Global options:");
            frg_stream_output_set_color(stream, FRG_STREAM_OUTPUT_COLOR_RESET);
            frg_stream_output_write_printf(stream, "\n");
        }

        frg_cli_option_set_print_help(
            stream,
            program->global_options
        );

        if (program->commands != NULL) {
            frg_stream_output_write_printf(stream, "\n");
            frg_stream_output_set_color(stream, FRG_STREAM_OUTPUT_COLOR_UNDERLINE);
            frg_stream_output_write_printf(stream, "Commands:");
            frg_stream_output_set_color(stream, FRG_STREAM_OUTPUT_COLOR_RESET);
            frg_stream_output_write_printf(stream, "\n");

            for (GList* command = program->commands; command != NULL; command = command->next) {
                frg_stream_output_set_color(stream, FRG_STREAM_OUTPUT_COLOR_BOLD);
                frg_stream_output_write_printf(stream, "  %s\n", ((frg_cli_command_t*)command->data)->name);
                frg_stream_output_set_color(stream, FRG_STREAM_OUTPUT_COLOR_RESET);
            }

            frg_stream_output_write_printf(stream, "\n");
            frg_stream_output_write_printf(stream, "To see more info about a given command, run:\n");
            frg_stream_output_set_color(stream, FRG_STREAM_OUTPUT_COLOR_BRIGHT_BLACK);
            frg_stream_output_write_printf(stream, "$ ");
            frg_stream_output_set_color(stream, FRG_STREAM_OUTPUT_COLOR_RESET);
            frg_stream_output_write_printf(stream, "%s help <command>\n", program->binary_name);
        }
    } else {
        frg_cli_command_t* command = (frg_cli_command_t*)g_hash_table_lookup(program->commands_by_name, command_name);
        
        if (command == NULL) {
            frg_message_emit_fc_11_unknown_command(
                message_buffer,
                command_name,
                program->binary_name
            );

            return false;
        }

        frg_stream_output_write_printf(stream, "Usage: ");
        frg_stream_output_set_color(stream, FRG_STREAM_OUTPUT_COLOR_BOLD);
        frg_stream_output_write_printf(stream, 
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
        frg_stream_output_set_color(stream, FRG_STREAM_OUTPUT_COLOR_RESET);
        
        frg_cli_command_print_help(stream, command);

        frg_stream_output_write_printf(stream, "\n");
        frg_stream_output_set_color(stream, FRG_STREAM_OUTPUT_COLOR_UNDERLINE);
        frg_stream_output_write_printf(stream, "Global options:");
        frg_stream_output_set_color(stream, FRG_STREAM_OUTPUT_COLOR_RESET);
        frg_stream_output_write_printf(stream, "\n");

        frg_cli_option_set_print_help(
            stream,
            program->global_options
        );

        if (command->option_set->options != NULL) {
            frg_stream_output_write_printf(stream, "\n");
            frg_stream_output_set_color(stream, FRG_STREAM_OUTPUT_COLOR_UNDERLINE);
            frg_stream_output_write_printf(stream, "Command options:");
            frg_stream_output_set_color(stream, FRG_STREAM_OUTPUT_COLOR_RESET);
            frg_stream_output_write_printf(stream, "\n");

            frg_cli_option_set_print_help(stream, command->option_set);
        }
    }

    return true;
}

void frg_cli_program_print_version_long(
    frg_stream_output_t* stream,
    const frg_cli_program_t* program
) {
    frg_assert_pointer_non_null(program);

    frg_stream_output_write_printf(stream, 
        "%s %d.%d.%d%s%s\n", program->name, program->version_major,
        program->version_minor, program->version_patch,
        program->version_label == NULL ? "" : "-", program->version_label);

    if (program->version_details != NULL) {
        frg_stream_output_set_color(stream, FRG_STREAM_OUTPUT_COLOR_BRIGHT_BLACK);
        frg_stream_output_write_printf(stream, "%s\n", program->version_details->str);
        frg_stream_output_set_color(stream, FRG_STREAM_OUTPUT_COLOR_RESET);
    }
}

void frg_cli_program_print_version_short(
    frg_stream_output_t* stream,
    const frg_cli_program_t* program
) {
    frg_assert_pointer_non_null(program);

    frg_stream_output_write_printf(stream, 
        "%d.%d.%d%s%s\n", program->version_major,
        program->version_minor, program->version_patch,
        program->version_label == NULL ? "" : "-", program->version_label);
}

int frg_cli_program_parse(
    frg_message_buffer_t* message_buffer,
    const frg_cli_program_t* program,
    int argc,
    const char** argv,
    void* user_data
) {
    frg_assert_pointer_non_null(program);
    frg_assert_int_gt(argc, 0);
    frg_assert_pointer_non_null(argv);

    GList* pos_args = NULL;

    int argi = 1;

    while (argi < argc) {
        frg_assert_string_non_empty(argv[argi]);
        
        if (argv[argi][0] == '-') {
            if (!frg_cli_option_set_parse_next(
                message_buffer,
                program->global_options,
                &argi,
                argc,
                argv,
                user_data
            )) {
                return 1;
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

    if (program->commands == NULL) {
        frg_assert_pointer_non_null(program->callback);

        return ((frg_cli_program_callback_t)(program->callback))(
            message_buffer,
            program,
            user_data,
            pos_args
        );
    } else if (argi >= argc) {
        frg_message_emit_fc_6_expected_command(
            message_buffer,
            program->binary_name
        );

        return 1;
    } {
        frg_cli_command_t* command = frg_cli_program_get_command_by_name(
            (frg_cli_program_t*)program,
            argv[argi]
        );

        if (command == NULL) {
            frg_message_emit_fc_11_unknown_command(
                message_buffer,
                argv[argi],
                program->binary_name
            );

            return 1;
        }

        argi++;

        while (argi < argc) {
            frg_assert_string_non_empty(argv[argi]);
            if (argv[argi][0] == '-') {
                if (!frg_cli_option_set_parse_next(
                    message_buffer,
                    command->option_set,
                    &argi,
                    argc,
                    argv,
                    user_data
                )) {
                    return 1;
                }
            } else {
                pos_args = g_list_append(pos_args, (void*)argv[argi]);
                argi++;
            }
        }

        frg_assert_pointer_non_null(command->callback);

        return ((frg_cli_command_callback_t)(command->callback))(
            message_buffer,
            (const struct frg_cli_program_t*)program,
            user_data,
            pos_args
        );
    }

    return 0;
}
