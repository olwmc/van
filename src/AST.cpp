#include "AST.h"
#include "value.h"

// TODO: Redo this
Value ASTVisitor::visit(BinaryExpression& binaryExpression) {
    double lhs = binaryExpression.lhs()->accept(*this).asNumber();
    double rhs = binaryExpression.rhs()->accept(*this).asNumber();
    double result = 0;
    
    /*
    Value lhs = binaryExpression.getLhs()->accept(*this);
    Value rhs = binaryExpression.getRhs()->accept(*this);
    
    Do operation then check if type() == Value_Type::ERROR. If so, report error
    */

    switch(binaryExpression.op()) {
        /* Arithmetic operations */
        case Operator::ADD:         result = lhs + rhs;  break;
        case Operator::SUBTRACT:    result = lhs - rhs;  break;
        case Operator::MULTIPLY:    result = lhs * rhs;  break;
        case Operator::DIVIDE:      result = lhs / rhs;  break;
            
        // TODO: Overload this to support proper double modulo
        case Operator::MOD:         result = (int)lhs % (int)rhs;  break;
            
        /* Logical operations */
        case Operator::EQUALS:      result = lhs == rhs; break;
        case Operator::NOTEQUAL:    result = lhs != rhs; break;
        case Operator::LESSEQUAL:   result = lhs <= rhs; break;
        case Operator::AND:         result = lhs || rhs; break;
        case Operator::OR:          result = lhs && rhs; break;
    }

    return Value(result);
}

// Return the literal representation of the Number
Value ASTVisitor::visit(NumberLiteral& numberLiteral) {
    return Value(numberLiteral.value());
}

// Return the literal representation of th String
Value ASTVisitor::visit(StringLiteral& stringLiteral) {
    return Value(stringLiteral.value());
}

// Resolve the identifier and return the result
Value ASTVisitor::visit(Identifier& identifier) {
    return this->m_context->resolveVariable(identifier.name());
}

// Return the expressed argument
Value ASTVisitor::visit(ReturnStatement& returnStatement) {
    return returnStatement.argument()->accept(*this);
}

Value ASTVisitor::visit(Block& block) {
    // When evaluating a block, pushing context beforehand is 
    // necessary to conserve the logic of loops and other things.
    // This is why context is not pushed *within* the block but
    // before and afterwards popped
    Value value;

    for(ASTNode* statement: block.body()) {  
        // Accept each ASTNode
        value = statement->accept(*this);

        /* Check if statement is a return statement
         * The reason why we check this, instead of just non-nil value
         * is because things like n++; are statements but return values
         * but are not terminal returns */
        if(statement->isReturn()) { return value; }
    }

    // Return empty value
    return Value();
}


Value ASTVisitor::visit(ForLoop& forLoop) {
    Value value;

    // Push a new scope and initialize init within it
    this->m_context->pushScope();

    forLoop.init()->accept(*this);

    // While test != False (0)
    while(forLoop.test()->accept(*this).asNumber() != false) {
        
        // Execute body and set return value
        value = forLoop.block()->accept(*this);

        // Update value
        forLoop.update()->accept(*this);
    }

    // The reason this returns something is you may have
    // a return somewhere conditionally within the loop,
    // therefore it's necessary to return from it.
    return value;
}