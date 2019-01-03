#pragma once
#include <vector>
#include <string>

#include "buffer.h"

typedef enum {
	END_OF_FILE = 0,
	VAR, IF, WHILE, SWITCH, CASE, DEFAULT, PRINT, ARRAY, FOR,
	PLUS, MINUS, DIV, MULT,
	EQUAL, COLON, COMMA, SEMICOLON,
	LBRAC, RBRAC, LPAREN, RPAREN, LBRACE, RBRACE,
	NOTEQUAL, GREATER, LESS,
	NUM, ID, ERROR
} TokenType;

class Token {
public:
	void Print();

	std::string lexeme;
	TokenType token_type;
	int line_no;
};

class LexicalAnalyzer {
public:
	Token GetToken();
	TokenType UngetToken(Token);
	LexicalAnalyzer();

private:
	std::vector<Token> tokens;
	int line_no;
	Token tmp;
	InputBuffer input;

	bool SkipSpace();
	bool IsKeyword(std::string);
	TokenType FindKeywordIndex(std::string);
	Token ScanIdOrKeyword();
	Token ScanNumber();
};