// This file defines the Lexer for the Cogito C Compiler.

#pragma once
#include "token.h"
#include <cstddef>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

class Lexer {
	public:
	explicit Lexer(const std::string_view source): source(source) {};
	std::vector<Token> tokenize();
	void initKeywords() {
		keywords = {
			{"auto",Type::tok_auto},
			{"enum",Type::tok_enum},
			{"restrict",Type::tok_restrict},
			{"unsigned",Type::tok_unsigned},
			{"break",Type::tok_break},
			{"extern",Type::tok_extern},
			{"return",Type::tok_return},
			{"void",Type::tok_void},
			{"case",Type::tok_case},
			{"float",Type::tok_float},
			{"short",Type::tok_short},
			{"volatile",Type::tok_volatile},
			{"char",Type::tok_char},
			{"for",Type::tok_for},
			{"signed",Type::tok_signed},
			{"while",Type::tok_while},
			{"const",Type::tok_const},
			{"goto",Type::tok_goto},
			{"sizeof",Type::tok_sizeof},
			{"_Bool",Type::tok_bool},
			{"default",Type::tok_default},
			{"inline",Type::tok_inline},
			{"struct",Type::tok_struct},
			{"_Imaginary",Type::tok_imaginary},
			{"continue",Type::tok_continue},
			{"if",Type::tok_if},
			{"static",Type::tok_static},
			{"_Complex",Type::tok_complex},
			{"double",Type::tok_double},
			{"long",Type::tok_long},
			{"typedef",Type::tok_typedef},
			{"else",Type::tok_else},
			{"register",Type::tok_register},
			{"union",Type::tok_union},
			{"do",Type::tok_do},
			{"int",Type::tok_int},
			{"switch",Type::tok_switch},

		};
	}

	private:
		std::string_view source;
		std::vector<Token> tokenList;
		std::unordered_map<std::string, Type> keywords;
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

		void identifier(std::vector<Token>* tokenList);
		void number(std::vector<Token>* tokenList);
		void scanTok(std::vector<Token>* tokenList);
		void handleNumber();
		void handleIdentifier();
		void ignoreWhitespace();

		bool isDigit(char c);
		bool isAlpha(char c);
		bool isAlphaNumeric(char c);
};

