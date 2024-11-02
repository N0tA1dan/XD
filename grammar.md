NodeReturn ::= <expression>
expression ::= <term> '+' <expression> | <term> '-' <expression> | <term>
term :: <factor> '*' <term> | <factor> '/' <term> | <factor>
factor ::= int_lit | var