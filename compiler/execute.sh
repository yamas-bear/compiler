#!/bin/sh

gcc -c ast.c
flex compiler.l
bison -d compiler.y
gcc ast.o compiler.tab.c lex.yy.c -o compiler -lfl -ly
echo "compile complete"