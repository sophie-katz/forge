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

#include <forge-testing/string_comparison.h>
#include <forge/assert.h>
#include <forge/die.h>
#include <forge/memory.h>
#include <forge/streams/output.h>

frg_testing_string_comparison_entry_t* frg_testing_string_comparison_entry_new_matching(
  GString* actual) {
  frg_assert_pointer_non_null(actual);

  frg_testing_string_comparison_entry_t* entry
    = (frg_testing_string_comparison_entry_t*)frg_malloc(
      sizeof(frg_testing_string_comparison_entry_t));

  entry->kind     = FRG_TESTING_STRING_COMPARISON_ENTRY_KIND_MATCHING;
  entry->expected = NULL;
  entry->actual   = actual;

  return entry;
}

frg_testing_string_comparison_entry_t*
  frg_testing_string_comparison_entry_new_only_expected(GString* expected) {
  frg_assert_pointer_non_null(expected);

  frg_testing_string_comparison_entry_t* entry
    = (frg_testing_string_comparison_entry_t*)frg_malloc(
      sizeof(frg_testing_string_comparison_entry_t));

  entry->kind     = FRG_TESTING_STRING_COMPARISON_ENTRY_KIND_ONLY_EXPECTED;
  entry->expected = expected;
  entry->actual   = NULL;

  return entry;
}

frg_testing_string_comparison_entry_t*
  frg_testing_string_comparison_entry_new_only_actual(GString* actual) {
  frg_assert_pointer_non_null(actual);

  frg_testing_string_comparison_entry_t* entry
    = (frg_testing_string_comparison_entry_t*)frg_malloc(
      sizeof(frg_testing_string_comparison_entry_t));

  entry->kind     = FRG_TESTING_STRING_COMPARISON_ENTRY_KIND_ONLY_ACTUAL;
  entry->expected = NULL;
  entry->actual   = actual;

  return entry;
}

frg_testing_string_comparison_entry_t*
  frg_testing_string_comparison_entry_new_different(GString* expected,
                                                    GString* actual) {
  frg_assert_pointer_non_null(expected);
  frg_assert_pointer_non_null(actual);

  frg_testing_string_comparison_entry_t* entry
    = (frg_testing_string_comparison_entry_t*)frg_malloc(
      sizeof(frg_testing_string_comparison_entry_t));

  entry->kind     = FRG_TESTING_STRING_COMPARISON_ENTRY_KIND_DIFFERENT;
  entry->expected = expected;
  entry->actual   = actual;

  return entry;
}

void frg_testing_string_comparison_entry_destroy(
  frg_testing_string_comparison_entry_t* entry) {
  frg_assert_pointer_non_null(entry);

  if (entry->expected) {
    g_string_free(entry->expected, TRUE);
  }

  if (entry->actual) {
    g_string_free(entry->actual, TRUE);
  }

  frg_free(entry);
}

bool _frg_testing_string_comparison_entry_should_print_second_line(
  const frg_testing_string_comparison_entry_t* entry) {
  return entry->kind == FRG_TESTING_STRING_COMPARISON_ENTRY_KIND_DIFFERENT;
}

void _frg_testing_string_comparison_entry_print_prefix_separator(
  frg_stream_output_t* mut_stream) {
  frg_assert_pointer_non_null(mut_stream);

  frg_stream_output_set_color(mut_stream, FRG_STREAM_OUTPUT_COLOR_BRIGHT_BLACK);
  frg_stream_output_write_string(
    mut_stream, frg_stream_output_choose_ascii_or_unicode(mut_stream, "|", "▏"));
  frg_stream_output_set_color(mut_stream, FRG_STREAM_OUTPUT_COLOR_RESET);
  frg_stream_output_write_character(mut_stream, ' ');
}

void _frg_testing_string_comparison_entry_print_prefix_blank(
  frg_stream_output_t* mut_stream) {
  frg_assert_pointer_non_null(mut_stream);

  frg_stream_output_write_string(mut_stream, "         ");
  _frg_testing_string_comparison_entry_print_prefix_separator(mut_stream);
}

