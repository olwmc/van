
#ifndef CALLABLE_H
#define CALLABLE_H

#include "forward.h"
#include "visitor.h"
#include "value.h"

#include <vector>

/* Base Callable Class */
class Callable {
  public:
    Callable() {}

    virtual Value accept(ASTVisitor& visitor) = 0;
};

/* User Function class */
class UserFunction : Callable {
  std::vector<std::string> m_args;
  Block * m_body;

  public:
    UserFunction(std::vector<std::string> args, Block * body)
      : m_args(args), m_body(body) {}

    virtual Value accept(ASTVisitor& visitor) override {
      return visitor.visit(*this);
    }

    Block* body() { return this->m_body; }
};

/* Builtin print statement class */
class builtin_Print : public Callable {
  std::vector<std::string> m_args = {"__input__"};

  public:
    builtin_Print() {}

    virtual Value accept(ASTVisitor& visitor) override {
      return visitor.visit(*this);
    }

    std::vector<std::string> args() { return this->m_args; }    
};

#endif /* CALLABLE_H */