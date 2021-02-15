// TODO: REWORK THIS WHOLE FILE

#ifndef CALLABLE_H
#define CALLABLE_H

#include "forward.h"
#include "visitor.h"
#include "value.h"

#include <vector>

/* Base Callable Class */
class Callable {
  protected:
  std::vector<std::string> m_args;
  
  public:
    Callable() {}

    virtual Value accept(ProgramVisitor& visitor) = 0;
    virtual std::vector<std::string> args() = 0;
};

/* User Function class */
class UserFunction : public Callable {
  std::vector<std::string> m_args;
  Block * m_body;

  public:
    UserFunction(std::vector<std::string> args, Block * body)
      : m_args(args), m_body(body) {}

    virtual Value accept(ProgramVisitor& visitor) override {
      return visitor.visit(*this);
    }

    virtual std::vector<std::string> args() override { return this->m_args; }
    Block* body() { return this->m_body; }
};

/* Builtin print statement class */
class builtin_Print : public Callable {
  std::vector<std::string> m_args;

  public:
    builtin_Print() : m_args({"__input__"}) {}

    virtual Value accept(ProgramVisitor& visitor) override {
      return visitor.visit(*this);
    }

    virtual std::vector<std::string> args() override { return this->m_args; }
};

class builtin_Assert: public Callable {
  std::vector<std::string> m_args;
  
  public:
    builtin_Assert() : m_args({"__assert_condition__"}) {}

    virtual Value accept(ProgramVisitor& visitor) override {
      return visitor.visit(*this);
    }

    virtual std::vector<std::string> args() override { return this->m_args; }
};

#endif /* CALLABLE_H */