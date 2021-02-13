#include "AST.h"
#include "value.h"

Value ASTVisitor::visit(BinaryExpression& binaryExpression) {
    double lhs = binaryExpression.lhs()->accept(*this).asNumber();
    double rhs = binaryExpression.rhs()->accept(*this).asNumber();
    double result = 0;
    
    // Switch on op
    /* Do all typechecking beforehand, don't let operator overloading do any work */
    /* Make special case for operator overloading of + for strings, doubles, and lists */
    /* Report errors to ErrorHandler member of this */
    
    /*
    Value lhs = binaryExpression.getLhs()->accept(*this);
    Value rhs = binaryExpression.getRhs()->accept(*this);
    
    if(binaryExpression.getOp()
    if(lhs.type() != rhs.type() {
        this->errorHandler().raise("Cannot perform this operation on two different types", 
                                    binaryExpression.index(),
                                    binaryExpression.lineNum());
    }
    
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

Value ASTVisitor::visit(NumberLiteral& numberLiteral) {
    return Value(numberLiteral.value());
}

Value ASTVisitor::visit(StringLiteral& stringLiteral) {
    return Value(stringLiteral.value());
}

Value ASTVisitor::visit(Identifier& identifier) {
    return this->m_context->resolveVariable(identifier.name());
}

Value ASTVisitor::visit(ReturnStatement& returnStatement) {
    return returnStatement.argument()->accept(*this);
}

Value ASTVisitor::visit(Block& block) {
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
    // make new context
    // run init with new context
    // this->context.pushContext();
    Value value;

    forLoop.init()->accept(*this);

    // While test != False (0)
    while(forLoop.test()->accept(*this).asNumber() != false) {
        
        // Execute body and set return value
        value = forLoop.block()->accept(*this);

        // Update value
        forLoop.update()->accept(*this);
    }
    return Value();
}
