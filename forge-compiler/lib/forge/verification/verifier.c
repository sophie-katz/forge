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

#include <forge/assert.h>
#include <forge/memory.h>
#include <forge/verification/verifier.h>
#include <forge/verification/visitor/supported_language_features.h>
#include <forge/verification/visitor/type_verification.h>

frg_verification_verifier_t* frg_verification_verifier_new(
  frg_message_buffer_t* mut_message_buffer, frg_ast_scope_t* mut_scope) {
  frg_verification_verifier_t* verifier
    = frg_malloc(sizeof(frg_verification_verifier_t));
  verifier->mut_message_buffer = mut_message_buffer;
  verifier->mut_scope          = mut_scope;
  return verifier;
}

void frg_verification_verifier_destroy(frg_verification_verifier_t* verifier) {
  frg_assert_pointer_non_null(verifier);

  frg_free(verifier);
}

frg_ast_visitor_t* frg_verification_verifier_new_visitor(
  frg_verification_verifier_t* mut_verifier) {
  frg_ast_visitor_t* visitor = frg_ast_visitor_new(mut_verifier);

  frg_verification_type_verification_add_handlers(visitor);

  return visitor;
}
