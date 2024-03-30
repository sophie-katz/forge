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

#include <forge-testing/compilation_test/auxiliary/shared_object.h>
#include <forge/assert.h>

GString* frg_testing_test_compilation_auxiliary_shared_object(
  const frg_testing_compilation_test_options_t* options,
  const frg_testing_compilation_test_context_linked_t* context_linked) {
  frg_assert_pointer_non_null(options);
  frg_assert_pointer_non_null(context_linked);
  frg_assert_pointer_non_null(context_linked->shared_library_handle);

  if (options->on_shared_library_handle != NULL) {
    options->on_shared_library_handle(context_linked->shared_library_handle,
                                      options->mut_user_data);
  }

  return NULL;
}
