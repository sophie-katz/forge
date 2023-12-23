#include <forge/common/stream.h>

frg_status_t frg_validate_console_stream(FILE* stream) {
    if (stream == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    } else if (stream != stdout && stream != stderr) {
        return FRG_STATUS_ERROR_UNEXPECTED_ARGUMENT_VALUE;
    } else {
        return FRG_STATUS_OK;
    }
}
