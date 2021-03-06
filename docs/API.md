# Making new builtin functions
```c++
#include "builtin.h"
#include "context.h"

class test : public Builtin {
    public:
        test() {
            this->m_args = {"x"};
        }
    
        virtual Value execute(Context& context) override;
};

Value test::execute(Context& context) {
    return Value(100) + context.resolveVariable(this->m_args[0]);
}

context.addBuiltinFunction("test", new test());
```

You can also pass a value to the constructor of your new builtin, allowing for 
added external functionality like communicatin with a pointer to a canvas.