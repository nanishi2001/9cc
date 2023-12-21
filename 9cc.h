#include<ctype.h>
#include<stdarg.h>
#include<stdbool.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

//入力プログラム
char *user_input;

//トークン型の宣言
typedef struct Token Token;

//現在着目しているトークン
Token *token;

//ノード型の宣言
typedef struct Node Node;

//プロトタイプ宣言
Token *tokenize(char *p);
Node *expr();
Node *equality();
Node *relational();
Node *add();
Node *mul();
Node *primary();
Node *unary();
void gen(Node *node);
bool consume(char *op);
void expect(char *op);
int expect_number();
