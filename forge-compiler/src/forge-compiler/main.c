#include <stdio.h>
#include <forge/ast.h>
#include <forge/debug.h>

int main(int argc, char *argv[]) {
    frg_ast_t* ast = frg_ast_new_value_log_and(
        frg_ast_new_value_log_not(
            frg_ast_new_value_false()
        ),
        frg_ast_new_value_true()
    );

    frg_debug_print_ast(ast);
    printf("\n");

    frg_ast_destroy(&ast);

    return 0;
}
