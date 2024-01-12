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
#include <forge/config/commands/dump_ir.h>
#include <forge/config/commands/common.h>
#include <forge/parsing/parsing.h>
#include <forge/config/config.h>
#include <forge/common/error.h>
#include <forge/codegen/codegen.h>

int _frg_config_commands_callback_dump_ir(
    frg_message_buffer_t* message_buffer,
    const struct frg_cli_program_t* program,
    void* user_data,
    GList* pos_args
) {
    frg_assert_pointer_non_null(program);

    const char* path = frg_config_commands_get_single_source_file(
        message_buffer,
        pos_args
    );
    
    if (path == NULL) {
        return 1;
    }

    FILE* file = fopen(path, "r");
    if (file == NULL) {
        frg_message_emit(
            message_buffer,
            FRG_MESSAGE_SEVERITY_FATAL_ERROR,
            "unable to open source file: %s (%s)",
            path,
            strerror(errno)
        );

        return 1;
    }

    frg_parsing_source_t* source = frg_parsing_source_new_file(
        file,
        path,
        false
    );

    frg_ast_t* ast = frg_parse(
        message_buffer,
        source
    );

    if (ast == NULL) {
        frg_parsing_source_destroy(&source);

        fclose(file);

        return 1;
    }

    frg_llvm_module_t* llvm_module = frg_codegen(
        ast
    );

    frg_ast_destroy(&ast);

    frg_parsing_source_destroy(&source);

    fclose(file);

    if (llvm_module == NULL) {
        return 1;
    }

    frg_codegen_print_module(
        llvm_module
    );

    frg_codegen_destroy_module(&llvm_module);

    return 0;
}

frg_cli_command_t* frg_config_commands_new_dump_ir() {
    return frg_cli_command_new(
        "dump-ir",
        "source file",
        "Dump the compiled intermediate representation of the source file.",
        _frg_config_commands_callback_dump_ir
    );
}
