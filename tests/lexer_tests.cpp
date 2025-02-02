#include <catch2/catch_test_macros.hpp>
#include "../src/lexer.h"

void checkToken(const Token& token, Type expectedType, const std::string& expectedOriginal) {
	REQUIRE(token.type == expectedType);
	REQUIRE(std::string(token.original.begin(), token.original.end()) == expectedOriginal);
}

TEST_CASE("Single byte tokens", "[lexer]") {
	SECTION("Basic operators and punctuation") {
		std::string sourceCode = "( ) [ ] { } ? ~ ; : , < > . - + / & | * % ^ ! # =";
		Lexer lexer(sourceCode);

		checkToken(lexer.tokenize(), Type::tok_lparen, "(");
		checkToken(lexer.tokenize(), Type::tok_rparen, ")");
		checkToken(lexer.tokenize(), Type::tok_lbracket, "[");
		checkToken(lexer.tokenize(), Type::tok_rbracket, "]");
		checkToken(lexer.tokenize(), Type::tok_lbrace, "{");
		checkToken(lexer.tokenize(), Type::tok_rbrace, "}");
		checkToken(lexer.tokenize(), Type::tok_question, "?");
		checkToken(lexer.tokenize(), Type::tok_tilde, "~");
		checkToken(lexer.tokenize(), Type::tok_semicolon, ";");
		checkToken(lexer.tokenize(), Type::tok_colon, ":");
		checkToken(lexer.tokenize(), Type::tok_comma, ",");
		checkToken(lexer.tokenize(), Type::tok_lt, "<");
		checkToken(lexer.tokenize(), Type::tok_gt, ">");
		checkToken(lexer.tokenize(), Type::tok_dot, ".");
		checkToken(lexer.tokenize(), Type::tok_minus, "-");
		checkToken(lexer.tokenize(), Type::tok_plus, "+");
		checkToken(lexer.tokenize(), Type::tok_slash, "/");
		checkToken(lexer.tokenize(), Type::tok_amp, "&");
		checkToken(lexer.tokenize(), Type::tok_pipe, "|");
		checkToken(lexer.tokenize(), Type::tok_star, "*");
		checkToken(lexer.tokenize(), Type::tok_percent, "%");
		checkToken(lexer.tokenize(), Type::tok_caret, "^");
		checkToken(lexer.tokenize(), Type::tok_not, "!");
		checkToken(lexer.tokenize(), Type::tok_hash, "#");
		checkToken(lexer.tokenize(), Type::tok_assign, "=");
	}
}

