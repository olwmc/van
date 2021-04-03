#include "AST.h"
#include "context.h"
#include "lexer.h"
#include "parser.h"

#include <fstream>
#include <iostream>

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
};


#include <cmath>

class builtin_Input : public Builtin {
    public:
    builtin_Input() {
        this->m_args = {};
    }

    virtual Value execute(Context& context) override {
        std::string input;
        std::getline(std::cin, input);

        return Value(input);
    }
};

class builtin_Print : public Builtin {
    public:
    builtin_Print() {
        this->m_args = {"__input__"};
    }

    virtual Value execute(Context& context) override {
        std::cout << context.resolveVariable(this->m_args[0]).toString();
        return Value();
    }
};

class builtin_NumCast : public Builtin {
    public:
    builtin_NumCast() {
        this->m_args = {"__input__"};
    }

    virtual Value execute(Context& context) override {
        std::string str = context.resolveVariable(this->m_args[0]).asString();
        double num = std::stod(str);

        return Value(num);
    }
};

int main() {
    std::ifstream infile("examples/forloop.van");
    std::string file_contents { std::istreambuf_iterator<char>(infile), std::istreambuf_iterator<char>() };
    
    Van_Interpreter interpreter(file_contents);
    interpreter.context()->addBuiltinFunction("input", new builtin_Input());
    interpreter.context()->addBuiltinFunction("print", new builtin_Print());
    interpreter.context()->addBuiltinFunction("num", new builtin_NumCast());

    try {
        interpreter.interpret();
    }
    catch(const std::runtime_error& error) {
        std::cout << error.what() << "\n";
    }
}