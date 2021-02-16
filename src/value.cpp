#include "value.h"
#include "err.h"

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
Value Value::operator+(const Value& other) {
    if(this->m_type == other.m_type) {
        if(this->m_type == Value_Type::STRING) {
            return Value(this->m_string + other.m_string);
        }

        return Value(this->m_number + other.m_number);
    }

    return Value(Value_Type::ERR);
}

Value Value::operator-(const Value& other) {
    if((this->m_type == other.m_type) && this->m_type == Value_Type::NUMBER) {
        return Value(this->m_number - other.m_number);
    }

    return Value(Value_Type::ERR);
}

Value Value::operator*(const Value& other) {
    if((this->m_type == other.m_type) && this->m_type == Value_Type::NUMBER) {
        return Value(this->m_number * other.m_number);
    }

    return Value(Value_Type::ERR);
}

Value Value::operator/(const Value& other) {
    if((this->m_type == other.m_type) && this->m_type == Value_Type::NUMBER) {
        return Value(this->m_number / other.m_number);
    }

    return Value(Value_Type::ERR);
}

Value Value::operator%(const Value& other) {
    if((this->m_type == other.m_type) && this->m_type == Value_Type::NUMBER) {
        return Value((int)this->m_number % (int)other.m_number);
    }

    return Value(Value_Type::ERR);
}

/* Comparators */
Value Value::operator==(const Value& other) {
    if(this->m_type == other.m_type) {
        if(this->m_type == Value_Type::STRING) {
            return Value(this->m_string == other.m_string);
        }

        return Value(this->m_number == other.m_number);
    }

    return Value(Value_Type::ERR);
}

Value Value::operator<=(const Value& other) {
    if((this->m_type == other.m_type) && this->m_type == Value_Type::NUMBER) {
        return Value((int)this->m_number <= (int)other.m_number);
    }

    return Value(Value_Type::ERR);
}


// Value  Value::operator!=(const Value& other);
// Value  Value::operator>=(const Value& other);
    
// /* Logical Operations */
// Value operator&&(const Value& other);
// Value operator||(const Value& other);