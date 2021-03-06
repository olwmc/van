#include "forward.h"
#include "value.h"
#include "visitor.h"
#include "context.h"
#include "builtin.h"
#include "err.h"

#include <iostream>

/* User defined function */
Value ProgramVisitor::visit(UserFunction& userFunction) {
  Value v = userFunction.body()->accept(*this);
  return v;
}

Value ProgramVisitor::visit(Builtin& builtin) {
  Value v = builtin.execute(*this->m_context);
  return v;
}

Value builtin_Println::execute(Context& context) {
  // Get the input from context
  // Using "printStatement.args()" instead of __input__ to make this more
  // maintainable
  Value input = context.resolveVariable(this->args()[0]);

  // Print the input
  std::cout << input.toString() << "\n";

  // Return NIL
  return Value();
}

Value builtin_Assert::execute(Context& context) {
  // Assert that the value is true
  Value condition = context.resolveVariable(this->args()[0]);

  if(condition.type() == Value_Type::NUMBER && condition.asNumber() == 1) {
    return Value("SUCCESSFULLY ASSERTED");
  }

  else {
    raiseLogicError("FAILED TO ASSERT");
    exit(1);
  }
}