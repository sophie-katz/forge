// Copyright (c) 2023-2024 Sophie Katz
//
// This file is part of Forge.
//
// Forge is free software: you can redistribute it and/or modify it under the terms of
// the GNU General Public License as published by the Free Software Foundation, either
// version 3 of the License, or (at your option) any later version.
//
// Forge is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
// PURPOSE. See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with Forge.
// If not, see <https://www.gnu.org/licenses/>.

#include <forge/assert.h>
#include <forge/memory.h>
#include <forge/streams/input.h>
#include <glib.h>
#include <string.h>

frg_stream_input_t* frg_stream_input_new_file(FILE* file,
                                              frg_stream_input_flags_t flags) {
  frg_assert_pointer_non_null(file);
  frg_assert(file != stdin);
  frg_assert((flags & FRG_STREAM_INPUT_FLAG_LENGTH_SET) == 0);
  frg_assert((flags & FRG_STREAM_INPUT_FLAG_EXTRA_NULL_BYTE) == 0);

  frg_stream_input_t* source = frg_malloc(sizeof(frg_stream_input_t));

  source->_flags             = flags;
  source->_file              = file;
  source->_text              = NULL;
  source->_length            = 0;
  source->_text_offset       = NULL;

  return source;
}

frg_stream_input_t* frg_stream_input_new_buffer(char* text,
                                                frg_stream_input_flags_t flags) {
  frg_assert_pointer_non_null(text);
  frg_assert((flags & FRG_STREAM_INPUT_FLAG_LENGTH_SET) == 0);

  size_t length = strlen(text) + 1;

  if (flags & FRG_STREAM_INPUT_FLAG_EXTRA_NULL_BYTE) {
    length++;

    frg_assert_int_equal_to(text[length - 1], 0);
    frg_assert_int_equal_to(text[length - 2], 0);
  }

  return frg_stream_input_new_buffer_with_length(text, length, flags);
}

frg_stream_input_t* frg_stream_input_new_buffer_with_length(
  char* text, size_t length, frg_stream_input_flags_t flags) {
  frg_assert_pointer_non_null(text);
  frg_assert((flags & FRG_STREAM_INPUT_FLAG_LENGTH_SET) == 0);

  frg_stream_input_t* source = frg_malloc(sizeof(frg_stream_input_t));

  source->_flags             = flags | FRG_STREAM_INPUT_FLAG_LENGTH_SET;
  source->_file              = NULL;
  source->_text              = text;
  source->_length            = length;
  source->_text_offset       = text;

  return source;
}

void frg_stream_input_destroy(frg_stream_input_t* input) {
  frg_assert_pointer_non_null(input);

  if (input->_file != NULL && (input->_flags & FRG_STREAM_INPUT_FLAG_OWNED)) {
    fclose(input->_file);
  }

  if (input->_text != NULL && (input->_flags & FRG_STREAM_INPUT_FLAG_OWNED)) {
    frg_free(input->_text);
  }

  frg_free(input);
}

bool frg_stream_input_is_buffer(const frg_stream_input_t* input) {
  frg_assert_pointer_non_null(input);

  return input->_text != NULL && input->_file == NULL;
}

bool frg_stream_input_is_file(const frg_stream_input_t* input) {
  frg_assert_pointer_non_null(input);

  return input->_text == NULL && input->_file != NULL;
}

bool _frg_stream_input_is_at_end_buffer(const frg_stream_input_t* input) {
  frg_assert_pointer_non_null(input);
  frg_assert(frg_stream_input_is_buffer(input));

  return input->_text_offset >= input->_text + input->_length;
}

bool _frg_stream_input_is_at_end_file(const frg_stream_input_t* input) {
  frg_assert_pointer_non_null(input);
  frg_assert(frg_stream_input_is_file(input));

  return feof(input->_file);
}

bool frg_stream_input_is_at_end(const frg_stream_input_t* input) {
  if (frg_stream_input_is_buffer(input)) {
    return _frg_stream_input_is_at_end_buffer(input);
  } else if (frg_stream_input_is_file(input)) {
    return _frg_stream_input_is_at_end_file(input);
  } else {
    frg_die(
      "'input' is incorrectly initialized (looks like both a file and a buffer "
      "stream)");
  }
}

