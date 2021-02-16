/*
Basically, variable resolution works as such

Variable:
    Resolving a variable is simple as looking in
    the local and the global variable scopes and
    seeing if there's a var. If there is, return 
    it's associated value. Else, raise an error.

Function:
    Function resolution works by checking if the 
    function has been defined, if it hasn't, an 
    error is raised. If it has, we push a new
    scope, get the args that the callable 
    requires and map our supplied arguments to
    their id within the current scope.
    
        i.e. foo(1,2,3) => x=1, y=2, z=3

    We then visit the Callable which returns a 
    value, NIL or otherwhise. We then pop the 
    scope and return the value.
*/

#ifndef CONTEXT_H
#define CONTEXT_H

#include <unordered_map>
#include <stack>
#include <utility>

#include "forward.h"
#include "AST.h"
#include "value.h"
#include "callable.h"
#include "builtin.h"

// Typedef Scope to improve readability
typedef std::unordered_map<std::string, Value> Scope;

class Context {
    Scope m_globalScope;
    std::stack<Scope> m_callStack;

    // Holds functions and buitins
    std::unordered_map<std::string, Callable*> m_functions;

    public:
        Context() {
            /* Add builtins */
            this->m_functions.insert( {"print", new builtin_Print()} );
            this->m_functions.insert( {"assert", new builtin_Assert()} );
        }
        
        ~Context() {
            for(auto it: this->m_functions) {
                delete it.second;
            } 
        }

        void addLocalVariable(std::string name, Value v);
        void addGlobalVariable(std::string name, Value v);
        void addFunction(std::string name, std::vector<std::string> args, ASTNode* body);
        
        // Resolves a variable name to its associated Value
        Value resolveVariable(std::string name);

        // ResolveFunction passes an ASTNode* to our visitor
        Callable* resolveFunction(std::string name);

        void pushScope()     {
            if(this->m_callStack.size() == 0) {
                this->m_callStack.push(Scope());
            }

            else { this->m_callStack.push(this->m_callStack.top()); }
        }

        void popScope()    { this->m_callStack.pop(); };
};

#endif /* CONTEXT_H */