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
#include <forge/enum_printing.h>

void frg_message_severity_print(frg_stream_output_t* mut_stream,
                                frg_message_severity_t severity) {
  switch (severity) {
  case FRG_MESSAGE_SEVERITY_DEBUG:
    frg_stream_output_write_string(mut_stream, "debug");
    break;
  case FRG_MESSAGE_SEVERITY_NOTE:
    frg_stream_output_write_string(mut_stream, "note");
    break;
  case FRG_MESSAGE_SEVERITY_WARNING:
    frg_stream_output_write_string(mut_stream, "warning");
    break;
  case FRG_MESSAGE_SEVERITY_ERROR:
    frg_stream_output_write_string(mut_stream, "error");
    break;
  case FRG_MESSAGE_SEVERITY_FATAL_ERROR:
    frg_stream_output_write_string(mut_stream, "fatal error");
    break;
  case FRG_MESSAGE_SEVERITY_INTERNAL_ERROR:
    frg_stream_output_write_string(mut_stream, "internal fatal");
    break;
  default:
    frg_stream_output_write_printf(mut_stream, "unknown severity (%i)", (int)severity);
    break;
  }
}

void frg_ast_node_type_int_flags_print(frg_stream_output_t* mut_stream,
                                       frg_ast_node_type_int_flags_t flags) {
  bool first = true;

  if (flags & FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED) {
    if (!first) {
      frg_stream_output_write_character(mut_stream, ',');
    }

    frg_stream_output_write_printf(mut_stream, "unsigned");

    first = false;
  }

  if (first) {
    frg_stream_output_write_printf(mut_stream, "none");
  }
}

void frg_ast_node_type_pointer_flags_print(frg_stream_output_t* mut_stream,
                                           frg_ast_node_type_pointer_flags_t flags) {
  bool first = true;

  if (flags & FRG_AST_NODE_TYPE_POINTER_FLAG_CONSTANT) {
    if (!first) {
      frg_stream_output_write_character(mut_stream, ',');
    }

    frg_stream_output_write_printf(mut_stream, "constant");

    first = false;
  }

  if (flags & FRG_AST_NODE_TYPE_POINTER_FLAG_IMPLICIT_DEREFERENCE) {
    if (!first) {
      frg_stream_output_write_character(mut_stream, ',');
    }

    frg_stream_output_write_printf(mut_stream, "implicit-dereference");

    first = false;
  }

  if (first) {
    frg_stream_output_write_printf(mut_stream, "none");
  }
}

void frg_ast_node_declaration_property_flags_print(
  frg_stream_output_t* mut_stream, frg_ast_node_declaration_property_flags_t flags) {
  bool first = true;

  if (flags & FRG_AST_NODE_DECLARATION_PROPERTY_FLAG_OPTIONAL) {
    if (!first) {
      frg_stream_output_write_character(mut_stream, ',');
    }

    frg_stream_output_write_printf(mut_stream, "optional");

    first = false;
  }

  if (flags & FRG_AST_NODE_DECLARATION_PROPERTY_FLAG_NON_OPTIONAL) {
    if (!first) {
      frg_stream_output_write_character(mut_stream, ',');
    }

    frg_stream_output_write_printf(mut_stream, "non-optional");

    first = false;
  }

  if (flags & FRG_AST_NODE_DECLARATION_PROPERTY_FLAG_SPREAD) {
    if (!first) {
      frg_stream_output_write_character(mut_stream, ',');
    }

    frg_stream_output_write_printf(mut_stream, "spread");

    first = false;
  }

  if (first) {
    frg_stream_output_write_printf(mut_stream, "none");
  }
}

void frg_ast_node_declaration_interface_flags_print(
  frg_stream_output_t* mut_stream, frg_ast_node_declaration_interface_flags_t flags) {
  bool first = true;

  if (flags & FRG_AST_NODE_DECLARATION_INTERFACE_FLAG_ABSTRACT) {
    if (!first) {
      frg_stream_output_write_character(mut_stream, ',');
    }

    frg_stream_output_write_printf(mut_stream, "abstract");

    first = false;
  }

  if (first) {
    frg_stream_output_write_printf(mut_stream, "none");
  }
}

void frg_ast_node_declaration_function_argument_flags_print(
  frg_stream_output_t* mut_stream,
  frg_ast_node_declaration_function_argument_flags_t flags) {
  bool first = true;

  if (flags & FRG_AST_NODE_DECLARATION_FUNCTION_ARGUMENT_FLAG_KEYWORD) {
    if (!first) {
      frg_stream_output_write_character(mut_stream, ',');
    }

    frg_stream_output_write_printf(mut_stream, "kw");

    first = false;
  }

  if (first) {
    frg_stream_output_write_printf(mut_stream, "none");
  }
}

void frg_ast_node_declaration_function_flags_print(
  frg_stream_output_t* mut_stream, frg_ast_node_declaration_function_flags_t flags) {
  bool first = true;

  if (flags & FRG_AST_NODE_DECLARATION_FUNCTION_FLAG_MUTABLE) {
    if (!first) {
      frg_stream_output_write_character(mut_stream, ',');
    }

    frg_stream_output_write_printf(mut_stream, "mut");

    first = false;
  }

  if (flags & FRG_AST_NODE_DECLARATION_FUNCTION_FLAG_OVERRIDE) {
    if (!first) {
      frg_stream_output_write_character(mut_stream, ',');
    }

    frg_stream_output_write_printf(mut_stream, "override");

    first = false;
  }

  if (first) {
    frg_stream_output_write_printf(mut_stream, "none");
  }
}
