#include "AST.h"
#include "value.h"

/* REFACTOR THIS TO ACCOMIDATE NUMBERS & STRINGS */
Value ASTVisitor::visit(BinaryExpression& binaryExpression) {
    double lhs = binaryExpression.getLhs()->accept(*this).asNumber();
    double rhs = binaryExpression.getRhs()->accept(*this).asNumber();
    double result = 0;
    
    // Switch on op
    /* Make operator overloading for this instead of dealing with raw values */
    /* Do all typechecking beforehand, don't let operator overloading do any work */
    /* Report errors to ErrorHandler member of this */
    
    switch(binaryExpression.getOp()) {
        /* Arithmetic operations */
        case Operator::ADD:         result = lhs + rhs;  break;
        case Operator::SUBTRACT:    result = lhs - rhs;  break;
        case Operator::MULTIPLY:    result = lhs * rhs;  break;
        case Operator::DIVIDE:      result = lhs / rhs;  break;
            
        /* Logical operations */
        case Operator::EQUALS:      result = lhs == rhs; break;
        case Operator::NOTEQUAL:    result = lhs != rhs; break;
        case Operator::AND:         result = lhs || rhs; break;
        case Operator::OR:          result = lhs && rhs; break;
    }

    return Value(result);
}

Value ASTVisitor::visit(NumberLiteral& numberLiteral) {
    return Value(numberLiteral.value());
}

Value ASTVisitor::visit(StringLiteral& stringLiteral) {
    return Value(stringLiteral.value());
}

Value ASTVisitor::visit(ReturnStatement& returnStatement) {
    return returnStatement.argument()->accept(*this);
}

Value ASTVisitor::visit(ForLoop& forLoop) {
    // make new context
    // run init with new context

    while(forLoop.test()->accept(*this).asNumber() != 0) {
        
        Value value;
        // For each ASTNode in the body of block of the for loop */
        for(ASTNode* statement: forLoop.block()->body()) {  
            // Accept each ASTNode
            
            // TODO: Implement operator overloading for Value= to do
            // Value = statement->accept(*this);
            // To keep track of returns
            statement->accept(*this);

            if(statement->isReturn()) { break; }
        }

        // Then update
        forLoop.update()->accept(*this);
    }
    return Value();
}