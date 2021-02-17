#include "context.h"

int main() {

    // Create a new context and push a local scope
    Context context;
    context.pushScope();

    // Give the scope to the visitor
    ProgramVisitor vis(&context);
    
    // init
    NumberLiteral num_0(0);
    VariableDeclaration init_1("myVar", &num_0, true);

    // Update
    Identifier id_1("myVar");
    NumberLiteral num_1(1);
    BinaryExpression bin_1(&id_1, &num_1, Operator::ADD);
    AssignmentStatement update_1("myVar", &bin_1);

    // Test
    NumberLiteral num_2(10);
    BinaryExpression test(&id_1, &num_2, Operator::LESSEQUAL);

    // Block
    FunctionCall print_call("print", { &id_1 });
    Block block({&print_call});

    ForLoop loop_1(&init_1, &update_1, &test, &block);

    loop_1.accept(vis);
    context.popScope();
}