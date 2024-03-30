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

#pragma once

#include <forge-testing/enums.h>
#include <forge/streams/output.h>
#include <glib.h>

#define FRG_TESTING_STRING_COMPARISON_DEFAULT_CONTEXT_AFTER_FIRST_DIFFERENCE 2

typedef struct {
  frg_testing_string_comparison_entry_kind_t kind;
  GString* expected;
  GString* actual;
} frg_testing_string_comparison_entry_t;

frg_testing_string_comparison_entry_t* frg_testing_string_comparison_entry_new_matching(
  GString* actual);

frg_testing_string_comparison_entry_t*
  frg_testing_string_comparison_entry_new_only_expected(GString* expected);

frg_testing_string_comparison_entry_t*
  frg_testing_string_comparison_entry_new_only_actual(GString* actual);

frg_testing_string_comparison_entry_t*
  frg_testing_string_comparison_entry_new_different(GString* expected, GString* actual);

void frg_testing_string_comparison_entry_destroy(
  frg_testing_string_comparison_entry_t* entry);

void frg_testing_string_comparison_entry_print(
  frg_stream_output_t* mut_stream, const frg_testing_string_comparison_entry_t* entry);

typedef struct {
  frg_testing_string_comparison_result_kind_t kind;
  GList* entries;
} frg_testing_string_comparison_result_t;

frg_testing_string_comparison_result_t* frg_testing_string_comparison_result_new(
  frg_testing_string_comparison_result_kind_t kind, GList* entries);

void frg_testing_string_comparison_result_destroy(
  frg_testing_string_comparison_result_t* result);

void frg_testing_string_comparison_result_print(
  frg_stream_output_t* mut_stream,
  const frg_testing_string_comparison_result_t* result);

frg_testing_string_comparison_result_t* frg_testing_compare_strings(
  const char* expected,
  const char* actual,
  frg_line_number_t context_after_first_difference);
