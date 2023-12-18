#include <stdlib.h>
#include <forge/list.h>

frg_list_t* frg_list_new(void (*destroyer)(void**)) {
    frg_list_t* list = malloc(sizeof(frg_list_t));
    list->first = NULL;
    list->last = NULL;
    list->length = 0;
    list->destroyer = destroyer;
    return list;
}

void frg_list_destroy(frg_list_t** list) {
    frg_list_element_t* element = (*list)->first;

    while (element != NULL) {
        frg_list_element_t* next = (frg_list_element_t*)(element->next);
        if ((*list)->destroyer != NULL) {
            (*list)->destroyer(&element->data);
        }
        free(element);
        element = next;
    }

    free(*list);
    *list = NULL;
}

void frg_list_push_end(frg_list_t* list, void* data) {
    frg_list_element_t* element = malloc(sizeof(frg_list_element_t));
    element->data = data;
    element->next = NULL;

    if (list->first == NULL) {
        list->first = element;
        list->last = element;
    } else {
        list->last->next = (struct frg_list_element_t*)element;
        list->last = element;
    }

    list->length++;
}

frg_size_t frg_list_get_length(const frg_list_t* list) {
    return list->length;
}

void* frg_list_get_first(const frg_list_t* list) {
    if (list->first == NULL) {
        return NULL;
    }

    return list->first->data;
}

void* frg_list_get_last(const frg_list_t* list) {
    if (list->last == NULL) {
        return NULL;
    }

    return list->last->data;
}

void* frg_list_get_at_index(const frg_list_t* list, frg_size_t index) {
    frg_list_element_t* element = list->first;

    for (frg_size_t i = 0; i < index; i++) {
        element = (frg_list_element_t*)element->next;
    }
    return element->data;
}
