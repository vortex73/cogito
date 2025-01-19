#include "lexer.h"
#include <iostream>

#include <string>

// Assuming Type is an enum
std::string typeToString(Type type) {
    switch (type) {
        case Type::tok_lparen: return "tok_lparen";
        case Type::tok_rparen: return "tok_rparen";
        case Type::tok_lbrace: return "tok_lbrace";
        case Type::tok_rbrace: return "tok_rbrace";
        case Type::tok_semicolon: return "tok_semicolon";
        case Type::tok_eof: return "tok_eof";
        case Type::tok_equal: return "tok_equal";
        case Type::tok_plus: return "tok_plus";
        case Type::tok_minus: return "tok_minus";
        case Type::tok_star: return "tok_star";
        case Type::tok_slash: return "tok_slash";
        default: return "unknown";
    }
}


int main() {
    std::string sourceCode = "({})+";
    Lexer lexer(sourceCode);

    // Tokenize the input source code
    auto tokens = lexer.tokenize();

    // Print the tokens
    for (const auto& token : tokens) {
        std::cout << "Token: " << typeToString(token.type) << " Literal: " << "\n"; }

    return 0;
}

