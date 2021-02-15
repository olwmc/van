#include "forward.h"
#include "context.h"

#include "err.h"

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
        raiseError("Could not resolve variable name: " + name);
    }

    return Value();
}

/*
	Check if in map \/
	mymap.count(x)<1
*/

/*

context.resolveFunction("Beans", [Value(100), Value(-10)]);

resolveFunction:
    // Push a new scope
    this->pushScope()

    // Get the callable function
    Callable func = this->functions.get(id);

    // Getg the argument names
    std::string argnames[] = func.args();

    if(argNames.length() != args.length()) {
        raise("Too few arguments for function call");
    }

    // Push each of the supplied arguments into scope as their
    // ordered argnames
    for(arg in args) { push(arg to scope as argname[i]) }

    // Visit the callable and get return value
    Value v = func.accept(this*);

    // Pop the scope
    this->popScope();

    // Reurn the value
    return v;
*/