#include "forward.h"
#include "value.h"
#include "visitor.h"
#include "context.h"
#include "callable.h"

#include <iostream>

Value ASTVisitor::visit(UserFunction& userFunction) {
  Value v = userFunction.body()->accept(*this);
  return v;
}


Value ASTVisitor::visit(builtin_Print& printStatment) {
  // Get the input from context
  // Using "printStatement.args()" instead of __input__ to make this more
  // maintainable
  Value input = this->m_context->resolveVariable(printStatment.args()[0]);

  // Print the input
  std::cout << input.toString() << "\n";

  // Return NIL
  return Value();
}