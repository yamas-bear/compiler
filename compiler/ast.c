#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

Symbols *symbol_table = NULL; //記号表
int label = 0;
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
    if ((p = (Node *)malloc(sizeof(Node))) == NULL)
    {
        printf("out of memory\n");
        //yyerror("out of memory");
    }
    p->type = t;
    //変数名を付与
    p->variable = (char *)malloc(sizeof(MAXBUF));
    if (p->variable == NULL)
    {
        printf("out of memory\n");
        //yyerror("out of memory");
    }
    strcpy(p->variable, s);
    //要素番号
    p->index[0] = n;
    p->index[1] = 0; //サイズゼロ

    return p;
}

Node *build_Array_Node_ref(Ntype t, char *s, int n)
{
    Node *p;
    if ((p = (Node *)malloc(sizeof(Node))) == NULL)
    {
        printf("out of memory\n");
        //yyerror("out of memory");
    }
    p->type = t;
    //変数名を付与
    p->variable = (char *)malloc(sizeof(MAXBUF));
    if (p->variable == NULL)
    {
        printf("out of memory\n");
        //yyerror("out of memory");
    }
    char hyp[8];
    char *str = (char *)malloc(sizeof(s) * 2);
    *str = *s;
    snprintf(hyp, 8, "%d", n); //char*に変換
    strcat(str, hyp);
    strcpy(p->variable, str);
    //要素番号
    p->index[0] = n;
    p->index[1] = 0; //サイズゼロ

    free(str);
    return p;
}

int depth = 0; //表示したときに深さを示したほうが分かりやすいという判断
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

        if (depth == 0)
            printf("root   :");
        printf("%d", node->type); //nodeタイプを表示
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
        else if (ARRAY_AST == node->type)
        {
            printf(" : %s[%d]", node->variable, node->index[0]);
        }
        printf("\n");

        //こどものノードが存在するなら
        if (node->child != NULL)
        {
            depth++;
            printf("child  :");
            printNodes(node->child);
            depth--;
        }
        //弟ノードが存在するなら（こどもノードと同じ深さなのでdepthは足さない）
        if (node->brother != NULL)
        {
            printf("brother:");
            printNodes(node->brother);
        }
    }
}

// 記号表から変数名に合う変数番号を探す
Symbols *searchVariable(char *s)
{
    Symbols *p = symbol_table;
    while (0 != strcmp(p->symbolname, s))
    {
        if (NULL == p->next)
        {
            //変数が宣言されていない
            printf("変数が宣言されていません\n");
            break;
        }
        p = p->next; //次の変数をみにいく
    }
    //一致したときはその変数を返す
    return p;
}

// 変数を記号表に登録

int varNumber = 0;

void registerVarTable(char *s)
{
    Symbols *p;
    // printf("%s\n", s);

    //変数名かぶりをチェック
    p = symbol_table;
    if (NULL != p)
    {
        if (0 == strcmp(s, p->symbolname))
        {
            printf("既に変数が宣言されています : %s\n", s);
            exit(1);
        }
        while (NULL != p->next) //2つめ以降の変数が存在する場合に過去に宣言されていないかをチェック
        {
            if (0 == strcmp(s, p->next->symbolname))
            {
                printf("既に変数が宣言されています 2: %s\n", s);
                exit(1);
            }
            p = p->next; //次の変数を見る
        }
    }

    //何も変数が宣言されていないとき
    if (NULL == symbol_table)
    {
        symbol_table = (Symbols *)malloc(sizeof(Symbols)); //Symbols型のSymbolsの領域を確保する
        if (NULL == symbol_table)
        {
            //領域の確保が出来なかった場合
            printf("領域の確保ができませんでした\n");
        }
        p = symbol_table;
        //2個目以降
    }
    else
    {
        //テーブルの終端を指す
        p = symbol_table;
        while (NULL != p->next)
        {
            p = p->next;
        }
        p->next = (Symbols *)malloc(sizeof(Symbols)); //symbol_tableの最後に領域を追加する
        if (NULL == p->next)
        {
            //領域を確保できなかった場合
            printf("領域の確保ができませんでした\n");
        }
        p = p->next;
    }

    //ユニークな値を与える
    p->symno = varNumber;
    varNumber++;

    //変数名
    if ((p->symbolname = (char *)malloc(100)) == NULL) //変数名は100文字まで
    {
        //領域を確保できなかった場合
        printf("領域の確保ができませんでした\n");
    }
    strcpy(p->symbolname, s); //確保いた領域に変数名格納する
    //ヌルにしておかないと次の変数を格納することができなくなる
    p->next = NULL;
}

