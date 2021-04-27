#include "forward.h"
#include "context.h"

#include <string>


void Context::bindLocalVariable(std::string name, Value v) {
    this->m_scopeStack.back()[name] = v;
}

void Context::bindGlobalVariable(std::string name, Value v) {
    this->m_globalScope[name] = v;
}

Value* Context::getValue(std::string name) {
    for(int i = (int)this->m_scopeStack.size(); i--;) {

        // If the variable is found in the map, set the new value and return
        if(this->m_scopeStack[i].find(name) != this->m_scopeStack[i].end()) {
            return &this->m_scopeStack[i][name];
        }
    }

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

// Update the index by recursively iterating downwards until the last index is achieved
void Context::updateIndex(std::string name, Value v, std::vector<int> indexes) {
    Value* val = getValue(name);
    updateIndex(val, v, indexes);
}

void Context::updateIndex(Value* valPtr, Value v, std::vector<int> indexes) {
    // BASE CASE: There's only one index to set
    if(indexes.size() == 1) {
        switch(valPtr->type()) {
            case STRING:
                // Set the index
                (*valPtr->getString())[indexes.back()] = v.toString()[0];
            break;

            case LIST:
                // Set the index
                (*valPtr->getList())[indexes.back()] = v;
            break;

            default:
                throw std::runtime_error("Only strings and lists can have indexes assigned");
        }

        return;
    }
    
    // Auxillary case, the value is a list, then recurse
    if(valPtr->type() == Value_Type::LIST) {
        valPtr = &(valPtr->getList())->at(indexes.front());
        
        // TODO: Maybe rework this into a queue lmao
        updateIndex(valPtr, v, std::vector<int>(indexes.begin() + 1, indexes.end()));
        return;
    }

    // Otherwhise just throw an error
    throw std::runtime_error("Only strings and lists can have indexes assigned");
    
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