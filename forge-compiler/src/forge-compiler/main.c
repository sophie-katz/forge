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
