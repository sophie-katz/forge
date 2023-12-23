#include <forge/memory.h>
#include <stdlib.h>

frg_status_t frg_safe_malloc(void** ptr, size_t size) {
    if (ptr == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    } else if (size == 0) {
        return FRG_STATUS_ERROR_UNEXPECTED_ARGUMENT_VALUE;
    }

    *ptr = malloc(size);

    if (*ptr == NULL) {
        return FRG_STATUS_ERROR_OUT_OF_MEMORY;
    }

    return FRG_STATUS_OK;
}

frg_status_t frg_safe_free(void** ptr) {
    if (ptr == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    }

    if (*ptr != NULL) {
        free(*ptr);
        *ptr = NULL;
    }

    return FRG_STATUS_OK;
}
