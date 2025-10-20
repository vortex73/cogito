#include "lexer.h"
#include <chrono>
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

std::unordered_map<Type, std::string> typeToString = {

	{Type::tok_unidentified, "tok_unidentified"},


	{Type::tok_string,"tok_string"},
	{Type::tok_percent,"tok_percent"},
	{Type::tok_dot,"tok_dot"},
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


	{Type::tok_numeric, "tok_numeric"},
	{Type::tok_identifier, "tok_identifier"},
	{Type::tok_eof, "tok_eof"},
	{Type::tok_lparen, "tok_lparen"},
	{Type::tok_rparen, "tok_rparen"},
	{Type::tok_lbrace, "tok_lbrace"},
	{Type::tok_rbrace, "tok_rbrace"},
	{Type::tok_semicolon, "tok_semicolon"},


	{Type::tok_assign, "tok_assign"},
	{Type::tok_plus, "tok_plus"},
	{Type::tok_minus, "tok_minus"},
	{Type::tok_star, "tok_star"},
	{Type::tok_hash, "tok_hash"},
	{Type::tok_slash, "tok_slash"},
	{Type::tok_comma, "tok_comma"},
	{Type::tok_assign, "tok_assign"},
	{Type::tok_arrow, "tok_arrow"}};


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
			nullptr,
			sb.st_size,
			PROT_READ,
			MAP_PRIVATE |
			MAP_POPULATE,
			fd,
			0
			);
	close(fd);

	if (mapped == MAP_FAILED) {
		std::cerr << "Memory mapping failed: " << std::endl;
		return std::nullopt;
	}


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
	size_t tokenCount = 0;
	auto start = std::chrono::high_resolution_clock::now();
	auto tokens = lexer.tokenize();



	while (tokens.type!=Type::tok_eof) {
		tokenCount++;
		/*std::cout << '[' << typeToStringLookup(tokens.type)<<  ']' <<  "->" << '[' << std::string(tokens.original.begin(),tokens.original.end())<< ']' << std::endl;*/
		tokens = lexer.tokenize();
	}
	auto end = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

	double tokensPerMicrosecond = static_cast<double>(tokenCount) / duration.count();
	double tokensPerSecond = tokensPerMicrosecond * 1'000'000;

	std::cout << "Tokens: " << tokenCount << std::endl;
	std::cout << "Time: " << duration.count() << " microseconds" << std::endl;
	std::cout << "Speed: " << std::fixed << std::setprecision(2)
		<< tokensPerMicrosecond << " tokens/microsecond ("
		<< tokensPerSecond << " tokens/second)" << std::endl;


		munmap(mapped, sb.st_size);
		close(fd);

	return 0;
}

