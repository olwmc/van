#include "forward.h"
#include "context.h"

#include <string>

void Context::bindLocalVariable(std::string name, Value v) {
    this->m_scopeStack.back()[name] = v;
}

void Context::bindGlobalVariable(std::string name, Value v) {
    this->m_globalScope[name] = v;
}

void Context::updateVariable(std::string name, Value v) {
    // Loop through each index of the scopeStack, looking for the variable
    for(int i = (int)this->m_scopeStack.size(); i--;) {

        // If the variable is found in the map, set the new value and return
        if(this->m_scopeStack[i].find(name) != this->m_scopeStack[i].end()) {
            this->m_scopeStack[i][name] = v;
            return;
        }
    }

    // If the variable isn't in the scopeStack, check the global scope and set if possible
    if(this->m_globalScope.find(name) != this->m_globalScope.end()) {
        this->m_globalScope[name] = v;
        return;
    }

    // Otherwhise, the variable has not been declared
    throw std::runtime_error("Unable to find variable: " + name);
}

void Context::updateIndex(std::string name, Value v, int index) {
    int foundIndex = -1;

    // Loop through each index of the scopeStack, looking for the variable
    for(int i = (int)this->m_scopeStack.size(); i--;) {

        // If the variable is found in the map, set the new value and return
        if(this->m_scopeStack[i].find(name) != this->m_scopeStack[i].end()) {
            foundIndex = i;
            break;
        }
    }

    // If the variable isn't in the scopeStack, check the global scope and set if possible
    if(this->m_globalScope.find(name) != this->m_globalScope.end()) {
        foundIndex = -2;
        return;
    }

    // Set the new index if it's in the global scope
    if(foundIndex != -1) {
        Value_Type t = this->m_scopeStack[foundIndex][name].type();

        // Ensure it's a list otherwhise throw an exception
        if(t == Value_Type::LIST) {
            // Get the original list
            std::vector<Value> alterated = this->m_scopeStack[foundIndex][name].asList();

            // Set the new value at index
            alterated[index] = v;

            // Set the value to the new list
            // TODO: NEGATIVE INDEXES
            this->m_scopeStack[foundIndex][name] = Value(alterated);
            return;

        } else if (t == Value_Type::STRING) {
            std::string alterated = this->m_scopeStack[foundIndex][name].asString();
            alterated[index] = (v.toString())[0];

            this->m_scopeStack[foundIndex][name] = Value(alterated);
            return;
        }

        else {
            throw std::runtime_error("Only lists and strings can be index assigned");
        }
    }


    throw std::runtime_error("Unable to find variable: " + name);
}

Value Context::resolveVariable(std::string name) {
    Scope local;
    Scope global = this->m_globalScope;

    // Look through call stack for variable
    for(int i = (int)this->m_scopeStack.size(); i--;) {
        local = this->m_scopeStack[i];

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