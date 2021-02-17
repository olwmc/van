#include "value.h"
#include "err.h"

#include <string>

// Helper function
bool bothNumber(Value lhs, Value rhs) {
    return (lhs.type() == rhs.type()) && (lhs.type() == Value_Type::NUMBER);
}

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
Value Value::operator+(Value other) {
    if(this->m_type == Value_Type::STRING || other.type() == Value_Type::STRING) {
        return Value(this->toString() + other.toString());
    }

    else if (bothNumber(*this, other)) {
        return Value(this->m_number + other.m_number);
    }

    return Value(Value_Type::ERR);
}

Value Value::operator-(Value other) {
    if(bothNumber(*this, other)) {
        return Value(this->m_number - other.m_number);
    }

    return Value(Value_Type::ERR);
}

Value Value::operator*(Value other) {
    if(bothNumber(*this, other)) {
        return Value(this->m_number * other.m_number);
    }

    return Value(Value_Type::ERR);
}

Value Value::operator/(Value other) {
    if(bothNumber(*this, other)) {
        return Value(this->m_number / other.m_number);
    }

    return Value(Value_Type::ERR);
}

Value Value::operator%(Value other) {
    if(bothNumber(*this, other)) {
        return Value((int)this->m_number % (int)other.m_number);
    }

    return Value(Value_Type::ERR);
}

/* Comparators */
Value Value::operator==(Value other) {
    if(this->m_type == other.m_type) {
        if(this->m_type == Value_Type::STRING) {
            return Value(this->m_string == other.m_string);
        }

        return Value(this->m_number == other.m_number);
    }

    return Value(Value_Type::ERR);
}

Value Value::operator<=(Value other) {
    if(bothNumber(*this, other)) {
        return Value(this->m_number <= other.m_number);
    }

    return Value(Value_Type::ERR);
}

Value Value::operator!=(Value other) {
    if(bothNumber(*this, other)) {
        return Value(this->m_number != other.m_number);
    }

    return Value(Value_Type::ERR);
}

Value Value::operator>=(Value other) {
    if(bothNumber(*this, other)) {
        return Value(this->m_number >= other.m_number);
    }

    return Value(Value_Type::ERR);
}

Value Value:: operator&&(Value other) {
    if(bothNumber(*this, other)) {
        return Value(this->m_number && other.m_number);
    }

    return Value(Value_Type::ERR);
}
Value Value:: operator||(Value other) {
    if(bothNumber(*this, other)) {
        return Value(this->m_number || other.m_number);
    }

    return Value(Value_Type::ERR);
}