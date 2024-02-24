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

#include <forge/streams/output.h>
#include <stdio.h>

int main() {
    frg_stream_output_init();

    frg_stream_output_write_string(frg_stream_output_get_stdout(), "Font styles:\n");

    frg_stream_output_set_color(frg_stream_output_get_stdout(), FRG_STREAM_OUTPUT_COLOR_BOLD);
    frg_stream_output_write_string(frg_stream_output_get_stdout(), "  bold\n");

    frg_stream_output_set_color(frg_stream_output_get_stdout(), FRG_STREAM_OUTPUT_COLOR_RESET);
    frg_stream_output_set_color(frg_stream_output_get_stdout(), FRG_STREAM_OUTPUT_COLOR_DIM);
    frg_stream_output_write_string(frg_stream_output_get_stdout(), "  dim\n");

    frg_stream_output_set_color(frg_stream_output_get_stdout(), FRG_STREAM_OUTPUT_COLOR_RESET);
    frg_stream_output_set_color(frg_stream_output_get_stdout(), FRG_STREAM_OUTPUT_COLOR_ITALIC);
    frg_stream_output_write_string(frg_stream_output_get_stdout(), "  italic\n");

    frg_stream_output_set_color(frg_stream_output_get_stdout(), FRG_STREAM_OUTPUT_COLOR_RESET);
    frg_stream_output_write_string(frg_stream_output_get_stdout(), "  ");
    frg_stream_output_set_color(frg_stream_output_get_stdout(), FRG_STREAM_OUTPUT_COLOR_UNDERLINE);
    frg_stream_output_write_string(frg_stream_output_get_stdout(), "underline");
    frg_stream_output_set_color(frg_stream_output_get_stdout(), FRG_STREAM_OUTPUT_COLOR_RESET);
    frg_stream_output_write_string(frg_stream_output_get_stdout(), "\n");

    frg_stream_output_write_string(frg_stream_output_get_stdout(), "\nColors:\n");

    frg_stream_output_set_color(frg_stream_output_get_stdout(), FRG_STREAM_OUTPUT_COLOR_RED);
    frg_stream_output_write_string(frg_stream_output_get_stdout(), "  red\n");

    frg_stream_output_set_color(frg_stream_output_get_stdout(), FRG_STREAM_OUTPUT_COLOR_GREEN);
    frg_stream_output_write_string(frg_stream_output_get_stdout(), "  green\n");

    frg_stream_output_set_color(frg_stream_output_get_stdout(), FRG_STREAM_OUTPUT_COLOR_YELLOW);
    frg_stream_output_write_string(frg_stream_output_get_stdout(), "  yellow\n");

    frg_stream_output_set_color(frg_stream_output_get_stdout(), FRG_STREAM_OUTPUT_COLOR_BLUE);
    frg_stream_output_write_string(frg_stream_output_get_stdout(), "  blue\n");

    frg_stream_output_set_color(frg_stream_output_get_stdout(), FRG_STREAM_OUTPUT_COLOR_MAGENTA);
    frg_stream_output_write_string(frg_stream_output_get_stdout(), "  magenta\n");

    frg_stream_output_set_color(frg_stream_output_get_stdout(), FRG_STREAM_OUTPUT_COLOR_CYAN);
    frg_stream_output_write_string(frg_stream_output_get_stdout(), "  cyan\n");

    frg_stream_output_set_color(frg_stream_output_get_stdout(), FRG_STREAM_OUTPUT_COLOR_WHITE);
    frg_stream_output_write_string(frg_stream_output_get_stdout(), "  white\n");

    frg_stream_output_set_color(frg_stream_output_get_stdout(), FRG_STREAM_OUTPUT_COLOR_BRIGHT_BLACK);
    frg_stream_output_write_string(frg_stream_output_get_stdout(), "  bright black\n");

    frg_stream_output_set_color(frg_stream_output_get_stdout(), FRG_STREAM_OUTPUT_COLOR_BRIGHT_RED);
    frg_stream_output_write_string(frg_stream_output_get_stdout(), "  bright red\n");

    frg_stream_output_set_color(frg_stream_output_get_stdout(), FRG_STREAM_OUTPUT_COLOR_BRIGHT_GREEN);
    frg_stream_output_write_string(frg_stream_output_get_stdout(), "  bright green\n");

    frg_stream_output_set_color(frg_stream_output_get_stdout(), FRG_STREAM_OUTPUT_COLOR_BRIGHT_YELLOW);
    frg_stream_output_write_string(frg_stream_output_get_stdout(), "  bright yellow\n");

    frg_stream_output_set_color(frg_stream_output_get_stdout(), FRG_STREAM_OUTPUT_COLOR_BRIGHT_BLUE);
    frg_stream_output_write_string(frg_stream_output_get_stdout(), "  bright blue\n");

    frg_stream_output_set_color(frg_stream_output_get_stdout(), FRG_STREAM_OUTPUT_COLOR_BRIGHT_MAGENTA);
    frg_stream_output_write_string(frg_stream_output_get_stdout(), "  bright magenta\n");

    frg_stream_output_set_color(frg_stream_output_get_stdout(), FRG_STREAM_OUTPUT_COLOR_BRIGHT_CYAN);
    frg_stream_output_write_string(frg_stream_output_get_stdout(), "  bright cyan\n");

    frg_stream_output_set_color(frg_stream_output_get_stdout(), FRG_STREAM_OUTPUT_COLOR_BRIGHT_WHITE);
    frg_stream_output_write_string(frg_stream_output_get_stdout(), "  bright white\n");

    frg_stream_output_set_color(frg_stream_output_get_stdout(), FRG_STREAM_OUTPUT_COLOR_DIM);

    frg_stream_output_set_color(frg_stream_output_get_stdout(), FRG_STREAM_OUTPUT_COLOR_RED);
    frg_stream_output_write_string(frg_stream_output_get_stdout(), "  dimmed red\n");

    frg_stream_output_set_color(frg_stream_output_get_stdout(), FRG_STREAM_OUTPUT_COLOR_GREEN);
    frg_stream_output_write_string(frg_stream_output_get_stdout(), "  dimmed green\n");

    frg_stream_output_set_color(frg_stream_output_get_stdout(), FRG_STREAM_OUTPUT_COLOR_YELLOW);
    frg_stream_output_write_string(frg_stream_output_get_stdout(), "  dimmed yellow\n");

    frg_stream_output_set_color(frg_stream_output_get_stdout(), FRG_STREAM_OUTPUT_COLOR_BLUE);
    frg_stream_output_write_string(frg_stream_output_get_stdout(), "  dimmed blue\n");

    frg_stream_output_set_color(frg_stream_output_get_stdout(), FRG_STREAM_OUTPUT_COLOR_MAGENTA);
    frg_stream_output_write_string(frg_stream_output_get_stdout(), "  dimmed magenta\n");

    frg_stream_output_set_color(frg_stream_output_get_stdout(), FRG_STREAM_OUTPUT_COLOR_CYAN);
    frg_stream_output_write_string(frg_stream_output_get_stdout(), "  dimmed cyan\n");

    frg_stream_output_set_color(frg_stream_output_get_stdout(), FRG_STREAM_OUTPUT_COLOR_WHITE);
    frg_stream_output_write_string(frg_stream_output_get_stdout(), "  dimmed white\n");

    frg_stream_output_set_color(frg_stream_output_get_stdout(), FRG_STREAM_OUTPUT_COLOR_BRIGHT_BLACK);
    frg_stream_output_write_string(frg_stream_output_get_stdout(), "  dimmed bright black\n");

    frg_stream_output_set_color(frg_stream_output_get_stdout(), FRG_STREAM_OUTPUT_COLOR_BRIGHT_RED);
    frg_stream_output_write_string(frg_stream_output_get_stdout(), "  dimmed bright red\n");

    frg_stream_output_set_color(frg_stream_output_get_stdout(), FRG_STREAM_OUTPUT_COLOR_BRIGHT_GREEN);
    frg_stream_output_write_string(frg_stream_output_get_stdout(), "  dimmed bright green\n");

    frg_stream_output_set_color(frg_stream_output_get_stdout(), FRG_STREAM_OUTPUT_COLOR_BRIGHT_YELLOW);
    frg_stream_output_write_string(frg_stream_output_get_stdout(), "  dimmed bright yellow\n");

    frg_stream_output_set_color(frg_stream_output_get_stdout(), FRG_STREAM_OUTPUT_COLOR_BRIGHT_BLUE);
    frg_stream_output_write_string(frg_stream_output_get_stdout(), "  dimmed bright blue\n");

    frg_stream_output_set_color(frg_stream_output_get_stdout(), FRG_STREAM_OUTPUT_COLOR_BRIGHT_MAGENTA);
    frg_stream_output_write_string(frg_stream_output_get_stdout(), "  dimmed bright magenta\n");

    frg_stream_output_set_color(frg_stream_output_get_stdout(), FRG_STREAM_OUTPUT_COLOR_BRIGHT_CYAN);
    frg_stream_output_write_string(frg_stream_output_get_stdout(), "  dimmed bright cyan\n");

    frg_stream_output_set_color(frg_stream_output_get_stdout(), FRG_STREAM_OUTPUT_COLOR_BRIGHT_WHITE);
    frg_stream_output_write_string(frg_stream_output_get_stdout(), "  dimmed bright white\n");

    frg_stream_output_cleanup();

    return 0;
}
