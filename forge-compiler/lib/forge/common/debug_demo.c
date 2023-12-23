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
