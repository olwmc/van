#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>

enum Token_type { 
    IDENTIFIER,
    NUM_LIT, STR_LIT, BOOL_LIT,
    END_FILE,
    OPERATOR, SEPARATOR, PUNCTUATION,
    
    FOR, RETURN, DEFUN, LOCAL, GLOBAL
};

class Token {
	Token_type m_type;
	std::string m_raw;
	bool m_keyword;
	int m_index, m_line;

	public:
		Token(Token_type type, std::string raw, bool keyword, int index, int line)
			: m_type(type), m_raw(raw), m_keyword(keyword),
			  m_index(index), m_line(line)
			{}
	
		bool isKeyword() 	{ return this->m_keyword; }

		Token_type type()	{ return this->m_type;    }
		std::string raw()	{ return this->m_raw;     }
		int line() 			{ return this->m_line;    }
		int index()			{ return this->m_index;   }

		std::string toString();

};

class Lexer {
	std::string m_prog, currentLexeme;
	int m_index, m_line;
	
	public:
		Lexer(std::string program)
			: m_prog(program), m_index(0), m_line(0)
			{}

		/* Emits the next token */
		Token getNextToken();
		
		int line() 			{ return this->m_line;    }
		int index()			{ return this->m_index;   }
};
#endif /* LEXER_H */