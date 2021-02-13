#include "forward.h"
#include "context.h"

void Context::addLocalVariable(std::string name, Value v) {
    this->callStack.top()[name] = v;
}
void Context::addGlobalVariable(std::string name, Value v) {
    this->globalScope[name] = v;
}

Value Context::resolveVariable(std::string name) {
    Scope local = this->callStack.top();
    Scope global = this->globalScope;

    if(local.find(name) != local.end()) {
        return local[name];
    }

    else if (global.find(name) != global.end()) {
        return global[name];
    }

    else {
    // Call error handler
    }

    return Value();
}