#include "AST.h"
#include "context.h"

int main() {

    // Create a new context and push a local scope
    Context context;
    context.pushScope();

    // Give the scope to the visitor
    ProgramVisitor vis(&context);

    // 25, 18
    NumberLiteral num_1(25);
    NumberLiteral num_2(18);

    BinaryExpression expr_1(&num_1, &num_2, Operator::MULTIPLY);

    // print(1)
    FunctionCall fc_1("print", {&expr_1});

    fc_1.accept(vis);
}