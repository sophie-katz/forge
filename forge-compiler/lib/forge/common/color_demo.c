#include <forge/common/color.h>
#include <stdio.h>

int main(void) {
    printf("Font styles:\n");

    frg_set_color(stdout, FRG_COLOR_ID_BOLD);
    printf("  bold\n");

    frg_set_color(stdout, FRG_COLOR_ID_RESET);
    frg_set_color(stdout, FRG_COLOR_ID_DIM);
    printf("  dim\n");

    frg_set_color(stdout, FRG_COLOR_ID_RESET);
    frg_set_color(stdout, FRG_COLOR_ID_ITALIC);
    printf("  italic\n");

    frg_set_color(stdout, FRG_COLOR_ID_RESET);
    printf("  ");
    frg_set_color(stdout, FRG_COLOR_ID_UNDERLINE);
    printf("underline");
    frg_set_color(stdout, FRG_COLOR_ID_RESET);
    printf("\n");

    printf("\nColors:\n");

    frg_set_color(stdout, FRG_COLOR_ID_RED);
    printf("  red\n");

    frg_set_color(stdout, FRG_COLOR_ID_GREEN);
    printf("  green\n");

    frg_set_color(stdout, FRG_COLOR_ID_YELLOW);
    printf("  yellow\n");

    frg_set_color(stdout, FRG_COLOR_ID_BLUE);
    printf("  blue\n");

    frg_set_color(stdout, FRG_COLOR_ID_MAGENTA);
    printf("  magenta\n");

    frg_set_color(stdout, FRG_COLOR_ID_CYAN);
    printf("  cyan\n");

    frg_set_color(stdout, FRG_COLOR_ID_WHITE);
    printf("  white\n");

    frg_set_color(stdout, FRG_COLOR_ID_BRIGHT_BLACK);
    printf("  bright black\n");

    frg_set_color(stdout, FRG_COLOR_ID_BRIGHT_RED);
    printf("  bright red\n");

    frg_set_color(stdout, FRG_COLOR_ID_BRIGHT_GREEN);
    printf("  bright green\n");

    frg_set_color(stdout, FRG_COLOR_ID_BRIGHT_YELLOW);
    printf("  bright yellow\n");

    frg_set_color(stdout, FRG_COLOR_ID_BRIGHT_BLUE);
    printf("  bright blue\n");

    frg_set_color(stdout, FRG_COLOR_ID_BRIGHT_MAGENTA);
    printf("  bright magenta\n");

    frg_set_color(stdout, FRG_COLOR_ID_BRIGHT_CYAN);
    printf("  bright cyan\n");

    frg_set_color(stdout, FRG_COLOR_ID_BRIGHT_WHITE);
    printf("  bright white\n");

    frg_set_color(stdout, FRG_COLOR_ID_DIM);

    frg_set_color(stdout, FRG_COLOR_ID_RED);
    printf("  dimmed red\n");

    frg_set_color(stdout, FRG_COLOR_ID_GREEN);
    printf("  dimmed green\n");

    frg_set_color(stdout, FRG_COLOR_ID_YELLOW);
    printf("  dimmed yellow\n");

    frg_set_color(stdout, FRG_COLOR_ID_BLUE);
    printf("  dimmed blue\n");

    frg_set_color(stdout, FRG_COLOR_ID_MAGENTA);
    printf("  dimmed magenta\n");

    frg_set_color(stdout, FRG_COLOR_ID_CYAN);
    printf("  dimmed cyan\n");

    frg_set_color(stdout, FRG_COLOR_ID_WHITE);
    printf("  dimmed white\n");

    frg_set_color(stdout, FRG_COLOR_ID_BRIGHT_BLACK);
    printf("  dimmed bright black\n");

    frg_set_color(stdout, FRG_COLOR_ID_BRIGHT_RED);
    printf("  dimmed bright red\n");

    frg_set_color(stdout, FRG_COLOR_ID_BRIGHT_GREEN);
    printf("  dimmed bright green\n");

    frg_set_color(stdout, FRG_COLOR_ID_BRIGHT_YELLOW);
    printf("  dimmed bright yellow\n");

    frg_set_color(stdout, FRG_COLOR_ID_BRIGHT_BLUE);
    printf("  dimmed bright blue\n");

    frg_set_color(stdout, FRG_COLOR_ID_BRIGHT_MAGENTA);
    printf("  dimmed bright magenta\n");

    frg_set_color(stdout, FRG_COLOR_ID_BRIGHT_CYAN);
    printf("  dimmed bright cyan\n");

    frg_set_color(stdout, FRG_COLOR_ID_BRIGHT_WHITE);
    printf("  dimmed bright white\n");

    return 0;
}
