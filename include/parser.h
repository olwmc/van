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
        return this->m_next.raw() == "=" || this->m_next.raw() == "+=";
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
        Block* body = makeBlock();
        return body;
    }

    /* Parse collection of statements */
    Block* makeBlock() {
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
                return new FunctionCall(id, args );
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
            // TODO: write makeDoBlock(); 
            expect("do");

            while(this->m_next.type() != Token_Type::END) {
                block.push_back(makeStatement());
            }
            expect("end");

            return new ForLoop(init, update, test, new Block(block));
        }

        /* Otherwhise, it's an unexpected token and shouldn't be there */
        else {
            advance();
            raiseError("Unexpected token: " + this->m_current.raw());

            return nullptr;
        }
    }
    
    ASTNode* makeVariableDeclaration() {
        bool islocal = this->m_current.raw() == "local";

        if(acceptType(IDENTIFIER)) {
            std::string id = this->m_current.raw();
            expect("=");
            ASTNode *expression = expr();

            return new VariableDeclaration(id, expression, islocal);
        }

        return nullptr;
    }

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

    std::vector<ASTNode*> makeArgs() {
        std::vector<ASTNode*> args = {};
        ASTNode* expression = expr();

        if(expression != nullptr) {
            args.push_back(expression);
        }
        
        while(accept(",")) {
            // TODO CHECK FOR NULLPTR
            args.push_back(expr());
        }
        
        return args;
    }

    ASTNode* expr() {
        ASTNode* exprval = term();
        while(accept("+") || accept("-") || accept("==") || accept("!=") || accept("<=") || accept(">=")) {
            if(exprval == nullptr) {
                std::cout << "Expected term before binary operator\n";
                this->m_error = true;
            }
            std::string op = m_current.raw();
            ASTNode* right = term();

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
            }
        }

        return exprval;
    }

    ASTNode* term() {
        ASTNode* termval = factor();
        while(accept("*") || accept("/")) {
            std::string op = m_current.raw();
            ASTNode* right = factor();

            if(op == "*") {
                termval = new BinaryExpression(termval, right, Operator::MULTIPLY);
            } else if(op == "/") {
                termval = new BinaryExpression(termval, right, Operator::DIVIDE);
            }
        }

        return termval;
    }

    ASTNode* factor() {
        if(acceptType(NUM_LIT)) {
            return new NumberLiteral(std::stod(m_current.raw()));
        }

        else if (acceptType(STR_LIT)) {
            return new StringLiteral(m_current.raw());
        }

        // TODO: Fix hanging )
        else if (accept("(")) {
            ASTNode* exprval = expr();
            expect(")");
            return exprval;
        }

        /*
        else if (accept("[")) {
            std::vector<ASTNode*> args = makeArgs();
            expect("]");

            return new ArrayLiteral(args);
        }
        */

        else if(acceptType(IDENTIFIER)) {
            std::string id = m_current.raw();

            if(accept("(")) {
                std::vector<ASTNode*> args = makeArgs();
                expect(")");
                
                return new FunctionCall(id, args);
            }

            /*
            else if(accept("[")) {
                
            }
            */
            else {
                return new Identifier(id);
            }
        }
        
        else if (accept("-")) {
            // This needs to be factor as to just negate the next factor
            ASTNode* exprval = factor();
            
            return new BinaryExpression(exprval, new NumberLiteral(-1), Operator::MULTIPLY);
        }

        return nullptr;
    }
};
#endif // PARSER_H