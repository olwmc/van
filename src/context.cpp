#include "forward.h"
#include "context.h"
#include "err.h"

#include <string>

void Context::bindLocalVariable(std::string name, Value v) {
    // TODO: Make this actually work dummy
    this->m_callStack.back()[name] = v;
}

void Context::bindGlobalVariable(std::string name, Value v) {
    this->m_globalScope[name] = v;
}

// TODO
// void updateVariable

Value Context::resolveVariable(std::string name) {
    Scope local;
    Scope global = this->m_globalScope;

    // Look through call stack for variable
    for(int i = (int)this->m_callStack.size(); i--;) {
        local = this->m_callStack[i];

        if(local.find(name) != local.end()) {
            return local[name];
        }
    }
    
    // Else look through globals
    if (global.find(name) != global.end()) {
        return global[name];
    }

    // Else raise error
    else {
        raiseLogicError("Could not resolve variable name: " + name);
    }

    return Value();
}

Callable* Context::resolveFunction(std::string name) {
    return this->m_functions[name];
}