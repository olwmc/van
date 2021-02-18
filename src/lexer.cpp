#include "lexer.h"

std::vector<std::string> Token_string = { 
    "IDEN",
    "NUM", "STR", "BOOL",
    "EOF",
    "OP", "SEP", "PUNC",

    "FOR", "RETURN", "DEFUN", "LOCAL", "GLOBAL"
};

std::string Token::toString() {
    	return "Type: " + Token_string[this->m_type] + "\tLine: " + \
               std::to_string(this->m_line) + "[" + std::to_string(this->m_index) + "]\tSymbol: \"" + this->m_raw + "\"";
}

Token Lexer::getNextToken() {
	/* while(testCondition(input[++index])); */
	
	if(this->m_index > (int)this->m_prog.length() - 1)  {
        return Token(Token_type::END_FILE, "EOF", false, this->m_index, this->m_line);
    }


    return Token(Token_type::END_FILE, "EOF", false, this->m_index, this->m_line);
}