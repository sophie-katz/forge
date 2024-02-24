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
// #include <forge/config/commands/common.h>
// #include <forge/config/commands/dump_ast.h>
// #include <forge/config/config.h>
// #include <forge/parsing/parsing.h>

// int _frg_config_commands_callback_dump_ast(frg_message_buffer_t* mut_message_buffer,
//                                            const struct frg_cli_program_t* program,
//                                            void* mut_user_data,
//                                            GList* positional_arguments) {
//   frg_assert_pointer_non_null(program);

//   const char* path
//     = frg_config_commands_get_single_source_file(message_buffer, positional_arguments);

//   if (path == NULL) {
//     return 1;
//   }

//   FILE* file = fopen(path, "r");
//   if (file == NULL) {
//     frg_message_emit_ff_2_open_for_reading(message_buffer, path, strerror(errno));

//     return 1;
//   }

//   frg_parsing_source_t* source = frg_parsing_source_new_file(file, path, false);

//   frg_ast_node_t* node         = frg_parse(message_buffer, source);

//   if (ast == NULL) {
//     frg_parsing_source_destroy(source);

//     fclose(file);

//     return 1;
//   }

//   frg_ast_print_debug(stdout, ast, 0);

//   printf("\n");

//   frg_ast_destroy(ast);

//   frg_parsing_source_destroy(source);

//   fclose(file);

//   return 0;
// }

// frg_cli_command_t* frg_config_commands_new_dump_ast() {
//   return frg_cli_command_new("dump-ast",
//                              "source file",
//                              "Dump the AST of the source file.",
//                              _frg_config_commands_callback_dump_ast);
// }
