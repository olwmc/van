#include "AST.h"

int main() {
	ASTVisitor vis("Context-Goes-Here");

	/* String literal */
	StringLiteral str_1("My string!!");

	/* 20 - 13 + 8 */
	NumberLiteral num_1 (20);
	NumberLiteral num_2 (13);
	NumberLiteral num_3 (8);

	/* 20 - 13 */
	BinaryExpression expr_1(&num_1, &num_2, Operator::SUBTRACT);

	/* (20 - 13) + 8 */
	BinaryExpression expr_2(&expr_1, &num_3, Operator::ADD);

	/* ((20 - 13) + 8) * 20 */
	BinaryExpression expr_3(&expr_2, &num_1, Operator::MULTIPLY);
	
	NumberLiteral num_4 (300);
    NumberLiteral num_5 (0);
	
	/* (((20 - 13) + 8) * 20) == 300 */
	BinaryExpression expr_4(&expr_3, &num_4, Operator::NOTEQUAL);

    /* ((((20 - 13) + 8) * 20) == 300) && 0 */
	BinaryExpression expr_5(&expr_4, &num_5, Operator::AND);
	
	std::cout << "Expr_3: " << expr_3.accept(vis).asNumber() << "\n";
	std::cout << expr_5.accept(vis).asBoolString() << "\n";
}