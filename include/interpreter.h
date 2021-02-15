/*

Going to need a static variable AST to reference 


class Interpreter {
    Parser* parser;
    ASTVisitor* vis;
    
    public:
        // The case for a static variable here is simply that the AST and the program
        // do actually work globally. They're accessed by builtins
        
        static ASTNode* ast;
        static std::string program;

        Interpreter(Parser* parser, ASTVisitor* vis);
}
*/