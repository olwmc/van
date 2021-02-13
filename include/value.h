#pragma once
#include <string>

enum Value_Type {
    NUMBER, STRING, NIL
};

/* Value holder class */
class Value {
    // Map : Dict
    // Vector : List
    double m_number;
    std::string m_string;
    
    Value_Type m_type;

    public:
        /* Constructors */
        Value(double v)      : m_number(v) { this->m_type = Value_Type::NUMBER; }
        Value(std::string v) : m_string(v) { this->m_type = Value_Type::STRING; }
        
        /* Default constructor is NIL */
        Value() { this->m_type = NIL; }
        
        /* Return representations */
        double asNumber()           { return this->m_number; }
        std::string asString()      { return this->m_string; }
        Value_Type type()           { return this->m_type;   }
    
        std::string asBoolString() {
            if(this->m_number == 1) { return "True"; }
            
            return "False";
        }
    
        // Value operator+(Value& other);
        // Value operator-(Value& other);
};