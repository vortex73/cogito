#include "lexer.h"
#include "token.h"
#include <cctype>

std::vector<Token> Lexer::tokenize() {
	std::vector<Token> tokenList;
	while (!isEnd()) {
		start = current;
		scanTok(&tokenList);
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

void Lexer::addTok(Type type,std::vector<Token>* tokenList) {
	std::string text(source.substr(start, current - start));
	tokenList->push_back(Token(type, text, line));
}

void Lexer::identifier(std::vector<Token>* tokenList) {
	while (isalnum(peek())) {
		proceed();
	}

	std::string text(source.substr(start, current - start));

	Type type = Type::tok_identifier;
	auto it = keywords.find(text);
	if (it!=keywords.end()) type = it->second;
	tokenList->emplace_back(type,text,line);


}

void Lexer::number(std::vector<Token>* tokenList) {
	while (isdigit(peek())) {
		proceed();
	}
	std::string numStr(source.substr(start, current - start));
	int value = std::stoi(numStr);
	Token token(Type::tok_numeric, numStr, line);
	token.literal = value;
	tokenList->push_back(token);
}

void Lexer::scanTok(std::vector<Token>* tokenList) {
	char c = proceed();
	switch (c) {
		case '(': 
			addTok(Type::tok_lparen, tokenList);
			break;
		case ')':
			addTok(Type::tok_rparen, tokenList);
			break;
		case '{':
			addTok(Type::tok_lbrace, tokenList);
			break;
		case '}':
			addTok(Type::tok_rbrace, tokenList);
			break;
		case ';':
			addTok(Type::tok_semicolon, tokenList);
			break;
		case '\0':
			addTok(Type::tok_eof, tokenList);
			break;
		case '=':
			addTok(Type::tok_equal, tokenList);
			break;
		case '+':
			addTok(Type::tok_plus, tokenList);
			break;
		case '-':
			addTok(Type::tok_minus, tokenList);
			break;
		case '*':
			addTok(Type::tok_star, tokenList);
			break;
		case '/':
			addTok(Type::tok_slash, tokenList);
			break;
		default:
			if (isdigit(c)) {
				number(tokenList);
			} else if (isalpha(c) || c == '_') {
				identifier(tokenList);
			}
			break;
	}
}
