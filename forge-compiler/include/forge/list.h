#pragma once

#include <forge/types.h>

typedef struct {
    void* data;
    struct frg_list_element_t* next;
} frg_list_element_t;

typedef struct {
    frg_list_element_t* first;
    frg_list_element_t* last;
    frg_size_t length;
    void (*destroyer)(void**);
} frg_list_t;

frg_list_t* frg_list_new(void (*destroyer)(void**));
void frg_list_destroy(frg_list_t** list);

void frg_list_push_end(frg_list_t* list, void* data);

frg_size_t frg_list_get_length(const frg_list_t* list);
void* frg_list_get_first(const frg_list_t* list);
void* frg_list_get_last(const frg_list_t* list);
void* frg_list_get_at_index(const frg_list_t* list, frg_size_t index);
