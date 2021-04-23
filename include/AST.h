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
        ASTNode() {}
        virtual ~ASTNode() {}

        virtual void toJson() = 0;

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

        virtual void toJson() override {
            std::cout <<  "{\"numericLiteral\":" << this->m_value << "}"; 
        }        
};

/* Array literal class */
class ArrayLiteral : public ASTNode {
    std::vector<ASTNode*> m_contents;

    public:
        ArrayLiteral(std::vector<ASTNode*> contents) : m_contents(contents) {}
        
        ~ArrayLiteral() {
            for(ASTNode* val : this->m_contents) {
                delete val;
            }
        }

        virtual Value accept(ProgramVisitor& visitor) override {
            return visitor.visit(*this);
        };

        std::vector<ASTNode*> contents() { return this->m_contents; };

        virtual void toJson() override {
            std::cout <<  "{\"arrayLiteral\": [";

            for(int i = 0; i < (int)m_contents.size() - 1; i++) {
                m_contents[i]->toJson();
                std::cout << ",";
            }
            
            if(m_contents.size() > 0) {
                m_contents.back()->toJson();
            }
            std::cout << "]}";
        }     
};

/* String literal class */
class StringLiteral : public ASTNode {
    std::string m_value;

    public:
        StringLiteral(std::string val) : m_value(val) {}

        virtual Value accept(ProgramVisitor& visitor) override {
            return visitor.visit(*this);
        }

        std::string value() { return this->m_value; }

        virtual void toJson() override {
            std::cout <<  "{\"stringLiteral\": \"";
            for(char c : m_value) {
                switch(c) {
                    case '\n': std::cout << "(newline)"; break;
                    case '\t': std::cout << "(tab)"; break;
                    default:
                        std::cout << c;
                } 
            }

            std::cout << "\"}";
        }   
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

        virtual void toJson() override {
            std::cout <<  "{\"identifier\": { \"id\": \"" << this->m_name << "\"}}";
        }  
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

        virtual void toJson() override {
            std::cout <<  "{\"binaryExpression\": {\"lhs\":";
            m_lhs->toJson(); std::cout << ",\"rhs\":";
            m_rhs->toJson(); std::cout << ",\"op\":" << this->m_op;
            std::cout << "}}";
        }  
};

/* Index Expression class */
class IndexExpression : public ASTNode {
    ASTNode *m_lhs, *m_rhs;

    public:
        IndexExpression(ASTNode* lhs, ASTNode* rhs)
            : m_lhs(lhs), m_rhs(rhs) {}
        
        ~IndexExpression() {
            delete this->m_lhs;
            delete this->m_rhs;
        }

        virtual Value accept(ProgramVisitor& visitor) override {
            return visitor.visit(*this);
        }

        ASTNode* lhs()  { return this->m_lhs; }
        ASTNode* rhs()  { return this->m_rhs; }

        virtual void toJson() override {
            std::cout << "{\"indexExpression\":{ \"lhs\":";
            m_lhs->toJson(); std::cout << ",\"arg\":";
            m_rhs->toJson(); std::cout << "}}";
        }

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

        virtual void toJson() override {
            std::cout <<  "{\"block\": [";

            for(int i = 0; i < (int)m_body.size() - 1; i++) {
                m_body[i]->toJson();
                std::cout << ",";
            }

            if(m_body.size() > 0) {
                m_body.back()->toJson();
            }
            std::cout << "]}";
        } 

};


class ReturnStatement: public ASTNode {
    ASTNode *m_argument;

    public:
        ReturnStatement(ASTNode *argument) : m_argument(argument) {}

        ~ReturnStatement() {
            delete this->m_argument;
        }
        virtual Value accept(ProgramVisitor& visitor) override {
            return visitor.visit(*this);
        };

        ASTNode *argument() { return this->m_argument; }

        virtual void toJson() override {
            std::cout <<  "{\"returnStatement\": { \"arg\":";
            m_argument->toJson();
            std::cout << "}}";
        }
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

