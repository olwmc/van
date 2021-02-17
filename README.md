# The Van Programming Language
Final Project for CSC212

## Grammar
```ebnf
block ::= {stmnt ';'}

stmnt ::= variableDeclaration |
variableAssignment |
functionDeclaration |
functionCall |
forLoop |
returnStatement

variableDeclaration ::= 'local' identifier '=' expr |
'global' identifier '=' expr

variableAssignment ::= identifier '=' expr

functionDeclaration ::= 'defun' identifer arglist 'is' block 'end'

functionCall ::= identifier arglist

forLoop ::= 'for' variableDeclaration ',' expr ',' expr ',' 'do' block 'end'

returnStatement ::= 'return' expr

expr ::= 'false' | 'true | Number | String | identifier |
         functionCall | expr binop expr | '(' expr ')'

binOp ::= `+´ | `-´ | `*´ | `/´ |  `%´ | `<´ | '>' |
          `<=´ | `>´ | `>=´ | `==´ | '&&' | '||'

argList ::= '(' [{exp ','} exp] ')'
```
