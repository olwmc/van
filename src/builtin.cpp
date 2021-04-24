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
    return Value("ASSERTION PASSED");
  }

  else {
    throw std::runtime_error("FAILED TO ASSERT");
  }
}

/* Takes in input and returns it as a string */
Value builtin_Input::execute(Context& context) {
  (void)context;
  
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
  std::string str = context.resolveVariable(this->m_args[0]).toString();

  double num;

  try {
    num = std::stod(str);
  } catch(...) {
    throw std::runtime_error("Only strings can be cast to numbers");
  }
  
  return Value(num);
}

/* Length of a list/string */
Value builtin_Len::execute(Context& context) {
  // Check if arr or str
  Value val = context.resolveVariable(this->m_args[0]);

  if(val.type() == Value_Type::LIST) {
    return val.asList().size();
  } 

  else if(val.type() == Value_Type::STRING) {
    return val.asString().size();
  }
  else {
    throw std::runtime_error("Cannot get length of: " + val.toString());
  }
}

Value builtin_Type::execute(Context& context) {
  Value val = context.resolveVariable(this->m_args[0]);

  std::string type;

  switch(val.type()) {
    // number string list nil
    case NUMBER:
      type = "number";
    break;

    case STRING:
      type = "string";
    break;

    case LIST:
      type = "list";
    break;

    default:
      type = "nil";
  }

  return Value(type);
}

Value builtin_StrCast::execute(Context& context) {
  Value val = context.resolveVariable(this->m_args[0]);

  return Value(val.toString());
}