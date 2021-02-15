#include <string>
class ErrorHandler {
    public:
        void raise(std::string message, int index, int lineNum);
};