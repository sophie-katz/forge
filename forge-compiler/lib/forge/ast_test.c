#include <unity.h>
#include <forge/ast.h>

void setUp(void) {}

void tearDown(void) {}

void test_simple_expression(void) {
    frg_ast_t* ast = frg_ast_new_value_log_and(
        frg_ast_new_value_log_not(
            frg_ast_new_value_false()
        ),
        frg_ast_new_value_true()
    );

    TEST_ASSERT_NOT_NULL(frg_ast_as_value_binary(ast)->rhs);

    frg_ast_destroy(&ast);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_simple_expression);
    return UNITY_END();
}
