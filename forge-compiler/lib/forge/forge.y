%locations
%start value
%parse-param { frg_ast_t** ast }
%union {
    void* ast;
}

%{
#include <stdio.h>
#include <forge/ast.h>
#include <forge/log.h>

extern int yylineno;
extern int yycolumnno;
extern int yylex();

extern const char* _frg_current_filename;

int yywrap() {
    return 1;
}

void yyerror(frg_ast_t**ast, const char* message) {
    frg_log_location(_frg_current_filename, yylineno, yycolumnno);
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
        $$ = frg_ast_new_value_true();
    }
    ;

value_false : KW_FALSE
    {
        $$ = frg_ast_new_value_false();
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
        $$ = frg_ast_new_value_log_not($2);
    }
    | value_primary
    {
        $$ = $1;
    }
    ;

value_log_and : value_log_not LOG_AND value_log_and
    {
        $$ = frg_ast_new_value_log_and($1, $3);
    }
    | value_log_not
    {
        $$ = $1;
    }
    ;

value_log_or : value_log_and LOG_OR value_log_or
    {
        $$ = frg_ast_new_value_log_and($1, $3);
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
