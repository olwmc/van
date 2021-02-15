/*
Basically, variable resolution works as such

Variable:
    Resolving a variable is simple as looking in
    the local and the global variable scopes and
    seeing if there's a var. If there is, return 
    it's associated value. Else, raise an error.

Function:
    Function resolution works by first pushing a
    new scope. Checking if the function has been
    defined, if it hasn't, an error is raised.
    If it has, we push a new scope, get the args
    that the callable requires and map our
    supplied arguments to their id within the
    current scope.
        i.e. foo(1,2,3) => x=1, y=2, z=3

    We then visit the Callable which returns a 
    value, NIL or otherwhise. We then pop the 
    scope and return the value.
*/

#ifndef CONTEXT_H
#define CONTEXT_H

#include <unordered_map>
#include <stack>

#include "forward.h"
#include "AST.h"
#include "value.h"
#include "callable.h"

// Typedef Scope to improve readability
typedef std::unordered_map<std::string, Value> Scope;

class Context {
    Scope globalScope;
    std::stack<Scope> callStack;

    // Holds functions and buitins
    std::unordered_map<std::string, Callable*> functions;

    public:
        Context() {}
        ~Context() {}

        void addLocalVariable(std::string name, Value v);
        void addGlobalVariable(std::string name, Value v);

        // Resolves a variable name to its associated Value
        Value resolveVariable(std::string name);

        void addFunction(std::string name, ASTNode* body);

        // ResolveFunction passes an ASTNode* to our visitor
        Value resolveFunction(std::string name, Value args[]);

        void pushScope()     {
            // TODO: Copy over previous scope to new one
            this->callStack.push(Scope());
        }

        void popScope()    { this->callStack.pop(); };
};

#endif /* CONTEXT_H */