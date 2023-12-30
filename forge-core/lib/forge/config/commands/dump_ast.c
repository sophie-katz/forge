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

#include <forge/ast/ast.h>
#include <forge/ast/debug.h>
#include <forge/config/commands/compile.h>
#include <forge/config/commands/common.h>
#include <forge/parse/parse.h>
#include <forge/config/config.h>
#include <forge/common/log.h>

frg_status_t _frg_config_commands_callback_dump_ast(
    int* exit_status,
    const struct frg_cli_program_t* program,
    void* user_data,
    GList* pos_args
) {
    const char* path = NULL;
    frg_status_t result = frg_config_commands_get_single_source_file(
        &path,
        pos_args
    );
    if (result != FRG_STATUS_OK) {
        return result;
    }

    frg_ast_t* ast = NULL;
    result = frg_parse_file_at_path(
        &ast,
        path
    );
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal(FRG_LOG_SEVERITY_INTERNAL_ERROR);
        frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "unable to parse source file: %s", frg_status_to_string(result));
        return FRG_STATUS_CLI_ERROR;
    }

    frg_ast_print_debug(ast, 0);

    return FRG_STATUS_OK;
}

frg_status_t frg_config_commands_new_dump_ast(frg_cli_command_t** command) {
    frg_status_t result = frg_cli_command_new(
        command,
        "dump-ast",
        "source file",
        "Dump the AST of the source file.",
        _frg_config_commands_callback_dump_ast
    );
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal(FRG_LOG_SEVERITY_INTERNAL_ERROR);
        frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "unable to create CLI command: %s", frg_status_to_string(result));
        return result;
    }

    return FRG_STATUS_OK;
}
