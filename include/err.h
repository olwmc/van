#ifndef ERR_H
#define ERR_H

#include <string>
#include <iostream>

void raiseError(std::string message) {
    std::cout << message << "\n";
    exit(1);
}

#endif /* ERR_H */