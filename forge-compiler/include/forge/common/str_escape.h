#pragma once

#include <forge/common/enums.h>
#include <forge/common/types.h>
#include <glib.h>

frg_status_t frg_escape_char(GString** escaped, frg_char_t value);

frg_status_t frg_escape_str(GString** escaped, GString* value);

frg_status_t frg_unescape_char(frg_char_t* value, const GString* escaped);

frg_status_t frg_unescape_str(GString** value, const GString* escaped);
