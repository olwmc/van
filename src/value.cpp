#include "value.h"

#include <string>

// Helper function
bool bothNumber(Value lhs, Value rhs) {
    return (lhs.type() == rhs.type()) && (lhs.type() == Value_Type::NUMBER);
}

std::string Value::toString() {

    // If the value is a number
    if(this->m_type == Value_Type::NUMBER) {
        // Check if decimal part. Truncate 0's accordingly
        if(this->m_number == int(this->m_number)) {
            return std::to_string(int(this->m_number));
        }
        
        return std::to_string(this->m_number);
    }

    // If the value is a string
    else if (this->m_type == Value_Type::STRING) {
        return this->m_string;
    }

    // If the value is a list
    else if (this->m_type == Value_Type::LIST) {
        std::string out = "[";

        if(this->m_list.size() > 0) {
            out += this->m_list[0].toString();

            for(int i = 1; i < (int)m_list.size(); i++) {
                out += ", ";
                out += m_list[i].toString();
            }    
        }

        out += "]";
        return out;
    }

    return "NIL";
}


/* Arithmetic operations */
Value Value::operator+(Value other) {
    if(this->m_type == Value_Type::LIST) {
        std::vector<Value> outVector = this->m_list;
        outVector.push_back(other);

        return Value(outVector);
    }

    else if(other.type() == Value_Type::LIST) {
        std::vector<Value> outVector = other.asList();
        outVector.push_back(*this);

        return Value(outVector);
    }

    else if(this->m_type == Value_Type::STRING || other.type() == Value_Type::STRING) {
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
        if(other.m_number == 0) {
            return Value(Value_Type::ERR);
        }

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
    // If the types are equal, compare them
    if(this->m_type == other.m_type) {

        // Check if the strings are the same
        if(this->m_type == Value_Type::STRING) {
            return Value(this->m_string == other.m_string);
        }

        // Iterate through the lists and check them
        if(this->m_type == Value_Type::LIST) {
            // Check if the size is the same
            if(this->asList().size() != other.asList().size()) {
                return Value(false);
            } 

            // If it is, check if the values are the same
            for(int i = 0; i < (int)this->asList().size(); i++) {
                if( (this->asList()[i] != other.asList()[i]).asNumber() ) {
                    return Value(false);
                }
            }

            // If it passes everything, return true
            return Value(true);
        }

        // Or, number
        return Value(this->m_number == other.m_number);
    }

    // Otherwhise return false
    return Value(0);
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

    else if (this->type() == other.type() && this->type() == Value_Type::STRING) {
        return Value(this->asString() != other.asString());
    }

    return Value(Value_Type::ERR);
}

Value Value::operator>=(Value other) {
    if(bothNumber(*this, other)) {
        return Value(this->m_number >= other.m_number);
    }


    return Value(Value_Type::ERR);
}

Value Value::operator>(Value other) {
    if(bothNumber(*this, other)) {
        return Value(this->m_number > other.m_number);
    }
    else if(this->type() == other.type() &&
            this->type() == Value_Type::STRING) {
        return Value(this->asString() > other.asString());
    }
    return Value(Value_Type::ERR);
}
Value Value::operator<(Value other) {
    if(bothNumber(*this, other)) {
        return Value(this->m_number < other.m_number);
    }

    else if(this->type() == other.type() &&
            this->type() == Value_Type::STRING) {
        return Value(this->asString() < other.asString());
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