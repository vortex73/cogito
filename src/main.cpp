#include "lexer.h"
#include <cerrno>
#include <iostream>
#include <fstream>
#include <optional>
#include <string>
#include <string_view>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <unordered_map>
#include <string>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
using std::ifstream;

#define TOKEN_TYPE_STR(x) #x

#define TOKEN_TYPE_STR(x) #x

#define STRINGIFY_TOKEN_ENUM(type) \
    ([]() { \
        switch(type) { \
            case Type::tok_int: return "int"; \
            case Type::tok_char: return "char"; \
            case Type::tok_double: return "double"; \
            case Type::tok_float: return "float"; \
            case Type::tok_auto: return "auto"; \
            case Type::tok_enum: return "enum"; \
            case Type::tok_restrict: return "restrict"; \
            case Type::tok_unsigned: return "unsigned"; \
            case Type::tok_break: return "break"; \
            case Type::tok_extern: return "extern"; \
            case Type::tok_return: return "return"; \
            case Type::tok_case: return "case"; \
            case Type::tok_signed: return "signed"; \
            case Type::tok_for: return "for"; \
            case Type::tok_while: return "while"; \
            case Type::tok_const: return "const"; \
            case Type::tok_volatile: return "volatile"; \
            case Type::tok_short: return "short"; \
            case Type::tok_goto: return "goto"; \
            case Type::tok_sizeof: return "sizeof"; \
            case Type::tok_bool: return "bool"; \
            case Type::tok_default: return "default"; \
            case Type::tok_inline: return "inline"; \
            case Type::tok_struct: return "struct"; \
            case Type::tok_imaginary: return "imaginary"; \
            case Type::tok_do: return "do"; \
            case Type::tok_switch: return "switch"; \
            case Type::tok_complex: return "complex"; \
            case Type::tok_if: return "if"; \
            case Type::tok_static: return "static"; \
            case Type::tok_continue: return "continue"; \
            case Type::tok_long: return "long"; \
            case Type::tok_typedef: return "typedef"; \
            case Type::tok_else: return "else"; \
            case Type::tok_register: return "register"; \
            case Type::tok_union: return "union"; \
            case Type::tok_void: return "void"; \
            case Type::tok_Bool: return "Bool"; \
            case Type::tok_Complex: return "Complex"; \
            case Type::tok_Imaginary: return "Imaginary"; \
            case Type::tok_string: return "string"; \
            case Type::tok_numeric: return "numeric"; \
            case Type::tok_identifier: return "identifier"; \
            case Type::tok_eof: return "eof"; \
            case Type::tok_lparen: return "("; \
            case Type::tok_rparen: return ")"; \
            case Type::tok_lbrace: return "{"; \
            case Type::tok_rbrace: return "}"; \
            case Type::tok_semicolon: return ";"; \
            case Type::tok_lbracket: return "["; \
            case Type::tok_rbracket: return "]"; \
            case Type::tok_comma: return ","; \
            case Type::tok_ellipsis: return "..."; \
            case Type::tok_dot: return "."; \
            case Type::tok_and: return "&&"; \
            case Type::tok_amp: return "&"; \
            case Type::tok_or: return "||"; \
            case Type::tok_pipe: return "|"; \
            case Type::tok_not: return "!"; \
            case Type::tok_le: return "<="; \
            case Type::tok_lshift: return "<<"; \
            case Type::tok_lt: return "<"; \
            case Type::tok_ge: return ">="; \
            case Type::tok_rshift: return ">>"; \
            case Type::tok_gt: return ">"; \
            case Type::tok_assign: return "="; \
            case Type::tok_inc: return "++"; \
            case Type::tok_dec: return "--"; \
            case Type::tok_arrow: return "->"; \
            case Type::tok_percent: return "%"; \
            case Type::tok_caret: return "^"; \
            case Type::tok_tilde: return "~"; \
            case Type::tok_question: return "?"; \
            case Type::tok_colon: return ":"; \
            case Type::tok_concat: return "##"; \
            case Type::tok_eq: return "=="; \
            case Type::tok_ne: return "!="; \
            case Type::tok_plus: return "+"; \
            case Type::tok_minus: return "-"; \
            case Type::tok_star: return "*"; \
            case Type::tok_hash: return "#"; \
            case Type::tok_slash: return "/"; \
            default: return "unknown"; \
        } \
    }())
std::unordered_map<Type, std::string> typeToString = {
	// Types
	{Type::tok_unidentified, "tok_unidentified"},

	// Keywords
	{Type::tok_string,"tok_string"},
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
	{Type::tok_assign, "tok_assign"},
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


std::optional<std::string_view> mmapFile(const std::string& file) {
	int fd = open(file.c_str(), O_RDONLY);
	if (fd == -1) {
		std::cerr << "Error opening file" << std::endl;
		return std::nullopt;
	}
	struct stat sb;
	if (fstat(fd, &sb) == -1) {
		std::cerr << "Error getting file stats: "<< std::endl;
		close(fd);
		return std::nullopt;
	}
	void* mapped = mmap(
			nullptr,           // Let OS choose mapping address
			sb.st_size, 
			PROT_READ,         // Read-only protection
			MAP_PRIVATE |      // Private copy-on-write mapping
			MAP_POPULATE,      // Prefault pages for better performance
			fd, 
			0
			);
	close(fd);

	if (mapped == MAP_FAILED) {
		std::cerr << "Memory mapping failed: " << std::endl;
		return std::nullopt;
	}

	// Use madvise for performance hints
	madvise(mapped, sb.st_size, MADV_SEQUENTIAL | MADV_WILLNEED);

	return std::string_view(static_cast<const char*>(mapped), sb.st_size);
}

int main() {
	int fd = open("./cogito_sample.c", O_RDONLY);
	if (fd == -1) {
		std::cerr << "Unable to open file" << std::endl;
		return 1;
	}

	struct stat sb;
	if (fstat(fd, &sb) == -1) {
		std::cerr << "Unable to get file size" << std::endl;
		close(fd);
		return 1;
	}

	char* mapped = static_cast<char*>(mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0));
	if (mapped == MAP_FAILED) {
		std::cerr << "Memory mapping failed" << std::endl;
		close(fd);
		return 1;
	}

	std::string_view sourceCode(mapped, sb.st_size);
	Lexer lexer(sourceCode);
	auto tokens = lexer.tokenize();


	// Print the tokens
	for (const auto& token : tokens) {
		std::cout << '[' << std::string(token.original.begin(),token.original.end()) << ']' << "->" << STRINGIFY_TOKEN_ENUM(token.type)<< std::endl; }
		// Clean up
		munmap(mapped, sb.st_size);
		close(fd);

	return 0;
}

