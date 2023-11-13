# **Compiler Design | `Syntax Analysis`**

Design and implementation of a syntactic analyzer (`parser`) for the SM (Stack Machine Language) defined in the first section. Syntax analysis is a crucial part of the compilation process that occurs after lexical analysis. At this stage, the grammatical structure and syntax of the source code are verified to ensure compliance with the rules defined in the language grammar.

## **SM (Stack Machine Language) Grammar**

The following grammar outlines the basic structure of instructions in the SM language, including the possibility of labels, jump instructions, and unary operations.

```plaintext
<program>       ::= <instruction>+
<instruction>   ::= <label>? (<unaryinstr> | <opinstr> <num> | <jmpinstr> <id>) <eol>
<unaryinstr>    ::= skip | pop | dup | swap | add | sub | mul | div | print
<opinstr>       ::= push | store | load
<jmpinstr>      ::= jmpeq | jmpgt | jmpge | jmplt | jmple | goto
```

The lexical specification defines rules to recognize tokens and lexemes in the SM source code. Here are the regular expressions used:

```plaintext
digit               ::= [0-9]
character           ::= [a-zA-Z]
<reserved-word>     ::= push | jmpeq | jmpgt | jmpge | jmplt | jmple | skip | pop | 
                        dup | swap | add | sub | mul | div | store | load | goto | print
<id>                ::= character | (character | digit | ‘_’)*
<label>             ::= <id>:
<num>               ::= digit+
<eol>               ::= ‘\n’+
<ws>                ::= (‘ ‘ | ‘\t’)+
```

According to these lexical rules, lexical units like `<id>`, `<num>`, `<eol>`, and all reserved words become tokens. However, the `<ws>` pattern, representing whitespace, does not generate tokens and is ignored. Token names are written in uppercase and match the corresponding lexical units.

## **Example input and output**

**input**: example0.svm

```plaintext
pop
add
add

```

**output**:

```plaintext
Reading program from file example0.svm
Program:
pop 
add 
add 
----------------
Running ....
error: Can't pop from an empty stack
```
