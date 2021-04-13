#include "interpreter.h"

#include <fstream>
#include <iostream>
#include <cmath>

int main() {
    std::ifstream infile("examples/fib.van");
    std::string file_contents { 
        std::istreambuf_iterator<char>(infile), std::istreambuf_iterator<char>()
    };
    
    Van_Interpreter interpreter(file_contents);
    interpreter.run();
}