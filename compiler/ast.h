#define MAXBUF 64

typedef enum
{
    PROGRAM_AST,
    DECLARATION_AST,
    STATEMENT_AST,
    STATEMENTS_AST,
    IDENT_AST,  //変数のノード型
    NUM_AST,    //整数のノード型
    ASSIGN_AST, //代入文の=のノード型
    ADD_AST,    //加算演算子のノード型
    SUB_AST,    //減算演算子のノード型
    MUL_AST,    // *
    DIV_AST,    // /
    MOD_AST,    // %
    EQ_AST,     //==
    LT_AST,     //<
    GT_AST,     //>
    GTEQ_AST,   //<=
    LTEQ_AST,   //>=
    WHILE_AST,  // whileのノード型
    FOR_AST,    // for
    IF_AST,     // ifのノード型
    ARRAY_AST

} Ntype;

typedef struct node
{
    Ntype type;
    int ivalue;
    char *variable;
    int index[1];
    struct node *child;
    struct node *brother; //同じ親を持つノードのつながりを表す
} Node;

typedef struct symbols
{
    int symno;            // 変数番号．ユニークになるようにつけること　
    char *symbolname;     // 変数名
    struct symbols *next; // 次の変数へのリンク
    // これに加えてこの変数が割り当てられるメモリの番地の情報を入れることもできる
} Symbols;
Node *build_num_node(Ntype t, int n);
Node *build_ident_node(Ntype t, char *s);
//こどもを2つ持つノードを作る
Node *build_Node_1(Ntype t, Node *p1);
Node *build_Node_2(Ntype Ntype, Node *p1, Node *p2);
Node *build_Node_3(Ntype t, Node *p1, Node *p2, Node *p3);
Node *build_Node_4(Ntype t, Node *p1, Node *p2, Node *p3, Node *p4);

Node *build_Array_Node(Ntype t, char *s, int n);

void printNodes(Node *obj);
/*
Node *p;
p = (Node *)malloc(sizeof(Node));
p->child = p1;
p1->brother = p2; // p->child->brother = p2 でもよい
p->brother = NULL;
*/