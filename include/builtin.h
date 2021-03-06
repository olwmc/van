#ifndef BUILTIN_H
#define BUILTIN_H

#include "callable.h"

class Builtin : public Callable {
  public:
    virtual Value accept(ProgramVisitor& visitor) override {
      return visitor.visit(*this);
    }

    virtual Value execute(Context& context) = 0;
};  

/* Builtin print statement class */
class builtin_Println : public Builtin {

  public:
    builtin_Println() {
      this->m_args = {"__input__"};
    }
  
    virtual Value execute(Context& context) override;

};

class builtin_Assert : public Builtin {

  public:
    builtin_Assert() {
      this->m_args = {"__assert_condition__"};
    }
  
    virtual Value execute(Context& context) override;

};

/* Builtin casts */
/*
class builtin_NumberCast;
class builtin_StringCast;
class builtin_GetKeys;
class builtin_GetType;

class builtin_Input;
class builtin_Len;
*/

#endif /* BUILTIN_H */
