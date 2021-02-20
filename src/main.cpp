#include "AST.h"
#include "context.h"
#include "lexer.h"

#include <fstream>
#include <iostream>

int main() {
    std::ifstream infile("examples/forloop.van");
    std::string file_contents { std::istreambuf_iterator<char>(infile), std::istreambuf_iterator<char>() };

    Lexer lexer(file_contents);

    Token t = lexer.getNextToken();
    
    while(t.type() != Token_type::END_FILE) {
        std::cout << t.toString() << "\n";
        t = lexer.getNextToken();
    }
}