        virtual void toJson() override {
            std::cout <<  "{\"forLoop\": { \"init\":";
            m_init->toJson();   std::cout << ", \"update\":";
            m_update->toJson(); std::cout << ", \"test\":";
            m_test->toJson();   std::cout << ", \"body\":";
            m_block->toJson();  std::cout << "}}";
        }
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

        virtual void toJson() override {
            std::cout <<  "{\"functionCall\": {\"callee\": \"" \
                      << m_callee << "\", \"args\":[";

            for(int i = 0; i < (int)m_args.size() - 1; i++) {
                m_args[i]->toJson();
                std::cout << ",";
            }

            if(m_args.size() > 0) {
                m_args.back()->toJson();
            }
            std::cout << "]}}";
        }
};

class AssignmentStatement : public ASTNode {
    std::string m_id;
    ASTNode* m_rhs;
    ASTNode* m_index;

    public:
        AssignmentStatement(std::string id, ASTNode* rhs) 
            : m_id(id), m_rhs(rhs), m_index(nullptr) {}
        
        AssignmentStatement(std::string id, ASTNode* rhs, ASTNode* index) 
            : m_id(id), m_rhs(rhs), m_index(index) {}
        
        ~AssignmentStatement() {
            delete this->m_rhs;
            delete this->m_index;
        }
        virtual Value accept(ProgramVisitor& visitor) override {
            return visitor.visit(*this);
        }

        std::string id() { return this->m_id;  }
        ASTNode* rhs()   { return this->m_rhs; }
        ASTNode* index() { return this->m_index; }

        virtual void toJson() override {
            std::cout <<  "{\"assignmentStatement\": { \"id\":";
            std::cout << "\"" << m_id << "\"";
            std::cout << ", \"rhs\":";
            m_rhs->toJson(); 
            
            if(m_index != nullptr) {
                std::cout << ", \"index\":";
                m_index->toJson();
            }
            std::cout << "}}";
        }
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

        virtual void toJson() override {
            std::cout <<  "{\"varDeclaration\": { \"id\":";
            std::cout << "\"" << m_id << "\"";
            
            if(m_init != nullptr) {
                std::cout << ", \"init\":";
                m_init->toJson(); 
            }

            std::cout << ", \"isLocal\":";
            std::cout << m_local << "}}";
        }
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

        virtual void toJson() override {
            std::cout <<  "{\"funcDeclaration\": { \"id\":";
            std::cout << "\"" << m_id << "\", ";
            std::cout << "\"args\": [";

            for(int i = 0; i < (int)m_args.size() - 1; i++) {
                std::cout << "\"" << m_args[i] << "\", ";
            }

            if(m_args.size() > 0) {
                std::cout << "\"" << m_args.back() << "\"";
            }

            std::cout << "], \"body\": ";
            m_body->toJson();
            std::cout << "}}";
        }
};

class WhileLoop : public ASTNode {
    ASTNode *m_test;
    Block *m_block;

    public:
        WhileLoop(ASTNode *test, Block* block)
            : m_test(test), m_block(block) {}
        
        ~WhileLoop() {
            delete this->m_test;
            delete this->m_block;
        }
        virtual Value accept(ProgramVisitor& visitor) override {
            return visitor.visit(*this);
        };

        ASTNode *test()    { return this->m_test;   }
        Block* block()     { return this->m_block;  }

        virtual void toJson() override {
            // std::cout << "{\"whileLoop\": "
        }
};

class ConditionalStatement : public ASTNode {
    std::vector<ASTNode*> m_tests;
    std::vector<Block*> m_blocks;

    public:
        ConditionalStatement(std::vector<ASTNode*> tests, std::vector<Block*> blocks)
        : m_tests(tests), m_blocks(blocks) {}

        ~ConditionalStatement() {
            for(ASTNode* test : this->m_tests) {
                delete test;
            }

            for(Block* block : this->m_blocks) {
                delete block;
            }
        }
        
        virtual Value accept(ProgramVisitor& visitor) override {
            return visitor.visit(*this);
        };
        
        std::vector<ASTNode*> tests() { return this->m_tests;  }
        std::vector<Block*> blocks()  { return this->m_blocks; }

        virtual void toJson() override {}

};
#endif /* AST_H */