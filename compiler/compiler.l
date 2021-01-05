%{
//定義部
#include "compiler.tab.h"
#include "ast.h"
%}
%%
"define" return DEFINE;
"array" return ARRAY;
"while" return WHILE;
"if" return IF;
"else" return ELSE;
";" return SEMIC;
"[" return L_BRACKET;
"]" return R_BRACKET;
"(" return L_PARAN;
")" return R_PARAN;
"{" return L_BRACE;
"}" return R_BRACE;
"=" return ASSIGN;
"+" return ADD;
"-" return SUB;
"*" return MUL;
"/" return DIV;
"==" return EQ;
"<" return LT;
">" return GT;
[a-zA-Z][0-9a-zA-Z]* return IDENT;
-?(([0-9]+)|([0-9]*\.[0-9]+)([eE][-+]?[0-9]+)?) return NUMBER;
[\t ] ; /* ignore whitespace */
\n ;
%%