#include "interpreter.h"

#include <fstream>
#include <iostream>
#include <cmath>

int main() {
    std::ifstream infile("examples/regression.van");
    std::string file_contents { std::istreambuf_iterator<char>(infile), std::istreambuf_iterator<char>() };
    
    Van_Interpreter interpreter(file_contents);

    try {
        interpreter.interpret();
    }
    catch(const std::runtime_error& error) {
        std::cout << error.what() << "\n";
    }
}