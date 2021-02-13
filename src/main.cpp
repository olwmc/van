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
}

/*
local n = 5;

for(global i = 1; i <= n; i++) do 
    i = i * n;
end

return i;
*/

// Maybe pass an InfoHandler class or something to hold the ast, program, etc.