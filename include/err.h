#ifndef ERR_H
#define ERR_H

#include <string>
#include <iostream>
void raise(std::string message, int index, int lineNum) {
    std::cout << message << ".\nLine: " << lineNum << "\n";
}

#endif /* ERR_H */