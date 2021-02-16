// TODO: REWORK THIS WHOLE FILE

#ifndef CALLABLE_H
#define CALLABLE_H

#include "forward.h"
#include "visitor.h"
#include "value.h"
#include "AST.h"

#include <vector>
#include <string>

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
  std::string m_name;

  public:
    UserFunction(std::string name, std::vector<std::string> args, Block * body)
      : m_body(body), m_name(name) 
      {
        this->m_args = args;
      }

    virtual Value accept(ProgramVisitor& visitor) override {
      return visitor.visit(*this);
    }

    Block* body() { return this->m_body; }
};
#endif /* CALLABLE_H */