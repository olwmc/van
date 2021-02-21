#include "AST.h"
#include "context.h"
#include "lexer.h"

#include <fstream>
#include <iostream>

int main() {
    std::ifstream infile("examples/forloop.van");
    std::string file_contents { std::istreambuf_iterator<char>(infile), std::istreambuf_iterator<char>() };

    Lexer lexer(file_contents);

    std::vector<Token> tokens = { lexer.getNextToken() };

    while(tokens.back().type() != Token_Type::END_FILE) {
        tokens.push_back(lexer.getNextToken());
    }

    for(auto t: tokens) {
        std::cout << t.toString() << "\n";
    }
}

/*
    Parser(context*, Block*, {option1, option2});
*/