#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

Symbols *varTable = NULL; //記号表
int varNum = 0, label = 0;
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

// 記号表から変数名に合う変数番号を探す
int searchVariableNum(char *s)
{
    Symbols *p = varTable;
    while (0 != strcmp(p->symbolname, s))
    {
        if (NULL == p->next)
        {
            //変数が宣言されていない
            return -1;
        }
        p = p->next;
    }
    //一致したときはその変数の番号を返す
    return p->symno;
}

// 変数を記号表に登録
void registerVarTable(char *s)
{
    Symbols *p;

    //変数名かぶりをチェック
    p = varTable;
    if (NULL != p)
    {
        if (0 == strcmp(s, p->symbolname))
        {
            printf("error invalid symbol name : %s\n", s);
            exit(1);
        }
        while (NULL != p->next) //2つめ以降の変数が存在する場合に過去に宣言されていないかをチェック
        {
            if (0 == strcmp(s, p->next->symbolname))
            {
                printf("error invalid symbol name : %s\n", s);
                exit(1);
            }
            p = p->next; //次の変数を見る
        }
    }

    //何も変数が宣言されていないとき
    if (NULL == varTable)
    {
        varTable = (Symbols *)malloc(sizeof(Symbols)); //Symbols型のSymbolsの領域を確保する
        if (NULL == varTable)
        {
            //領域の確保が出来なかった場合
            printf("out of memory\n");
        }
        p = varTable;
        //2個目以降
    }
    else
    {
        //テーブルの終端を指す
        p = varTable;
        while (NULL != p->next)
        {
            p = p->next;
        }
        p->next = (Symbols *)malloc(sizeof(Symbols));
        if (NULL == p->next)
        {
            //領域を確保できなかった場合
            printf("out of memory\n");
        }
        p = p->next;
    }

    //ユニークな値を与える
    p->symno = varNum;
    varNum++;

    //変数名
    if ((p->symbolname = (char *)malloc(100)) == NULL) //変数名は100文字まで
    {
        //領域を確保できなかった場合
        printf("out of memory\n");
    }
    strcpy(p->symbolname, s);

    //ヌルにしておかないと次の変数を格納することができなくなる
    p->next = NULL;
}

void printFirstMessage()
{
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
    printf("stop:\n");
    printf("	j stop\n");
    printf("	nop\n\n");
    printf("  　.text 0x00001000\n");
    printf("main:\n");
}

/* 四則演算のコード生成を行う */
void generateArith(Node *obj,
                   Ntype setType,
                   int stackDepth)
{
    Ntype opr;
    int countLabel = 0;

    //スタックに2つの値を格納する
    //子供
    opr = obj->child->type;
    int stackOffset = (stackDepth + 1) * 4;
    switch (opr)
    {
        //四則演算
    case ADD_AST:
    case SUB_AST:
    case MUL_AST:
    case DIV_AST:
    case MOD_AST:
        //スタックから計算結果をレジスタに取り出す
        generateArith(obj->child, opr, stackDepth + 1);
        break;

        //変数
    case IDENT_AST:
        printf("        add      $v0, $t%d, $zero\n",
               searchVariableNum(obj->child->variable));
        printf("        nop\n");
        printf("        sw       $v0, %d($sp)\n",
               stackOffset);
        break;

        //値
    case NUM_AST:
        printf("        addi     $v0, $zero, %d\n",
               obj->child->ivalue);
        printf("        sw       $v0, %d($sp)\n",
               stackOffset);
        break;
    }
    //兄弟
    opr = obj->child->brother->type;
    stackOffset = (stackDepth + 2) * 4;
    switch (opr)
    {
        //四則演算
    case ADD_AST:
    case SUB_AST:
    case MUL_AST:
    case DIV_AST:
    case MOD_AST:
        //スタックから計算結果をレジスタに取り出す
        generateArith(obj->child->brother,
                      opr,
                      stackDepth + 2);
        break;

        //変数
    case IDENT_AST:
        printf("        add      $v0, $t%d, $zero\n",
               searchVariableNum(obj->child->brother->variable));
        printf("        nop\n");
        printf("        sw       $v0, %d($sp)\n",
               stackOffset);
        break;

        //値
    case NUM_AST:
        printf("        addi      $v0, $zero, %d\n",
               obj->child->brother->ivalue);
        printf("        sw       $v0, %d($sp)\n",
               stackOffset);
        break;
    }

    //スタックから$v0, $v1に書き込む
    printf("        lw       $v0, %d($sp)\n",
           (stackDepth + 1) * 4);
    printf("        nop\n");
    printf("        lw       $v1, %d($sp)\n",
           (stackDepth + 2) * 4);
    printf("        nop\n");

    //演算
    switch (setType)
    {
    case ADD_AST:
        printf("        add      $v0, $v0, $v1");
        break;
    case SUB_AST:
        printf("        sub      $v0, $v0, $v1");
        break;
    case MUL_AST:
        printf("        mult     $v0, $v1\n");
        printf("        mflo     $v0");
        break;
    case DIV_AST:
        printf("        div       $v0, $v1\n");
        printf("        mflo     $v0");
        break;
    }
    printf("# arith\n");
    printf("        sw       $v0, %d($sp)\n",
           stackDepth * 4);
}