bool _frg_stream_input_has_error_buffer(const frg_stream_input_t* input) {
  frg_assert_pointer_non_null(input);
  frg_assert(frg_stream_input_is_buffer(input));

  return input->_text_offset < input->_text
      || input->_text_offset > input->_text + input->_length;
}

bool _frg_stream_input_has_error_file(const frg_stream_input_t* input) {
  frg_assert_pointer_non_null(input);
  frg_assert(frg_stream_input_is_file(input));

  return ferror(input->_file);
}

bool frg_stream_input_has_error(const frg_stream_input_t* input) {
  if (frg_stream_input_is_buffer(input)) {
    return _frg_stream_input_has_error_buffer(input);
  } else if (frg_stream_input_is_file(input)) {
    return _frg_stream_input_has_error_file(input);
  } else {
    frg_die(
      "'input' is incorrectly initialized (looks like both a file and a buffer "
      "stream)");
  }
}

char _frg_stream_input_peek_char_buffer(frg_stream_input_t* mut_input) {
  frg_assert_pointer_non_null(mut_input);
  frg_assert(frg_stream_input_is_buffer(mut_input));

  return *mut_input->_text_offset;
}

char _frg_stream_input_peek_char_file(frg_stream_input_t* mut_input) {
  frg_assert_pointer_non_null(mut_input);
  frg_assert(frg_stream_input_is_file(mut_input));

  int result = getc(mut_input->_file);
  if (result < 0) {
    return 0;
  }

  ungetc(result, mut_input->_file);

  return result;
}

char frg_stream_input_peek_character(frg_stream_input_t* mut_input) {
  if (frg_stream_input_is_buffer(mut_input)) {
    return _frg_stream_input_peek_char_buffer(mut_input);
  } else if (frg_stream_input_is_file(mut_input)) {
    return _frg_stream_input_peek_char_file(mut_input);
  } else {
    frg_die(
      "'mut_input' is incorrectly initialized (looks like both a file and a buffer "
      "stream)");
  }
}

char _frg_stream_input_read_char_buffer(frg_stream_input_t* mut_input) {
  frg_assert_pointer_non_null(mut_input);
  frg_assert(frg_stream_input_is_buffer(mut_input));

  return *(mut_input->_text_offset++);
}

char _frg_stream_input_read_char_file(frg_stream_input_t* mut_input) {
  frg_assert_pointer_non_null(mut_input);
  frg_assert(frg_stream_input_is_file(mut_input));

  int result = getc(mut_input->_file);
  if (result < 0) {
    return 0;
  }

  return result;
}

char frg_stream_input_read_character(frg_stream_input_t* mut_input) {
  if (frg_stream_input_is_buffer(mut_input)) {
    return _frg_stream_input_read_char_buffer(mut_input);
  } else if (frg_stream_input_is_file(mut_input)) {
    return _frg_stream_input_read_char_file(mut_input);
  } else {
    frg_die(
      "'mut_input' is incorrectly initialized (looks like both a file and a buffer "
      "stream)");
  }
}

size_t _frg_stream_input_read_into_buffer_buffer(frg_stream_input_t* mut_input,
                                                 char* mut_buffer,
                                                 size_t length) {
  frg_assert_pointer_non_null(mut_input);
  frg_assert(frg_stream_input_is_buffer(mut_input));

  length = MIN(length, mut_input->_text + mut_input->_length - mut_input->_text_offset);

  if (length > 0) {
    memcpy(mut_buffer, mut_input->_text_offset, length);
  }

  mut_input->_text_offset += length;

  return length;
}

size_t _frg_stream_input_read_into_buffer_file(frg_stream_input_t* mut_input,
                                               char* mut_buffer,
                                               size_t length) {
  frg_assert_pointer_non_null(mut_input);
  frg_assert(frg_stream_input_is_file(mut_input));

  return fread(mut_buffer, 1, length, mut_input->_file);
}

