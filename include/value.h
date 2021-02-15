#pragma once
#include <string>
#include <vector>

enum Value_Type {
    NUMBER, STRING, LIST, TABLE, NIL
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
        Value() : m_number(0) { this->m_type = NIL; }
        
        /* Return representations */
        double asNumber()           { return this->m_number; }
        std::string asString()      { return this->m_string; }
        Value_Type type()           { return this->m_type;   }
	    
		/* ToString as opposed to AsString. Used in casting.*/
	    std::string toString(); 
        
        /* Check const state */
        bool isConst() { return this->m_const; }
};