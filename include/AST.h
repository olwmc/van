/* HOW TO ADD A NEW ASTNODE

1. FORWARD DECLARE IT WITH THE OTHERS
2. ADD A VISITOR METHOD 
3. CREATE THE CLASS
    ORDER OF 0) MEMBERS 1) CONSTRUCTOR 2) ACCEPTOR 3) HELPERS
3. IMPLEMENT THAT VISITOR METHOD
*/

#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "value.h"

class NumberLiteral;
class StringLiteral;
class BinaryExpression;
class Block;
class ForLoop;
class ReturnStatement;

/* ##################################################################################### */

/* AST Visitor "executes" each node */
class ASTVisitor {
    std::string m_context;
    // error handler
    // Actual context

    public:
        ASTVisitor(std::string c) : m_context(c) {}
        Value visit(NumberLiteral& numberLiteral);
        Value visit(BinaryExpression& binaryExpression);
        Value visit(StringLiteral& stringLiteral);
        Value visit(ReturnStatement& returnStatement);
        Value visit(ForLoop& forLoop);
};

/* ##################################################################################### */

/* Used for arithmetic expressions */
enum Operator {
    ADD, SUBTRACT, MULTIPLY, DIVIDE,
    
    EQUALS, NOTEQUAL, AND, OR
};

/* AST Node abstract class */
class ASTNode {
    int m_index, m_lineNum;
    
    public:
        virtual Value accept(ASTVisitor& visitor) = 0;

        int index()     { return this->m_index;   }
        int lineNum()   { return this->m_lineNum; }
        bool isReturn() { return this->m_return;  }
        
        void setIndexAndLine(int index, int line) {
            this->m_index = index;
            this->m_lineNum = line;
        }

    protected:
        bool m_return = false;
    
};

/* Number literal class */
class NumberLiteral : public ASTNode {
    double m_value;

    public:
        NumberLiteral(double val) : m_value(val) {}
    
        virtual Value accept(ASTVisitor& visitor) override {
            return visitor.visit(*this);
        };

        double value() { return this->m_value; };
};

/* String literal class */
class StringLiteral : public ASTNode {
    std::string m_value;

    public:
        StringLiteral(std::string val) : m_value(val) {}

        virtual Value accept(ASTVisitor& visitor) override {
            return visitor.visit(*this);
        }

        std::string value() { return this->m_value; }
};

/* BinaryExpression class */
class BinaryExpression : public ASTNode {
    ASTNode *m_lhs, *m_rhs;
    Operator m_op;
    
    public:
        BinaryExpression(ASTNode* l, ASTNode* r, Operator op)
            : m_lhs(l), m_rhs(r), m_op(op) {}

        virtual Value accept(ASTVisitor& visitor) override {
            return visitor.visit(*this);
        };

        ASTNode* getLhs()   { return this->m_lhs; }
        ASTNode* getRhs()   { return this->m_rhs; }
        Operator getOp()    { return this->m_op;  }
};

/* Block class (Denotes beginning of new scope) */
class Block : ASTNode{
    std::vector<ASTNode*> m_body;

    public:
        Block(std::vector<ASTNode*> body) : m_body(body) {}
        std::vector<ASTNode*> body() { return this->m_body; }
};


class ReturnStatement: public ASTNode {
    ASTNode *m_argument;

    public:
        ReturnStatement(ASTNode *argument) : m_argument(argument) {
            // Set return bool
            m_return = true;
        }

        virtual Value accept(ASTVisitor& visitor) override {
            return visitor.visit(*this);
        };

        ASTNode *argument() { return this->m_argument; }
};


class ForLoop : public ASTNode {
    /* Initializer (local i = 0)
     * Update (i++)
     * Test (i < n)
     * Block (Statements in body)
     *  * Is type: "Block", therefore only run the body of the block 
     */
    ASTNode *m_init, *m_update, *m_test;
    Block* m_block;

    public:
        ForLoop(ASTNode* init, ASTNode* update, ASTNode* test, Block* block)
            : m_init(init), m_update(update), m_test(test), m_block(block) {}
        
        virtual Value accept(ASTVisitor& visitor) override {
            return visitor.visit(*this);
        };

        ASTNode* test()    { return this->m_test;  }
        ASTNode* update()  { return this->m_update;  }
        Block* block()     { return this->m_block; }
};
/* ##################################################################################### */