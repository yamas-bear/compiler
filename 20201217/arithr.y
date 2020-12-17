%{
#include <stdio.h>
#include "arithrv.tab.h"
 extern int yylex();//変数宣言
 extern int yyerror();//変数宣言
 double vbltable[26];//領域確保
 %}
%union{
 double dval;
 int vblno;
 }
%token <vblno> NAME
%token <dval> NUMBER
%type <dval> expression mulexp primary
%%
//何行か分からないときはこのように書く。区切りは改行文字
statement_list : statement '\n'
| statement_list statement '\n'
;

statement : NAME '=' expression {
    vbltable[$1] = $3; 
    printf("%c =%g\n> ", $1+'a', $3); }
| expression {printf("= %g\n> ", $1);}
;

expression : expression '+' mulexp {$$ = $1 + $3; }
| expression '-' mulexp {$$ = $1 - $3; }
| mulexp {$$ = $1;}
;

mulexp : mulexp '*' primary {$$ = $1 * $3; }
| mulexp '/' primary
{if($3 == 0) {/* printf("divide by zero\n"); or */ yyerror("divideby zero"); return 0;} else $$ = $1 / $3; }| primary {$$ = $1;};

primary : '(' expression ')' {$$ = $2; }
| NUMBER {$$ = $1;}
| NAME {$$ = vbltable[$1];}
;

%%
int main(void)
{
 printf("> ");
 if (yyparse()) {
 fprintf(stderr, "Error\n");
 return 1;
 }
} 