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
        
        case Operator::NOT:
            if(lhs.type() == Value_Type::NUMBER) {
                result = !lhs.asNumber();
            } else {
                result = Value(0);
            }
        break;

    }

    if(result.type() == Value_Type::ERR) {
        throw std::runtime_error("Incorrect types on values: " + lhs.toString() + " and " + rhs.toString());
    }

    return result;
}

// Index expression
Value ProgramVisitor::visit(IndexExpression& indexExpression) {
    Value lhs = indexExpression.lhs()->accept(*this);
    Value rhs = indexExpression.rhs()->accept(*this);

    // Check if index is valid
    if(rhs.type() != Value_Type::NUMBER) {
        throw std::runtime_error("Cannot index with value: " + rhs.toString());
    }

    // If the lhs is a list
    if(lhs.type() == Value_Type::LIST) {
        int index = rhs.asNumber();
        index = (index >= 0) ? index : lhs.asList().size() + index;

        if(index >= (int)lhs.asList().size() || index < 0) {
            throw std::runtime_error("Invalid index");
        }

        return Value(lhs.asList()[index]);
    }

    // If the lhs is a string
    else if(lhs.type() == Value_Type::STRING) {
        int index = rhs.asNumber();
        index = (index >= 0) ? index : lhs.asString().size() + index;

        if(index >= (int)lhs.asString().size() || index < 0) {
            throw std::runtime_error("Invalid index");
        }

        char val = (char)lhs.asString()[index];
        
        return Value(std::string(1, val));
    }

    // Else its not indexable
    else {
        throw std::runtime_error("Cannot index this type");
    }
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
    // This is the only statement that can return something
    // on it's own, values can only be returned here at the
    // statement level
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

        if(value.type() != Value_Type::NIL) {
            return value;
        }
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

        if(value.type() != Value_Type::NIL) { break; }
        
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

    // Check if the functioncall is a statement or an expression
    // this distinction is necessary because one expresses values
    // and the other, although it may return one, does not
    if(functionCall.isStatement()) {
        return Value();
    }
    
    return v;
}

Value ProgramVisitor::visit(AssignmentStatement& assignmentStatement) {
    std::string id = assignmentStatement.id();
    Value init = assignmentStatement.rhs()->accept(*this);
    
    if(assignmentStatement.indexes().size() != 0) {
        std::vector<int> indexes;
        Value temp;

        // Don't need to check for nullptr b/c I already do that in the 
        // parser
        for(int i = 0; i < (int)assignmentStatement.indexes().size(); i++) {
            temp = assignmentStatement.indexes()[i]->accept(*this);

            if(temp.type() != Value_Type::NUMBER) {
                throw std::runtime_error("Indexes can only be numbers, got " + temp.toString());
            }

            else {
                indexes.push_back(temp.asNumber());
            }
        }
        this->m_context->updateIndex(id, init, indexes);
    }

    else {
        this->m_context->updateVariable(id, init);
    }

    return Value();
}

Value ProgramVisitor::visit(VariableDeclaration& variableDeclaration) {
    std::string id = variableDeclaration.id();
    ASTNode *init = variableDeclaration.init();

    Value v;
    
    if(init != nullptr) {
        v = init->accept(*this);
    }
    
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

Value ProgramVisitor::visit(WhileLoop& whileLoop) {
    Value value;

    this->m_context->pushScope();

    // While test != False (0)
    while(whileLoop.test()->accept(*this).asNumber() != false) {
        
        // Execute body and set return value
        value = whileLoop.block()->accept(*this);
        
        // Check if something has been returned
        if(value.type() != Value_Type::NIL) { break; }
    }

    this->m_context->popScope();
    
    return value;
}

Value ProgramVisitor::visit(ConditionalStatement& conditionalStatement) {
    // Starting index
    int index = -1;

    // Cursor index for tests
    int cursor = 0;
    
    // Loop through tests, checking if any of them are true
    for(ASTNode* test : conditionalStatement.tests()) {
        Value res = test->accept(*this);

        // If one is true, set the index and break
        if(res.asNumber() == 1) {
            index = cursor;
            break;
        }
        cursor += 1;
    }

    // Output value
    Value outVal;

    // If one of the tests was found
    if(index != -1) {
        // Push a new scope
        this->m_context->pushScope();
        
        // Get the block
        Block* runBlock = conditionalStatement.blocks()[index];

        // Run the block
        outVal = runBlock->accept(*this);

        // Pop scope
        this->m_context->popScope();
    }

    // Else if no tests were true and there's an else block
    else if(index == -1 
            && conditionalStatement.tests().size() < conditionalStatement.blocks().size()) {
        
        // Push a new scope
        this->m_context->pushScope();

        // Get the last block
        Block* runBlock = conditionalStatement.blocks().back();

        // Run the block
        outVal = runBlock->accept(*this);

        // Pop scope
        this->m_context->popScope();
    }

    // Return the value
    return outVal;
}