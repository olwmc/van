#include "interpreter.h"

#include <fstream>
#include <iostream>

#include <cmath>

class builtin_Input : public Builtin {
    public:
    builtin_Input() {
        this->m_args = {};
    }

    virtual Value execute(Context& context) override {
        std::string input;
        std::getline(std::cin, input);

        return Value(input);
    }
};

class builtin_Print : public Builtin {
    public:
    builtin_Print() {
        this->m_args = {"__input__"};
    }

    virtual Value execute(Context& context) override {
        std::cout << context.resolveVariable(this->m_args[0]).toString();
        return Value();
    }
};

class builtin_NumCast : public Builtin {
    public:
    builtin_NumCast() {
        this->m_args = {"__input__"};
    }

    virtual Value execute(Context& context) override {
        std::string str = context.resolveVariable(this->m_args[0]).asString();
        double num = std::stod(str);

        return Value(num);
    }
};

int main() {
    std::ifstream infile("examples/forloop.van");
    std::string file_contents { std::istreambuf_iterator<char>(infile), std::istreambuf_iterator<char>() };
    
    Van_Interpreter interpreter(file_contents);
    interpreter.context()->addBuiltinFunction("input", new builtin_Input());
    interpreter.context()->addBuiltinFunction("print", new builtin_Print());
    interpreter.context()->addBuiltinFunction("num", new builtin_NumCast());

    try {
        interpreter.interpret();
    }
    catch(const std::runtime_error& error) {
        std::cout << error.what() << "\n";
    }
}