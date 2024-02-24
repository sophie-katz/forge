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

#include <forge/ast/node.h>

typedef struct {
  GHashTable* _map_ast;
  GHashTable* _map_ir;
} frg_ast_scope_frame_t;

frg_ast_scope_frame_t* frg_ast_scope_frame_new();

void frg_ast_scope_frame_destroy(frg_ast_scope_frame_t* scope_frame);

void frg_ast_scope_frame_add_ast(frg_ast_scope_frame_t* mut_scope_frame,
                                 frg_ast_node_t* node);

frg_ast_node_t* frg_ast_scope_frame_get_ast(frg_ast_scope_frame_t* mut_scope_frame,
                                            const char* name);

void frg_ast_scope_frame_add_ir(frg_ast_scope_frame_t* mut_scope_frame,
                                const char* name,
                                void* ir);

void* frg_ast_scope_frame_get_ir(frg_ast_scope_frame_t* mut_scope_frame,
                                 const char* name);

void frg_ast_scope_frame_load_declaration_block(
  frg_ast_scope_frame_t* mut_scope_frame,
  const frg_ast_node_declaration_block_t* block);

void frg_ast_scope_frame_load_declaration_function_args(
  frg_ast_scope_frame_t* mut_scope_frame,
  const frg_ast_node_declaration_function_t* function);

typedef struct {
  GList* _frames;
} frg_ast_scope_t;

frg_ast_scope_t* frg_ast_scope_new();

void frg_ast_scope_destroy(frg_ast_scope_t* scope);

void frg_ast_scope_push_frame(frg_ast_scope_t* mut_scope);

void frg_ast_scope_pop_frame(frg_ast_scope_t* mut_scope);

frg_ast_scope_frame_t* frg_ast_scope_get_current_frame(frg_ast_scope_t* mut_scope);

void frg_ast_scope_add_ast(frg_ast_scope_t* mut_scope, frg_ast_node_t* node);

frg_ast_node_t* frg_ast_scope_get_ast(frg_ast_scope_t* mut_scope, const char* name);

void frg_ast_scope_add_ir(frg_ast_scope_t* mut_scope, const char* name, void* ir);

void* frg_ast_scope_get_ir(frg_ast_scope_t* mut_scope, const char* name);

void frg_ast_scope_load_declaration_block(
  frg_ast_scope_t* mut_scope, const frg_ast_node_declaration_block_t* block);

void frg_ast_scope_load_declaration_function_args(
  frg_ast_scope_t* mut_scope, const frg_ast_node_declaration_function_t* function);
