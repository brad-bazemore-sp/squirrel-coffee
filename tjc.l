/* Brad Bazemore */
%option yylineno

%{
#include "Ast.h"
#include "y.tab.h"
void error( const char *fmt, ... );
// we have to copy because we can't rely on yytext not changing underneath us:
#define SAVE_TOKEN yylval.str = strdup(yytext);
%}

ws          [ \n\t\r]
comment     (\/\/)(.)*|"/*"([^*]|(\*+[^*/]))*\*+\/

letter      [a-z]|[A-Z]|_
n_num       [1-9]
w_num       [0-9]

d_int       0|{n_num}{w_num}*
exp         (e|E)((({plus}|{minus}){w_num}+)|{w_num}+)
f_point     (({w_num}+\.{w_num}+)(f|F))|(({w_num}+\.{w_num}+{exp})(f|F))

ident       {letter}({letter}|{w_num})*
str_l       \"(.)*\"

plus	    \+
minus	    \-
times	    \*
divide	    \/
equals	    =

increment   "++"
decrement   "--"

le          "<"
ge          ">"
leq         "<="
geq         ">="
eq          "=="
ne          "!="

l_par	    \(
r_par	    \)
l_brac      \{
r_brac      \}
l_brc       \[
r_brc       \]

semicolon   \;
comma       \,
period      \.

t_int       "int"
t_float     "float"
t_null      "null"

public      "public"
class       "class"
static      "static"
void        "void"
return      "return"
new         "new"

if          "if"
else        "else"
while       "while"
for         "for"

bad_int     "0"+{n_num}+
bad_ident   {w_num}+{letter}+
bad_string  \"(.)*\n\"
bad_eof     (\"[^\"]*|\/\*[^\*\/]*)

%%

{t_int}     return T_INT;
{t_float}   return T_FLOAT;
{t_null}    return T_NULL;

{public}    return PUBLIC;
{class}     return CLASS;
{static}    return STATIC;
{void}      return VOID;
{return}    return RETURN;
{new}       return NEW;

{if}        return IF;
{else}      return ELSE;
{while}     return WHILE;
{for}       return FOR;

{ident}     SAVE_TOKEN; return IDENT;

{d_int}     SAVE_TOKEN; return D_INT;
{f_point}   SAVE_TOKEN; return F_POINT;
{str_l}     SAVE_TOKEN; return STR_L;

{increment} return INC;
{decrement} return DEC;

{plus}      return PLUS;
{minus}     return MINUS;
{times}     return TIMES;
{divide}    return DIVIDE;
{equals}    return EQUALS;

{le}        return LE;
{ge}        return GE;
{leq}       return LEQ;
{geq}       return GEQ;
{eq}        return EQ;
{ne}        return NE;

{l_par}     return L_PAR;
{r_par}     return R_PAR;
{l_brac}    return L_BRAC;
{r_brac}    return R_BRAC;
{l_brc}     return L_BRC;
{r_brc}     return R_BRC;

{semicolon} return SEMICOLON;
{comma}     return COMMA;
{period}    return PERIOD;

{ws}		;
{comment}   ;

{bad_int}       {
        error("Line %d: Illegal int: %s\n", yytext);
    }
{bad_ident}     {
        error("Line %d: Illegal identifier: %s\n", yytext);
    }
{bad_string}    {
        error("Illegal string: %s\n", yytext);
    }
. 		        {
        error("Illegal input: %s\n", yytext );
    }

%%

/* single reading of input */
int yywrap( void ){
  return 1;
}

/*Have line number and lexeme, attempt to continue
  illegal char in input: <out> <ex $>
  illegal int const: <out> <ex 0434>
  illegal string const: <out> <ex new line in string>
  Number followed by identifier: <out> <ex 22length>
  Unexpected EOF:<> <ex no closeing " or no closing comment>*/
