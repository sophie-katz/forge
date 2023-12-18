#pragma once

#include <forge/types.h>

typedef uint8_t frg_char_t;

typedef struct {
    frg_char_t* data;
    frg_size_t length;
} frg_str_t;

frg_str_t* frg_str_new_from_char_ptr(const char* ptr);
frg_str_t* frg_str_new_from_data(frg_char_t* data, frg_size_t length);
void frg_str_destroy(frg_str_t** str);
