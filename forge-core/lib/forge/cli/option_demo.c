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

#include <forge/cli/option.h>

bool callback(frg_message_buffer_t* message_buffer, void* user_data, const char* value) {
    return true;
}

void flag(void) {
    frg_cli_option_t* option = frg_cli_option_new_flag(
        "long-name",
        "A description of the option",
        callback
    );

    frg_cli_option_print_help(frg_stream_output_get_stdout(), option);

    frg_cli_option_destroy(&option);
}

void flag_short(void) {
    frg_cli_option_t* option = frg_cli_option_new_flag_short(
        'l',
        "long-name",
        "A description of the option",
        callback
    );

    frg_cli_option_print_help(frg_stream_output_get_stdout(), option);

    frg_cli_option_destroy(&option);
}

void argument(void) {
    frg_cli_option_t* option = frg_cli_option_new_argument(
        "long-name",
        "value",
        "A description of the option",
        callback
    );

    frg_cli_option_print_help(frg_stream_output_get_stdout(), option);

    frg_cli_option_destroy(&option);
}

void argument_short(void) {
    frg_cli_option_t* option = frg_cli_option_new_argument_short(
        'l',
        "long-name",
        "value",
        "A description of the option",
        callback
    );

    frg_cli_option_print_help(frg_stream_output_get_stdout(), option);

    frg_cli_option_destroy(&option);
}

void choice(void) {
    frg_cli_option_t* option = frg_cli_option_new_choice(
        "long-name",
        "value",
        "A description of the option",
        callback
    );

    frg_cli_option_add_choice(
        option,
        frg_cli_choice_new(
            "option-1",
            "A description of the choice"
        )
    );

    frg_cli_option_add_choice(
        option,
        frg_cli_choice_new(
            "option-2",
            "A description of the choice"
        )
    );

    frg_cli_option_print_help(frg_stream_output_get_stdout(), option);

    frg_cli_option_destroy(&option);
}

void choice_short(void) {
    frg_cli_option_t* option = frg_cli_option_new_choice_short(
        'l',
        "long-name",
        "value",
        "A description of the option",
        callback
    );

    frg_cli_option_add_choice(
        option,
        frg_cli_choice_new(
            "option-1",
            "A description of the choice"
        )
    );

    frg_cli_option_add_choice(
        option,
        frg_cli_choice_new(
            "option-2",
            "A description of the choice"
        )
    );

    frg_cli_option_print_help(frg_stream_output_get_stdout(), option);

    frg_cli_option_destroy(&option);
}

int main(void) {
    printf("== FLAG ==\n\n");
    flag();
    printf("\n== FLAG (SHORT) ==\n\n");
    flag_short();
    printf("\n== ARGUMENT ==\n\n");
    argument();
    printf("\n== ARGUMENT (SHORT) ==\n\n");
    argument_short();
    printf("\n== CHOICE ==\n\n");
    choice();
    printf("\n== CHOICE (SHORT) ==\n\n");
    choice_short();

    return 0;
}
