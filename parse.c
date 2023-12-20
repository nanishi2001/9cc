#include "9cc.h"

//トークンの種類
typedef enum {
	TK_RESERVED,	//記号
	TK_NUM,			//整数トークン
	TK_EOF			//入力の終わりを表すトークン
} TokenKind;

//トークン型
struct Token
{
	TokenKind kind;	//トークンの型
	Token *next;	//次の入力のトークン
	int val;		//kindがTK_NUMの場合、その数値
	char *str;		//トークン文字列
	int len;		//トークンの長さ
};

//現在着目しているトークン
Token *token;

//エラー箇所を報告する
void error_at(char *loc, char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);

	int pos = loc - user_input;
	fprintf(stderr, "%s\n", user_input);
	fprintf(stderr, "%*s", pos, " ");	//pos個の空白を出力
	fprintf(stderr, "^");
	vfprintf(stderr, fmt, ap);
	fprintf(stderr, "\n");
	exit(1);
}

//次のトークンが期待している記号の時には、トークンを1つ読み進めて
//真を返す。それ以外の場合には偽を返す。
bool consume(char *op) {
	if(token->kind != TK_RESERVED || strlen(op) != token->len || memcmp(token->str, op, token->len))
		return false;
	token = token->next;
	return true;
}

//次のトークンが期待している記号の場合は、トークンを1つ読み進める
//それ以外の場合はエラーを報告する
void expect(char *op) {
	if(token->kind != TK_RESERVED || strlen(op) != token->len || memcmp(token->str, op, token->len))
		error_at(token->str, "`%c`ではありません\n", op);
	token = token->next;
}

//次のトークンが期待している数値の場合、トークンを1つ読み進めてその数値を返す
//それ以外の場合はエラーを報告する
int expect_number() {
	if (token->kind != TK_NUM)
		error_at(token->str, "数ではありません\n");
	int val = token->val;
	token = token->next;
	return val;
}

//終端トークンかどうか確かめる
bool at_eof() {
	return token->kind == TK_EOF;
}

//新しいトークンを作成してcurにつなげる
Token *new_token(TokenKind kind, Token *cur, char *str, int len) {
	Token *tok = calloc(1, sizeof(Token));
	tok->kind = kind;
	tok->str = str;
	tok->len = len;
	cur->next = tok;
	return tok;
}

//引数同士が同一かどうか確かめる
bool startswith(char *p, char *q) {
	return memcmp(p, q, strlen(q)) == 0;
}

//入力文字列pをトークナイズしてそれを返す
Token *tokenize(char *p) {
	Token head;
	head.next = NULL;
	Token *cur = &head;

	while (*p) {
		//空白文字はスキップ
		if(isspace(*p)) {
			p++;
			continue;
		}

		//比較演算子をトークン化
		if (startswith(p, "==") || startswith(p, "!=") || startswith(p, "<=") || startswith(p, ">=")) {
			cur = new_token(TK_RESERVED, cur, p, 2);
			p += 2;
			continue;
		}

		//四則演算と大なり小なりをトークン化
		if(strchr("+-*/()<>", *p)) {
			cur = new_token(TK_RESERVED, cur, p++, 1);
			continue;
		}

		//数字をトークン化
		if (isdigit(*p)) {
			cur = new_token(TK_NUM, cur, p, 0);
			char *q = p;
			cur->val = strtol(p, &p, 10);
			cur->len = p - q;
			continue;
		}

		error_at(p, "トークナイズできません\n");
		
	}

	new_token(TK_EOF, cur, p, 0);
	return head.next;
	
}
