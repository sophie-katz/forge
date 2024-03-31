// This program is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software Foundation,
// either version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
// PARTICULAR PURPOSE. See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with this
// program. If not, see <https://www.gnu.org/licenses/>.
//
// Copyright 2023-2024 Sophie Katz

#include <ctype.h>
#include <forge/assert.h>
#include <forge/lexical_casts/utilities.h>
#include <utf8proc.h>

bool frg_is_char_printable(frg_character_t value) {
  if (value < 0x80) {
    return isprint(value);
  }

  switch (utf8proc_category((utf8proc_int32_t)value)) {
  case UTF8PROC_CATEGORY_CN:
  case UTF8PROC_CATEGORY_CC:
  case UTF8PROC_CATEGORY_CF:
  case UTF8PROC_CATEGORY_CS:
  case UTF8PROC_CATEGORY_CO:
    return false;
  default:
    return true;
  }
}

void frg_print_base_prefix(frg_stream_output_t* mut_stream, frg_base_t base) {
  if (base == 2) {
    frg_stream_output_write_string(mut_stream, "0b");
  } else if (base == 8) {
    frg_stream_output_write_string(mut_stream, "0o");
  } else if (base == 16) {
    frg_stream_output_write_string(mut_stream, "0x");
  }
}

bool frg_parse_base_prefix(frg_base_t* out_base,
                           frg_message_buffer_t* mut_message_buffer,
                           frg_parsing_token_reader_t* mut_reader) {
  frg_assert_pointer_non_null(out_base);
  frg_assert_pointer_non_null(mut_reader);

  if (frg_parsing_token_reader_get_current_character(mut_reader) == '0') {
    bool result                      = true;
    frg_parsing_token_reader_t* save = frg_parsing_token_reader_clone(mut_reader);
    frg_parsing_token_reader_step(mut_reader);

    if (frg_parsing_token_reader_get_current_character(mut_reader) == 'b') {
      *out_base = 2;
      frg_parsing_token_reader_step(mut_reader);
    } else if (frg_parsing_token_reader_get_current_character(mut_reader) == 'o') {
      *out_base = 8;
      frg_parsing_token_reader_step(mut_reader);
    } else if (frg_parsing_token_reader_get_current_character(mut_reader) == 'x') {
      *out_base = 16;
      frg_parsing_token_reader_step(mut_reader);
    } else {
      frg_parsing_token_reader_restore(mut_reader, save);
      result = false;
    }

    frg_parsing_token_reader_destroy(save);

    return result;
  } else {
    return false;
  }
}
