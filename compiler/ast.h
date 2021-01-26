#define MAXBUF 64

typedef enum
{
    PROGRAM_AST,      //0
    DECLARATION_AST,  //1
    DECLARATIONS_AST, //2
    STATEMENT_AST,    //3
    STATEMENTS_AST,   //4
    IDENT_AST,        //変数のノード型(5)
    NUM_AST,          //整数のノード型(6)
    ASSIGN_AST,       //代入文の=のノード型(7)
    ADD_AST,          //加算演算子のノード型(8)
    SUB_AST,          //減算演算子のノード型(9)
    MUL_AST,          // *(10)
    DIV_AST,          // /(11)
    MOD_AST,          // %(12)
    EQ_AST,           //==(13)
    LT_AST,           //<(14)
    GT_AST,           //>(15)
    GTEQ_AST,         //<=(16)
    LTEQ_AST,         //>=(17)
    WHILE_AST,        // whileのノード型(18)
    FOR_AST,          // for(19)
    IF_AST,           // ifのノード型(20)
    ARRAY_AST         //21

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

extern int depth;
extern Symbols *varTable, *tableStart;
extern int varNum;

Node *build_num_node(Ntype t, int n);
Node *build_ident_node(Ntype t, char *s);
//こどもを2つ持つノードを作る
Node *build_Node_1(Ntype t, Node *p1);
Node *build_Node_2(Ntype Ntype, Node *p1, Node *p2);
Node *build_Node_3(Ntype t, Node *p1, Node *p2, Node *p3);
Node *build_Node_4(Ntype t, Node *p1, Node *p2, Node *p3, Node *p4);

Node *build_Array_Node(Ntype t, char *s, int n);

void printNodes(Node *obj);

void generateArith(Node *obj, Ntype setType, int stackDepth);
void codegen(Node *obj, int depth);
void printFirstMessage();
void registerVarTable(char *s);
