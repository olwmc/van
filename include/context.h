/*

#include "builtin.h"

class Scope;
class Context {
    Scope globalScope;
    Scope localScope;
    Builtins builtins;
}

*/

/*
Basically, variable resolution works as such

The interpreter will call "resolve" on first the global scope, and then
the current local scope.

They will both return an ASTNode which the interpreter will then evaluate

The point of this class is *Just* to hold the associations, it doesnt do 
any evaluation or checking or anything.

*/