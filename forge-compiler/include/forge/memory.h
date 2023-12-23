#pragma once

#include <stddef.h>
#include <forge/enums.h>

frg_status_t frg_safe_malloc(void** ptr, size_t size);
frg_status_t frg_safe_free(void** ptr);