void registerVarTable_byArray(char *s, int n)
{
    char hyp[8];
    char *str = (char *)malloc(sizeof(s) * 2);
    *str = *s;
    snprintf(hyp, 8, "%d", n); //char*に変換
    strcat(str, hyp);
    registerVarTable(str);
    // build_ident_node(IDENT_AST, str);
    free(str);
}

//四則演算のための関数
void code_calc(Node *child, Node *brother)
{
    Symbols *child_symbols = searchVariable(child->variable);
    if (brother->child->brother != NULL)
    {
        if (brother->type == ADD_AST)
        {
            if (brother->child->brother->type == IDENT_AST && brother->child->type == IDENT_AST)
            {
                Symbols *brother_child_ident = searchVariable(brother->child->variable);
                Symbols *brother_brother_ident = searchVariable(brother->child->brother->variable);
                printf("        add      $t%d, $t%d, $t%d # %s = %s + %s", child_symbols->symno, brother_child_ident->symno, brother_brother_ident->symno, child_symbols->symbolname, brother_child_ident->symbolname, brother_brother_ident->symbolname);
            }
            else if (brother->child->brother->type == NUM_AST && brother->child->type == IDENT_AST)
            {
                Symbols *brother_child_ident = searchVariable(brother->child->variable);
                int brother_brother_number = brother->child->brother->ivalue;
                printf("        addi     $t%d, $t%d, %d # %s = %s + %d", child_symbols->symno, brother_child_ident->symno, brother_brother_number, child_symbols->symbolname, brother_child_ident->symbolname, brother_brother_number);
            }
            else
            {
                int brother_brother_number = brother->child->brother->ivalue;
                int brother_child_number = brother->child->ivalue;
                printf("        addi     $t%d, $t%d, %d # %s = %s + %d\n", child_symbols->symno, child_symbols->symno, brother_child_number, child_symbols->symbolname, child_symbols->symbolname, brother_child_number);
                printf("        addi     $t%d, $t%d, %d # %s = %s + %d", child_symbols->symno, child_symbols->symno, brother_brother_number, child_symbols->symbolname, child_symbols->symbolname, brother_brother_number);
            }
        }
        else if (brother->type == SUB_AST)
        {
        }
        else if (brother->type == MUL_AST)
        {
            if (brother->child->brother->type == IDENT_AST && brother->child->type == IDENT_AST)
            {
                Symbols *brother_child_ident = searchVariable(brother->child->variable);
                Symbols *brother_brother_ident = searchVariable(brother->child->brother->variable);
                printf("        mult      $t%d,  $t%d # %s = %s * %s\n", child_symbols->symno, brother_brother_ident->symno, child_symbols->symbolname, brother_child_ident->symbolname, brother_brother_ident->symbolname);
                printf("        mflo      $t%d", child_symbols->symno); //下位16bitを格納する
            }
            else if (brother->child->brother->type == NUM_AST && brother->child->type == IDENT_AST)
            {
                Symbols *brother_child_ident = searchVariable(brother->child->variable);
                int brother_brother_number = brother->child->brother->ivalue;
                printf("        li       $s7,%d\n", brother_brother_number);
                printf("        mult     $t%d, $s7 # %s = %s * $7\n", brother_child_ident->symno, child_symbols->symbolname, brother_child_ident->symbolname);
                printf("        mflo     $t%d", brother_child_ident->symno); //下位16bitを格納する
            }
            else
            {
                int brother_brother_number = brother->child->brother->ivalue;
                int brother_child_number = brother->child->ivalue;
                printf("        mul     $t%d, %d, %d # %s = %d * %d", child_symbols->symno, brother_child_number, brother_brother_number, child_symbols->symbolname, brother_child_number, brother_brother_number);
            }
        }
        else if (brother->type == DIV_AST)
        {
            Symbols *brother_child_ident = searchVariable(brother->child->variable);
            if (brother->child->brother->type == IDENT_AST)
            {
                Symbols *brother_brother_ident = searchVariable(brother->child->brother->variable);
                printf("        div      $t%d,  $t%d # %s = %s * %s\n", child_symbols->symno, brother_brother_ident->symno, child_symbols->symbolname, brother_child_ident->symbolname, brother_brother_ident->symbolname);
                printf("        mflo      $t%d", child_symbols->symno); //下位16bitを格納する
            }
            else
            {
                int brother_brother_number = brother->child->brother->ivalue;
                printf("        div     $t%d, $t%d, %d # %s = %s * %d", child_symbols->symno, brother_child_ident->symno, brother_brother_number, child_symbols->symbolname, brother_child_ident->symbolname, brother_brother_number);
            }
        }
    }
    else
    {
    }
}

