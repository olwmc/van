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
    context.addLocalVariable("x", Value(100));

    // x, 100
    Identifier id_x("x");
    NumberLiteral num_1(101);

    // x + 100
    BinaryExpression bin_1(&id_x, &num_1, Operator::ADD);

    // x == 100
    BinaryExpression bin_2(&id_x, &num_1, Operator::EQUALS);

    double are_equal = bin_2.accept(vis).asNumber();

    // __assert_condition__ = (x==101);
    context.addLocalVariable("__assert_condition__", Value(are_equal));

    // Fails and exits
    builtin_Assert().accept(vis);

    context.addLocalVariable("__input__", Value("Hello! My name is myVar!"));

    /* Simulate function call */
    builtin_Print().accept(vis);
}