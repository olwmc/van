// TODO: REWORK THIS WHOLE FILE

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
    virtual ~Callable() {}

    virtual Value accept(ProgramVisitor& visitor) = 0;
    std::vector<std::string> args() { return this->m_args; }

  protected:
    std::vector<std::string> m_args;
};

/* User Function class */
class UserFunction : public Callable {
  Block * m_body;

  public:
    UserFunction(std::vector<std::string> args, Block * body)
      : m_body(body) {
        this->m_args = args;
      }

    virtual Value accept(ProgramVisitor& visitor) override {
      return visitor.visit(*this);
    }

    Block* body() { return this->m_body; }
};

/* Builtin print statement class */
class builtin_Print : public Callable {

  public:
    builtin_Print() {
      this->m_args = {"__input__"};
    }

    virtual Value accept(ProgramVisitor& visitor) override {
      return visitor.visit(*this);
    }

};

class builtin_Assert: public Callable {  
  public:
    builtin_Assert() {
      this->m_args = {"__assert_condition__"};
    }

    virtual Value accept(ProgramVisitor& visitor) override {
      return visitor.visit(*this);
    }
};

#endif /* CALLABLE_H */