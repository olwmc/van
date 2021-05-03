/*
Oliver Wendell McLaughlin CSC212 Fall 2021
*/

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

class builtin_Input : public Builtin {
    public:
    builtin_Input() {
        this->m_args = {};
    }

    virtual Value execute(Context& context) override;
};

class builtin_Print : public Builtin {
    public:
    builtin_Print() {
        this->m_args = {"__input__"};
    }

    virtual Value execute(Context& context) override;
};

class builtin_NumCast : public Builtin {
    public:
    builtin_NumCast() {
        this->m_args = {"__input__"};
    }

    virtual Value execute(Context& context) override;
};

class builtin_Len : public Builtin {
  public:
  builtin_Len() {
    this->m_args = {"__input__"};
  }

  virtual Value execute(Context& context) override;
};

class builtin_Type : public Builtin {
  public:
  builtin_Type() {
    this->m_args = {"__input__"};
  }

  virtual Value execute(Context& context) override; 
};


class builtin_StrCast : public Builtin {
    public:
    builtin_StrCast() {
        this->m_args = {"__input__"};
    }

    virtual Value execute(Context& context) override;
};

class builtin_Rand : public Builtin {
    public:
    builtin_Rand() {
        this->m_args = {};
    }

    virtual Value execute(Context& context) override;
};

// Quicksort implementation
class builtin_Sort : public Builtin {
    public:
    builtin_Sort() {
        this->m_args = {"__list__"};
    }

    int partition (Value arr[], int low, int high);
    void quickSort(Value arr[], int low, int high);
    virtual Value execute(Context &context) override;

    friend class LinkedList;
};

// Binary search
class builtin_Contains : public Builtin {
    // Do a check to see if all numbers, if so, sort
    // else do a linear search
    public:
    builtin_Contains() {
        this->m_args = {"__list__", "__element__"};
    }

    int binarySearch(std::vector<Value> arr, int l, int r, Value v);
    virtual Value execute(Context &context) override;
};

#endif /* BUILTIN_H */