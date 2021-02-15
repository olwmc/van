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

    // 25 * 18
    BinaryExpression expr_1(&num_1, &num_2, Operator::MULTIPLY);

    // 450
    NumberLiteral num_3(450);

    // (25 * 18) == 450
    BinaryExpression expr_2(&expr_1, &num_3, Operator::EQUALS);

    FunctionCall fn_1("assert", {&expr_2});

    StringLiteral str_1("If this doesn't show, I asserted correctly!");
    FunctionCall fn_2("print", {&str_1});

    fn_1.accept(vis);
    fn_2.accept(vis);
}