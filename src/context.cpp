#include "forward.h"
#include "context.h"

#include <string>

void Context::bindLocalVariable(std::string name, Value v) {
    this->m_callStack.back()[name] = v;
}

void Context::bindGlobalVariable(std::string name, Value v) {
    this->m_globalScope[name] = v;
}

void Context::updateVariable(std::string name, Value v) {
    // Loop through each index of the callstack, looking for the variable
    for(int i = (int)this->m_callStack.size(); i--;) {

        // If the variable is found in the map, set the new value and return
        if(this->m_callStack[i].find(name) != this->m_callStack[i].end()) {
            this->m_callStack[i][name] = v;
            return;
        }
    }

    // If the variable isn't in the callstack, check the global scope and set if possible
    if(this->m_globalScope.find(name) != this->m_globalScope.end()) {
        this->m_globalScope[name] = v;
        return;
    }

    // Otherwhise, the variable has not been declared
    throw std::runtime_error("Unable to find variable: " + name);
}

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
        throw std::runtime_error("Could not resolve variable name: " + name);
    }

    return Value();
}

Callable* Context::resolveFunction(std::string name) {
    if(this->m_functions.find(name) != this->m_functions.end()) {
        return this->m_functions[name];
    }

    else {
        throw std::runtime_error("Unknown function: " + name);
    }
}