#pragma once

#include <string>
#include <variant>
enum class Type {
	// Types
	tok_int,
	tok_char,
	tok_double,
	tok_float,

	// Keywords
	tok_break,
	tok_case,        
	tok_const,
	tok_continue,
	tok_default,
	tok_do,
	tok_else,
	tok_enum,
	tok_extern,
	tok_for,
	tok_goto,
	tok_if,
	tok_long,
	tok_inline,
	tok_register,
	tok_restrict,
	tok_return,
	tok_short,
	tok_signed,
	tok_sizeof,
	tok_static,
	tok_struct,
	tok_switch,
	tok_typedef,
	tok_union,
	tok_unsigned,
	tok_void,
	tok_volatile,
	tok_while,
	tok_Bool,
	tok_Complex,
	tok_Imaginary,

	// Language Constructs
	tok_identifier,
	tok_eof,
	tok_lparen,
	tok_rparen,
	tok_lbrace,
	tok_rbrace,
	tok_semicolon,

	// Operators
	tok_equal,
	tok_plus,
	tok_minus,
	tok_star,
	tok_hash,
	tok_slash
};

struct Token{
	Type type;
	std::string original;
	std::variant<int,char,std::monostate> literal;
	size_t line;

	Token(Type type, const std::string& original, size_t line): type(type), original(original),literal(std::monostate()), line(line) {};
};
