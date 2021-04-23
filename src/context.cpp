#include "forward.h"
#include "context.h"

#include <string>

// TODO: Refactor variable update to return a 
// pointer from a helper function

void Context::bindLocalVariable(std::string name, Value v) {
    this->m_scopeStack.back()[name] = v;
}

void Context::bindGlobalVariable(std::string name, Value v) {
    this->m_globalScope[name] = v;
}

Value* Context::getValue(std::string name) {
    // Loop through each index of the scopeStack, looking for the variable
    for(int i = (int)this->m_scopeStack.size(); i--;) {

        // If the variable is found in the map, set the new value and return
        if(this->m_scopeStack[i].find(name) != this->m_scopeStack[i].end()) {
            return &this->m_scopeStack[i][name];
        }
    }

    // If the variable isn't in the scopeStack, check the global scope and set if possible
    if(this->m_globalScope.find(name) != this->m_globalScope.end()) {
        return &this->m_globalScope[name];
    }

    // Otherwhise, the variable has not been declared
    throw std::runtime_error("Unable to find variable: " + name); 
}

void Context::updateVariable(std::string name, Value v) {
    // Set the value
    *(getValue(name)) = v;
}

void Context::updateIndex(std::string name, Value v, int index) {
    Value* val = getValue(name);

    if(val->type() == Value_Type::LIST) {
        std::vector<Value> alterated = val->asList();

        index = (index < 0) ? alterated.size() + index : index;
        alterated[index] = v;

        *val = Value(alterated);

        return;
    }

    else if(val->type() == Value_Type::STRING) {
        std::string alterated = val->asString();

        index = (index < 0) ? alterated.size() + index : index;
        alterated[index] = (v.toString())[0];

        *val = Value(alterated);
        return;
    }

    else {
        throw std::runtime_error("Only lists and strings can be index assigned");
    }

}

Value Context::resolveVariable(std::string name) {
    return *getValue(name);
}

Callable* Context::resolveFunction(std::string name) {
    if(this->m_functions.find(name) != this->m_functions.end()) {
        return this->m_functions[name];
    }

    else {
        throw std::runtime_error("Unknown function: " + name);
    }
}