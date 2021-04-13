#ifndef INTERP_H
#define INTERP_H

#include "parser.h"
#include <string>

class Van_Interpreter {
    std::string m_program;
    Context* m_context;
    Block* m_start;

    public:
    Van_Interpreter(std::string program) {
        this->m_program = program;
        this->m_context = new Context;
    }

    ~Van_Interpreter() {
        delete this->m_context;
        delete this->m_start;
    }

    Value interpret() {
        // Lexer and tokens
        Lexer lexer(this->m_program);
        std::vector<Token> tokens = lexer.makeTokens();

        // Parsed AST
        Parser parser(tokens, this->m_program);
        m_start = parser.parse();

        // Return value
        Value v;

        // If there were no parser errors, try to run
        if(!parser.error()) {
            // Context and visitor
            this->m_context->pushScope();

            ProgramVisitor vis(this->m_context);
            
            // Accept start node (This can throw an exception error)
            v = m_start->accept(vis);
        }

        return v;
    }

    Context* context() {
        return this->m_context;
    }

    void setContext(Context* context) {
        this->m_context = context;
    }

    void run() {
        try {
            this->interpret();
        }
        catch(const std::runtime_error& error) {
            std::cout << error.what() << "\n";
        }
    }
};

#endif