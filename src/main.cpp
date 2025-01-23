#include "lexer.h"
#include <iostream>
#include <fstream>
#include <string>

#include <unordered_map>
#include <string>

using std::ifstream;

std::unordered_map<Type, std::string> typeToString = {
    // Types
    {Type::tok_unidentified, "tok_unidentified"},

    // Keywords
    {Type::tok_int, "tok_int"},
    {Type::tok_char, "tok_char"},
    {Type::tok_double, "tok_double"},
    {Type::tok_float, "tok_float"},
    {Type::tok_auto, "tok_auto"},
    {Type::tok_enum, "tok_enum"},
    {Type::tok_restrict, "tok_restrict"},
    {Type::tok_unsigned, "tok_unsigned"},
    {Type::tok_break, "tok_break"},
    {Type::tok_extern, "tok_extern"},
    {Type::tok_return, "tok_return"},
    {Type::tok_case, "tok_case"},
    {Type::tok_signed, "tok_signed"},
    {Type::tok_for, "tok_for"},
    {Type::tok_while, "tok_while"},
    {Type::tok_const, "tok_const"},
    {Type::tok_volatile, "tok_volatile"},
    {Type::tok_short, "tok_short"},
    {Type::tok_goto, "tok_goto"},
    {Type::tok_sizeof, "tok_sizeof"},
    {Type::tok_bool, "tok_bool"},
    {Type::tok_default, "tok_default"},
    {Type::tok_inline, "tok_inline"},
    {Type::tok_struct, "tok_struct"},
    {Type::tok_imaginary, "tok_imaginary"},
    {Type::tok_do, "tok_do"},
    {Type::tok_switch, "tok_switch"},
    {Type::tok_complex, "tok_complex"},
    {Type::tok_if, "tok_if"},
    {Type::tok_static, "tok_static"},
    {Type::tok_continue, "tok_continue"},
    {Type::tok_long, "tok_long"},
    {Type::tok_typedef, "tok_typedef"},
    {Type::tok_else, "tok_else"},
    {Type::tok_register, "tok_register"},
    {Type::tok_union, "tok_union"},
    {Type::tok_void, "tok_void"},
    {Type::tok_Bool, "tok_Bool"},
    {Type::tok_Complex, "tok_Complex"},
    {Type::tok_Imaginary, "tok_Imaginary"},

    // Language Constructs
    {Type::tok_numeric, "tok_numeric"},
    {Type::tok_identifier, "tok_identifier"},
    {Type::tok_eof, "tok_eof"},
    {Type::tok_lparen, "tok_lparen"},
    {Type::tok_rparen, "tok_rparen"},
    {Type::tok_lbrace, "tok_lbrace"},
    {Type::tok_rbrace, "tok_rbrace"},
    {Type::tok_semicolon, "tok_semicolon"},

    // Operators
    {Type::tok_equal, "tok_equal"},
    {Type::tok_plus, "tok_plus"},
    {Type::tok_minus, "tok_minus"},
    {Type::tok_star, "tok_star"},
    {Type::tok_hash, "tok_hash"},
    {Type::tok_slash, "tok_slash"},
    {Type::tok_comma, "tok_comma"},
    {Type::tok_assign, "tok_assign"},
    {Type::tok_arrow, "tok_arrow"}};

// Usage example
std::string typeToStringLookup(Type type) {
	auto it = typeToString.find(type);
	if (it != typeToString.end()) {
		return it->second;
	}
	return "unknown";
}



int main() {
	std::string sourceCode;
	std::ifstream inputFile("./cogito_sample.c");
	if (inputFile.is_open()) {
		inputFile.seekg(0, std::ios::end);
		sourceCode.reserve(inputFile.tellg());
		inputFile.seekg(0, std::ios::beg);

		sourceCode.assign((std::istreambuf_iterator<char>(inputFile)),
				std::istreambuf_iterator<char>());
		inputFile.close();
	} else {
		std::cerr << "Unable to open file" << std::endl;
	}
	Lexer lexer(sourceCode);
	lexer.initKeywords();

	// Tokenize the input source code
	auto tokens = lexer.tokenize();

	// Print the tokens
	for (const auto& token : tokens) {
		std::cout <<'[' <<token.original << ']'<< "->" << typeToStringLookup(token.type)<< std::endl; }

	return 0;
}

