#include "AST.h"
#include "value.h"
#include "err.h"

// TODO: Redo this
Value ProgramVisitor::visit(BinaryExpression& binaryExpression) {
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
Value ProgramVisitor::visit(NumberLiteral& numberLiteral) {
    return Value(numberLiteral.value());
}

// Return the literal representation of th String
Value ProgramVisitor::visit(StringLiteral& stringLiteral) {
    return Value(stringLiteral.value());
}

// Resolve the identifier and return the result
Value ProgramVisitor::visit(Identifier& identifier) {
    return this->m_context->resolveVariable(identifier.name());
}

// Return the expressed argument
Value ProgramVisitor::visit(ReturnStatement& returnStatement) {
    return returnStatement.argument()->accept(*this);
}

Value ProgramVisitor::visit(Block& block) {
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


Value ProgramVisitor::visit(ForLoop& forLoop) {
    Value value;

    // Push a new scope and initialize init within it
    this->m_context->pushScope();

    forLoop.init()->accept(*this);
    
    // While test != False (0)
    while(forLoop.test()->accept(*this).asNumber() != 0) {
        
        // Execute body and set return value
        value = forLoop.block()->accept(*this);

        // Update value
        forLoop.update()->accept(*this);
    }

    // The reason this returns something is you may have
    // a return somewhere conditionally within the loop,
    // therefore it's necessary to return from it.

    this->m_context->popScope();
    return value;
}

Value ProgramVisitor::visit(FunctionCall& functionCall) {
    // Push new scope
    this->m_context->pushScope();

    // Get the callable function
    Callable* func = this->m_context->resolveFunction(functionCall.callee());

    // Get the function argument names
    std::vector<std::string> funcArgs = func->args();

    // Get the function call values
    std::vector<ASTNode*> callArgs = functionCall.args();

    if(callArgs.size() != funcArgs.size()) {
        raiseError("Incorrect number of arguments for function \"" + functionCall.callee() + "\"");
    }

    // Add each argument as a local variable with value passed through call
    for(int i = 0; i < (int)funcArgs.size(); ++i) {
        this->m_context->addLocalVariable(funcArgs[i], callArgs[i]->accept(*this));
    }

    // Visit the Callable
    Value v = func->accept(*this);

    // Pop the scope
    this->m_context->popScope();

    // Return return value
    return v;
}

Value ProgramVisitor::visit(AssignmentStatement& assignmentStatement) {
    std::string id = assignmentStatement.id();
    Value init = assignmentStatement.rhs()->accept(*this);

    this->m_context->addLocalVariable(id, init);

    return Value();
}

Value ProgramVisitor::visit(VariableDeclaration& variableDeclaration) {
    std::string id = variableDeclaration.id();
    ASTNode *init = variableDeclaration.init();

    Value v = init->accept(*this);
    
    if(variableDeclaration.isLocal()) {
        this->m_context->addLocalVariable(id, v);
    }

    else {
        this->m_context->addGlobalVariable(id, v);
    }

    return Value();
}