#ifndef VISITOR_H
#define VISITOR_H

#include "forward.h"
#include "value.h"

/* AST Visitor "executes" each node */
class ASTVisitor {
    Context* m_context;

    public:
        ASTVisitor(Context* c) : m_context(c) {}
        Value visit(NumberLiteral& numberLiteral);
        Value visit(BinaryExpression& binaryExpression);
        Value visit(StringLiteral& stringLiteral);
        Value visit(Identifier& identifier);
        Value visit(ReturnStatement& returnStatement);
        Value visit(ForLoop& forLoop);
        Value visit(Block& block);
        Value visit(builtin_Print& printStatment);
        Value visit(UserFunction& userFunction);
};

#endif /* VISITOR_H */