# minipl-interpreter

An interpreter for a language defined by the following LL(1) grammar:
```
<prog> ::= <stmts>
<stmts> ::= <stmt> “;” ( <stmt> “;” )*
<stmt> ::= “var” <var_ident> “:” <type> [ “:=” <expr> ]
	| <var_ident> “:=” <expr>
	| “for” <var_ident> “in” <expr> “..” <expr> “do” <stmts> “end” “for”
	| “read” <var_ident>
	| “print” <expr>
	| “assert” “(” <expr> “)”
<expr> ::= <opnd> <opnd_tail> | <unary_opnd> <opnd>
<opnd_tail> ::= <op> <opnd> | epsilon
<opnd>::= <int> | <string> | <var_ident> | “(” <expr> “)”
<type> ::= “int” | “string” | “bool”
<var_ident> ::= <ident>
<reserved_keyword> ::= “var” | “for” | “end” | “in” | “do” | “read” | “print” | “int” | “string” | “bool” | “assert”
```
Consists of a simple scanner/tokenizer and a recursive descent parser. Builds an AST while parsing.