//-------------------------------------------------------------
/* コード生成 */
void codegen(Node *np)
{
    Ntype node_type;
    Symbols *hyp;
    int start_label = 0, if_label = 0;
    if (np != NULL)
    {
        switch (np->type)
        {
        case PROGRAM_AST:
            //STATEMENT_AST
            codegen(np->child->brother);
            //DECLARATION_AST
            codegen(np->child);
            printf("#End_Of_File\n\n");
            break;

        case IDENT_AST:
            hyp = searchVariable(np->variable);
            int number = hyp->symno;
            printf("$t%d", number);
            break;

        case NUM_AST: //done
            printf("$s1");
            break;

        case ASSIGN_AST: //done
            node_type = np->child->brother->type;
            //右辺が計算の場合
            if (node_type == ADD_AST ||
                node_type == SUB_AST ||
                node_type == MUL_AST ||
                node_type == DIV_AST)
            {
                code_calc(np->child, np->child->brother);
            }
            else if (node_type == NUM_AST) //数字ならそのまま代入
            {
                Symbols *hyp = searchVariable(np->child->variable);
                int number = hyp->symno;
                char *name = hyp->symbolname;
                int ivalue = np->child->brother->ivalue;

                //そのまま左辺に代入
                printf("        li       $t%d, %d # %s = %d", number, ivalue, name, ivalue);
            }

            printf("\n");
            break;

        case LT_AST: //done
            if (np->child->brother->type == NUM_AST)
            { //条件文の右が数字の時
                printf("        li       $s1, %d # $s1 = %d\n", np->child->brother->ivalue, np->child->brother->ivalue);
            }
            printf("        slt      $v0, "); //set less than
            codegen(np->child);
            printf(", ");
            codegen(np->child->brother);
            printf(" # true=1,false=0\n");
            break;

        case GT_AST:
            if (np->child->brother->type == NUM_AST)
            { //条件文の右が数字の時
                printf("        li       $s1, %d # $s1 = 11\n", np->child->brother->ivalue);
            }
            printf("        sgt      $v0, "); //set greater than
            codegen(np->child);
            printf(", ");
            codegen(np->child->brother);
            printf(" # true=1,false=0\n");
            break;

        case WHILE_AST:
            //開始ラベル
            printf("$L%d:\n", start_label);
            //ループの条件によって相対分岐
            codegen(np->child); //$v0に条件を満たすかどうかを格納する
            printf("        beq      $v0, $zero, $L%dEnd # $v0 == 0 = false\n", start_label);
            printf("        nop\n");
            //処理
            codegen(np->child->brother);
            //ループ条件
            printf("        beq      $v0, $s0, $L%d\n", start_label);
            printf("        nop\n");
            //終了ラベル
            printf("$L%dEnd:\n", start_label);
            start_label++;
            break;

        case IF_AST:
            codegen(np->child); //$v0に条件を満たすかどうかを格納する
            printf("        beq      $v0, $zero, $IF%dEnd # $v0 == 0 = false\n", if_label);
            printf("        nop\n");
            codegen(np->child->brother);
            //終了ラベル
            printf("$IF%dEnd:\n", if_label);
            if_label++;
            break;

        case ARRAY_AST:
            // codegen(np->child);
            // codegen(np->child->brother);
            break;

        case EQ_AST:
            break;

        case DECLARATION_AST: //done
            //文の内容
            printf("%s:     .word 0x0000\n", np->child->variable);
            //次の文へ
            codegen(np->child->brother);
            break;

        case DECLARATIONS_AST: //done
            printf("        .data\n\n");
            codegen(np->child);
            printf("\n");
            break;
        case STATEMENT_AST:
            printf("        INITIAL_GP = 0x10008000\n");
            printf("        INITIAL_SP = 0x7ffffffc\n");
            printf("	    stop_service = 99\n");
            printf("	    .text\n");
            printf("init:\n");
            printf("	#initialize $gp (global pointer)and $sp (stack pointer)\n");
            printf("	la	$gp, INITIAL_GP\n");
            printf("	la	$sp, INITIAL_SP\n");
            printf("	jal	main\n");
            printf("	nop\n");
            printf("	li	$v0, stop_service\n");
            printf("	syscall\n");
            printf("	nop\n\n");
            printf("main:\n");
            printf("        li       $s0, 1 # $s0 = 1 真偽判定に使用\n");
            codegen(np->child);
            printf("\n");
            printf("        jr         $ra\n");
            printf("        nop\n\n");
            break;
        case STATEMENTS_AST:
            codegen(np->child);
            //2つめの要素がある場合
            codegen(np->child->brother);
            break;
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