#include "lexer.h"

#include <vector>
#include <string>
#include <unordered_map>

/* Ordered strings for Token_type */
std::vector<std::string> token_string = { 
    "IDENT",
    "NUM", "STR", "TRUE", "FALSE"
    "EOF",
    "OP", "SEP", "PUNCT",

    "FOR", "RETURN", "DEFUN", "LOCAL", "GLOBAL",
	"AS", "BEGIN", "END",
    "COMMENT"
};

std::string Token::toString() {
    	return "Type: " + token_string[this->m_type] + "\tLine: " + \
               std::to_string(this->m_line) + "{" + std::to_string(this->m_index) + "}" + \
               "\tSymbol: \"" + this->m_raw + "\"";
}

/* Maps keywords to Token_type enum */
std::unordered_map<std::string, Token_type> type_map {
    {"true",   Token_type::TRUE_LIT}, {"false",  Token_type::FALSE_LIT},
    {"for",    Token_type::FOR},      {"return", Token_type::RETURN},
    {"defun",  Token_type::DEFUN},    {"local",  Token_type::LOCAL},
    {"global", Token_type::GLOBAL},   {"as",     Token_type::AS},
    {"begin",     Token_type::FOR},   {"end",    Token_type::END}
};

Token_type Lexer::resolveKeyword(std::string raw) {
    if(type_map.find(raw) != type_map.end()) {
        return type_map[raw];
    }

    return Token_type::IDENTIFIER;
}

// /* Get different types of tokens */
// Token getPunctuation();
// Token getSeparator();
// Token getOperator();
// Token getNumberLiteral();
// Token getKeywordOrId();
// Token getComment();
// Token getStringLiteral();

//  /* Different character catagorizers */
//  bool isWhiteSpace();
//  bool isPunctuation();
//  bool isSeparator();
//  bool isOperator();
//  bool isDigit();
//  bool isAlpha();