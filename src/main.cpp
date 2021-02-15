#include "AST.h"
#include "context.h"

int main() {

    // Create a new context and push a local scope
    Context context;
    context.pushScope();

    // Give the scope to the visitor
    ASTVisitor vis(&context);
    
    // Add a new local variable to the context
    context.addLocalVariable("Beans", Value("Heyooo"));

    // "return Beans"
    Identifier b_beans("Beans");
    ReturnStatement ret_1(&b_beans);

    // Visit the return statement and resolve the argument
    std::cout << ret_1.accept(vis).asString() << "\n";

    // Pop scope
    context.popScope();

    /* For a for loop we need
       VariableAsignment i = i + 1
                         i = i * n

        Block
        ForLoop

    */
}

/*
local n = 5;
local i;

for(i = 0; i <= n; i = i + 1) do 
    i = i * n;
end

return i;
*/