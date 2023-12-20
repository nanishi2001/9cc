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
