#pragma once

#include <span>
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
	tok_string, //42
	tok_numeric,
	tok_identifier,
	tok_eof,
	tok_lparen,
	tok_rparen,
	tok_lbrace,
	tok_rbrace,
	tok_semicolon,
	tok_lbracket,
	tok_rbracket,
	tok_comma,
	tok_ellipsis,
	tok_dot,
	tok_and,
	tok_amp,
	tok_or,
	tok_pipe,
	tok_not,
	tok_le,
	tok_lshift,
	tok_lt,
	tok_ge,
	tok_rshift,
	tok_gt,
	tok_assign,
	tok_inc,
	tok_dec,
	tok_arrow,
	tok_percent,
	tok_caret,
	tok_tilde,
	tok_question,
	tok_colon,
	tok_concat,
	tok_eq,
	tok_ne,


	// Operators
	tok_plus,
	tok_minus,
	tok_star,
	tok_hash,
	tok_slash,
	tok_char_literal,
	tok_and_assign,
	tok_or_assign,
	tok_plus_assign,
	tok_minus_assign,
	tok_multiply_assign,
	tok_divide_assign,
	tok_xor_assign,
	tok_modulo_assign
};

struct Token{
	Type type;
	std::span<const char> original;
	std::variant<int,char,float,long double,double, long long,std::monostate> literal;
	size_t line;

	Token(Type type, const std::span<const char> original, size_t line): type(type), original(original),literal(std::monostate()), line(line) {};
	template<typename StringLike, 
		std::enable_if_t<std::is_convertible_v<StringLike, std::string_view>, int> = 0>
			Token(Type type, const StringLike& original, size_t line) 
			: type(type), 
			original(reinterpret_cast<const char*>(original.data()), original.size()), 
			literal(std::monostate()), 
			line(line) {}

	template<size_t N>
		Token(Type type, const char (&original)[N], size_t line) 
		: type(type), 
		original(original, N-1), 
		literal(std::monostate()), 
		line(line) {}
};
