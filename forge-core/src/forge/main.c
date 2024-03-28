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

#include <build_configuration.h>
#include <forge/configuration/options.h>

int main(int argc, char* argv[]) {
  // frg_stream_output_init();

  // frg_parsing_source_context_t* source_context = frg_parsing_source_context_new();

  // frg_message_buffer_t* message_buffer         = frg_message_buffer_new();

  // frg_configuration_options_t* options = frg_configuration_options_new_default();

  // int exit_status                      = 0;
  // if (!frg_configuration_options_parse_environment(message_buffer, options)) {
  //   exit_status = 1;
  // }

  // if (exit_status == 0) {
  //   exit_status = frg_configuration_options_parse_cli(
  //     message_buffer, options, argc, (const char**)argv);
  // }

  // frg_message_buffer_print(frg_stream_output_get_stderr(),
  //                          source_context,
  //                          message_buffer,
  //                          options->minimum_message_severity,
  //                          5 // TODO: Make this configurable
  // );

  // frg_stream_output_cleanup();

  // frg_message_buffer_destroy(message_buffer);

  // frg_parsing_source_context_destroy(source_context);

  // return exit_status;

  return 0;
}
