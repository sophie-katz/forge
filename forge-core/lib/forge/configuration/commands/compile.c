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

// #include <forge/assert.h>
// #include <forge/ast/debug.h>
// #include <forge/ast/node.h>
// #include <forge/codegen/codegen.h>
// #include <forge/config/commands/common.h>
// #include <forge/config/commands/compile.h>
// #include <forge/config/config.h>
// #include <forge/parsing/parsing.h>

// int _frg_config_commands_callback_compile(frg_message_buffer_t* mut_message_buffer,
//                                           const struct frg_cli_program_t* program,
//                                           void* mut_user_data,
//                                           GList* positional_arguments) {
//   const frg_config_t* config = (const frg_config_t*)user_data;
//   if (config->compile_output_path == NULL) {
//     frg_message_emit_fc_15_missing_required_argument(
//       message_buffer, "output-path", "o");

// return 1;
// }

// const char* path
//   = frg_config_commands_get_single_source_file(message_buffer, positional_arguments);

// if (path == NULL) {
//   return 1;
// }

// FILE* file = fopen(path, "r");
// if (file == NULL) {
//   frg_message_emit_ff_2_open_for_reading(message_buffer, path, strerror(errno));

// return 1;
// }

// frg_parsing_source_t* source = frg_parsing_source_new_file(file, path, false);

// frg_ast_node_t* node         = frg_parse(message_buffer, source);

// if (ast == NULL) {
//   frg_parsing_source_destroy(source);

// fclose(file);

// return 1;
// }

// frg_llvm_module_t* llvm_module = frg_codegen(ast);

// frg_ast_destroy(ast);

// frg_parsing_source_destroy(source);

// fclose(file);

// if (llvm_module == NULL) {
//   return 1;
// }

// frg_recoverable_status_t result = frg_codegen_write_object_file(
//   message_buffer, llvm_module, config->compile_output_path);

// frg_codegen_destroy_module(&llvm_module);

// if (result != FRG_RECOVERABLE_STATUS_OK) {
//   return 1;
// }

// return 0;
// }

// bool _frg_config_commands_option_callback_output_path(
//   frg_message_buffer_t* mut_message_buffer, void* mut_user_data, const char* value) {
//   frg_config_t* config        = (frg_config_t*)user_data;

// config->compile_output_path = value;

// return true;
// }

// frg_cli_command_t* frg_config_commands_new_compile() {
//   frg_cli_command_t* command
//     = frg_cli_command_new("compile",
//                           "source file",
//                           "Compiles one source file into an object file.",
//                           _frg_config_commands_callback_compile);

// frg_cli_option_t* option = frg_cli_option_new_argument_short(
//   'o',
//   "output-path",
//   "path",
//   "The path to output the *.o file to.",
//   _frg_config_commands_option_callback_output_path);

// frg_cli_option_set_add_option(command->option_set, option);

// return command;
// }
