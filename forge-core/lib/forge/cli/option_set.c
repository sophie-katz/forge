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
#include <forge/cli/option_set.h>
#include <forge/messages/codes.h>

int _frg_cli_get_short_name_index(char short_name) {
    if (short_name >= 'a' && short_name <= 'z') {
        return short_name - 'a';
    } else if (short_name >= 'A' && short_name <= 'Z') {
        return short_name - 'A' + 26;
    } else {
        frg_die("invalid short name '%c' (%i)", short_name, short_name);
    }
}

frg_cli_option_set_t* frg_cli_option_set_new() {
    frg_cli_option_set_t* option_set = frg_safe_malloc(sizeof(frg_cli_option_set_t));

    option_set->options = NULL;
    memset(option_set->options_by_short_name, 0, sizeof(option_set->options_by_short_name));
    option_set->options_by_long_name = g_hash_table_new(g_str_hash, g_str_equal);

    return option_set;
}

void frg_cli_option_set_destroy(
    frg_cli_option_set_t** option_set
) {
    frg_assert_pointer_non_null(option_set);
    frg_assert_pointer_non_null(*option_set);

    for (GList* option = (*option_set)->options; option != NULL; option = option->next) {
        frg_cli_option_destroy((frg_cli_option_t**)&option->data);
    }

    g_list_free((*option_set)->options);

    g_hash_table_destroy((*option_set)->options_by_long_name);

    frg_safe_free((void**)option_set);
}

void frg_cli_option_set_add_option(
    frg_cli_option_set_t* option_set,
    frg_cli_option_t* option
) {
    frg_assert_pointer_non_null(option_set);
    frg_assert_pointer_non_null(option);

    option_set->options = g_list_append(option_set->options, option);

    if (option->short_name != FRG_CLI_OPTION_SHORT_NAME_NULL) {
        int index = _frg_cli_get_short_name_index(option->short_name);

        if (option_set->options_by_short_name[index] != NULL) {
            frg_die("short option already set for '%c'", option->short_name);
        }

        option_set->options_by_short_name[index] = option;
    }

    if (g_hash_table_contains(option_set->options_by_long_name, option->long_name)) {
        frg_die("long option already set for '%s'", option->long_name);
    } else {
        g_hash_table_insert(option_set->options_by_long_name, (void*)option->long_name, option);
    }
}

frg_cli_option_t* frg_cli_option_set_get_option_by_long_name(
    frg_cli_option_set_t* option_set,
    const char* name
) {
    frg_assert_pointer_non_null(option_set);
    frg_assert_string_non_empty(name);
    frg_assert_int_ne(name[0], '-');

    return (frg_cli_option_t*)g_hash_table_lookup(option_set->options_by_long_name, name);
}

frg_cli_option_t* frg_cli_option_set_get_option_by_short_name(
    frg_cli_option_set_t* option_set,
    char name
) {
    frg_assert_pointer_non_null(option_set);
    frg_assert(frg_cli_is_valid_short_name(name));

    return option_set->options_by_short_name[_frg_cli_get_short_name_index(name)];
}

void frg_cli_option_set_print_help(
    frg_stream_output_t* stream,
    const frg_cli_option_set_t* option_set
) {
    frg_assert_pointer_non_null(option_set);

    bool first = true;

    for (GList* option = option_set->options; option != NULL; option = option->next) {
        if (first) {
            first = false;
        } else {
            frg_stream_output_write_char(stream, '\n');
        }

        frg_cli_option_print_help(stream, (const frg_cli_option_t*)option->data);
    }
}

bool _frg_cli_option_set_parse_next_long(
    frg_message_buffer_t* message_buffer,
    const frg_cli_option_set_t* option_set,
    int* argi,
    int argc,
    const char** argv,
    void* user_data,
    const char* name
) {
    // Error checking
    frg_assert_pointer_non_null(option_set);
    frg_assert_pointer_non_null(argi);
    frg_assert_pointer_non_null(argv);
    frg_assert_int_ge(*argi, 0);
    frg_assert_int_gt(argc, 0);
    frg_assert_int_lt(*argi, argc);
    frg_assert_string_non_empty(argv[*argi]);
    frg_assert_int_eq(argv[*argi][0], '-');
    frg_assert_int_eq(argv[*argi][1], '-');
    frg_assert_pointer_non_null(name);

    if (name[0] == 0) {
        frg_message_emit_fc_7_invalid_argument_dash_dash(
            message_buffer
        );
        return false;
    }

    frg_cli_option_t* option = frg_cli_option_set_get_option_by_long_name(
        (frg_cli_option_set_t*)option_set,
        name
    );

    if (option == NULL) {
        frg_message_emit_fc_9_unknown_long_argument(
            message_buffer,
            name
        );
        return false;
    }

    return frg_cli_option_parse_next(
        message_buffer,
        option,
        argi,
        argc,
        argv,
        user_data
    );
}

bool _frg_cli_option_set_parse_next_short(
    frg_message_buffer_t* message_buffer,
    const frg_cli_option_set_t* option_set,
    int* argi,
    int argc,
    const char** argv,
    void* user_data,
    char name
) {
    // Error checking
    frg_assert_pointer_non_null(option_set);
    frg_assert_pointer_non_null(argi);
    frg_assert_pointer_non_null(argv);
    frg_assert_int_ge(*argi, 0);
    frg_assert_int_gt(argc, 0);
    frg_assert_int_lt(*argi, argc);
    frg_assert_string_non_empty(argv[*argi]);
    frg_assert_int_eq(argv[*argi][0], '-');

    frg_cli_option_t* option = frg_cli_option_set_get_option_by_short_name(
        (frg_cli_option_set_t*)option_set,
        name
    );

    if (option == NULL) {
        frg_message_emit_fc_9_unknown_long_argument(
            message_buffer,
            name
        );

        return false;
    }

    return frg_cli_option_parse_next(
        message_buffer,
        option,
        argi,
        argc,
        argv,
        user_data
    );
}

bool frg_cli_option_set_parse_next(
    frg_message_buffer_t* message_buffer,
    const frg_cli_option_set_t* option_set,
    int* argi,
    int argc,
    const char** argv,
    void* user_data
) {
    // Error checking
    frg_assert_pointer_non_null(option_set);
    frg_assert_pointer_non_null(argi);
    frg_assert_pointer_non_null(argv);
    frg_assert_int_ge(*argi, 0);
    frg_assert_int_gt(argc, 0);
    frg_assert_int_lt(*argi, argc);
    frg_assert_string_non_empty(argv[*argi]);
    frg_assert_int_eq(argv[*argi][0], '-');

    // Try to parse the next argument
    if (argv[*argi][1] == '-') {
        // It is a long argument like --argument
        return _frg_cli_option_set_parse_next_long(
            message_buffer,
            option_set,
            argi,
            argc,
            argv,
            user_data,
            argv[*argi] + 2
        );
    } else {
        if (argv[*argi][2] != 0) {
            // It is a short argument like -abc
            frg_message_emit_fc_10_long_argument_with_single_dash(
                message_buffer,
                argv[*argi]
            );
            return false;
        }

        // It is a short argument like -a
        return _frg_cli_option_set_parse_next_short(
            message_buffer,
            option_set,
            argi,
            argc,
            argv,
            user_data,
            argv[*argi][1]
        );
    }
}
