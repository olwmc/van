#include "AST.h"
#include "context.h"

int main() {

    // Create a new context and push a local scope
    Context context;
    context.pushScope();

    // Give the scope to the visitor
    ASTVisitor vis(&context);
    
    // Add a new local variable to the context
    // local x = 35;
    context.addLocalVariable("x", Value(35));

    // x, 100
    Identifier id_x("x");
    NumberLiteral num_1(100);

    // x + 100
    BinaryExpression bin_1(&id_x, &num_1, Operator::ADD);

    // 135
    std::cout << bin_1.accept(vis).asNumber() << "\n";

    context.addLocalVariable("__input__", Value("Hello! My name is myVar!"));

    /* Simulate function call */
    builtin_Print().accept(vis);
}