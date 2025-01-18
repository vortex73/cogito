#include "lexer.h"
#include "token.h"

std::vector<Token> Lexer::tokenize() {
	std::vector<Token> tokenList;
	while (!isEnd()) {
		start = current;
		scanTok();
	}
	tokenList.emplace_back(Type::tok_eof, "", line);
	return tokenList;
}

bool const Lexer::isEnd() {
	return current >= source.length();
}

char Lexer::proceed() {
	return source[current++];
}

char const Lexer::peek() {
	return isEnd() ? '\0' : source[current];
}

char const Lexer::peekNext() {
	return (current+1 < source.length()) ? source[current+1] : '\0';
}

bool Lexer::match(char c) {
	if (isEnd() || source[current]!=c) return false;
	current++;
	return true;
}

void Lexer::addTok(Type type){

}

void Lexer::scanTok() {
	char c = proceed();
	switch (c) {
		case '(': 
			addTok(Type::tok_lparen);
			break;
		case ')':
			addTok(Type::tok_rparen);
			break;
		case '{':
			addTok(Type::tok_lbrace);
			break;
		case '}':
			addTok(Type::tok_rbrace);
			break;
		case ';':
			addTok(Type::tok_semicolon);
			break;
		case '\0':
			addTok(Type::tok_eof);
			break;
		case '=':
			addTok(Type::tok_equal);
			break;
		case '+':
			addTok(Type::tok_plus);
			break;
		case '-':
			addTok(Type::tok_minus);
			break;
		case '*':
			addTok(Type::tok_star);
			break;
		case '/':
			addTok(Type::tok_slash);
			break;
		/*default:*/
		/*	// handle identifiers, numbers and others*/
	}
}
