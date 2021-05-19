# Making new builtin functions
Let's make a quick builtin function that returns the `sine` of whatever number the user passes
```c++
#include "builtin.h"
#include "context.h"

#include <cmath>

class bu_Sin : public Builtin { 
    public:
        bu_Sin() {
            this->m_args = {"x"};
        }

    virtual Value execute(Context& context) override {
        Value x = context.resolveVariable(this->m_args[0]);
        
        // Check if the input is a number, if so return sin(x), else nil
        if(x.type() == Value_Type::NUMBER) {
            return Value(  sin (x.asNumber()) );
        }
        return Value();
    }
};

Van_Interpreter interpreter(myProgramContents);
interpreter.context()->addBuiltinFunction("sin", new bu_Sin());
```

You can also pass a value to the constructor of your new builtin, allowing for 
added external functionality like communicating with a pointer to a canvas.
