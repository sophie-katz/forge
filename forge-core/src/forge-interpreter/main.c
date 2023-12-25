// Copyright (c) 2023 Sophie Katz
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

#include <forge/ast/ast.h>
#include <forge/common/debug.h>
#include <forge/common/log.h>
#include <forge/exec/exec.h>
#include <forge/parse/parse.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
    // frg_ast_t* ast = frg_parse_string("true \n&& (!false)", "--");

    // if (ast != NULL) {
    //     frg_debug_print_ast(ast);
        
    //     printf("\n\n=> ");

    //     frg_value_t* value = frg_exec(ast);

    //     frg_value_print(stdout, value);

    //     printf("\n");

    //     frg_value_destroy(&value);
    //     frg_ast_destroy(&ast);
    // }

    return 0;
}
