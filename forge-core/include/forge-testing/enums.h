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

#pragma once

typedef enum {
  FRG_TESTING_STRING_COMPARISON_ENTRY_KIND_MATCHING,
  FRG_TESTING_STRING_COMPARISON_ENTRY_KIND_ONLY_EXPECTED,
  FRG_TESTING_STRING_COMPARISON_ENTRY_KIND_ONLY_ACTUAL,
  FRG_TESTING_STRING_COMPARISON_ENTRY_KIND_DIFFERENT,

  FRG_TESTING_STRING_COMPARISON_ENTRY_KIND_LAST
    = FRG_TESTING_STRING_COMPARISON_ENTRY_KIND_DIFFERENT,
} frg_testing_string_comparison_entry_kind_t;

#define FRG_TESTING_STRING_COMPARISON_ENTRY_KIND_COUNT                                 \
  (FRG_TESTING_STRING_COMPARISON_ENTRY_KIND_LAST + 1)

typedef enum {
  FRG_TESTING_STRING_COMPARISON_RESULT_KIND_MATCHING,
  FRG_TESTING_STRING_COMPARISON_RESULT_KIND_DIFFERENT_INCOMPLETE, // 1
  FRG_TESTING_STRING_COMPARISON_RESULT_KIND_DIFFERENT_COMPLETE,   // 2

  FRG_TESTING_STRING_COMPARISON_RESULT_KIND_LAST
    = FRG_TESTING_STRING_COMPARISON_RESULT_KIND_DIFFERENT_COMPLETE,
} frg_testing_string_comparison_result_kind_t;

#define FRG_TESTING_STRING_COMPARISON_RESULT_KIND_COUNT                                \
  (FRG_TESTING_STRING_COMPARISON_RESULT_KIND_LAST + 1)

typedef enum {
  FRG_TESTING_COMPILATION_TEST_KIND_EXPECT_SUCCESS,
  FRG_TESTING_COMPILATION_TEST_KIND_EXPECT_UNABLE_TO_PARSE,
  FRG_TESTING_COMPILATION_TEST_KIND_EXPECT_UNABLE_TO_VERIFY,

  FRG_TESTING_COMPILATION_TEST_KIND_LAST
    = FRG_TESTING_COMPILATION_TEST_KIND_EXPECT_UNABLE_TO_VERIFY,
} frg_testing_compilation_test_kind_t;

#define FRG_TESTING_COMPILATION_TEST_KIND_COUNT                                        \
  (FRG_TESTING_COMPILATION_TEST_KIND_LAST + 1)
