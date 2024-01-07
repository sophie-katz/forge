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

#include <forge/ast/ast.h>
#include <forge/ast/debug.h>
#include <forge/config/commands/compile.h>
#include <forge/config/config.h>
#include <forge/parsing/parsing.h>
#include <forge/common/error.h>
#include <forge/common/log.h>
#include <forge/codegen/codegen.h>
#include <forge/config/commands/common.h>

int _frg_config_commands_callback_compile(
    const struct frg_cli_program_t* program,
    void* user_data,
    GList* pos_args
) {
    const frg_config_t* config = (const frg_config_t*)user_data;
    if (config->compile_output_path == NULL) {
        frg_log_fatal_error("no output path specified with -o or --output-path");
        return 1;
    }

    const char* path = frg_config_commands_get_single_source_file(
        pos_args
    );

    if (path == NULL) {
        return 1;
    }

    frg_ast_t* ast = frg_parse_file_at_path(
        path
    );

    if (ast == NULL) {
        return 1;
    }

    frg_llvm_module_t* llvm_module = frg_codegen(
        ast
    );

    if (llvm_module == NULL) {
        return 1;
    }

    frg_recoverable_status_t result = frg_codegen_write_object_file(
        llvm_module,
        config->compile_output_path
    );
    if (result != FRG_RECOVERABLE_STATUS_OK) {
        return 1;
    }

    frg_codegen_destroy_module(&llvm_module);

    return 0;
}

bool _frg_config_commands_option_callback_output_path(
    void* user_data,
    const char* value
) {
    frg_config_t* config = (frg_config_t*)user_data;
    config->compile_output_path = value;

    return true;
}

frg_cli_command_t* frg_config_commands_new_compile() {
    frg_cli_command_t* command = frg_cli_command_new(
        "compile",
        "source file",
        "Compiles one source file into an object file.",
        _frg_config_commands_callback_compile
    );

    frg_cli_option_t* option = frg_cli_option_new_argument_short(
        'o',
        "output-path",
        "path",
        "The path to output the *.o file to.",
        _frg_config_commands_option_callback_output_path
    );

    frg_cli_option_set_add_option(
        command->option_set,
        option
    );

    return command;
}
