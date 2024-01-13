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

#include <forge/messages/message_buffer.h>
#include <forge/parsing/source_context.h>

void open_sources(frg_parsing_source_context_t* source_context) {
    static char buffer_file[] =
        "fn add(a: i32, b: i32) -> i32 {\n"
        "  return a + b;\n"
        "}\n";

    FILE* file = fmemopen(buffer_file, sizeof(buffer_file), "r");

    frg_stream_input_t* stream_file = frg_stream_input_new_file(
        file,
        true
    );

    frg_parsing_source_t* source_file = frg_parsing_source_new(
        "lib.frg",
        stream_file
    );

    frg_parsing_source_context_add(
        source_context,
        source_file
    );

    static char buffer_buffer[] =
        "fn main() {\n"
        "  print(\"hello, world\\n\");\n"
        "}\n";

    frg_stream_input_t* stream_buffer = frg_stream_input_new_buffer(
        buffer_buffer,
        false
    );

    frg_parsing_source_t* source_buffer = frg_parsing_source_new(
        "main.frg",
        stream_buffer
    );

    frg_parsing_source_context_add(
        source_context,
        source_buffer
    );
}

int main(void) {
    frg_parsing_source_context_t* source_context = frg_parsing_source_context_new();

    open_sources(source_context);

    frg_message_buffer_t* message_buffer = frg_message_buffer_new();

    frg_message_t* message = frg_message_emit(
        message_buffer,
        FRG_MESSAGE_SEVERITY_DEBUG,
        "This is a global debug message"
    );

    frg_message_emit_child(
        message_buffer,
        message,
        FRG_MESSAGE_SEVERITY_NOTE,
        "This is a global note on a debug message"
    );

    frg_message_emit(
        message_buffer,
        FRG_MESSAGE_SEVERITY_WARNING,
        "This is a global warning message"
    );

    frg_message_emit(
        message_buffer,
        FRG_MESSAGE_SEVERITY_ERROR,
        "This is a global error message"
    );

    frg_message_emit(
        message_buffer,
        FRG_MESSAGE_SEVERITY_FATAL_ERROR,
        "This is a global fatal error message"
    );

    frg_message_emit(
        message_buffer,
        FRG_MESSAGE_SEVERITY_INTERNAL_ERROR,
        "This is a global internal error message"
    );

    frg_parsing_range_t range_file = {
        .length = 0,
        .start = {
            .path = "main.frg",
            .offset = 0,
            .lineno = 0,
            .columnno = 0
        }
    };

    frg_message_emit_from_source_range(
        message_buffer,
        &range_file,
        FRG_MESSAGE_SEVERITY_ERROR,
        "This is an error message for a whole file"
    );

    frg_parsing_range_t range_line = {
        .length = 0,
        .start = {
            .path = "main.frg",
            .offset = 0,
            .lineno = 2,
            .columnno = 0
        }
    };

    frg_message_emit_from_source_range(
        message_buffer,
        &range_line,
        FRG_MESSAGE_SEVERITY_ERROR,
        "This is an error message for a source line"
    );

    frg_parsing_range_t range_char = {
        .length = 1,
        .start = {
            .path = "lib.frg",
            .offset = 46,
            .lineno = 2,
            .columnno = 15
        }
    };

    frg_message_emit_from_source_range(
        message_buffer,
        &range_char,
        FRG_MESSAGE_SEVERITY_ERROR,
        "This is an error message for a character"
    );

    frg_parsing_range_t range_single_line = {
        .length = 4,
        .start = {
            .path = "lib.frg",
            .offset = 41,
            .lineno = 2,
            .columnno = 10
        }
    };

    frg_message_emit_from_source_range(
        message_buffer,
        &range_single_line,
        FRG_MESSAGE_SEVERITY_WARNING,
        "This is a warning message for multiple characters on a single line"
    );

    frg_parsing_range_t range_multi_line = {
        .length = 8,
        .start = {
            .path = "lib.frg",
            .offset = 41,
            .lineno = 2,
            .columnno = 10
        }
    };

    frg_message_emit_from_source_range(
        message_buffer,
        &range_multi_line,
        FRG_MESSAGE_SEVERITY_ERROR,
        "This is an error message for multiple characters on a multiple lines"
    );

    frg_message_buffer_print(
        frg_stream_output_get_stdout(),
        message_buffer,
        source_context,
        FRG_MESSAGE_SEVERITY_DEBUG,
        0
    );

    frg_message_buffer_destroy(&message_buffer);
    frg_parsing_source_context_destroy(&source_context);

    return 0;
}
