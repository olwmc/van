#include "AST.h"
#include "context.h"
#include "lexer.h"

#include <fstream>
#include <iostream>

int main() {
    std::ifstream infile("examples/fib.van");
    std::string file_contents { std::istreambuf_iterator<char>(infile), std::istreambuf_iterator<char>() };

    // Create a new context and push a local scope
    Context context;
    context.pushScope();

    // Give the scope to the visitor
    ProgramVisitor vis(&context);
    
    // init
    NumberLiteral num_0(0);
    VariableDeclaration init_1("i", &num_0, true);

    // Update
    Identifier id_1("i");
    NumberLiteral num_1(1);
    BinaryExpression bin_1(&id_1, &num_1, Operator::ADD);
    AssignmentStatement update_1("i", &bin_1);

    // Test
    NumberLiteral num_2(10);
    BinaryExpression test(&id_1, &num_2, Operator::LESSEQUAL);

    // Block
    FunctionCall print_call("print", { &id_1 });
    Block block({&print_call});

    // For loop
    ForLoop loop_1(&init_1, &update_1, &test, &block);

    StringLiteral str_1("Goodbye!");
    FunctionCall pr_1("print", {&str_1});

    StringLiteral hello("Hello!");
    FunctionCall pr_2("print", {&hello});

    // Outer block
    Block block_2{ {&pr_2, &loop_1, &pr_1} };

    block_2.accept(vis);
    context.popScope();
}