/* HOW TO ADD A NEW ASTNODE

1. FORWARD DECLARE IT WITH THE OTHERS
2. ADD A VISITOR METHOD 
3. CREATE THE CLASS
    ORDER OF 0) MEMBERS 1) CONSTRUCTOR 2) ACCEPTOR 3) HELPERS
3. IMPLEMENT THAT VISITOR METHOD
*/

#ifndef AST_H
#define AST_H

#include "forward.h"

#include "visitor.h"
#include "value.h"
#include "context.h"

#include <iostream>
#include <string>
#include <vector>

/* Used for arithmetic expressions */
enum Operator {
    ADD, SUBTRACT, MULTIPLY, DIVIDE, MOD,
    EQUALS, LESSEQUAL, GREATEQUAL, NOTEQUAL, AND, OR,
    LESSTHAN, GREATERTHAN
};

/* AST Node abstract class */
class ASTNode {    
    public:
        virtual Value accept(ProgramVisitor& visitor) = 0;

        // String comparisons are costly, this is a simple way
        // of checking if the node is a return w/o having a type
        // string nor clogging up with type enums
        bool isReturn() { return this->m_return;  }
        ASTNode() {}
        virtual ~ASTNode() {}

    protected:
        bool m_return = false;
};

/* Number literal class */
class NumberLiteral : public ASTNode {
    double m_value;

    public:
        NumberLiteral(double val) : m_value(val) {}
    
        virtual Value accept(ProgramVisitor& visitor) override {
            return visitor.visit(*this);
        };

        double value() { return this->m_value; };
};

// /* Array literal class */
// class ArrayLiteral : public ASTNode {
//     std::vector<ASTNode*> m_contents;

//     public:
//         ArrayLiteral(std::vector<ASTNode*> contents) : m_contents(contents) {}
    
//         virtual Value accept(ProgramVisitor& visitor) override {
//             return visitor.visit(*this);
//         };

//         std::vector<ASTNode*> contents() { return this->m_contents; };
// };

/* String literal class */
class StringLiteral : public ASTNode {
    std::string m_value;

    public:
        StringLiteral(std::string val) : m_value(val) {}

        virtual Value accept(ProgramVisitor& visitor) override {
            return visitor.visit(*this);
        }

        std::string value() { return this->m_value; }
};

/* Identifier class */
class Identifier : public ASTNode {
    std::string m_name;

    public:
        Identifier(std::string name) : m_name(name) {}
        
        virtual Value accept(ProgramVisitor& visitor) override {
            return visitor.visit(*this);
        }

        std::string name() { return this->m_name; }
};

/* BinaryExpression class */
class BinaryExpression : public ASTNode {
    ASTNode *m_lhs, *m_rhs;
    Operator m_op;
    
    public:
        BinaryExpression(ASTNode *lhs, ASTNode *rhs, Operator op)
            : m_lhs(lhs), m_rhs(rhs), m_op(op) {}

        ~BinaryExpression() {
            delete m_lhs;
            delete m_rhs;
        }

        virtual Value accept(ProgramVisitor& visitor) override {
            return visitor.visit(*this);
        };

        ASTNode *lhs()   { return this->m_lhs; }
        ASTNode *rhs()   { return this->m_rhs; }
        Operator op()    { return this->m_op;  }
};

/* Block class (Denotes beginning of new scope) */
class Block : public ASTNode {
    // Maybe refactor this later to an array to decrease
    // memory usage. It doesn't really need to be a vector
    // since it's not mutated
    std::vector<ASTNode*> m_body;

    public:
        Block(std::vector<ASTNode*> body) : m_body(body) {}
        ~Block() {
            for(ASTNode* node: this->m_body) {
                delete node;
            }
        }
        virtual Value accept(ProgramVisitor& visitor) override {
            return visitor.visit(*this);
        };
        std::vector<ASTNode*> body() { return this->m_body; }
};


class ReturnStatement: public ASTNode {
    ASTNode *m_argument;

    public:
        ReturnStatement(ASTNode *argument) : m_argument(argument) {
            // Set is_return_statement flag
            m_return = true;
        }

        ~ReturnStatement() {
            delete this->m_argument;
        }
        virtual Value accept(ProgramVisitor& visitor) override {
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
    Block *m_block;

    public:
        ForLoop(ASTNode *init, ASTNode *update, ASTNode *test, Block* block)
            : m_init(init), m_update(update), m_test(test), m_block(block) {}
        
        ~ForLoop() {
            delete this->m_init;
            delete this->m_update;
            delete this->m_test;
            delete this->m_block;
        }
        virtual Value accept(ProgramVisitor& visitor) override {
            return visitor.visit(*this);
        };

        ASTNode *test()    { return this->m_test;   }
        ASTNode *init()    { return this->m_init;   }
        ASTNode *update()  { return this->m_update; }
        Block* block()     { return this->m_block;  }
};

class FunctionCall : public ASTNode {
    std::string m_callee;
    std::vector<ASTNode*> m_args;

    public:
        FunctionCall(std::string callee, std::vector<ASTNode*> args)
            : m_callee(callee), m_args(args) {}

        ~FunctionCall() {
            for(ASTNode* node : m_args) {
                delete node;
            }
        }

        virtual Value accept(ProgramVisitor& visitor) override {
            return visitor.visit(*this);
        };

        std::string callee() { return this->m_callee; }
        std::vector<ASTNode*> args()   { return this->m_args; }
};

class AssignmentStatement : public ASTNode {
    std::string m_id;
    ASTNode* m_rhs;

    public:
        AssignmentStatement(std::string id, ASTNode* rhs) 
            : m_id(id), m_rhs(rhs) {}
        
        ~AssignmentStatement() {
            delete this->m_rhs;
        }
        virtual Value accept(ProgramVisitor& visitor) override {
            return visitor.visit(*this);
        }

        std::string id() { return this->m_id;  }
        ASTNode* rhs()   { return this->m_rhs; }
};

class VariableDeclaration: public ASTNode {
    std::string m_id;
    ASTNode* m_init;
    bool m_local;

    public:
        VariableDeclaration(std::string id, ASTNode* init, bool local) 
                : m_id(id), m_init(init), m_local(local) {}

        ~VariableDeclaration() {
            delete this->m_init;
        }
        virtual Value accept(ProgramVisitor& visitor) override {
                return visitor.visit(*this);
        }

        std::string id() { return this->m_id;    }
        ASTNode* init()  { return this->m_init;  }
        bool isLocal()   { return this->m_local; }
};

class FunctionDeclaration : public ASTNode {
    std::string m_id;
    std::vector<std::string> m_args;
    Block* m_body;

    public:
        FunctionDeclaration(std::string id, std::vector<std::string> args, Block* body) 
            : m_id(id), m_args(args), m_body(body) {}

        ~FunctionDeclaration() {
            delete this->m_body;
        }

        virtual Value accept(ProgramVisitor& visitor) override {
            return visitor.visit(*this);
        }

        std::string id()                { return this->m_id;   }
        std::vector<std::string> args() { return this->m_args; }
        Block* body()                   { return this->m_body; }
};

#endif /* AST_H */