#include "lexer.h"
#include "token.h"
#include <cctype>
#include <stdexcept>

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

void Lexer::scanPunc(std::vector<Token>* tokenList,char c) {
	switch (c) {
		case '[': addTok(Type::tok_lbracket, tokenList); break;
		case ']': addTok(Type::tok_rbracket, tokenList); break;
		case '(': addTok(Type::tok_lparen, tokenList); break;
		case ')': addTok(Type::tok_rparen, tokenList); break;
		case '{': addTok(Type::tok_lbrace, tokenList); break;
		case '}': addTok(Type::tok_rbrace, tokenList); break;
		case ',': addTok(Type::tok_comma, tokenList); break;
		case ';': addTok(Type::tok_semicolon, tokenList); break;
		case '.': 
				  if (match('.') && match('.')) addTok(Type::tok_ellipsis, tokenList);
				  else addTok(Type::tok_dot, tokenList);
				  break;
		case '&':
				  if (match('&')) addTok(Type::tok_and, tokenList);
				  else addTok(Type::tok_amp, tokenList);
				  break;
		case '|':
				  if (match('|')) addTok(Type::tok_or, tokenList);
				  else addTok(Type::tok_pipe, tokenList);
				  break;
		case '!':
				  if (match('=')) addTok(Type::tok_ne, tokenList);
				  else addTok(Type::tok_not, tokenList);
				  break;
		case '<':
				  if (match('=')) addTok(Type::tok_le, tokenList);
				  else if (match('<')) addTok(Type::tok_lshift, tokenList);
				  else addTok(Type::tok_lt, tokenList);
				  break;
		case '>':
				  if (match('=')) addTok(Type::tok_ge, tokenList);
				  else if (match('>')) addTok(Type::tok_rshift, tokenList);
				  else addTok(Type::tok_gt, tokenList);
				  break;
		case '=':
				  if (match('=')) addTok(Type::tok_eq, tokenList);
				  else addTok(Type::tok_assign, tokenList);
				  break;
		case '+':
				  if (match('+')) addTok(Type::tok_inc, tokenList);
				  else addTok(Type::tok_plus, tokenList);
				  break;
		case '-':
				  if (match('-')) addTok(Type::tok_dec, tokenList);
				  else if (match('>')) addTok(Type::tok_arrow, tokenList);
				  else addTok(Type::tok_minus, tokenList);
				  break;
		case '*': addTok(Type::tok_star, tokenList); break;
		case '/':
				  if (match('/')) {
					  while (peek() != '\n' && !isEnd()) proceed();
				  } else if (match('*')) {
					  while (true) {
						  if (isEnd()) throw std::runtime_error("Unterminated block comment");
						  if (match('*') && match('/')) break;
						  proceed();
					  }
				  } else {
					  addTok(Type::tok_slash, tokenList);
				  }
				  break;
		case '%': addTok(Type::tok_percent, tokenList); break;
		case '^': addTok(Type::tok_caret, tokenList); break;
		case '~': addTok(Type::tok_tilde, tokenList); break;
		case '?': addTok(Type::tok_question, tokenList); break;
		case ':': addTok(Type::tok_colon, tokenList); break;
		case '#':
				  if (match('#')) addTok(Type::tok_concat, tokenList);
				  else addTok(Type::tok_hash, tokenList);
				  break;
	}
}

void Lexer::number(std::vector<Token>* tokenList) {
	while (isdigit(peek())) {
		proceed();
	}

	std::string suffix;
	while (isalpha(peek())) {
		suffix += proceed();
	}

	std::string numStr(source.substr(start, current - start - suffix.length()));
	Token token(Type::tok_numeric, numStr + suffix, line);

	try {
		if (suffix.empty()) {
			token.literal = std::stoi(numStr);
		}
		else if (suffix == "u" || suffix == "U") {
			token.literal = static_cast<int>(std::stoul(numStr));
		}
		else if (suffix == "l" || suffix == "L") {
			token.literal = static_cast<int>(std::stol(numStr));
		}
		else if (!(suffix == "lL" || suffix == "Ll")) {
			token.literal = static_cast<int>(std::stoul(numStr));
		}
		else {
			throw std::runtime_error("Invalid numeric suffix: " + suffix);
		}
	}
	catch (const std::out_of_range& e) {
		throw std::runtime_error("Number out of range: " + numStr + suffix);
	}
	catch (const std::invalid_argument& e) {
		throw std::runtime_error("Invalid number format: " + numStr + suffix);
	}

	tokenList->push_back(token);
}


void Lexer::scanTok(std::vector<Token>* tokenList) {
	char c = proceed();
	if (isdigit(c)) {
		number(tokenList);
	} else if (isalpha(c) || c == '_') {
		identifier(tokenList);
	} else {
		scanPunc(tokenList,c);
	}
}
