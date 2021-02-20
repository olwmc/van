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

/* Builtin casts */
/*
class builtin_NumberCast;
class builtin_StringCast;
class builtin_GetKeys;
class builtin_GetType;

class builtin_Input;
*/

#endif /* BUILTIN_H */