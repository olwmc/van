# The Van Programming Language
Final Project for CSC212

## Purpose
Van is a language with a simple grammar. It uses a tree-walk interpreter to execute.

Van is extendable, meaning you can add your own functionality and bindings to C++ functions.

### Example of an extension: Adding a sin(x) function
```c++
#include <cmath>

class bu_sin : public Builtin {
    public:
    bu_sin() {
        this->m_args = {"x"};
    }

    virtual Value execute(Context& context) override {
        double x = context.resolveVariable("x").asNumber();
        return Value(sin(x));
    }
};

// Later on, when you declare the interpreter
Interpreter interpreter(input_program);
interpreter.context()->addBuiltinFunction("sin", bu_sin);
```