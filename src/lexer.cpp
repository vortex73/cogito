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



Token Lexer::tokenize() {
	Token token = Token(Type::tok_eof, std::span<const char>{},line);
	while(!isEnd()) {
		start = current;
		ignoreWhitespace();
		char c = proceed();
		if (c=='\n') proceed();
		if (isdigit(c)||(c=='.' && isdigit(peek()))) {
			token = number();
			return token;
		} else if (isalpha(c) || c == '_') {
			token = identifier();
			return token;
		} else {
			token = scanPunc();
			return token;
		}
		return Token(Type::tok_unidentified,source.subspan(start,current-start),line);
	}
	return token;
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
	return current >= source.size();
}

char Lexer::proceed() {
	__builtin_prefetch(&source[current + 1],0,0);
	return source[current++];
}

char const Lexer::peek() {
	return UNLIKELY(isEnd() ? '\0' : source[current]);
}

char const Lexer::peekNext() {
	return (current+1 < source.size()) ? source[current+1] : '\0';
}

Token Lexer::character() {
	if (isEnd()) throw std::runtime_error("Unterminated character literal");

	size_t charStart = current;
	char c = proceed();

	if (c == '\\') {
		proceed();
	}

	if (proceed() != '\'') throw std::runtime_error("Expected closing single quote");

	return Token(Type::tok_char_literal, source.subspan(charStart, current - charStart-1), line);
}


Token Lexer::string() {
	size_t stringBegin = current;
	bool escape = false;

	while (!isEnd()) {
		char current_char = peek();
		if (current_char == '\\' && !escape) { // Detect escape character
			escape = true;
			proceed();
			continue;
		}

		if (current_char == '"' && !escape) {
			proceed(); 
			break;
		}
		escape = false;

		proceed();
	}

	if (isEnd()) {
		throw std::runtime_error("Unterminated string literal");
	}

	std::span<const char> stringSpan = source.subspan(stringBegin,current-stringBegin-1);
	return Token(Type::tok_string,stringSpan,line);
}

bool Lexer::match(char c) {
	if (isEnd() || source[current]!=c) return false;
	current++;
	return true;
}

Token Lexer::identifier() {
	while (LIKELY(isalnum(peek())) || peek() == '_') {
		proceed();
	}

    std::span text(source.data() + start, current - start);
	const char* text_start = source.data() + start;


	const KeywordEntry* keyword = Perfect_Hash::in_word_set(text);
	Type type = keyword ? keyword->tokenType : Type::tok_identifier;
	return Token(type,text,line);
}

Token Lexer::scanPunc() {
	char c = source[current-1];
	Type t = Type::tok_unidentified;
	switch (c) {
		case '[': t = Type::tok_lbracket;break;
		case ']':t = Type::tok_rbracket ; break; 
		case '(':t = Type::tok_lparen ; break;
		case ')':t = Type::tok_rparen ; break;
		case '{':t = Type::tok_lbrace ; break;
		case '}':t = Type::tok_rbrace ; break;
		case ',':t = Type::tok_comma ; break;
		case ';': t = Type::tok_semicolon ; break;
		case '.':
				  if (match('.') && match('.')) {t = Type::tok_ellipsis;}
				  else {t = Type::tok_dot ;} break;
		case '&':
				  if (match('&')) {t = Type::tok_and;}
				  else if (match('=')) {t = Type::tok_and_assign;break;}
				  else {t = Type::tok_amp;} break;
		case '|':
				  if (match('|')) {t = Type::tok_or;}
				  else if (match('=')) {t = Type::tok_or_assign;break;}
				  else {t = Type::tok_pipe;} break;
		case '!':
				  if (match('=')) {t = Type::tok_ne;}
				  else {t = Type::tok_not;} break;
		case '<':
				  if (match('=')){ t = Type::tok_le;break;}
				  else if (match('<')) {t = Type::tok_lshift;break;}
				  else {t = Type::tok_lt;break;}
		case '>':
				  if (match('=')){ t = Type::tok_ge;break;}
				  else if (match('>')) {t = Type::tok_rshift;break;}
				  else {t = Type::tok_gt;break;}
		case '=':
				  if (match('=')) {t = Type::tok_eq;break;}
				  else {t = Type::tok_assign;break;}
		case '+':
				  if (match('+')) {t = Type::tok_inc;break;}
				  else if (match('=')) {t = Type::tok_plus_assign;break;}
				  else {t = Type::tok_plus;break;}
		case '-':
				  if (match('-')) {t = Type::tok_dec;break;}
				  else if (match('=')) {t = Type::tok_minus_assign;break;}
				  else if (match('>')) {t = Type::tok_arrow;break;}
				  else {t = Type::tok_minus;break;}
		case '*':
				  if (match('=')) {t = Type::tok_multiply_assign;break;}
				  else {t = Type::tok_star;break;}
		case '/':
				  if (match('/')) {
					  while (peek() != '\n' && !isEnd()) proceed();
					  return tokenize();
				  } 
				  else if (match('=')) {t = Type::tok_divide_assign;break;}
				  else if (match('*')) {
					  while (true) {
						  if (isEnd()) throw std::runtime_error("Unterminated block comment");
						  if (match('*') && match('/')) return tokenize();
						  proceed();
					  }
				  } else {
					  t = Type::tok_slash;
				  }
				  break;
		case '%': 
				  if (match('=')) {t = Type::tok_modulo_assign;break;}
				  else {t = Type::tok_percent; break;}
		case '^': 
				  if (match('=')) {t = Type::tok_xor_assign;break;}
				  else {t = Type::tok_caret; break;}
		case '~': t = Type::tok_tilde; break;
		case '?': t = Type::tok_question;break;
		case ':': t = Type::tok_colon;break;
		case '\'':
				  return character();
		case '"': 
				  return string();
		case '#':
				  if (match('#')) {t = Type::tok_concat;break;}
				  else {t = Type::tok_hash;break;}
	}
	return Token(t,source.subspan(start,current-start),line);
}


Token Lexer::number() {
	size_t numStart = current-1;
	bool isHex = false, isOctal = false, isFloat = false;
	int base = 10;
	std::span<const char> fullLiteralSpan = source.subspan(numStart);

	if (source[numStart] == '.') isFloat = 1;
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
		char ch = proceed();
		suffix += (ch == 'F' || ch == 'L' || ch == 'U') ? (ch + 32) : ch;
	}

	std::span<const char> numSpan = source.subspan(
			numStart + (isHex ? 2 : 0), 
			current - numStart - (isHex ? 2 : 0) - suffix.length()
			);
	Token token(Type::tok_numeric, numSpan, line);

	try {
		if (isFloat) {
			std::string numStr(numSpan.begin(), numSpan.end());
			std::istringstream iss(numStr);
			double value;

			iss >> std::hexfloat >> value;
			token.literal = (suffix == "l") ? static_cast<long double>(value) : static_cast<float>(value);

		} else {
			
			int base = isHex ? 16 : (isOctal ? 8 : 10);
			std::string numStr(numSpan.begin(), numSpan.end());
			
			
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
				std::cerr << "Numeric literal out of range: " << std::string(numSpan.begin(), numSpan.end())
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
			<< " (Literal: " << std::string(numSpan.begin(), numSpan.end()) << ")" << std::endl;
		throw;
	}

	return token;
}
