// Copyright (c) 2024 Sophie Katz
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

#include <forge/common/debug.h>
#include <stdio.h>

int main(void) {
    frg_debug_print_node("node %d", 0);
    frg_debug_print_newline(2);
    frg_debug_print_property("a", "hi");
    frg_debug_print_newline(4);
    frg_debug_print_property("b", "%d", 5);
    frg_debug_print_newline(4);
    frg_debug_print_property("c", NULL);
    frg_debug_print_node("node %d", 1);
    frg_debug_print_newline(6);
    frg_debug_print_node("node %d", 2);
    frg_debug_print_newline(8);
    frg_debug_print_property_with_index("d", 0, "%d", 1);
    frg_debug_print_newline(8);
    frg_debug_print_property_with_index("d", 1, "%d", 2);
    frg_debug_print_newline(8);
    frg_debug_print_property_with_index("d", 2, "%d", 3);
    printf("\n");
    return 0;
}
