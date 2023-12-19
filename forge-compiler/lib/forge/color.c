#include <forge/color.h>

#include <unistd.h>

// Implementation is heavily based off of:
// https://github.com/ararslan/termcolor-c/blob/master/include/termcolor-c.h

#if !(defined(__unix__) || defined(__unix) || (defined(__APPLE__) && defined(__MACH__)))
#error Only UNIX-based OSs are supported by this implementation
#endif

static frg_color_mode_t _frg_color_mode = FRG_COLOR_MODE_AUTO;

frg_color_mode_t _frg_resolve_color_mode_from_hint(FILE* stream, frg_color_mode_t hint) {
    if (hint == FRG_COLOR_MODE_AUTO) {
        if (isatty(fileno(stream))) {
            return FRG_COLOR_MODE_ENABLED;
        } else {
            return FRG_COLOR_MODE_DISABLED;
        }
    } else {
        return hint;
    }
}

frg_color_mode_t frg_get_color_mode(FILE* stream) {
    return (_frg_color_mode = _frg_resolve_color_mode_from_hint(stream, _frg_color_mode));
}

frg_color_mode_t frg_set_color_mode(FILE* stream, frg_color_mode_t hint) {
    return (_frg_color_mode = _frg_resolve_color_mode_from_hint(stream, hint));
}

bool frg_set_color(FILE* stream, frg_color_code_t code) {
    if (frg_get_color_mode(stream) == FRG_COLOR_MODE_ENABLED) {
        switch (code) {
            case FRG_COLOR_CODE_RESET:
                fprintf(stream, "\033[0;0m");
                return true;
            case FRG_COLOR_CODE_NOTE:
                fprintf(stream, "\033[1;90m");
                return true;
            case FRG_COLOR_CODE_DEBUG:
                fprintf(stream, "\033[1;34m");
                return true;
            case FRG_COLOR_CODE_WARNING:
                fprintf(stream, "\033[1;37m");
                return true;
            case FRG_COLOR_CODE_ERROR:
                fprintf(stream, "\033[1;31m");
                return true;
            case FRG_COLOR_CODE_FATAL_ERROR:
                fprintf(stream, "\033[1;31m");
                return true;
            case FRG_COLOR_CODE_INTERNAL_ERROR:
                fprintf(stream, "\033[1;35m");
                return true;
            case FRG_COLOR_CODE_MESSAGE:
                fprintf(stream, "\033[1;28m");
                return true;
            case FRG_COLOR_CODE_LOCATION:
                fprintf(stream, "\033[0;90m");
                return true;
            default:
                return false;
        }
    } else {
        return true;
    }
}