TEST_CASE("Two byte tokens", "[lexer]") {
	SECTION("Compound operators") {
		std::string sourceCode = "-- ++ && || <= >= != == -> >> << += -= *= /= %= ^= &= |=";
		Lexer lexer(sourceCode);

		checkToken(lexer.tokenize(), Type::tok_dec, "--");
		checkToken(lexer.tokenize(), Type::tok_inc, "++");
		checkToken(lexer.tokenize(), Type::tok_and, "&&");
		checkToken(lexer.tokenize(), Type::tok_or, "||");
		checkToken(lexer.tokenize(), Type::tok_le, "<=");
		checkToken(lexer.tokenize(), Type::tok_ge, ">=");
		checkToken(lexer.tokenize(), Type::tok_ne, "!=");
		checkToken(lexer.tokenize(), Type::tok_eq, "==");
		checkToken(lexer.tokenize(), Type::tok_arrow, "->");
		checkToken(lexer.tokenize(), Type::tok_rshift, ">>");
		checkToken(lexer.tokenize(), Type::tok_lshift, "<<");
		checkToken(lexer.tokenize(), Type::tok_plus_assign, "+=");
		checkToken(lexer.tokenize(), Type::tok_minus_assign, "-=");
		checkToken(lexer.tokenize(), Type::tok_multiply_assign, "*=");
		checkToken(lexer.tokenize(), Type::tok_divide_assign, "/=");
		checkToken(lexer.tokenize(), Type::tok_modulo_assign, "%=");
		checkToken(lexer.tokenize(), Type::tok_xor_assign, "^=");
		checkToken(lexer.tokenize(), Type::tok_and_assign, "&=");
		checkToken(lexer.tokenize(), Type::tok_or_assign, "|=");
	}
}
TEST_CASE("Keywords", "[lexer]") {
	SECTION("C keywords") {
		std::string sourceCode = "auto break case char const continue default do double else "
			"enum extern float for goto if inline int long register "
			"restrict return short signed sizeof static struct switch "
			"typedef union unsigned void volatile while _Bool _Complex _Imaginary";
		Lexer lexer(sourceCode);

		checkToken(lexer.tokenize(), Type::tok_auto, "auto");
		checkToken(lexer.tokenize(), Type::tok_break, "break");
		checkToken(lexer.tokenize(), Type::tok_case, "case");
		checkToken(lexer.tokenize(), Type::tok_char, "char");
		checkToken(lexer.tokenize(), Type::tok_const, "const");
		checkToken(lexer.tokenize(), Type::tok_continue, "continue");
		checkToken(lexer.tokenize(), Type::tok_default, "default");
		checkToken(lexer.tokenize(), Type::tok_do, "do");
		checkToken(lexer.tokenize(), Type::tok_double, "double");
		checkToken(lexer.tokenize(), Type::tok_else, "else");
		checkToken(lexer.tokenize(), Type::tok_enum, "enum");
		checkToken(lexer.tokenize(), Type::tok_extern, "extern");
		checkToken(lexer.tokenize(), Type::tok_float, "float");
		checkToken(lexer.tokenize(), Type::tok_for, "for");
		checkToken(lexer.tokenize(), Type::tok_goto, "goto");
		checkToken(lexer.tokenize(), Type::tok_if, "if");
		checkToken(lexer.tokenize(), Type::tok_inline, "inline");
		checkToken(lexer.tokenize(), Type::tok_int, "int");
		checkToken(lexer.tokenize(), Type::tok_long, "long");
		checkToken(lexer.tokenize(), Type::tok_register, "register");
		checkToken(lexer.tokenize(), Type::tok_restrict, "restrict");
		checkToken(lexer.tokenize(), Type::tok_return, "return");
		checkToken(lexer.tokenize(), Type::tok_short, "short");
		checkToken(lexer.tokenize(), Type::tok_signed, "signed");
		checkToken(lexer.tokenize(), Type::tok_sizeof, "sizeof");
		checkToken(lexer.tokenize(), Type::tok_static, "static");
		checkToken(lexer.tokenize(), Type::tok_struct, "struct");
		checkToken(lexer.tokenize(), Type::tok_switch, "switch");
		checkToken(lexer.tokenize(), Type::tok_typedef, "typedef");
		checkToken(lexer.tokenize(), Type::tok_union, "union");
		checkToken(lexer.tokenize(), Type::tok_unsigned, "unsigned");
		checkToken(lexer.tokenize(), Type::tok_void, "void");
		checkToken(lexer.tokenize(), Type::tok_volatile, "volatile");
		checkToken(lexer.tokenize(), Type::tok_while, "while");
		checkToken(lexer.tokenize(), Type::tok_bool, "_Bool");
		checkToken(lexer.tokenize(), Type::tok_complex, "_Complex");
		checkToken(lexer.tokenize(), Type::tok_imaginary, "_Imaginary");
	}
}

TEST_CASE("Identifiers", "[lexer]") {
	SECTION("Various identifier formats") {
		std::string sourceCode = "__identifier UPPERCASE lower_case mixed123Case";
		Lexer lexer(sourceCode);

		checkToken(lexer.tokenize(), Type::tok_identifier, "__identifier");
		checkToken(lexer.tokenize(), Type::tok_identifier, "UPPERCASE");
		checkToken(lexer.tokenize(), Type::tok_identifier, "lower_case");
		checkToken(lexer.tokenize(), Type::tok_identifier, "mixed123Case");
	}
}

