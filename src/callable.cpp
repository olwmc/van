#include "forward.h"
#include "value.h"
#include "visitor.h"
#include "context.h"
#include "callable.h"

#include <iostream>

Value ASTVisitor::visit(builtin_Print& printStatment) {
  Value input = this->m_context->resolveVariable(printStatment.args()[0]);

  std::cout << input.toString() << "\n";

  return Value();
}