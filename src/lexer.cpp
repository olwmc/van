#include "lexer.h"

#include <vector>
#include <string>
#include <unordered_map>

/* Ordered strings for Token_Type */
std::vector<std::string> token_string = { 
    "IDENT",
    "NUM", "STR", "TRUE", "FALSE",
    "EOF",
    "OP", "SEP", "PUNCT",
    
    "FOR", "RET", "DEFUN", "LOCAL", "GLOBAL",
    "AS", "BEGIN", "END",
    "COMMENT",

    "UNDEF",

    "IF", "THEN", "ELSE", "ELIF",
    "WHILE",

    "OR", "AND"
};

/* Maps keywords to Token_Type enum */
std::unordered_map<std::string, Token_Type> type_map {
    {"true",   Token_Type::TRUE_LIT},  {"false",  Token_Type::FALSE_LIT},
    {"for",    Token_Type::FOR},       {"return", Token_Type::RETURN},
    {"defun",  Token_Type::DEFUN},     {"local",  Token_Type::LOCAL},
    {"global", Token_Type::GLOBAL},    {"as",     Token_Type::AS},
    {"begin",  Token_Type::FOR},       {"end",    Token_Type::END},
    {"if",    Token_Type::IF},         {"then",    Token_Type::THEN},
    {"else",    Token_Type::ELSE},     {"elif",    Token_Type::ELIF},
    {"while",    Token_Type::WHILE},   {"and",    Token_Type::WHILE},
    {"or",    Token_Type::WHILE}
};

std::string Token::toString() {
        return "Type: " + token_string[this->m_type] + "\tLine: " + \
               std::to_string(this->m_line) + "{" + std::to_string(this->m_index) + "}" + \
               "\tSymbol: \"" + this->m_raw + "\"";
}


Token_Type Lexer::resolveKeyword(std::string raw) {
    if(type_map.find(raw) != type_map.end()) {
        return type_map[raw];
    }

    return Token_Type::IDENTIFIER;
}

Token Lexer::getNextToken() {
    if(this->m_index > (int)this->m_prog.length() - 1) {
        return Token(Token_Type::END_FILE, "EOF", true, this->m_index, this->m_line);
    }

    else if(isWhiteSpace())  {
        advance();
        while(isWhiteSpace()) { advance(); }

        // This assures that trailing whitespace at the end of thefile doesn't return a badchar
        if(this->m_index > (int)this->m_prog.length() - 1) {
            return Token(Token_Type::END_FILE, "EOF", true, this->m_index, this->m_line);
        }
    }

    if (isAlpha())             { return makeKeywordOrId();   }
    else if (isPunctuation())  { return makePunctuation();   }
    else if (isCommentStart()) { return makeComment();       }
    else if (isSeparator())    { return makeSeparator();     }
    else if (isOperator())     { return makeOperator();      }
    else if (isDigit())        { return makeNumberLiteral(); }
    else if (isStringStart())  { return makeStringLiteral(); }

    Token t = Token(Token_Type::IDENTIFIER, "BADCHAR", false, this->m_index, this->m_line);
    advance();
    return t;
}

char Lexer::peek() {
    return this->m_prog[this->m_index + 1];
}

char Lexer::peekNext() {
    return this->m_prog[this->m_index + 2];
}

void Lexer::advance() {
    this->m_index++;
    this->m_currentChar = this->m_prog[this->m_index];
}

/* Different character catagorizers */
bool Lexer::isWhiteSpace() {
    switch(this->m_currentChar) {
        case ' ':
        case '\t':
            return true;
        break;

        case '\n':
            this->m_line++;
            return true;
        break;

        default:
            return false;
        break;
    }
}

bool Lexer::isPunctuation() {
    switch(this->m_currentChar) {
        case ';':
        case ',':
            return true;
        break;

        default:
            return false;
        break;
    }
}
 bool Lexer::isSeparator() {
    switch(this->m_currentChar) {
        case '[': case ']':
        case '(': case ')':
           return true;
        break;
    
       default:
           return false;
       break;
    } 
 }

 bool Lexer::isCommentStart() {
     if(this->m_currentChar == '-' && this->peek() == '-') {
         return true;
     }

     return false;
 }
 
 bool Lexer::isOperator() {
    switch(this->m_currentChar) {
       case '=': case '+':
       case '-': case '/':
       case '*': case '<':
       case '>': case '!':
       case '%':
           return true;
       break;
    
       default:
           return false;
       break;
    }
}
 
 bool Lexer::isDigit() {
    return this->m_currentChar <= '9' && this->m_currentChar >= '0';
 }

bool Lexer::isAlpha() {
    return (this->m_currentChar >= 'a' && this->m_currentChar <= 'z') || \
           (this->m_currentChar >= 'A' && this->m_currentChar <= 'Z') || \
           (this->m_currentChar == '_');
}

bool Lexer::isStringStart() {
    return this->m_currentChar == '\"' || this->m_currentChar == '\'';
}

Token Lexer::makeKeywordOrId() {
    std::string keyOrId(1, this->m_currentChar);
    advance();

    while(isAlpha()) {
        keyOrId += this->m_currentChar;
        advance();       
    }

    Token_Type tt = resolveKeyword(keyOrId);

    if(tt == Token_Type::IDENTIFIER) {
        return Token(tt, keyOrId, false, this->m_index, this->m_line);
    }

    return Token(tt, keyOrId, true, this->m_index, this->m_line);
    
}

Token Lexer::makePunctuation() {
    std::string punctuation(1, this->m_currentChar);
    advance();

    return Token(Token_Type::PUNCTUATION, punctuation, false, this->m_index, this->m_line);
}

Token Lexer::makeComment() {
    std::string comment = "";
    advance();
    advance();

    while(this->m_currentChar != '\n'
          && this->m_index < (int)this->m_prog.length())
    {
        comment += this->m_currentChar;
        advance();
    }

    return Token(Token_Type::COMMENT, comment, false,this->m_index, this->m_line);
}

Token Lexer::makeSeparator() {
    std::string separator(1, this->m_currentChar);
    advance();

    return Token(Token_Type::SEPARATOR, separator, false, this->m_index, this->m_line);
}

Token Lexer::makeOperator() {
    std::string op(1, this->m_currentChar);

    if(peek() == '=') {
        advance();
        op += this->m_currentChar;
    }

    advance();

    return Token(Token_Type::OPERATOR, op, false, this->m_index, this->m_line);
}

Token Lexer::makeNumberLiteral() {
    std::string number(1, this->m_currentChar);

    advance();
    while(isDigit() || this->m_currentChar == '.') {
        number += this->m_currentChar;
        advance();
    }

    return Token(Token_Type::NUM_LIT, number, false, this->m_index, this->m_line);
}

Token Lexer::makeStringLiteral() {
    std::string str;
    advance();

    while(!isStringStart()) {
        if(this->m_currentChar == '\\') {
            advance();

            // Check next char after \, see if it needs
            // to be escaped
            switch(this->m_currentChar) {
                case 't': str += '\t'; break;
                case 'n': str += '\n'; break;

                default:
                    str += this->m_currentChar;
            }

            //else { str += this->m_currentChar; }
            advance();
        }

        // Else just add the char
        else {
            str += this->m_currentChar;
            advance();
        }
    }
    advance();

    return Token(Token_Type::STR_LIT, str, false, this->m_index, this->m_line);
}
