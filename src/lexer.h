// This file defines the Lexer for the Cogito C Compiler.

#pragma once
#include "keywords_hash.h"
#include "token.h"
#include <cstddef>
#include <string_view>
/*#include <vector>*/
#include <span>

class Lexer {
	public:
	explicit Lexer(const std::string_view source): source(source) {};
	Token tokenize();

	private:
		std::span<const char> source;
		/*std::vector<Token> tokenList;*/
		size_t start = 0;
		size_t current = 0;
		size_t line = 1;

		// helpers
		bool const isEnd();
		char proceed();
		char const peek();
		char const peekNext();
		bool match(char c);

		Token character();
		Token string();
		Token identifier();
		Token number();
		Token scanPunc();
		void handleNumber();
		void handleIdentifier();
		void ignoreWhitespace();

		bool isDigit(char c);
		bool isAlpha(char c);
		bool isAlphaNumeric(char c);
};

