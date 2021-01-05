%{
//定義部
#include <stdio.h>
#include "compiler.tab.h"
#include "ast.h"
 extern int yylex();//変数宣言
 extern int yyerror();//変数宣言
 %}
// プログラムで用いるトークンを宣言
%token DEFINE ARRAY WHILE IF ELSE SEMIC L_BRACKET R_BRACKET L_PARAN R_PARAN 
%token L_BRACE R_BRACE ASSIGN ADD SUB MUL DIV EQ LT GT 
%token IDENT NUMBER
//非終端記号の型を指定するときに用いる
//%type <dval> expression mulexp primary
%union{
 Node* np; // 抽象構文木
 int ival; // 数
 char* sp; // 変数名
}
%token <sp> IDENT
%token <ival> NUMBER
%type <np> expression term factor
%type <ival> add_op mul_op
%%
//規則部
/*<プログラム> ::= <変数宣言部><文集合>*/
program : declarations statements;
/*<変数宣言部> ::= <宣言部><変数宣言部>|<宣言文>*/
declarations : decl_statement declarations | decl_statement;
/*<宣言文> ::= define<識別子>;|array<識別子>[<数>];*/
decl_statement : DEFINE IDENT SEMIC
                | ARRAY IDENT L_BRACKET NUMBER R_BRACKET SEMIC;
/*<文集合> ::= <文><文集合>|<文>*/
statements : statement statements
            | statement;
/*<文> ::= <代入文>|<ループ分>|<条件分岐分>*/
statement : assignment_stmt | loop_stmt | cond_stmt;
/*<代入分> ::= <識別子>=<算術式>;|<識別子>[<数>] = <算術式>;*/
assignment_stmt : IDENT ASSIGN expression SEMIC | IDENT L_BRACKET NUMBER R_BRACKET ASSIGN expression SEMIC;
/*<算術式> ::= <算術式><加減演算子><項>|<項>*/
expression : expression add_op term | term;
/*<項> ::= <項><乗除演算子><因子>|<因子>*/
term : term mul_op factor | factor;
/*<因子> ::= <変数>|(<算術式>)*/
factor : var | L_PARAN expression R_PARAN;
/*<加減演算子> ::= + | -*/
add_op : ADD | SUB;
/*<乗除演算子> ::= * | /*/
mul_op : MUL | DIV;
/*<変数> ::= <識別子>|<数>|<識別子>[<数>]*/
var : IDENT | NUMBER | IDENT L_BRACKET NUMBER R_BRACKET;
/*<ループ文> ::= while(<条件式>){<文集合>}*/
loop_stmt : WHILE L_PARAN condition R_PARAN L_BRACE statements R_BRACE;
/*<条件分岐文> ::= if(<条件式>){<文集合>}|if(<条件式>){<文集合>}else{<文集合>}*/
cond_stmt : IF L_PARAN condition R_PARAN L_BRACE statements R_BRACE
          | IF L_PARAN condition R_PARAN L_BRACE statements R_BRACE ELSE L_BRACE statements R_BRACE;
/*<条件式> ::= <算術式><比較演算子><算術式>*/
condition : expression cond_op expression;
/*<比較演算子> ::= ==|'<'|'>'*/
cond_op : EQ | LT | GT;
/*<識別子> ::= <英字><英数字列>|<英字>*/
// IDENT : alphabetic_char alphanumeric_char_string | alphabetic_char;
// /*<英数字列> ::= <英数字><英数字列>|<英数字>*/
// alphanumeric_char_string : alphanumeric_char alphanumeric_char_string | alphanumeric_char;
// /*<英数字> ::- <英字>|<数字>*/
// alphanumeric_char : alphabetic_char | number;
// /*<数> ::= <数字><数>|<数字>*/
// NUMBER : number NUMBER | number;
// /*<英字>*/
// alphabetic_char : a|b|c|d|e|f|g|h|i|j|k|l|m|n|o|p|q|r|s|t|u|v|w|x|y|z|A|B|C|D|E|F|G|H|I|J|K|L|M|N|O|P|Q|R|S|T|U|V|W|X|Y|Z;
// /*<数字>*/
// number : 0|1|2|3|4|5|6|7|8|9;


%%
//ユーザ定義サブルーチン
int main(void)
{
 if (yyparse()) {
 fprintf(stderr, "Error\n");
 return 1;
 }
 return 0;
} 