# Making new builtin functions
Let's make a quick builtin function that adds 100 to whatever the user passes as an argument
```c++
#include "builtin.h"
#include "context.h"

// Make the class
class myFunBuiltin : public Builtin {
    public:
        // Add the argument name
        myFunBuiltin() {
            this->m_args = {"x"};
        }

        // Override the execute method
        virtual Value execute(Context& context) override;
};

// Implement the execute method
Value myFunBuiltin::execute(Context& context) {
    // Return Value(100) + whatever the user passes
    return Value(100) + context.resolveVariable(this->m_args[0]);
}

...

Van_Interpreter interpreter(myFileContents);
interpreter.context()->addBuiltinFunction("myFunBuiltin", new myFunBuiltin());
```

You can also pass a value to the constructor of your new builtin, allowing for 
added external functionality like communicating with a pointer to a canvas.