void _frg_testing_string_comparison_entry_print_prefix_expected(
  frg_stream_output_t* mut_stream) {
  frg_assert_pointer_non_null(mut_stream);

  frg_stream_output_set_color(mut_stream, FRG_STREAM_OUTPUT_COLOR_GREEN);
  frg_stream_output_write_string(mut_stream, "Expected ");
  frg_stream_output_set_color(mut_stream, FRG_STREAM_OUTPUT_COLOR_RESET);
  _frg_testing_string_comparison_entry_print_prefix_separator(mut_stream);
}

void _frg_testing_string_comparison_entry_print_prefix_actual(
  frg_stream_output_t* mut_stream) {
  frg_assert_pointer_non_null(mut_stream);

  frg_stream_output_set_color(mut_stream, FRG_STREAM_OUTPUT_COLOR_RED);
  frg_stream_output_write_string(mut_stream, "Actual   ");
  frg_stream_output_set_color(mut_stream, FRG_STREAM_OUTPUT_COLOR_RESET);
  _frg_testing_string_comparison_entry_print_prefix_separator(mut_stream);
}

void _frg_testing_string_comparison_entry_print_prefix_first_line(
  frg_stream_output_t* mut_stream, const frg_testing_string_comparison_entry_t* entry) {
  frg_assert_pointer_non_null(mut_stream);
  frg_assert_pointer_non_null(entry);

  switch (entry->kind) {
  case FRG_TESTING_STRING_COMPARISON_ENTRY_KIND_MATCHING:
    _frg_testing_string_comparison_entry_print_prefix_blank(mut_stream);
    break;
  case FRG_TESTING_STRING_COMPARISON_ENTRY_KIND_ONLY_EXPECTED:
  case FRG_TESTING_STRING_COMPARISON_ENTRY_KIND_DIFFERENT:
    _frg_testing_string_comparison_entry_print_prefix_expected(mut_stream);
    break;
  case FRG_TESTING_STRING_COMPARISON_ENTRY_KIND_ONLY_ACTUAL:
    _frg_testing_string_comparison_entry_print_prefix_actual(mut_stream);
    break;
  default:
    frg_die_unexpected_enum_value(entry->kind);
  }
}

void _frg_testing_string_comparison_entry_print_prefix_second_line(
  frg_stream_output_t* mut_stream) {
  frg_assert_pointer_non_null(mut_stream);

  _frg_testing_string_comparison_entry_print_prefix_actual(mut_stream);
}

const char* _frg_testing_string_comparison_entry_get_first_line(
  const frg_testing_string_comparison_entry_t* entry) {
  frg_assert_pointer_non_null(entry);

  switch (entry->kind) {
  case FRG_TESTING_STRING_COMPARISON_ENTRY_KIND_MATCHING:
  case FRG_TESTING_STRING_COMPARISON_ENTRY_KIND_ONLY_ACTUAL:
    frg_assert_pointer_non_null(entry->actual);
    return entry->actual->str;
  case FRG_TESTING_STRING_COMPARISON_ENTRY_KIND_ONLY_EXPECTED:
  case FRG_TESTING_STRING_COMPARISON_ENTRY_KIND_DIFFERENT:
    frg_assert_pointer_non_null(entry->expected);
    return entry->expected->str;
  default:
    frg_die_unexpected_enum_value(entry->kind);
  }
}

const char* _frg_testing_string_comparison_entry_get_second_line(
  const frg_testing_string_comparison_entry_t* entry) {
  frg_assert_pointer_non_null(entry);

  switch (entry->kind) {
  case FRG_TESTING_STRING_COMPARISON_ENTRY_KIND_DIFFERENT:
    frg_assert_pointer_non_null(entry->actual);
    return entry->actual->str;
  default:
    frg_die_unexpected_enum_value(entry->kind);
  }
}

