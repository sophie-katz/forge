#include <stdio.h>
#include <string.h>
#include <forge/log.h>
#include <forge/enums.h>

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
        frg_log(FRG_LOG_SEVERITY_NOTE, "this is a note");

        frg_log(FRG_LOG_SEVERITY_DEBUG, "this is a debug message without a location prefix");

        frg_log_prefix_source_file("main.frg");
        frg_log(FRG_LOG_SEVERITY_DEBUG, "this is a debug message with a source file location prefix");

        frg_log_prefix_source_line("main.frg", 11);
        frg_log(FRG_LOG_SEVERITY_DEBUG, "this is a debug message with a source line location prefix");

        frg_log_prefix_source_char("main.frg", 11, 5);
        frg_log(FRG_LOG_SEVERITY_DEBUG, "this is a debug message with a source character location prefix");

        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_DEBUG, "this is a debug message with an internal location prefix");

        if (strcmp(argv[1], "only-warnings-single") == 0
            || strcmp(argv[1], "only-warnings-multiple") == 0
            || strcmp(argv[1], "both") == 0) {
            frg_log(FRG_LOG_SEVERITY_WARNING, "this is a warning");

            if (strcmp(argv[1], "only-warnings-multiple") == 0) {
                frg_log(FRG_LOG_SEVERITY_WARNING, "this is another warning");
            }
        }

        if (strcmp(argv[1], "only-errors-single") == 0
            || strcmp(argv[1], "only-errors-multiple") == 0
            || strcmp(argv[1], "both") == 0) {
            frg_log(FRG_LOG_SEVERITY_ERROR, "this is an error");

            if (strcmp(argv[1], "only-errors-multiple") == 0) {
                frg_log(FRG_LOG_SEVERITY_FATAL_ERROR, "this is a fatal error");

                frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "this is an internal error");
            }
        }
    }

    if (frg_log_summary_if_errors()) {
        return 1;
    }

    return 0;
}
