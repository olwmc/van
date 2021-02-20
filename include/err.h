#ifndef ERR_H
#define ERR_H

#include <string>

void raiseLogicError(std::string message);
void raiseLexicalError(std::string message, int line, int index, std::string program);

#endif /* ERR_H */