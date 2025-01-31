#include "lexer.h"
#include <sstream>
#include "token.h"
#include <cctype>
#include <cstdio>
#include <iostream>
#include <ostream>
#include <stdexcept>
#include <immintrin.h>
#include <string>

#ifdef __GNUC__
#define LIKELY(x)    __builtin_expect((x),1)
#define UNLIKELY(x)  __builtin_expect((x),0)
#else
#define LIKELY(x)    (x)
#define UNLIKELY(x)  (x)
#endif

std::vector<Token> Lexer::tokenize() {
	std::vector<Token> tokenList;
	tokenList.reserve(source.length()/4 + 16);
	while (LIKELY(current < source.length())) {
		start = current;
		scanTok(&tokenList);
	}
	tokenList.emplace_back(Type::tok_eof, "", line);
	return tokenList;
}

void Lexer::ignoreWhitespace() {
	while (!isEnd()) {
		char c = peek();
		if (c == ' ' || c == '\t' || c == '\n') {
			if (c == '\n') {
				line++;  
			}
			proceed();  
			start = current;
		} else {
			break;  
		}
	}
}

bool const Lexer::isEnd() {
	return current >= source.length();
}

char Lexer::proceed() {
	__builtin_prefetch(&source[current + 1],0,0);
	return source[current++];
}

char const Lexer::peek() {
	return UNLIKELY(isEnd() ? '\0' : source[current]);
}

char const Lexer::peekNext() {
	return (current+1 < source.length()) ? source[current+1] : '\0';
}

void Lexer::string(std::vector<Token>* tokenList) {
	std::string stringValue;
	/*bool escape = false;*/

	while (!isEnd()) {
		char current_char = peek();


		if (current_char == '"') {
			proceed(); 
			break;
		}


		/*if (escape) {*/
		/*	switch (current_char) {*/
		/*		case 'n': stringValue += '\n'; break;*/
		/*		case 't': stringValue += '\t'; break;*/
		/*		case 'r': stringValue += '\r'; break;*/
		/*		case '\\': stringValue += '\\'; break;*/
		/*		case '"': stringValue += '"'; break;*/
		/*		case '0': stringValue += '\0'; break;*/
		/*		default: */
		/*				  stringValue += '\\';*/
		/*				  stringValue += current_char;*/
		/*				  break;*/
		/*	}*/
		/*	escape = false;*/
		/*} else if (current_char == '\\') {*/
		/*	escape = true;*/
		/*} else {*/
		/*	stringValue += current_char;*/
		/*}*/
		stringValue += current_char;

		proceed();
	}

	if (isEnd()) {
		throw std::runtime_error("Unterminated string literal");
	}


	tokenList->emplace_back(Type::tok_string, stringValue, line);
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
	while (LIKELY(isalnum(peek())) || peek() == '_') {
		proceed();
	}

	std::string text(source.substr(start, current - start));

	const KeywordEntry* keyword = Perfect_Hash::in_word_set(text.c_str(), text.length());
	Type type = keyword ? keyword->tokenType : Type::tok_identifier;
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
		case '"': 
				  string(tokenList);
				  break;
		case '#':
				  if (match('#')) addTok(Type::tok_concat, tokenList);
				  else addTok(Type::tok_hash, tokenList);
				  break;
	}
}


