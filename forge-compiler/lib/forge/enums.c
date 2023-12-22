#include <forge/enums.h>

const char* frg_status_to_string(frg_status_t status) {
    switch (status) {
        case FRG_STATUS_OK:
            return "ok";
        case FRG_STATUS_ERROR_NULL_ARGUMENT:
            return "null argument";
        case FRG_STATUS_ERROR_UNEXPECTED_ARGUMENT_VALUE:
            return "unexpected argument value";
        case FRG_STATUS_ERROR_UNEXPECTED_ENUM_VALUE:
            return "unexpected enum value";
        case FRG_STATUS_ERROR_EMPTY_STRING:
            return "empty string";
        default:
            return "(unknown value for frg_status_t)";
    }
}
