#ifndef BUILTIN_H
#define BUILTIN_H

#include "callable.h"

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

#endif /* BUILTIN_H */