size_t frg_stream_input_read_into_buffer(frg_stream_input_t* mut_input,
                                         char* mut_buffer,
                                         size_t length) {
  if (frg_stream_input_is_buffer(mut_input)) {
    return _frg_stream_input_read_into_buffer_buffer(mut_input, mut_buffer, length);
  } else if (frg_stream_input_is_file(mut_input)) {
    return _frg_stream_input_read_into_buffer_file(mut_input, mut_buffer, length);
  } else {
    frg_die(
      "'mut_input' is incorrectly initialized (looks like both a file and a buffer "
      "stream)");
  }
}

size_t _frg_stream_input_get_offset_buffer(const frg_stream_input_t* input) {
  frg_assert_pointer_non_null(input);
  frg_assert(frg_stream_input_is_buffer(input));

  return input->_text_offset - input->_text;
}

size_t _frg_stream_input_get_offset_file(const frg_stream_input_t* input) {
  frg_assert_pointer_non_null(input);
  frg_assert(frg_stream_input_is_file(input));

  return ftell(input->_file);
}

size_t frg_stream_input_get_offset(const frg_stream_input_t* input) {
  if (frg_stream_input_is_buffer(input)) {
    return _frg_stream_input_get_offset_buffer(input);
  } else if (frg_stream_input_is_file(input)) {
    return _frg_stream_input_get_offset_file(input);
  } else {
    frg_die(
      "'input' is incorrectly initialized (looks like both a file and a buffer "
      "stream)");
  }
}

void _frg_stream_input_seek_to_offset_buffer(frg_stream_input_t* mut_input,
                                             size_t offset) {
  frg_assert_pointer_non_null(mut_input);
  frg_assert(frg_stream_input_is_buffer(mut_input));
  frg_assert_int_greater_than_or_equal_to(offset, 0);
  frg_assert_int_less_than_or_equal_to(offset, mut_input->_length);

  offset                  = MIN(offset, mut_input->_length);

  mut_input->_text_offset = mut_input->_text + offset;
}

void _frg_stream_input_seek_to_offset_file(frg_stream_input_t* mut_input,
                                           size_t offset) {
  frg_assert_pointer_non_null(mut_input);
  frg_assert(frg_stream_input_is_file(mut_input));

  fseek(mut_input->_file, offset, SEEK_SET);
}

void frg_stream_input_seek_to_offset(frg_stream_input_t* mut_input, size_t offset) {
  if (frg_stream_input_is_buffer(mut_input)) {
    _frg_stream_input_seek_to_offset_buffer(mut_input, offset);
  } else if (frg_stream_input_is_file(mut_input)) {
    _frg_stream_input_seek_to_offset_file(mut_input, offset);
  } else {
    frg_die(
      "'mut_input' is incorrectly initialized (looks like both a file and a buffer "
      "stream)");
  }
}

size_t frg_stream_get_length(const frg_stream_input_t* input) {
  frg_assert_pointer_non_null(input);

  if ((input->_flags & FRG_STREAM_INPUT_FLAG_LENGTH_SET) == 0
      && frg_stream_input_is_file(input)) {
    size_t current_offset = frg_stream_input_get_offset(input);
    fseek(input->_file, 0, SEEK_END);
    ((frg_stream_input_t*)input)->_length = ftell(input->_file);
    frg_stream_input_seek_to_offset((frg_stream_input_t*)input, current_offset);
    ((frg_stream_input_t*)input)->_flags |= FRG_STREAM_INPUT_FLAG_LENGTH_SET;
  }

  frg_assert((input->_flags & FRG_STREAM_INPUT_FLAG_LENGTH_SET) != 0);

  return input->_length;
}

char* frg_stream_get_buffer(frg_stream_input_t* mut_input) {
  frg_assert_pointer_non_null(mut_input);
  frg_assert(frg_stream_input_is_buffer(mut_input));

  return mut_input->_text;
}

FILE* frg_stream_get_file(frg_stream_input_t* mut_input) {
  frg_assert_pointer_non_null(mut_input);
  frg_assert(frg_stream_input_is_file(mut_input));

  return mut_input->_file;
}
