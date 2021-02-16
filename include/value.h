#ifndef VALUE_H
#define VALUE_H

#include <string>
#include <vector>

enum Value_Type {
    // Basic types
    NUMBER, STRING, LIST, TABLE, NIL,

    // Error type used for checking
    ERR
};

/* Value holder class */
class Value {
    // We also need a table (map)
    std::vector<Value> m_list;
    double m_number;
    std::string m_string;
    
    // Value type
    Value_Type m_type;

    // Const specifier
    bool m_const;

    public:
        /* Constructors */
        Value(double v)      : m_number(v) { this->m_type = Value_Type::NUMBER; }
        Value(std::string v) : m_string(v) { this->m_type = Value_Type::STRING; }
        Value(Value_Type v)  : m_type(v) {};
        Value() { this->m_type = NIL; }

        /* Return representations */
        double asNumber()           { return this->m_number; }
        std::string asString()      { return this->m_string; }
        Value_Type type()           { return this->m_type;   }
	    
		/* ToString as opposed to AsString. Used in casting.*/
	    std::string toString(); 
        
        /* Check const state */
        bool isConst() { return this->m_const; }

        /* Arithmetic operations */
        Value operator+(const Value& other);
        Value operator-(const Value& other);
        Value operator*(const Value& other);
        Value operator/(const Value& other);
        Value operator%(const Value& other);

        /* Comparators */
        Value operator==(const Value& other);
        Value operator!=(const Value& other);
        Value operator<=(const Value& other);
        Value operator>=(const Value& other);
            
        /* Logical Operations */
        Value operator&&(const Value& other);
        Value operator||(const Value& other);
};

#endif /* VALUE_H */