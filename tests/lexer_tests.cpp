#define CATCH_CONFIG_MAIN  // This generates the main() function for Catch2
#include <catch2/catch_all.hpp>
#include "../src/lexer.h"


TEST_CASE("Lexer handles single-character tokens", "[lexer]") {
    Lexer lexer("(");
    auto tokens = lexer.tokenize();
    INFO("Number of tokens: " << tokens.size());
    INFO("Token type value: " << static_cast<int>(tokens[0].type));
    REQUIRE(tokens.size() == 2);  // '(' + EOF
    REQUIRE(tokens[0].type == Type::tok_lparen);
    REQUIRE(tokens[1].type == Type::tok_eof);  // Check for EOF token
}

TEST_CASE("Lexer handles numbers", "[lexer]") {
    Lexer lexer("12345");
    auto tokens = lexer.tokenize();
    INFO("Number of tokens: " << tokens.size());
    REQUIRE(tokens.size() == 2);  // Number + EOF
    REQUIRE(tokens[0].type == Type::tok_int);
    REQUIRE(tokens[1].type == Type::tok_eof);
    REQUIRE(std::get<int>(tokens[0].literal) == 12345);
}
