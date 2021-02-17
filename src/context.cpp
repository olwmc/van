#include "forward.h"
#include "context.h"

#include "err.h"

#include <string>

void Context::addLocalVariable(std::string name, Value v) {
    this->m_callStack.back()[name] = v;
}
void Context::addGlobalVariable(std::string name, Value v) {
    this->m_globalScope[name] = v;
}

Value Context::resolveVariable(std::string name) {
	Scope local;
    Scope global = this->m_globalScope;
	
	for(int i = (int)this->m_callStack.size(); i--;) {
        local = this->m_callStack[i];

        if(local.find(name) != local.end()) {
            return local[name];
        }
    }
    
    if (global.find(name) != global.end()) {
        return global[name];
    }

    else {
        raiseError("Could not resolve variable name: " + name);
    }

    return Value();
}

Callable* Context::resolveFunction(std::string name) {
    return this->m_functions[name];
}