#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

//数値ノードの作成
Node *
build_num_node(Ntype t, int n)
{
    Node *p;
    p = (Node *)malloc(sizeof(Node));
    if (p == NULL)
    {
        printf("out of memory");
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
    p = (Node *)malloc(sizeof(Node));
    if (p == NULL)
    {
        printf("out of memory");
    }
    p->type = t;
    p->variable = (char *)malloc(sizeof(MAXBUF));
    if (p->variable == NULL)
    {
        printf("out of memory");
    }
    //文字列のコピー
    strcpy(p->variable, s);
    // strncpy(p->variable, s, MAXBUF);
    p->child = NULL;
    p->brother = NULL;
    return p;
}

// こどもを1つ持つノードを作る
// t:ノード型, p*:子ノード
Node *build_Node_1(Ntype t, Node *p1)
{
    Node *p;
    p = (Node *)malloc(sizeof(Node));
    if (p == NULL)
    {
        printf("out of memory\n");
    }

    p->type = t;
    p->child = p1;
    p->brother = NULL;
    p1->brother = NULL;
    return p;
}

//子供を2つ持つノードを作る
Node *
build_Node_2(Ntype Ntype, Node *p1, Node *p2)
{
    Node *p;
    p = (Node *)malloc(sizeof(Node));
    if (p == NULL)
    {
        printf("out of memory");
    };
    p->type = Ntype;
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

Node *build_Array_Node(Ntype t, char *s, int n)
{
    Node *p;
    p = (Node *)malloc(sizeof(Node));
    if (p == NULL)
    {
        printf("out of memory\n");
    }
    p->type = t;
    p->variable = (char *)malloc(MAXBUF);
    if (p->variable == NULL)
    {
        printf("out of memory\n");
    }
    strcpy(p->variable, s);
    p->index[0] = n;
    return p;
}

int depth = 0;
/* Node の表示*/
void printNodes(Node *node)
{
    if (node != NULL)
    {
        int i = 0;
        for (; i < depth; i++)
        {
            printf("   ");
        }
        printf("ト");
        //print content
        //数値ならば
        if (NUM_AST == node->type)
        {
            printf(" : %d", node->ivalue);
        }
        //文字ならば
        else if (IDENT_AST == node->type)
        {
            printf(" : %s", node->variable);
        }
        printf("\n");

        //こどものノードが存在するなら
        if (node->child != NULL)
        {
            depth++;
            printNodes(node->child);
            depth--;
        }
        //弟ノードが存在するなら（こどもノードと同じ深さなのでdepthは足さない）
        if (node->brother != NULL)
        {
            printNodes(node->brother);
        }
    }
}

// int main(void)
// {
//     Node *n1, *n2, *n3;

//     n2 = build_ident_node(IDENT_AST, "x");
//     n3 = build_num_node(NUM_AST, 5);
//     n1 = build_Node_2(ASSIGN_AST, n2, n3);
//     printNodes(n1);
//     return 0;
// }