//-------------------------------------------------------------
/* コード生成 */
void codegen(Node *obj, int depth)
{
    Ntype opr;
    int labelMem = 0, condNum = 0;
    if (obj != NULL)
    {
        switch (obj->type)
        {
        case PROGRAM_AST:
            //テキスト部(STATEMENT_AST)
            codegen(obj->child->brother, depth + 1);
            //データ部
            codegen(obj->child, depth + 1);
            printf("#EOF\n\n");
            break;

        case IDENT_AST: //done
            printf("$t%d", searchVariableNum(obj->variable));
            break;

        case NUM_AST: //done
            printf("$s0");
            break;

        case ASSIGN_AST: //done
            opr = obj->child->brother->type;
            //右辺が計算なら計算
            if (ADD_AST == opr ||
                SUB_AST == opr ||
                MUL_AST == opr ||
                DIV_AST == opr ||
                MOD_AST == opr)
            {
                codegen(obj->child->brother, depth + 1);
                //計算結果を左辺に代入
                printf("        ori      $t%d, $v0, 0 # assign result", searchVariableNum(obj->child->variable));

                //右辺が数字ならそのまま代入
            }
            else if (NUM_AST == opr)
            {
                //そのまま左辺に代入
                printf("        li     $t%d, %d # assign num", searchVariableNum(obj->child->variable), obj->child->brother->ivalue);
            }

            printf("\n");
            break;

        case ADD_AST: //done
            generateArith(obj, ADD_AST, 0);
            break;

        case SUB_AST: //done
            generateArith(obj, SUB_AST, 0);
            break;

        case MUL_AST: //done
            generateArith(obj, MUL_AST, 0);
            break;

        case DIV_AST: //done
            generateArith(obj, DIV_AST, 0);
            break;

        case LT_AST: //done
            if (NUM_AST == obj->child->brother->type)
            { //条件文の右が数字の時
                printf("        li       $s0, %d\n", obj->child->brother->ivalue);
            }
            printf("        slt      $v0, ");
            codegen(obj->child, depth + 1);
            printf(", ");
            codegen(obj->child->brother, depth + 1);
            printf("\n");
            break;

        case GT_AST:
            printf("        sgt        $v0, ");
            codegen(obj->child, depth + 1);
            printf(", ");
            codegen(obj->child->brother, depth + 1);
            printf("\n");
            break;

        case WHILE_AST:
            //開始ラベル
            printf("$L%d:\n", labelMem);
            //ループ開始条件
            codegen(obj->child, depth + 1);
            printf("        beq      $v0, $zero, $L%dEnd\n", labelMem);
            printf("        nop\n");
            //処理
            codegen(obj->child->brother, depth + 1);
            //ループ条件
            codegen(obj->child, depth + 1);
            printf("        beq      $v0, $s7, $L%d\n", labelMem);
            printf("        nop\n");
            //終了ラベル
            printf("$L%dEnd:\n", labelMem);
            labelMem++;
            break;

        // case IF_AST:
        //     //条件
        //     generateCode(obj->child, depth + 1);
        //     labelMem = label++;
        //     printf("        beq        $v0, $zero, $IFELSE%d\n", labelMem);
        //     printf("        nop\n");
        //     //処理
        //     generateCode(obj->child->brother, depth + 1);
        //     printf("        j         $IFEND%d\n", labelMem);
        //     printf("        nop\n");
        //     //else処理
        //     if (NULL != obj->child->brother->brother)
        //     {
        //         printf("$IFELSE%d:\n", labelMem);
        //         generateCode(obj->child->brother->brother,
        //                      depth + 1);
        //     }

        //     //処理終了ラベル
        //     printf("$IFEND%d:\n", labelMem);
        //     break;

        // case ARRAY_AST:

        //     break;
        case DECLARATION_AST: //done
            //文の内容
            printf("%s:     .word 0x0000\n", obj->child->variable);
            //次の文へ
            codegen(obj->child->brother, depth + 1);
            break;

        case DECLARATIONS_AST: //done
            printf("        .data\n\n");
            codegen(obj->child, depth + 1);
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
            printf("    li        $s7, 1\n");
            codegen(obj->child, depth + 1);
            printf("\n");
            printf("        jr         $ra\n");
            printf("        nop\n\n");
            break;
        case STATEMENTS_AST:
            codegen(obj->child, depth + 1);
            //2つめの要素がある場合
            codegen(obj->child->brother, depth + 1);
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