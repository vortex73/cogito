// This file defines the Lexer for the Cogito C Compiler.

#pragma once
#include "token.h"
#include <cstddef>
#include <iterator>
#include <string>
#include <string_view>
#include <vector>

class Lexer {
	public:
	explicit Lexer(const std::string_view source): source(source) {};
	std::vector<Token> tokenize();

	private:
		std::string_view source;
		size_t start = 0;
		size_t current = 0;
		size_t line = 1;

		// helpers
		bool const isEnd();
		char proceed();
		char const peek();
		char const peekNext();
		bool match(char c);

		void addTok(Type type);
		void addTok(Type type, int i);
		void addTok(Type type, char c);

		void scanTok();
		void handleNumber();
		void handleIdentifier();
		void ignoreWhitespace();

		bool isDigit(char c);
		bool isAlpha(char c);
		bool isAlphaNumeric(char c);
};