void _frg_testing_string_comparison_entry_print_first_line(
  frg_stream_output_t* mut_stream,
  const frg_testing_string_comparison_entry_t* entry,
  const char* line) {
  frg_assert_pointer_non_null(mut_stream);
  frg_assert_pointer_non_null(entry);
  frg_assert_pointer_non_null(line);

  _frg_testing_string_comparison_entry_print_prefix_first_line(mut_stream, entry);

  if (entry->kind == FRG_TESTING_STRING_COMPARISON_ENTRY_KIND_DIFFERENT) {
    frg_stream_output_set_color(mut_stream, FRG_STREAM_OUTPUT_COLOR_GREEN);
  }

  frg_stream_output_write_string(mut_stream, line);
  frg_stream_output_write_character(mut_stream, '\n');

  frg_stream_output_set_color(mut_stream, FRG_STREAM_OUTPUT_COLOR_RESET);
}

void _frg_testing_string_comparison_entry_print_second_line(
  frg_stream_output_t* mut_stream,
  const frg_testing_string_comparison_entry_t* entry,
  const char* first_line,
  const char* second_line) {
  frg_assert_pointer_non_null(mut_stream);
  frg_assert_pointer_non_null(entry);
  frg_assert_pointer_non_null(first_line);
  frg_assert_pointer_non_null(second_line);
  frg_assert(_frg_testing_string_comparison_entry_should_print_second_line(entry));

  _frg_testing_string_comparison_entry_print_prefix_second_line(mut_stream);

  const char* first_line_iter  = first_line;
  const char* second_line_iter = second_line;

  while (true) {
    if (*first_line_iter != 0) {
      // Have a character from the first line

      if (*second_line_iter != 0) {
        // Have characters from both lines

        if (*first_line_iter != *second_line_iter) {
          // Characters are different

          frg_stream_output_set_color(mut_stream, FRG_STREAM_OUTPUT_COLOR_RED);
          frg_stream_output_write_character(mut_stream, *second_line_iter);
          frg_stream_output_set_color(mut_stream, FRG_STREAM_OUTPUT_COLOR_RESET);
        } else {
          // Characters are the same

          frg_stream_output_write_character(mut_stream, *second_line_iter);
        }

        first_line_iter++;
        second_line_iter++;
      } else {
        // Have character only from first line

        first_line_iter++;
      }
    } else {
      // Have no character from the first line

      if (*second_line_iter != 0) {
        // Have character only from the second line

        frg_stream_output_set_color(mut_stream, FRG_STREAM_OUTPUT_COLOR_RED);
        frg_stream_output_write_character(mut_stream, *second_line_iter);
        frg_stream_output_set_color(mut_stream, FRG_STREAM_OUTPUT_COLOR_RESET);

        second_line_iter++;
      } else {
        // Have characters from neither line

        break;
      }
    }
  }

  frg_stream_output_write_character(mut_stream, '\n');
}

void frg_testing_string_comparison_entry_print(
  frg_stream_output_t* mut_stream, const frg_testing_string_comparison_entry_t* entry) {
  frg_assert_pointer_non_null(mut_stream);
  frg_assert_pointer_non_null(entry);

  const char* first_line = _frg_testing_string_comparison_entry_get_first_line(entry);

  _frg_testing_string_comparison_entry_print_first_line(mut_stream, entry, first_line);

  if (_frg_testing_string_comparison_entry_should_print_second_line(entry)) {
    const char* second_line
      = _frg_testing_string_comparison_entry_get_second_line(entry);

    _frg_testing_string_comparison_entry_print_second_line(
      mut_stream, entry, first_line, second_line);
  }
}

frg_testing_string_comparison_result_t* frg_testing_string_comparison_result_new(
  frg_testing_string_comparison_result_kind_t kind, GList* entries) {
  frg_testing_string_comparison_result_t* result
    = (frg_testing_string_comparison_result_t*)frg_malloc(
      sizeof(frg_testing_string_comparison_result_t));

  result->kind    = kind;
  result->entries = entries;

  return result;
}

