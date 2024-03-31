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

#include <forge-testing/string_substitution.h>
#include <forge/assert.h>
#include <forge/memory.h>

void _frg_testing_string_substituter_value_destroyer(gpointer value) {
  g_string_free((GString*)value, TRUE);
}

frg_testing_string_substituter_t* frg_testing_string_substituter_new() {
  frg_testing_string_substituter_t* substituter
    = frg_malloc(sizeof(frg_testing_string_substituter_t));

  substituter->_substitutions
    = g_hash_table_new_full((GHashFunc)g_str_hash,
                            (GEqualFunc)g_str_equal,
                            NULL,
                            _frg_testing_string_substituter_value_destroyer);

  return substituter;
}

void frg_testing_string_substituter_destroy(
  frg_testing_string_substituter_t* substituter) {
  frg_assert_pointer_non_null(substituter);

  g_hash_table_destroy(substituter->_substitutions);

  frg_free(substituter);
}

void frg_testing_string_substituter_add_str(
  frg_testing_string_substituter_t* mut_substituter,
  const char* key,
  const char* value) {
  frg_assert_pointer_non_null(mut_substituter);
  frg_assert_string_non_empty(key);
  frg_assert_pointer_non_null(value);

  frg_testing_string_substituter_add_string(mut_substituter, key, g_string_new(value));
}

void frg_testing_string_substituter_add_string(
  frg_testing_string_substituter_t* mut_substituter, const char* key, GString* value) {
  frg_assert_pointer_non_null(mut_substituter);
  frg_assert_string_non_empty(key);
  frg_assert_pointer_non_null(value);

  g_hash_table_insert(mut_substituter->_substitutions, (void*)key, (void*)value);
}

void frg_testing_string_substituter_add_printf(
  frg_testing_string_substituter_t* mut_substituter,
  const char* key,
  const char* format,
  ...) {
  frg_assert_pointer_non_null(mut_substituter);
  frg_assert_string_non_empty(key);
  frg_assert_pointer_non_null(format);

  va_list args;
  va_start(args, format);
  frg_testing_string_substituter_add_vprintf(mut_substituter, key, format, args);
  va_end(args);
}

void frg_testing_string_substituter_add_vprintf(
  frg_testing_string_substituter_t* mut_substituter,
  const char* key,
  const char* format,
  va_list args) {
  frg_assert_pointer_non_null(mut_substituter);
  frg_assert_string_non_empty(key);
  frg_assert_pointer_non_null(format);

  GString* value = g_string_new(NULL);
  g_string_vprintf(value, format, args);

  frg_testing_string_substituter_add_string(mut_substituter, key, value);
}

GString* frg_testing_string_substituter_run(
  const frg_testing_string_substituter_t* substituter, const char* input) {
  GString* result  = g_string_new(NULL);

  const char* iter = input;

  char key_buffer[FRG_TESTING_STRING_SUBSTITUTER_MAX_KEY_LENGTH + 1];
  size_t key_buffer_length = 0;

  while (*iter != 0) {
    if (*iter == '%') {
      iter++;

      if (*iter == 0) {
        g_string_append_c(result, '%');
        break;
      } else if (*iter == '(') {
        iter++;

        key_buffer_length = 0;

        while (*iter != 0 && *iter != ')') {
          frg_assert(key_buffer_length < FRG_TESTING_STRING_SUBSTITUTER_MAX_KEY_LENGTH);

          key_buffer[key_buffer_length] = *iter;
          key_buffer_length++;

          iter++;
        }

        key_buffer[key_buffer_length] = 0;

        if (*iter == ')') {
          iter++;

          const GString* value = (const GString*)g_hash_table_lookup(
            substituter->_substitutions, key_buffer);

          if (value != NULL) {
            g_string_append_len(result, value->str, value->len);
          }
        } else {
          break;
        }
      } else {
        g_string_append_c(result, *iter);
        iter++;
      }
    } else {
      g_string_append_c(result, *iter);
      iter++;
    }
  }

  return result;
}