TEST_CASE("Numeric literals", "[lexer]") {
	SECTION("Integer literals") {
		std::string sourceCode = "42 0x2A 052 123uL 0xFFLL";
		Lexer lexer(sourceCode);

		checkToken(lexer.tokenize(), Type::tok_numeric, "42");
		checkToken(lexer.tokenize(), Type::tok_numeric, "2A");
		checkToken(lexer.tokenize(), Type::tok_numeric, "052");
		checkToken(lexer.tokenize(), Type::tok_numeric, "123");
		checkToken(lexer.tokenize(), Type::tok_numeric, "FF");
	}

	SECTION("Float literals") {
		std::string sourceCode = "1.1 .1 1. 1.1e1 .1e1 1.e1 1.1e+1 .1e+1 1.e+1 1e1 1e+1 1.1L .1L 1.L 1.1e1L .1e1L 1.e1L 1.1e+1L .1e+1L 1.e+1L 1e1L 1e+1L 1.1f .1f 1.f 1.1e1f .1e1f 1.e1f 1.1e+1f .1e+1f 1.e+1f 1e1f 1e+1f";
		Lexer lexer(sourceCode);

		checkToken(lexer.tokenize(), Type::tok_numeric, "1.1");
		checkToken(lexer.tokenize(), Type::tok_numeric, ".1");
		checkToken(lexer.tokenize(), Type::tok_numeric, "1.");
		checkToken(lexer.tokenize(), Type::tok_numeric, "1.1e1");
		checkToken(lexer.tokenize(), Type::tok_numeric, ".1e1");
		checkToken(lexer.tokenize(), Type::tok_numeric, "1.e1");
		checkToken(lexer.tokenize(), Type::tok_numeric, "1.1e+1");
		checkToken(lexer.tokenize(), Type::tok_numeric, ".1e+1");
		checkToken(lexer.tokenize(), Type::tok_numeric, "1.e+1");
		checkToken(lexer.tokenize(), Type::tok_numeric, "1e1");
		checkToken(lexer.tokenize(), Type::tok_numeric, "1e+1");
		checkToken(lexer.tokenize(), Type::tok_numeric, "1.1");
		checkToken(lexer.tokenize(), Type::tok_numeric, ".1");
		checkToken(lexer.tokenize(), Type::tok_numeric, "1.");
		checkToken(lexer.tokenize(), Type::tok_numeric, "1.1e1");
		checkToken(lexer.tokenize(), Type::tok_numeric, ".1e1");
		checkToken(lexer.tokenize(), Type::tok_numeric, "1.e1");
		checkToken(lexer.tokenize(), Type::tok_numeric, "1.1e+1");
		checkToken(lexer.tokenize(), Type::tok_numeric, ".1e+1");
		checkToken(lexer.tokenize(), Type::tok_numeric, "1.e+1");
		checkToken(lexer.tokenize(), Type::tok_numeric, "1e1");
		checkToken(lexer.tokenize(), Type::tok_numeric, "1e+1");
		checkToken(lexer.tokenize(), Type::tok_numeric, "1.1");
		checkToken(lexer.tokenize(), Type::tok_numeric, ".1");
		checkToken(lexer.tokenize(), Type::tok_numeric, "1.");
		checkToken(lexer.tokenize(), Type::tok_numeric, "1.1e1");
		checkToken(lexer.tokenize(), Type::tok_numeric, ".1e1");
		checkToken(lexer.tokenize(), Type::tok_numeric, "1.e1");
		checkToken(lexer.tokenize(), Type::tok_numeric, "1.1e+1");
		checkToken(lexer.tokenize(), Type::tok_numeric, ".1e+1");
		checkToken(lexer.tokenize(), Type::tok_numeric, "1.e+1");
		checkToken(lexer.tokenize(), Type::tok_numeric, "1e1");
		checkToken(lexer.tokenize(), Type::tok_numeric, "1e+1");
	}
}

TEST_CASE("String literals", "[lexer]") {
	SECTION("Basic strings") {
		std::string sourceCode = R"("Simple string")";
		Lexer lexer(sourceCode);

		checkToken(lexer.tokenize(), Type::tok_string, "Simple string");
	}
}

TEST_CASE("Character literals", "[lexer]") {
	SECTION("Basic characters") {
		std::string sourceCode = R"('a' '\n' '\t' '\'' '\\')";
		Lexer lexer(sourceCode);

		checkToken(lexer.tokenize(), Type::tok_char_literal, "a");
		checkToken(lexer.tokenize(), Type::tok_char_literal, "\\n");
		checkToken(lexer.tokenize(), Type::tok_char_literal, "\\t");
		checkToken(lexer.tokenize(), Type::tok_char_literal, "\\'");
		checkToken(lexer.tokenize(), Type::tok_char_literal, "\\\\");
	}
}
