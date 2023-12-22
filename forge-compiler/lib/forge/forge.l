%option noyywrap
%option nounput
%option noinput
%option yylineno
/* %option bison-locations */

%{
#include "forge.tab.h"
#include <stdbool.h>
#include <ctype.h>
#include <forge/log.h>
#include <forge/types.h>

#define YY_USER_ACTION yycolumnno += yyleng;

extern const char* _frg_current_filename;

frg_columnno_t yycolumnno = 1;
%}

%%

\n {
    yycolumnno = 0;
}

[ \r\t\v]+ ;

true {
    return KW_TRUE;
}

false {
    return KW_FALSE;
}

"(" {
    return LPAREN;
}

")" {
    return RPAREN;
}

"!" {
    return LOG_NOT;
}

"&&" {
    return LOG_AND;
}

"||" {
    return LOG_OR;
}

. {
    frg_log_prefix_source_char(_frg_current_filename, yylineno, yycolumnno);
    char as_char = yytext[0];
    int as_int = ((int)as_char) & 0xff;
    if (isprint(yytext[0])) {
        frg_log(FRG_LOG_SEVERITY_ERROR, "unexpected character '%c' (0x%2x)", as_char, as_int);
    } else {
        frg_log(FRG_LOG_SEVERITY_ERROR, "unexpected byte 0x%2x", as_int);
    }
}

%%
