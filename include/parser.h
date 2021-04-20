#ifndef PARSER_H
#define PARSER_H
#include "AST.h"
#include "lexer.h"

#include <iostream>
#include <sstream>

class Parser {
    std::string m_program;
    std::vector<Token> m_tokens;
    Token m_current, m_next;
    int index = 0;
    
    bool m_error;

    public:
    Parser(std::vector<Token> tokens, std::string program) {
        m_tokens = tokens;
        m_error = false;
        m_program = program;
    }
    
    ~Parser() {}

    /* Raise syntactical error */
    void raiseError(std::string err) {
        int line = this->m_current.line();

        std::stringstream ss(this->m_program);
        std::string line_contents;

        for(int i = 0; i < line; i ++) {
            std::getline(ss, line_contents, '\n');
        }

        std::cout << "Error: " << err << "\n";
        std::cout << "  " << line << " | " << line_contents << "\n\n";

        this->m_error = true;
    }

    /* Check if error */
    bool error() { return this-> m_error; }

    /* Check if assignment operator */
    bool isAssignmentOp() {
        return this->m_next.raw() == "=" || this->m_next.raw() == "+=" \
            || this->m_next.raw() == "-=" || this->m_next.raw() == "*=";
    }

    /* Advance to next token */
    void advance() {
        if(this->m_next.type() != Token_Type::END_FILE) {
            this->m_current = this->m_tokens[index];
            this->m_next = this->m_tokens[index + 1];
            index++;
        }

        else {
            this->m_current = this->m_next;
        }
    }

    /* Accept / reject token raw */
    bool accept(std::string tt) {
        if((m_next.type() != Token_Type::END_FILE) && (m_next.raw() == tt)) {
            advance();
            return true;
        }

        return false;
    }

    /* Accept / reject token type */
    bool acceptType(Token_Type tt) {
        if((m_next.type() != Token_Type::END_FILE) && (m_next.type() == tt)) {
            advance();
            return true;
        }

        return false;
    }

    /* Expect token string otherwhise raiseError */
    void expect(std::string tt) {
        if(!accept(tt)) {
            raiseError("Expected " + tt);
            this->m_error = true;
        }
    }

    bool notEOF() {
        return this->m_current.type() != Token_Type::END_FILE;
    }

    /* Parse program */
    Block* parse() {
        this->m_next = m_tokens[0];
        Block* body = makeProgram();
        return body;
    }

    /* Parse collection of statements */
    Block* makeProgram() {
        std::vector<ASTNode*> block;

        while(this->m_next.type() != Token_Type::END_FILE) {
            block.push_back(makeStatement());
        }
        
        Block* retBlock = new Block(block);

        return retBlock;
    }

