%locations
%start value
%parse-param { frg_ast_t** ast }
%union {
    void* ast;
}

%{
#include <forge/ast/ast.h>
#include <forge/common/log.h>
#include <stdio.h>

extern int yylineno;
extern frg_columnno_t yycolumnno;
extern int yylex();

extern const char* _frg_current_filename;

int yywrap() {
    return 1;
}

void yyerror(frg_ast_t**ast, const char* message) {
    frg_log_prefix_source_char(_frg_current_filename, yylineno, yycolumnno);
    frg_log(FRG_LOG_SEVERITY_ERROR, "%s\n", message);
}
%}

%token KW_TRUE
%token KW_FALSE
%token LPAREN
%token RPAREN
%token LOG_NOT
%token LOG_AND
%token LOG_OR

%type <ast> value_true
%type <ast> value_false
%type <ast> value_primary
%type <ast> value_log_not
%type <ast> value_log_and
%type <ast> value_log_or
%type <ast> value

%%
value_true : KW_TRUE
    {
        frg_status_t result = frg_ast_new_value_primary((frg_ast_t**)&$$, FRG_AST_ID_VALUE_TRUE);
        if (result != FRG_STATUS_OK) {
            frg_log_prefix_internal();
            frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "unable to create ast: %s", frg_status_to_string(result));
        }
    }
    ;

value_false : KW_FALSE
    {
        frg_status_t result = frg_ast_new_value_primary((frg_ast_t**)&$$, FRG_AST_ID_VALUE_FALSE);
        if (result != FRG_STATUS_OK) {
            frg_log_prefix_internal();
            frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "unable to create ast: %s", frg_status_to_string(result));
        }
    }
    ;

value_primary : value_true
    {
        $$ = $1;
    }
    | value_false
    {
        $$ = $1;
    }
    | LPAREN value RPAREN
    {
        $$ = $2;
    }
    ;

value_log_not : LOG_NOT value_primary
    {
        frg_status_t result = frg_ast_new_value_unary((frg_ast_value_unary_t**)&$$, FRG_AST_ID_VALUE_LOG_NOT, $2);
        if (result != FRG_STATUS_OK) {
            frg_log_prefix_internal();
            frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "unable to create ast: %s", frg_status_to_string(result));
        }
    }
    | value_primary
    {
        $$ = $1;
    }
    ;

value_log_and : value_log_not LOG_AND value_log_and
    {
        frg_status_t result = frg_ast_new_value_binary((frg_ast_value_binary_t**)&$$, FRG_AST_ID_VALUE_LOG_AND, $1, $3);
        if (result != FRG_STATUS_OK) {
            frg_log_prefix_internal();
            frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "unable to create ast: %s", frg_status_to_string(result));
        }
    }
    | value_log_not
    {
        $$ = $1;
    }
    ;

value_log_or : value_log_and LOG_OR value_log_or
    {
        frg_status_t result = frg_ast_new_value_binary((frg_ast_value_binary_t**)&$$, FRG_AST_ID_VALUE_LOG_OR, $1, $3);
        if (result != FRG_STATUS_OK) {
            frg_log_prefix_internal();
            frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "unable to create ast: %s", frg_status_to_string(result));
        }
    }
    | value_log_and
    {
        $$ = $1;
    }
    ;

value : value_log_or
    {
        *ast = $1;
    }
    ;
%%
