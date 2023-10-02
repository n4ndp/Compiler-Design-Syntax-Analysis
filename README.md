# **Compiler Design | `Syntax Analysis`**

Diseño e implementación de un analizador sintáctico (`parser`) para el lenguaje SM (Stack Machine Language) definido en la primera sección. El análisis sintáctico es una parte crucial del proceso de compilación que se produce después del análisis léxico. En esta etapa, se verifica la estructura gramatical y la sintaxis del código fuente para asegurarse de que cumple con las reglas definidas en la gramática del lenguaje.

## **Lenguaje SM (Stack Machine Language)**

La siguiente gramática establece la estructura básica de las instrucciones en el lenguaje SM, incluyendo la posibilidad de etiquetas (labels), instrucciones de salto (jump instructions), y operaciones unarias (unary instructions).

```plaintext
<program>       ::= <instruction>+
<instruction>   ::= <label>? (<unaryinstr> | <opinstr> <num> | <jmpinstr> <id>) <eol>
<unaryinstr>    ::= skip | pop | dup | swap | add | sub | mul | div | print
<opinstr>       ::= push | store | load
<jmpinstr>      ::= jmpeq | jmpgt | jmpge | jmplt | jmple | goto
```

La especificación léxica define las reglas para reconocer tokens y lexemas en el código fuente SM. Aquí están las expresiones regulares que se utilizan:

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

Según estas reglas léxicas, las unidades léxicas como `<id>`, `<num>`, `<eol>` y todas las palabras reservadas se convierten en tokens. Sin embargo, el patrón `<ws>`, que representa espacios en blanco, no genera tokens y se ignora. Los nombres de los tokens se escriben en mayúsculas y coinciden con las unidades léxicas correspondientes.

## **Ejemplo input y output**

**input**: ejemplo0.svm

```plaintext
pop
add
add

```

**output**:

```plaintext
Reading program from file ejemplo0.svm
Program:
pop 
add 
add 
----------------
Running ....
error: Can't pop from an empty stack
```
