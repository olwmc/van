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

            /* If an assignment operator is accepted, then it's an assignment statement */
            else if(isAssignmentOp()) {
                ASTNode *assign = makeAssignmentStatement();
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
            
            // TODO CHECK IF NULLPTR
            ASTNode* test = expr();

            expect(")");
            
            // Block to hold statements
            std::vector<ASTNode*> block;

            // Expect beginning of do block
            expect("do");

            while(this->m_next.type() != Token_Type::END) {
                block.push_back(makeStatement());
            }
            expect("end");

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

            while(this->m_next.type() != Token_Type::END) {
                block.push_back(makeStatement());
            }
            expect("end");

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
            while(!accept("end")) {
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

            // TODO nullptr check
            tests.push_back(expr());
            expect(")");
            expect("then");

            while(!accept("end") && !accept("else")) {
                block.push_back(makeStatement());
            }

            if (this->m_current.raw() == "else") {
                blocks.push_back(new Block(block));
                block.clear();

                while(!accept("end")) {
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
        // Get the identifier
        std::string id = this->m_current.raw();
        advance();

        // Get the operator
        std::string op = this->m_current.raw();

        // Get the expression
        ASTNode* expression = expr();

        // Decide what to do with the operator
        if(op == "+=") {
            ASTNode *addExpr = new BinaryExpression(new Identifier(id), expression, Operator::ADD);
            return new AssignmentStatement(id, addExpr);
        }

        else if (op == "-=") {
            ASTNode *subExpr = new BinaryExpression(new Identifier(id), expression, Operator::SUBTRACT);
            return new AssignmentStatement(id, subExpr);
        }
        
        else if (op == "*=") {
            ASTNode *multExpr = new BinaryExpression(new Identifier(id), expression, Operator::MULTIPLY);
            return new AssignmentStatement(id, multExpr);
        }

        // Basic "=" operator
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
        while(accept(",")) {
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

    /* Expression parser */
    ASTNode* expr() {
        // Get first term  
        ASTNode* exprval = term();

        // Accept the binary opertor if available
        while(accept("+") || accept("-") || accept("==") || accept("!=") 
              || accept("<=") || accept(">=") || accept(">") || accept("<")
              || accept("%")) {

            // Make a check on the lhs
            if(exprval == nullptr) {
                raiseError("Expected term before binary operator");
            }

            // Get the operator 
            std::string op = m_current.raw();

            // Get the right term
            ASTNode* right = term();

            // Figure out what to do with the operator
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
            // Get the operator and rhs
            std::string op = m_current.raw();
            ASTNode* right = factor();

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
        // If accept a number literal, make one
        if(acceptType(NUM_LIT)) {
            return new NumberLiteral(std::stod(m_current.raw()));
        }

        // If accept a string literal, make one
        else if (acceptType(STR_LIT)) {
            // Get the string literal value
            std::string raw = m_current.raw();
            
            // If there's a "[" after a string, it's a literal index
            if(accept("[")) {
                ASTNode* argument = expr();
                expect("]");

                return new IndexExpression( new StringLiteral(raw) , argument );
            }
            
            // Otherwhise, return the stringliteral
            else {
                return new StringLiteral(raw);
            }
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

            // If a "[" is encountered after an array, it's a literal indexing
            if(accept("[")) {
                ASTNode* argument = expr();
                expect("]");

                return new IndexExpression( new ArrayLiteral(args) , argument );            
            }

            // Otherwhise just make the arrayLiteral
            else {
                return new ArrayLiteral(args);
            }
        }

        // Figure out what to do with an identifier
        else if(acceptType(IDENTIFIER)) {
            // Get the identifier
            std::string id = m_current.raw();
            
            // If encounter a "(", it's a function call
            if(accept("(")) {
                std::vector<ASTNode*> args = makeArgs();
                expect(")");

                // Indexing a function call
                if(accept("[")) {
                    ASTNode* argument = expr();
                    expect("]");
                    
                    return new IndexExpression(new FunctionCall(id, args), argument);
                }

                return new FunctionCall(id, args);
            }

            // If you encounter a "[", it's an index expression
            else if(accept("[")) {
                ASTNode* argument = expr();
                expect("]");

                return new IndexExpression( new Identifier(id), argument );            
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