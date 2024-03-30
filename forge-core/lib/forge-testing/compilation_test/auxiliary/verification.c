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

#include <forge-testing/compilation_test/auxiliary/ast.h>
#include <forge-testing/compilation_test/utilities.h>
#include <forge/assert.h>
#include <forge/verification/verifier.h>
#include <unity.h>

GString* frg_testing_test_compilation_auxiliary_verification(
  const frg_testing_compilation_test_options_t* options,
  const frg_testing_compilation_test_context_initialized_t* context_initialized,
  const frg_testing_compilation_test_context_parsed_t* context_parsed) {
  frg_assert_pointer_non_null(options);
  frg_assert_pointer_non_null(context_parsed);
  frg_assert_pointer_non_null(context_parsed->ast_node);

  frg_ast_scope_t* scope = frg_ast_scope_new();

  frg_verification_verifier_t* verifier
    = frg_verification_verifier_new(context_initialized->message_buffer, scope);

  frg_ast_visitor_t* visitor = frg_verification_verifier_new_visitor(verifier);

  frg_ast_visitor_status_t status
    = frg_ast_accept((frg_ast_node_t**)&context_parsed->ast_node, visitor);

  frg_ast_visitor_destroy(visitor);

  frg_verification_verifier_destroy(verifier);

  frg_ast_scope_destroy(scope);

  switch (status) {
  case FRG_AST_VISITOR_STATUS_OK:
    return NULL;
  case FRG_AST_VISITOR_STATUS_SKIP:
    return g_string_new("Visitor returned unexpected status skip");
  case FRG_AST_VISITOR_STATUS_STOP:
    return g_string_new("Visitor was stopped early");
  }
}
