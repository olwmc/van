#include "forward.h"
#include "value.h"
#include "visitor.h"
#include "context.h"
#include "builtin.h"

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

/* Builtin println function, takes and input and prints it on its own line */
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

/* Asserts a given condition */
Value builtin_Assert::execute(Context& context) {
  // Assert that the value is true
  Value condition = context.resolveVariable(this->args()[0]);

  if(condition.type() == Value_Type::NUMBER && condition.asNumber() == 1) {
    return Value("SUCCESSFULLY ASSERTED");
  }

  else {
    throw std::runtime_error("FAILED TO ASSERT");
  }
}

/* Takes in input and returns it as a string */
Value builtin_Input::execute(Context& context) {
  std::string input;
  std::getline(std::cin, input);

  return Value(input);
}

/* Prints input without newline */
Value builtin_Print::execute(Context& context) {
  std::cout << context.resolveVariable(this->m_args[0]).toString();
  return Value();
}

/* Casts to number if possible */
Value builtin_NumCast::execute(Context& context) {
  // TODO: ADD CHECK HERE
  std::string str = context.resolveVariable(this->m_args[0]).asString();
  double num = std::stod(str);
  
  return Value(num);
}