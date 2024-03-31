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

#include <forge-testing/string_comparison.h>
#include <forge/memory.h>
#include <forge/streams/output.h>

int main() {
  frg_stream_output_init();

  frg_testing_string_comparison_result_t* result = frg_testing_compare_strings(
    "fn add(a: i32, b: i32) -> i32 {\n"
    "  # Some addition\n"
    "  return a + b;\n"
    "}\n",
    "fn add(a: i64, b: i64) -> i64 {\n"
    "  # Some addition\n"
    "  return a + b;\n"
    "}\n",
    2);

  frg_testing_string_comparison_result_print(frg_stream_output_get_stdout(), result);

  frg_testing_string_comparison_result_destroy(result);

  frg_stream_output_cleanup();

  return 0;
}
