#include "AST.h"
#include "value.h"

#include <cmath>

Value ProgramVisitor::visit(BinaryExpression& binaryExpression) {
    Value lhs = binaryExpression.lhs()->accept(*this);
    Value rhs = binaryExpression.rhs()->accept(*this);
    Value result;

    switch(binaryExpression.op()) {
        /* Arithmetic operations */
        case Operator::ADD:         result = lhs + rhs;  break;
        case Operator::SUBTRACT:    result = lhs - rhs;  break;
        case Operator::MULTIPLY:    result = lhs * rhs;  break;
        case Operator::DIVIDE:      result = lhs / rhs;  break;
            
        case Operator::MOD:
            result = fmod(lhs.asNumber(), rhs.asNumber());
        break;
            
        /* Logical operations */
        case Operator::EQUALS:      result = lhs == rhs; break;
        case Operator::NOTEQUAL:    result = lhs != rhs; break;
        case Operator::LESSEQUAL:   result = lhs <= rhs; break;
        case Operator::GREATEQUAL:  result = lhs >= rhs; break;
        case Operator::OR:          result = lhs || rhs; break;
        case Operator::AND:         result = lhs && rhs; break;
        case Operator::LESSTHAN:    result = lhs < rhs;  break;
        case Operator::GREATERTHAN: result = lhs > rhs;  break;
    }

    if(result.type() == Value_Type::ERR) {
        throw std::runtime_error("Incorrect types on values: " + lhs.toString() + " & " + rhs.toString());
    }

    return result;
}

// Return the literal representation of the Number
Value ProgramVisitor::visit(NumberLiteral& numberLiteral) {
    return Value(numberLiteral.value());
}

// Return the literal representation of th String
Value ProgramVisitor::visit(StringLiteral& stringLiteral) {
    return Value(stringLiteral.value());
}

Value ProgramVisitor::visit(ArrayLiteral& arrayLiteral ) {
    std::vector<Value> values;
    for(ASTNode* arg : arrayLiteral.contents()) {
        values.push_back(arg->accept(*this));
    }

    return Value(values);
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

        // TODO: FIGURE OUT
        if(value.toString() != "NIL") { return value; }
    }

    // Return empty value
    return Value();
}


Value ProgramVisitor::visit(ForLoop& forLoop) {
    Value value;

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
        throw std::runtime_error("Incorrect number of arguments for function \"" + functionCall.callee() + "\"");
    }

    // Add each argument as a local variable with value passed through call
    // foo(x,y,z) ::: foo(1,2+3,4) => pushes x=1, y=5, z=4
    for(int i = 0; i < (int)funcArgs.size(); ++i) {
        this->m_context->bindLocalVariable(funcArgs[i], callArgs[i]->accept(*this));
    }

    // Visit the Callable
    Value v = func->accept(*this);

    // Pop the scope
    this->m_context->popScope();

    return v;
}

Value ProgramVisitor::visit(AssignmentStatement& assignmentStatement) {
    std::string id = assignmentStatement.id();
    Value init = assignmentStatement.rhs()->accept(*this);

    this->m_context->updateVariable(id, init);

    return Value();
}

Value ProgramVisitor::visit(VariableDeclaration& variableDeclaration) {
    std::string id = variableDeclaration.id();
    ASTNode *init = variableDeclaration.init();

    Value v = init->accept(*this);
    
    if(variableDeclaration.isLocal()) {
        this->m_context->bindLocalVariable(id, v);
    }

    else {
        this->m_context->bindGlobalVariable(id, v);
    }

    return Value();
}

Value ProgramVisitor::visit(FunctionDeclaration& functionDeclaration) {
    UserFunction* userFunc = \
    new UserFunction(functionDeclaration.args(), functionDeclaration.body());

    this->m_context->bindFunction(functionDeclaration.id(), userFunc);

    return Value();
}