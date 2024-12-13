# **Compiler Design | `Syntax Analysis`**

Diseño e implementación de un analizador sintáctico (`parser`) para el lenguaje SM (Lenguaje de Máquina de Pila). El análisis sintáctico es una parte crucial del proceso de compilación que ocurre después del análisis léxico. En esta etapa, se verifica la estructura gramatical y sintáctica del código fuente para asegurar el cumplimiento de las reglas definidas en la gramática del lenguaje.

## **SM (Stack Machine Language)**

La siguiente gramática define la estructura básica de las instrucciones del lenguaje SM, incluyendo la posibilidad de etiquetas, instrucciones de salto y operaciones unarias.

```plaintext
<program>       ::= <instruction>+
<instruction>   ::= <label>? (<unaryinstr> | <opinstr> <num> | <jmpinstr> <id>) <eol>
<unaryinstr>    ::= skip | pop | dup | swap | add | sub | mul | div | print
<opinstr>       ::= push | store | load
<jmpinstr>      ::= jmpeq | jmpgt | jmpge | jmplt | jmple | goto
```

La especificación léxica establece las reglas para identificar los tokens y lexemas presentes en el código fuente del lenguaje SM. A continuación, se presentan las expresiones regulares utilizadas:

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

De acuerdo con estas reglas léxicas, las unidades léxicas como `<id>`, `<num>`, `<eol>` y todas las palabras reservadas se convierten en *tokens*. Sin embargo, el patrón `<ws>`, que representa los espacios en blanco, no genera tokens y se ignora. Los nombres de los tokens se escriben en mayúsculas y coinciden con las unidades léxicas correspondientes.

## **Ejemplo de entrada y salida**

**Entrada**: `example0.svm`

```plaintext
pop
add
add

```

**Salida**:

```plaintext
Leyendo el programa desde el archivo example0.svm
Programa:
pop 
add 
add 
----------------
Ejecutando....
error: No se puede hacer pop en una pila vacía
```
