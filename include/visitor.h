#include "forward.h"
#include "value.h"

/* AST Visitor "executes" each node */
class ASTVisitor {
    Context* m_context;
    // error handler

    public:
        ASTVisitor(Context* c) : m_context(c) {}
        Value visit(NumberLiteral& numberLiteral);
        Value visit(BinaryExpression& binaryExpression);
        Value visit(StringLiteral& stringLiteral);
        Value visit(Identifier& identifier);
        Value visit(ReturnStatement& returnStatement);
        Value visit(ForLoop& forLoop);
        Value visit(Block& block);
};