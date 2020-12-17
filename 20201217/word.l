%{
 int num_lines = 0, num_chars = 0, wordCount = 0;
%}
%%
/*[^ ]:括弧内に含まれない1文字
　\t:tab
  \n:改行
   +:直前の文字が1回以上繰り返す場合*/
[^ \t\n]+ wordCount++; num_chars += yyleng;
\n ++num_lines; ++num_chars;
. ++num_chars;
%%
int main(void) {
 yylex();
 printf("# of lines = %d, # of chars = %d, # of words = %d\n",
num_lines, num_chars, wordCount);
 return 0;
}