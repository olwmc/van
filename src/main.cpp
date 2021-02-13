#include "AST.h"

int main() {
    ASTVisitor vis("Context-Goes-Here");

    // 1.253, 3.687
    NumberLiteral num_1(1.253);
    NumberLiteral num_2(3.687);

    // 1.253 * 3.687
    BinaryExpression expr_1(&num_1, &num_2, Operator::MULTIPLY);

    // return (1.253 * 3.687)
    ReturnStatement ret_1(&expr_1);
    
    std::vector<ASTNode*> body;
    body.push_back(&ret_1);

    Block block_1(body);

    std::cout << block_1.accept(vis).asNumber() << "\n";
}