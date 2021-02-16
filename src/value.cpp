#include "value.h"
#include <string>

// TODO Refactor this to remove trailing 0's
std::string Value::toString() {
    if(this->m_type == Value_Type::NUMBER) {
        return std::to_string(this->m_number);
    }

    else if (this->m_type == Value_Type::STRING) {
        return this->m_string;
    }

    return "NIL";
}

/* Arithmetic operations */
// Value operator+(const Value& other);
// Value operator-(const Value& other);
// Value operator*(const Value& other);
// Value operator%(const Value& other);

// /* Comparators */
// Value operator==(const Value& other);
// Value operator!=(const Value& other);
// Value operator<=(const Value& other);
// Value operator>=(const Value& other);
    
// /* Logical Operations */
// Value operator&&(const Value& other);
// Value operator||(const Value& other);