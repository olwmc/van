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

#include "forward.h"
#include "AST.h"
#include "value.h"
#include "callable.h"
#include "builtin.h"

#include <unordered_map>
#include <utility>

// Typedef Scope to improve readability
typedef std::unordered_map<std::string, Value> Scope;

class Context {
    Scope m_globalScope;
    std::vector<Scope> m_scopeStack;

    // Holds functions and buitins
    std::unordered_map<std::string, Callable*> m_functions;

    public:
        Context() {
            /* Add builtins */
            this->m_functions.insert( {"println", new builtin_Println() });
            this->m_functions.insert( {"assert",  new builtin_Assert()  });
            this->m_functions.insert( {"input",   new builtin_Input()   });
            this->m_functions.insert( {"print",   new builtin_Print()   });
            this->m_functions.insert( {"num",     new builtin_NumCast() });
            this->m_functions.insert( {"len",     new builtin_Len()     });
            this->m_functions.insert( {"type",    new builtin_Type()    });
            this->m_functions.insert( {"str",     new builtin_StrCast() });
            this->m_functions.insert( {"rand",    new builtin_Rand()    });
            this->m_functions.insert( {"sort",    new builtin_Sort()    });
            this->m_functions.insert( {"contains",new builtin_Contains()});

            // Seed the RNG
            srand(time(NULL));
        }
        
        ~Context() {
            for(auto it: this->m_functions) {
                delete it.second;
            } 
        }

        void addBuiltinFunction(std::string name, Builtin* func) {
            this->m_functions.insert( {name, func} );
        }

        void bindFunction(std::string name, UserFunction* func) {
            this->m_functions.insert( {name, func} );
        }
        
        void bindLocalVariable(std::string name, Value v);
        void bindGlobalVariable(std::string name, Value v);

        void updateVariable(std::string name, Value v);
        void updateIndex(std::string name, Value v, std::vector<int> indexes);
        void updateIndex(Value* valPtr, Value v, std::vector<int> indexes);

        Value* getValue(std::string name);
        
        // Resolves a variable name to its associated Value
        Value resolveVariable(std::string name);

        // ResolveFunction passes an ASTNode* to our visitor
        Callable* resolveFunction(std::string name);

        void pushScope()   { this->m_scopeStack.push_back(Scope()); }
        void popScope()    { this->m_scopeStack.pop_back(); };
};

#endif /* CONTEXT_H */