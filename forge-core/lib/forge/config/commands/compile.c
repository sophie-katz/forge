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
#include <forge/parse/parse.h>
#include <forge/common/check.h>
#include <forge/common/log.h>
#include <forge/codegen/codegen.h>
#include <forge/config/commands/common.h>

frg_status_t _frg_config_commands_callback_compile(
    int* exit_status,
    const struct frg_cli_program_t* program,
    void* user_data,
    GList* pos_args
) {
    frg_config_t* config = (frg_config_t*)user_data;
    if (config->compile_output_path == NULL) {
        frg_log_error("no output path specified with -o or --output-path");
        return FRG_STATUS_CLI_ERROR;
    }

    const char* path = NULL;
    frg_check(
        frg_config_commands_get_single_source_file(
            &path,
            pos_args
        )
    );

    frg_ast_t* ast = NULL;
    frg_check(
        frg_parse_file_at_path(
            &ast,
            path
        )
    );

    frg_llvm_module_t* llvm_module = NULL;
    frg_check(
        frg_codegen(
            &llvm_module,
            ast
        )
    );

    frg_check(
        frg_codegen_write_object_file(
            llvm_module,
            config->compile_output_path
        )
    );

    frg_check(
        frg_codegen_destroy_module(&llvm_module)
    );

    return FRG_STATUS_OK;
}

frg_status_t _frg_config_commands_option_callback_output_path(
    void* user_data,
    const char* value
) {
    frg_config_t* config = (frg_config_t*)user_data;
    config->compile_output_path = value;

    return FRG_STATUS_OK;
}

frg_status_t frg_config_commands_new_compile(frg_cli_command_t** command) {
    frg_check(
        frg_cli_command_new(
            command,
            "compile",
            "source file",
            "Compiles one source file into an object file.",
            _frg_config_commands_callback_compile
        )
    );

    frg_cli_option_t* option = NULL;
    frg_check(
        frg_cli_option_new_argument_short(
            &option,
            'o',
            "output-path",
            "path",
            "The path to output the *.o file to.",
            _frg_config_commands_option_callback_output_path
        )
    );

    frg_check(
        frg_cli_option_set_add_option(
            (*command)->option_set,
            option
        )
    );

    return FRG_STATUS_OK;
}
