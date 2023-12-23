#include <forge/common/debug.h>
#include <stdio.h>

int main(void) {
    frg_debug_print_node("node %d", 0);
    frg_debug_print_newline(2);
    frg_debug_print_property("a", "hi");
    frg_debug_print_newline(4);
    frg_debug_print_property("b", "%d", 5);
    frg_debug_print_newline(4);
    frg_debug_print_node_property("c", "node %d", 1);
    frg_debug_print_newline(6);
    frg_debug_print_node("node %d", 2);
    printf("\n");
    return 0;
}