    /* Parse statement */
    ASTNode* makeStatement() {

        /* Identifier branch */
        if(acceptType(IDENTIFIER)) {
            
            /* Get identifier */
            std::string id = m_current.raw();

            /* If '(' is accepted, it's a function call */
            if(accept("(")) {
                // Make a function that deals with args
                std::vector<ASTNode*> args = makeArgs();                
                expect(")");
                expect(";");

                return new FunctionCall(id, args);
            }

            else if (isAssignmentOp() || this->m_next.raw() == "[") {
                ASTNode* assign = makeAssignmentStatement();
                expect(";");
                return assign;
            }
        
            /* Otherwhise, it's an unexpected identifier */
            else {
                raiseError("Unexpected identifer: " + this->m_current.raw());
                return nullptr;
            }
        }

        /* Variable declaration branch */
        else if(acceptType(Token_Type::LOCAL) || acceptType(Token_Type::GLOBAL)) {
            ASTNode* varDec = makeVariableDeclaration();
            expect(";");

            return varDec;
        }

        /* While loop branch */
        else if(acceptType(Token_Type::WHILE)) {
            expect("(");
            
            ASTNode* test = expr();
            if(test == nullptr) {
                raiseError("Expected expression for while loop");
            }

            expect(")");
            
            // Block to hold statements
            std::vector<ASTNode*> block;

            // Expect beginning of do block
            expect("do");

            while(!accept("end") && notEOF()) {
                block.push_back(makeStatement());
            }

            return new WhileLoop(test, new Block(block));
        }

        /* For loop branch */
        else if(acceptType(Token_Type::FOR)) {
            // Get past first open parenthesis
            expect("(");
            advance();

            // Get for-loop initializer
            ASTNode* init = makeVariableDeclaration();
            expect(";");
            
            // Get loop test
            ASTNode* test = expr();
            expect(";");
            advance();

            // Get loop update
            ASTNode* update = makeAssignmentStatement();
            expect(")");

            // Block to hold statements
            std::vector<ASTNode*> block;

            // Expect beginning of do block
            expect("do");

            // TODO: SEE IF YOU WANT TO DO EXPECT BASED HERE
            while(!accept("end") && notEOF()) {
                block.push_back(makeStatement());
            }

            return new ForLoop(init, update, test, new Block(block));
        }
        
        /* Defun function */
        else if (acceptType(Token_Type::DEFUN)) {
            // Get identifier and advance
            std::string id = this->m_next.raw();
            advance();

            expect("(");

            // Get identifier arguments
            std::vector<std::string> args;

            if(!accept(")")) {
                if(this->m_next.type() == Token_Type::IDENTIFIER) {
                    args.push_back(this->m_next.raw());
                    advance();
                }

                while(accept(",")) {
                    if(this->m_next.type() == Token_Type::IDENTIFIER) {
                        args.push_back(this->m_next.raw());
                        advance();
                    }

                    else {
                        raiseError("Expected Identifier");
                    }
                }
                expect(")");
            }

            expect("as");
            
            std::vector<ASTNode*> block;
            while(!accept("end") && notEOF()) {
                block.push_back(makeStatement());
            }

            return new FunctionDeclaration(id, args, new Block(block));
        }

        else if(acceptType(Token_Type::RETURN)) {
            ASTNode* argument = expr();
            expect(";");

            if(argument == nullptr) {
                raiseError("Exprected expression to return");
            }
            
            return new ReturnStatement(argument);
        }

        /* If statement branch */

        else if(acceptType(Token_Type::IF)) {
            expect("(");
            std::vector<ASTNode*> tests;
            std::vector<Block*> blocks;
            std::vector<ASTNode*> block;

            ASTNode* test = expr();

            // Test for nullptr
            if(test == nullptr) {
                raiseError("Expected expression in if statement");
            }

            tests.push_back(test);
            expect(")");
            expect("then");
            
            // While we haven't encountered an "end" or "else"
            while(!accept("end") && !accept("else") && notEOF()) {
                // Push a new statement
                block.push_back(makeStatement());

                // Check for "elif"
                if(accept("elif")) {
                    // If elif, push the new block and clear the temp block
                    blocks.push_back(new Block(block));
                    block.clear();

                    // Expect ( expr ) then
                    expect("(");

                    // Get inner expression and check for null
                    test = expr();
                    if(test == nullptr) {
                        raiseError("Expected expression in if statement");
                    }

                    tests.push_back(test);
                    expect(")");
                    expect("then");
                }
            }

            // If there's an else clause, add it
            if (this->m_current.raw() == "else") {
                blocks.push_back(new Block(block));
                block.clear();

                while(!accept("end") && notEOF()) {
                    block.push_back(makeStatement());
                }
            }

            blocks.push_back(new Block(block));
            return new ConditionalStatement(tests, blocks);

        }
        /* Otherwhise, it's an unexpected token and shouldn't be there */
        else {
            advance();
            raiseError("Unexpected token: " + this->m_current.raw());

            return nullptr;
        }
    }
    
    /* Make variableDeclaration AST node */
    ASTNode* makeVariableDeclaration() {
        // Check if local or global
        bool islocal = this->m_current.raw() == "local";

        // TODO: Add support for forward delcaration local x;
        
        // If accept identifier, parse out the statement and return the node
        if(acceptType(IDENTIFIER)) {
            std::string id = this->m_current.raw();
            expect("=");
            ASTNode *expression = expr();

            return new VariableDeclaration(id, expression, islocal);
        }

        return nullptr;
    }

