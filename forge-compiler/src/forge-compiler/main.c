#include <stdio.h>
#include <string.h>
#include <forge/ast.h>
#include <forge/debug.h>
#include <forge/log.h>
#include <forge/parse.h>

int main(int argc, char *argv[]) {
    frg_ast_t* ast = frg_parse_string("true \n&& (!false)", "--");

    if (ast != NULL) {
        frg_debug_print_ast(ast);
        printf("\n");

        frg_ast_destroy(&ast);
    }

    return 0;
}
