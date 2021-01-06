#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

#define MAXBUF 1024
//数値ノードの作成
Node *
build_num_node(Ntype t, int n)
{
    Node *p;
    if (p = (Node *)malloc(sizeof(Node)) == NULL)
    {
        yyerror("out of memory");
    }
    p->type = t;
    p->ivalue = n;
    p->child = NULL;
    p->brother = NULL;
    return p;
}

//変数ノードの作成
Node *
build_ident_node(Ntype t, char *s)
{
    Node *p;
    if (p = (Node *)malloc(sizeof(Node)) == NULL)
    {
        yyerror("out of memory");
    }
    p->type = t;
    if (p->variable = (char *)malloc(sizeof(MAXBUF)) == NULL)
    {
        yyerror("out of memory");
    }
    strncpy(p->variable, s, MAXBUF);
    p->child = NULL;
    p->brother = NULL;
    return p;
}

//子供を2つ持つノードを作る
Node *
build_Node_2(Ntype Ntype, Node *p1, Node *p2)
{
    Node *p;
    if (p = (Node *)malloc(sizeof(Node) == NULL))
    {
        yyerror("out of memory");
    };
    p->type = Ntype;
    // p->child = p1;
    // p1->brother = p2;
    // p->brother = NULL;
    if (p1 != NULL)
    {
        p->child = p1;
        p1->brother = p2;
        p->brother = NULL;
    }
    else
    {
        p->child = NULL;
    }
    return p;
}

// こどもを3つ持つノードを作る
Node *build_Node_3(Ntype t, Node *p1, Node *p2, Node *p3)
{
    Node *p;
    p = build_Node_2(t, p1, p2);
    p2->brother = p3;

    return p;
}

// こどもを4つ持つノードを作る
Node *build_Node_4(Ntype t, Node *p1, Node *p2, Node *p3, Node *p4)
{
    Node *p;
    p = build_Node_3(t, p1, p2, p3);
    p3->brother = p4;

    return p;
}

int depth = 0;
int cnt = 1;
void *printNodes(Node *node)
{
    if (node != NULL)
    {
        if (node->child != NULL)
        {
            cnt++;
            depth++;
            printNodes(node->child);
            depth--;
        }
        else if (node->brother != NULL)
        {
            cnt++;
            printNodes(node->brother);
        }
    }

    printf("%d", node->type);
    printf("cnt:%d ", cnt);
    printf("depth%d\n", depth);
    cnt--;
}

// int main(void)
// {
//     Node *top;
//     Node *p1;
//     p1 = build_Node_2(ADD_AST,
//                       build_Node_2(NUM_AST, NULL, NULL),
//                       build_Node_2(NUM_AST, NULL, NULL));
//     top = build_Node_2(ASSIGN_AST,
//                        build_Node_2(IDENT_AST, NULL, NULL),
//                        p1);
//     printNodes(top);
//     fprintf(stdout, "\n");
//     return 0;
// }