    /* Make assignmentStatement AST node */
    ASTNode* makeAssignmentStatement() {
        bool isIndex = false;
        ASTNode* index;
        std::string op;

        // Get the identifier
        std::string id = this->m_current.raw();

        if(accept("[")) {
            // Check off that an indexexpression is coming
            isIndex = true;

            // Get the index and expect a closing bracker
            index = expr();
            expect("]");

            // Get the operator and advance
            op = this->m_next.raw();
            advance();
            
            // Check if the index is null
            if(index == nullptr) {
                raiseError("Expected expression in index");
            }
        } else {
            advance();
            // Get the operator
            op = this->m_current.raw();
        }

        // Get the expression
        ASTNode* expression = expr();

        // Decide what to do with the operator
        if(op == "+=") {
            ASTNode *addExpr = new BinaryExpression(new Identifier(id), expression, Operator::ADD);

            if(isIndex) {
                return new AssignmentStatement(id, addExpr, index);
            }

            return new AssignmentStatement(id, addExpr);
        }

        else if (op == "-=") {
            ASTNode *subExpr = new BinaryExpression(new Identifier(id), expression, Operator::SUBTRACT);
            
            if(isIndex) {
                return new AssignmentStatement(id, subExpr, index);
            }

            return new AssignmentStatement(id, subExpr);
        }
        
        else if (op == "*=") {
            ASTNode *multExpr = new BinaryExpression(new Identifier(id), expression, Operator::MULTIPLY);

            if(isIndex) {
                return new AssignmentStatement(id, multExpr, index);
            }

            return new AssignmentStatement(id, multExpr);
        }

        // Basic "=" operator
        if(isIndex) {
            return new AssignmentStatement(id, expression, index);
        }

        return new AssignmentStatement(id, expression);
    }

    /* Make arguments (Function call or array) */
    std::vector<ASTNode*> makeArgs() {
        // Argument holder
        std::vector<ASTNode*> args = {};

        // Get first expression if available
        ASTNode* expression = expr();

        // Push first expression
        if(expression != nullptr) {
            args.push_back(expression);
        }
        
        // While there's still ",", push a new argument
        while(accept(",") && notEOF()) {
            ASTNode* expression = expr();

            if(expression != nullptr) {
                args.push_back(expression);
            }

            else {
                raiseError("Expected expression");
            }
        }

        return args;
    }
/*
 else if (op == "and") {
                exprval = new BinaryExpression(exprval, right, Operator::AND);
            } else if (op == "or") {
                exprval = new BinaryExpression(exprval, right, Operator::OR);
            }
            */

    /* Expression parser */
    ASTNode* expr() {
        ASTNode* subval = subexpr();

        while(accept("and") || accept("or")) {
            // Make a check on the lhs
            if(subval == nullptr) {
                raiseError("Expected term before binary operator");
            }

            // Get the operator 
            std::string op = m_current.raw();

            // Get the right term
            ASTNode* right = subexpr();

            if(right == nullptr) {
                raiseError("Expected term after binary operator");
            }

            if (op == "and") {
                subval = new BinaryExpression(subval, right, Operator::AND);
            } else if (op == "or") {
                subval = new BinaryExpression(subval, right, Operator::OR);
            }
        }

        return subval;
    }

