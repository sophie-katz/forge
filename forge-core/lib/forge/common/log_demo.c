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

#include <forge/common/enums.h>
#include <forge/common/log.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("usage: %s <mode>\n", argv[0]);
        printf("\n");
        printf("where mode is one of:\n");
        printf("  none\n");
        printf("  only-errors-single\n");
        printf("  only-errors-multiple\n");
        printf("  only-warnings-single\n");
        printf("  only-warnings-multiple\n");
        printf("  both\n");

        return 1;
    }

    if (strcmp(argv[1], "none") != 0) {
        frg_log_result_t result = frg_log_debug("this is a debug message");

        frg_log_note(&result, "this is a note");

        if (strcmp(argv[1], "only-warnings-single") == 0
            || strcmp(argv[1], "only-warnings-multiple") == 0
            || strcmp(argv[1], "both") == 0) {
            frg_log_warning("this is a warning");

            if (strcmp(argv[1], "only-warnings-multiple") == 0) {
                frg_log_warning_in_source_file("example.frg", "this is another warning in a source file");
                frg_log_warning_on_source_line("--", 5, "this is another warning on a source line");
                frg_log_warning_at_source_char("--", 5, 11, "this is another warning at a source character");
            }
        }

        if (strcmp(argv[1], "only-errors-single") == 0
            || strcmp(argv[1], "only-errors-multiple") == 0
            || strcmp(argv[1], "both") == 0) {
            frg_log_error("this is an error");

            if (strcmp(argv[1], "only-errors-multiple") == 0) {
                frg_log_error_in_source_file("example.frg", "this is another error in a source file");
                frg_log_error_on_source_line("--", 5, "this is another error on a source line");
                frg_log_error_at_source_char("--", 5, 11, "this is another error at a source character");

                frg_log_fatal_error("this is a fatal error");

                frg_log_internal_error("this is an internal error");
            }
        }
    }

    if (frg_log_summary_if_errors()) {
        return 1;
    }

    return 0;
}
