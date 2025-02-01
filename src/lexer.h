// This file defines the Lexer for the Cogito C Compiler.

#pragma once
#include "keywords_hash.h"
#include "token.h"
#include <cstddef>
#include <string_view>
#include <vector>
#include <span>

class Lexer {
	public:
	explicit Lexer(const std::string_view source): source(source) {};
	std::vector<Token> tokenize();

	private:
		std::span<const char> source;
		std::vector<Token> tokenList;
		size_t start = 0;
		size_t current = 0;
		size_t line = 1;

		// helpers
		bool const isEnd();
		char proceed();
		char const peek();
		char const peekNext();
		bool match(char c);

		void addTok(Type type, std::vector<Token>* tokenList);
		void addTok(Type type, int i, std::vector<Token>* tokenList);
		void addTok(Type type, char c, std::vector<Token>* tokenList);

		void string(std::vector<Token>* tokenList);
		void handleFloats(std::vector<Token>* tokenList, std::string_view whole,Token* token);
		void identifier(std::vector<Token>* tokenList);
		void number(std::vector<Token>* tokenList);
		void scanPunc(std::vector<Token>* tokenList, char c);
		void scanTok(std::vector<Token>* tokenList);
		void handleNumber();
		void handleIdentifier();
		void ignoreWhitespace();

		bool isDigit(char c);
		bool isAlpha(char c);
		bool isAlphaNumeric(char c);
};

