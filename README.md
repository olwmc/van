# The Van Programming Language
Final Project for CSC212

## Grammar
```ebnf
program ::= block

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

functionDeclaration ::= 'defun' identifer arglist 'as' block 'end'

functionCall ::= identifier arglist

forLoop ::= 'for' '(' variableDeclaration ',' expr ',' expr ')' 'do' block 'end'

returnStatement ::= 'return' expr

expr ::= 'false' | 'true' | Number | String | identifier 
         | expr binop expr | prefixExpr

prefixExpr ::=  functioncall  |  '(' expr ')'

binOp ::= '+' | '-' | '*' | '/' |  '%' | '<' | '>' |
          '<=' | '>' | '>=' | '==' | '&&' | '||'

argList ::= '(' [{exp ','} exp] ')'
```