void Lexer::number(std::vector<Token>* tokenList) {

	start = current-1;
	bool isHex = false;
	bool isOctal = false;
	bool isFloat = false;
	std::string numStr;
	int base = 10;
	std::string fullLiteral(source.substr(start, current - start));
	Token token(Type::tok_numeric, fullLiteral, line);

	if (source[current-1] == '0' && (peek() == 'x' || peek() == 'X')) {
		isHex = true;
		proceed(); 

		
		while (LIKELY(isxdigit(peek()))) {
			proceed();
		}

		
		if (peek() == '.') {
			isFloat = true;
			proceed();
			while (LIKELY(isxdigit(peek()))) {
				proceed();
			}
		}

		
		if (peek() == 'p' || peek() == 'P') {
			isFloat = true;
			proceed();
			if (peek() == '+' || peek() == '-') proceed();
			if (!isdigit(peek())) {
				throw std::runtime_error("Expected decimal digits after hex float exponent");
			}
			while (isdigit(peek())) {
				proceed();
			}
		} else if (isFloat) {
			throw std::runtime_error("Hex float requires 'p' or 'P' exponent");
		}
	} 
	else if (source[current-1] == '0' && isdigit(peek())) {
		isOctal = true;


		while (LIKELY(peek() >= '0' && peek() <= '7')) {
			proceed();
		}
	}

	else {
		while (LIKELY(isdigit(peek()))) {
			proceed();
		}
		if (peek() == '.') {
			isFloat = true;
			proceed();
			while (isdigit(peek())) {
				proceed();
			}
		}

		
		if (peek() == 'e' || peek() == 'E') {
			isFloat = true;
			proceed();
			if (peek() == '+' || peek() == '-') proceed();
			if (!isdigit(peek())) {
				throw std::runtime_error("Expected decimal digits after float exponent");
			}
			while (isdigit(peek())) {
				proceed();
			}
		}
	}

	std::string suffix;
	while (isalpha(peek()) && peek() != '\n') {
		suffix += std::tolower(proceed());
	}


	if (isHex) {
		numStr = source.substr(start + 2, current - start - 2 - suffix.length());
	} else if (isOctal) {
		numStr = source.substr(start, current - start - suffix.length());
	} else {
		numStr = source.substr(start, current - start - suffix.length());
	}
	token.original = numStr;


	try {
		if (isFloat) {
			std::istringstream iss(numStr);
			double value;

			iss >> std::hexfloat >> value;

			if (suffix == "f") {
				token.literal = static_cast<float>(value);
			} else if (suffix == "l") {
				token.literal = static_cast<float>(static_cast<long double>(value));
			} else {
				token.literal = static_cast<float>(value);
			}
		} else {
			
			int base = isHex ? 16 : (isOctal ? 8 : 10);

			
			/*if (!suffix.empty()) {*/
			/*	if (suffix.length() > 3) {*/
			/*		throw std::runtime_error("Invalid suffix length: " + suffix);*/
			/*	}*/
			/**/
			/*	
			/*	bool hasU = suffix.find('u') != std::string::npos;*/
			/*	int lCount = std::__count_if(suffix.begin(), suffix.end(), [](char c) {return c == 'l';});*/
			/**/
			/*	if (lCount > 2 || (hasU && suffix[0] != 'u' && suffix.back() != 'u')) {*/
			/*		throw std::runtime_error("Invalid suffix combination: " + suffix);*/
			/*	}*/
			/*}*/

			
			try {
				if (suffix.empty()) {
					token.literal = static_cast<int>(std::stol(numStr, nullptr, base));
				} else if (suffix == "u") {
					token.literal = static_cast<int>(std::stoul(numStr, nullptr, base));
				} else if (suffix == "l") {
					token.literal = static_cast<int>(std::stol(numStr, nullptr, base));
				} else if (suffix == "ul" || suffix == "lu") {
					token.literal = static_cast<int>(std::stoul(numStr, nullptr, base));
				} else if (suffix == "ll") {
					token.literal = static_cast<int>(std::stoll(numStr, nullptr, base));
				} else if (suffix == "ull" || suffix == "llu") {
					token.literal = static_cast<int>(std::stoull(numStr, nullptr, base));
				} else {
					throw std::runtime_error("Invalid numeric suffix: " + suffix);
				}
			} catch (const std::out_of_range& e) {
				std::cerr << "Numeric literal out of range: " << fullLiteral 
					<< " (Base " << base << ")" << std::endl;

				
				try {
					unsigned long long largeVal = std::stoull(numStr, nullptr, base);
					token.literal = static_cast<int>(largeVal & 0xFFFFFFFF);
				} catch (...) {
					throw std::runtime_error("Failed to parse extremely large numeric literal");
				}
			}
		}
	} catch (const std::exception& e) {
		std::cerr << "Error processing numeric literal: " << e.what() 
			<< " (Literal: " << fullLiteral << ")" << std::endl;
		throw;
	}

	tokenList->emplace_back(token);
}
void Lexer::scanTok(std::vector<Token>* tokenList) {
	if (isEnd()) return; 
	ignoreWhitespace();
	char c = proceed();
	if (c=='\n') proceed();
	if (isdigit(c)||(c=='.' && isdigit(peekNext()))) {
		number(tokenList);
	} else if (isalpha(c) || c == '_') {
		identifier(tokenList);
	} else {
		scanPunc(tokenList,c);
	}
}