void frg_testing_string_comparison_result_destroy(
  frg_testing_string_comparison_result_t* result) {
  frg_assert_pointer_non_null(result);

  for (GList* iter = result->entries; iter != NULL; iter = g_list_next(iter)) {
    frg_testing_string_comparison_entry_destroy(
      (frg_testing_string_comparison_entry_t*)iter->data);
  }

  g_list_free(result->entries);

  frg_free(result);
}

void frg_testing_string_comparison_result_print(
  frg_stream_output_t* mut_stream,
  const frg_testing_string_comparison_result_t* result) {
  frg_assert_pointer_non_null(mut_stream);
  frg_assert_pointer_non_null(result);

  for (const GList* iter = result->entries; iter != NULL; iter = g_list_next(iter)) {
    frg_testing_string_comparison_entry_print(
      mut_stream, (frg_testing_string_comparison_entry_t*)iter->data);
  }

  if (result->kind == FRG_TESTING_STRING_COMPARISON_RESULT_KIND_DIFFERENT_INCOMPLETE) {
    frg_stream_output_write_string(mut_stream, "...\n");
  }
}

frg_testing_string_comparison_result_t* frg_testing_compare_strings(
  const char* expected,
  const char* actual,
  frg_line_number_t context_after_first_difference) {
  frg_assert_int_positive(context_after_first_difference);

  GList* entries = NULL;

  if (strcmp(expected, actual) == 0) {
    return frg_testing_string_comparison_result_new(
      FRG_TESTING_STRING_COMPARISON_RESULT_KIND_MATCHING, entries);
  }

  gchar** lines_expected      = g_strsplit(expected, "\n", -1);
  gchar** lines_actual        = g_strsplit(actual, "\n", -1);

  gchar** lines_expected_iter = lines_expected;
  gchar** lines_actual_iter   = lines_actual;

  bool had_difference         = false;

  while (context_after_first_difference >= 0
         && (*lines_expected != NULL || *lines_actual != NULL)) {
    if (*lines_expected_iter != NULL) {
      if (*lines_actual_iter != NULL) {
        // Haves lines from both

        if (strcmp(*lines_expected_iter, *lines_actual_iter) != 0) {
          entries = g_list_append(
            entries,
            frg_testing_string_comparison_entry_new_different(
              g_string_new(*lines_expected_iter), g_string_new(*lines_actual_iter)));

          had_difference = true;
        } else {
          entries = g_list_append(entries,
                                  frg_testing_string_comparison_entry_new_matching(
                                    g_string_new(*lines_actual_iter)));
        }

        lines_expected_iter++;
        lines_actual_iter++;
      } else {
        // Only have expected line

        entries        = g_list_append(entries,
                                frg_testing_string_comparison_entry_new_only_expected(
                                  g_string_new(*lines_expected_iter)));

        had_difference = true;

        lines_expected_iter++;
      }
    } else {
      if (*lines_actual_iter != NULL) {
        // Only have actual line

        entries        = g_list_append(entries,
                                frg_testing_string_comparison_entry_new_only_actual(
                                  g_string_new(*lines_actual_iter)));

        had_difference = true;

        lines_actual_iter++;
      } else {
        // Have lines from neither

        break;
      }
    }

    if (had_difference) {
      context_after_first_difference--;
    }
  }

  bool is_complete = *lines_expected_iter != NULL || *lines_actual_iter != NULL;

  g_strfreev(lines_expected);
  g_strfreev(lines_actual);

  return frg_testing_string_comparison_result_new(
    is_complete ? FRG_TESTING_STRING_COMPARISON_RESULT_KIND_DIFFERENT_INCOMPLETE
                : FRG_TESTING_STRING_COMPARISON_RESULT_KIND_DIFFERENT_COMPLETE,
    entries);
}
