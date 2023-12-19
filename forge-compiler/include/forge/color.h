#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

typedef uint8_t frg_color_mode_t;

#define FRG_COLOR_MODE_DISABLED 0
#define FRG_COLOR_MODE_AUTO 1
#define FRG_COLOR_MODE_ENABLED 2

frg_color_mode_t frg_get_color_mode(FILE* stream);
frg_color_mode_t frg_set_color_mode(FILE* stream, frg_color_mode_t hint);

typedef uint8_t frg_color_code_t;

#define FRG_COLOR_CODE_RESET 0
#define FRG_COLOR_CODE_NOTE 1
#define FRG_COLOR_CODE_DEBUG 2
#define FRG_COLOR_CODE_WARNING 3
#define FRG_COLOR_CODE_ERROR 4
#define FRG_COLOR_CODE_FATAL_ERROR 5
#define FRG_COLOR_CODE_INTERNAL_ERROR 6
#define FRG_COLOR_CODE_MESSAGE 7
#define FRG_COLOR_CODE_LOCATION 8

bool frg_set_color(FILE* stream, frg_color_code_t code);
