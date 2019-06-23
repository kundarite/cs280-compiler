/*
 * projlex.h
 *
 * CS280
 * Spring 2018
 */

#ifndef TOKENS_H_
#define TOKENS_H_

#include <string>
#include <iostream>
using std::string;
using std::istream;
using std::ostream;

enum TokenType {
		// keywords
	PRINT,
	IF,
	THEN,
	TRUE,
	FALSE,

		// an identifier
	IDENT,

		// an integer and string constant
	ICONST,
	SCONST,

		// the operators, parens and semicolon
	PLUS,
	MINUS,
	STAR,
	SLASH,
	ASSIGN,
	EQ,
	NEQ,
	LT,
	LEQ,
	GT,
	GEQ,
	LOGICAND,
	LOGICOR,
	LPAREN,
	RPAREN,
	SC,

		// any error returns this token
	ERR,

		// when completed (EOF), return this token
	DONE
};

class Token {
	TokenType	tt;
	string		lexeme;
	int			lnum;

public:
	Token() {
		tt = ERR;
		lnum = -1;
	}
	Token(TokenType tt, string lexeme, int line) {
		this->tt = tt;
		this->lexeme = lexeme;
		this->lnum = line;
	}

	bool operator==(const TokenType tt) const { return this->tt == tt; }
	bool operator!=(const TokenType tt) const { return this->tt != tt; }

	TokenType	GetTokenType() const { return tt; }
	string		GetLexeme() const { return lexeme; }
	int			GetLinenum() const { return lnum; }
};

extern ostream& operator<<(ostream& out, const Token& tok);

extern Token getNextToken(istream *in, int *linenum);


#endif /* TOKENS_H_ */
