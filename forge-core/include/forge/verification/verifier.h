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

#include <forge/ast/scope.h>
#include <forge/ast/visitor.h>

typedef struct {
  frg_message_buffer_t* mut_message_buffer;
  frg_ast_scope_t* mut_scope;
} frg_verification_verifier_t;

frg_verification_verifier_t* frg_verification_verifier_new(
  frg_message_buffer_t* mut_message_buffer, frg_ast_scope_t* mut_scope);

void frg_verification_verifier_destroy(frg_verification_verifier_t* verifier);

frg_ast_visitor_t* frg_verification_verifier_new_visitor(
  frg_verification_verifier_t* mut_verifier);
