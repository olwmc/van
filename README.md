# The Van Programming Language DUE: May 6th 2021 @ 3:30 PM
Final Project for CSC212

## About
Van is a language with a simple grammar. It uses a tree-walk interpreter to execute.

Van is extendable, meaning you can add your own functionality and bindings to C++ functions.

### Example of an extension: Adding a sin(x) function
```c++
#include <cmath>

/* Create our "[bu]iltin [sin]" class, inheriting from Builtin */
class bu_sin : public Builtin {
    public:

        /* In the constructor, we must set the argument names to
           be used later */
        bu_sin() {
            this->m_args = {"x"};
        }

        /* Override the execute function (Gets called when the function does) */
        virtual Value execute(Context& context) override {
            // Get the variable "x" from the context
            double x = context.resolveVariable("x").asNumber();

            // Return sin(x)
            return Value(sin(x));
    }
};

// Later on, when you declare the interpreter
Interpreter interpreter(input_program);
interpreter.context()->addBuiltinFunction("sin", new bu_sin());
```
