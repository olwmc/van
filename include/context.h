/*
Basically, variable resolution works as such

The interpreter will call "resolve" on first the global scope, and then
the current local scope.

They will both return an ASTNode which the interpreter will then evaluate

The point of this class is *Just* to hold the associations, it doesnt do 
any evaluation or checking or anything.
*/

#pragma once

#include <unordered_map>
#include <stack>

#include "forward.h"
#include "AST.h"
#include "value.h"

typedef std::unordered_map<std::string, Value> Scope;

class Context {
    Scope globalScope;
    std::stack<Scope> callStack;
    std::unordered_map<std::string, ASTNode*> functions;

    public:
        Context() {}
        ~Context() {}

        void addLocalVariable(std::string name, Value v);
        void addGlobalVariable(std::string name, Value v);
        Value resolveVariable(std::string name);

        void addFunction(std::string name, ASTNode* body);
        ASTNode* resolveFunction(std::string name, Value args[]);

        void pushScope()     {
            // TODO: Copy over previous scope to new one
            this->callStack.push(Scope());
        }
        void popScope()    { this->callStack.pop(); };
};