    ASTNode* subexpr() {
        // Get first term  
        ASTNode* exprval = term();

        // Accept the binary opertor if available
        while(accept("+")     || accept("-")  || accept("==") || accept("!=") 
              || accept("<=") || accept(">=") || accept(">")  || accept("<")
              || accept("%")) {

            // Make a check on the lhs
            if(exprval == nullptr) {
                raiseError("Expected term before binary operator");
            }

            // Get the operator 
            std::string op = m_current.raw();

            // Get the right term
            ASTNode* right = term();

            if(right == nullptr) {
                raiseError("Expected term after binary operator");
            }

            // Figure out what to do with the operator
            /* MIGHT REFACTOR THIS LATER B/C STRING COMPS */
            if(op == "+") {
                exprval = new BinaryExpression(exprval, right, Operator::ADD);
            } else if(op == "-") {
                exprval = new BinaryExpression(exprval, right, Operator::SUBTRACT);
            } else if (op == "==") {
                exprval = new BinaryExpression(exprval, right, Operator::EQUALS);
            } else if (op == "!=") {
                exprval = new BinaryExpression(exprval, right, Operator::NOTEQUAL);
            } else if (op == "<=") {
                exprval = new BinaryExpression(exprval, right, Operator::LESSEQUAL);
            } else if (op == ">=") {
                exprval = new BinaryExpression(exprval, right, Operator::GREATEQUAL);
            } else if (op == ">") {
                exprval = new BinaryExpression(exprval, right, Operator::GREATERTHAN);
            } else if (op == "<") {
                exprval = new BinaryExpression(exprval, right, Operator::LESSTHAN);
            } else if (op == "%") {
                exprval = new BinaryExpression(exprval, right, Operator::MOD);
            }
        }

        // Return the expression
        return exprval;
    }

    /* Term level parser */
    ASTNode* term() {
        // Get factor
        ASTNode* termval = factor();

        // If accept operator
        while(accept("*") || accept("/")) {
            if(termval == nullptr) {
                raiseError("Expected term before binary operator");
            }

            // Get the operator and rhs
            std::string op = m_current.raw();
            ASTNode* right = factor();

            if(right == nullptr) {
                raiseError("Expected term after binary operator");
            }

            // Figure out what to do with the operator
            if(op == "*") {
                termval = new BinaryExpression(termval, right, Operator::MULTIPLY);
            } else if(op == "/") {
                termval = new BinaryExpression(termval, right, Operator::DIVIDE);
            }
        }

        return termval;
    }

    
    /* Factor level expression parsing */
    ASTNode* factor() {
        // Get the subfactor
        ASTNode* subfact = subfactor();

        // See if there's an index expression
        while(accept("[")) {
            ASTNode* rhs = expr();
            expect("]");
            
            if(subfact == nullptr) {
                raiseError("Expected expression before index");    
            }

            if(rhs == nullptr) {
                raiseError("Exprected expression in index");
            }

            subfact = new IndexExpression(subfact, rhs);
        }

        return subfact;
    }

    ASTNode* subfactor() {
        // If accept a number literal, make one
        if(acceptType(NUM_LIT)) {
            return new NumberLiteral(std::stod(m_current.raw()));
        }

        // If accept a string literal, make one
        else if (acceptType(STR_LIT)) {
            // Get the string literal value
            std::string raw = m_current.raw();
            
            return new StringLiteral(raw);
        }

        // Make a subexpression when encountering a "(" at the factor level
        else if (accept("(")) {
            ASTNode* exprval = expr();
            expect(")");
            return exprval;
        }

        // Make a list factor if encountering a "["
        else if (accept("[")) {
            std::vector<ASTNode*> args = makeArgs();
            expect("]");

            return new ArrayLiteral(args);
        }

        // Figure out what to do with an identifier
        else if(acceptType(IDENTIFIER)) {
            // Get the identifier
            std::string id = m_current.raw();
            
            // If encounter a "(", it's a function call
            if(accept("(")) {
                std::vector<ASTNode*> args = makeArgs();
                expect(")");

                return new FunctionCall(id, args);
            }
            
            // Otherwhise the factor is just an identifier
            else {
                return new Identifier(id);
            }
        }
        
        // Otherwhise, you've encoutnered a unary expression, which in this case is
        // only negation
        else if (accept("-")) {
            // This needs to be factor as to just negate the next factor
            ASTNode* exprval = factor();
            
            return new BinaryExpression(exprval, new NumberLiteral(-1), Operator::MULTIPLY);
        }

        // If nothing has been fulfilled, return a nullptr
        return nullptr;
    }
};
#endif // PARSER_H