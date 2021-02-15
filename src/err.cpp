#include "err.h"

#include <iostream>

void raiseError(std::string message) {
    std::cout << message << "\n";
    exit(1);
}