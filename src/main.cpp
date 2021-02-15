#include "AST.h"
#include "context.h"

int main() {

    // Create a new context and push a local scope
    Context context;
    context.pushScope();

    // Give the scope to the visitor
    ProgramVisitor vis(&context);

    StringLiteral str_1("Hello!!!!!");
    FunctionCall fc_1("print", {&str_1});

    fc_1.accept(vis);
}