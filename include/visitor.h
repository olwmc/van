#ifndef VISITOR_H
#define VISITOR_H

#include "forward.h"
#include "value.h"

/* AST Visitor "executes" each node */
class ProgramVisitor {
    Context* m_context;

    public:
        ProgramVisitor(Context* c) : m_context(c) {}

        /* Literal nodes */
        Value visit(NumberLiteral& numberLiteral);
        Value visit(StringLiteral& stringLiteral);
        Value visit(Identifier& identifier);

        /* Statement nodes */
        Value visit(BinaryExpression& binaryExpression);
        Value visit(ReturnStatement& returnStatement);
        Value visit(ForLoop& forLoop);
        Value visit(Block& block);
        Value visit(FunctionCall& functionCall);
        Value visit(AssignmentStatement& assignmentStatement);
        Value visit(VariableDeclaration& variableDeclaration);
        Value visit(FunctionDeclaration& functionDeclaration);

        /* Function callables */
        Value visit(Builtin& builtin);
        Value visit(UserFunction& userFunction);
};
#endif /* VISITOR_H */
