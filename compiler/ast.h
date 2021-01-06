typedef enum
{
    PROGRAM_AST,
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
    ARRAY_AST,
    DEC_AST,
    DECS_AST,
    STATE_AST,
    STATES_AST,

} Ntype;

typedef struct node
{
    Ntype type;
    int ivalue;
    char *variable;
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

//こどもを2つ持つノードを作る
Node *build_Node_2(Ntype Ntype, Node *p1, Node *p2);

void *printNodes(Node *obj);
/*
Node *p;
p = (Node *)malloc(sizeof(Node));
p->child = p1;
p1->brother = p2; // p->child->brother = p2 でもよい
p->brother = NULL;
*/