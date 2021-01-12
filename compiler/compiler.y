%{
//定義部
#include <stdio.h>
#include "ast.h"
#include "compiler.tab.h"
extern int yylex();//変数宣言
extern int yyerror();//変数宣言
Node *top;
%}
//非終端記号の型を指定するときに用いる
//シンボル値を指定するための共用体
%union{
 Node* np; // 抽象構文木
 int ival; // 数
 char* sp; // 変数名
}
// プログラムで用いるトークンを宣言
%token DEFINE ARRAY WHILE IF ELSE SEMIC L_BRACKET R_BRACKET L_PARAN R_PARAN 
%token L_BRACE R_BRACE ASSIGN ADD SUB MUL DIV EQ LT GT 
%type <np>program statements statement decl_statement declarations assignment_stmt factor loop_stmt cond_stmt condition expression term var
%token <sp> IDENT
%token <ival> NUMBER
// %type <np> expression term factor
%type <ival> add_op mul_op cond_op
%%

/*規則部*/
/*<プログラム> ::= <変数宣言部><文集合>*/
program:declarations statements 
{top = build_Node_2(PROGRAM_AST,build_Node_1(DECLARATION_AST,$1),build_Node_1(STATEMENT_AST,$2));}
;

/*<変数宣言部> ::= <宣言部><変数宣言部>|<宣言文>*/
declarations:decl_statement declarations 
{$$ = build_Node_2(DECLARATION_AST,$1,$2);}
| decl_statement 
{$$ = build_Node_1(DECLARATION_AST,$1);};

/*<宣言文> ::= define<識別子>;|array<識別子>[<数>];*/
decl_statement:DEFINE IDENT SEMIC
{$$ = build_ident_node(IDENT_AST,$2);}
| ARRAY IDENT L_BRACKET NUMBER R_BRACKET SEMIC
{$$ = build_Array_Node(ARRAY_AST,$2,$4);};

/*<文集合> ::= <文><文集合>|<文>*/
statements:statement statements
{$$ = build_Node_2(STATEMENTS_AST,$1,$2);}
| statement
{$$ = build_Node_1(STATEMENTS_AST,$1);};

/*<文> ::= <代入文>|<ループ分>|<条件分岐分>
文は下にノードを作るのではなく、入れ替えるだけで大丈夫*/
statement:assignment_stmt
{$$ = $1;} 
| loop_stmt
{$$ = $1;}
| cond_stmt
{$$ = $1;};

/*<代入分> ::= <識別子>=<算術式>;|<識別子>[<数>] = <算術式>;*/
assignment_stmt:IDENT ASSIGN expression SEMIC
{$$ = build_Node_2(ASSIGN_AST,build_ident_node(IDENT_AST,$1),$3);} 
| IDENT L_BRACKET NUMBER R_BRACKET ASSIGN expression SEMIC
{$$ = build_Node_3(ASSIGN_AST,build_ident_node(IDENT_AST,$1),build_num_node(NUM_AST,$3),$6);};

/*<算術式> ::= <算術式><加減演算子><項>|<項>*/
expression:expression add_op term 
{$$ = build_Node_2($2, $1, $3);}
| term
{$$ = $1;};

/*<項> ::= <項><乗除演算子><因子>|<因子>*/
term:term mul_op factor 
{$$ = build_Node_2($2,$1,$3);}
| factor
{$$ = $1;};

/*<因子> ::= <変数>|(<算術式>)*/
factor:var 
{$$ = $1;}
| L_PARAN expression R_PARAN
{$$ = $2;};

/*<加減演算子> ::= + | -*/
add_op:ADD 
{$$ = ADD_AST;}
| SUB
{$$ = SUB_AST;};

/*<乗除演算子> ::= * | /*/
mul_op:MUL
{$$ = MUL_AST;} 
| DIV
{$$ = DIV_AST;};

/*<変数> ::= <識別子>|<数>|<識別子>[<数>]*/
var:IDENT 
{ $$ = build_ident_node(IDENT_AST, $1);}
| NUMBER 
{ $$ = build_num_node(NUM_AST, $1);}
| IDENT L_BRACKET NUMBER R_BRACKET
{$$ = build_Array_Node(ARRAY_AST,$1,$3);};

/*<ループ文> ::= while(<条件式>){<文集合>}*/
loop_stmt:WHILE L_PARAN condition R_PARAN L_BRACE statements R_BRACE
{$$ = build_Node_2(WHILE_AST,$3,$6);};

/*<条件分岐文> ::= if(<条件式>){<文集合>}|if(<条件式>){<文集合>}else{<文集合>}*/
cond_stmt:IF L_PARAN condition R_PARAN L_BRACE statements R_BRACE
{$$ = build_Node_2(IF_AST,$3,$6);}
| IF L_PARAN condition R_PARAN L_BRACE statements R_BRACE ELSE L_BRACE statements R_BRACE
{$$ = build_Node_3(IF_AST,$3,$6,$10);};

/*<条件式> ::= <算術式><比較演算子><算術式>*/
condition:expression cond_op expression
{$$ = build_Node_2($2,$1,$3);};

/*<比較演算子> ::= ==|'<'|'>'*/
cond_op:EQ 
{$$ = EQ_AST;}
| LT
{$$ = LT_AST;} 
| GT
{$$ = GT_AST;};
%%
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
//ユーザ定義サブルーチン

int main(void)
{
    // int result;
    // result = yyparse();
    if(yyparse()){
         return 1;
    }
    printNodes(top); // 抽象構文木を出力するための関数．
    return 0;
} 