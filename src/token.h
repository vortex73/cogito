#pragma once

#include <string>
#include <variant>
enum class Type {
	// Types
	tok_unidentified,   // For testing purposes

	// Keywords
	tok_int,
	tok_char,
	tok_double,
	tok_float,
	tok_auto,
	tok_enum,
	tok_restrict,
	tok_unsigned,
	tok_break,
	tok_extern,
	tok_return,
	tok_case,
	tok_signed,
	tok_for,
	tok_while,
	tok_const,
	tok_volatile,
	tok_short,
	tok_goto,
	tok_sizeof,
	tok_bool,
	tok_default,
	tok_inline,
	tok_struct,
	tok_imaginary,
	tok_do,
	tok_switch,
	tok_complex,
	tok_if,
	tok_static,
	tok_continue,
	tok_long,
	tok_typedef,
	tok_else,
	tok_register,
	tok_union,
	tok_void,
	tok_Bool,
	tok_Complex,
	tok_Imaginary,

	// Language Constructs
	tok_numeric,
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
