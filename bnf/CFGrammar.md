# Context-free grammars

Names: 
- Luis Arias Sarabia
- Pablo Sedano Morlett

## Grammar definitions

Describe the basic grammar necessary to write a valid *construction* in the language indicated.

The contents of the *construction* do not need to be specified.

Consider all the requirements for the construction according to the language. For example, the rules for the names of the constructions

---
### C Code

```c
int age(int one, int two, char three) {
    return one + two;
}

void greet(char* name, char* surname) {
    printf("Hello %s %s\n", name, surname);
}

float something(void) {
    int a = 5;
    int b = 34;
    return b / a;
}

void nothing(int solution) {
    int test = solution % 5;
    printf("%d", test);
}

int main() {
    return 0;
}
```

### BNF C / C++ functions

```xml
<function> ::= <type> <nombre> ( <params> ) { <body> }

<type> ::= int | void | float | char | char*
<nombre> ::= <letra> | <letra> <nombre>
<letra> ::= a | ... | z | A | ... | Z | _
<params> ::= | void | <param_list>
<param_list> ::= <param> | <param> ',' <param_list>
<param> ::= <type> <nombre> | <type> '*' <nombre>

<body> ::= | <statement> <body>
<statement> ::= <expresion>
```

### EBNF C / C++ functions

```bash
FUNCTION ::= TYPE NOMBRE '(' [PARAMS] ')' '{' {STATEMENT} '}'

TYPE ::= 'int' | 'void' | 'float' | 'char' | 'char' '*'
NOMBRE ::= LETRA {LETRA}
LETRA ::= 'a' | ... | 'z' | 'A' | ... | 'Z' | '_'

PARAMS ::= 'void' | PARAM {',' PARAM}
PARAM  ::= TYPE ['*'] NOMBRE
```
---
### Elixir Code
```elixir
defmodule Training do

    def quick, do: IO.puts("HELLO!")

    def add(a, b), do: a + b

    def factorial(0), do: 1
    def factorial(n), do: n * factorial(n - 1)

    def fibo(n) when n < 0, do: :error
    def fibo(n) when n < 2, do: n
    def fibo(n), do: fibo(n - 1) + fibo(n - 2)

    defp quadratic(a, b, c) do
      d = b ** 2 - 4 * a * c
      cond do
        a == 0 and b == 0 -> {:error, :unsolvable}
        a == 0 -> {:ok, [-c / b]}
        d < 0 -> {:error, :complex}
        true ->
          x1 = (-b + :math.sqrt(d)) / (2 * a)
          x2 = (-b - :math.sqrt(d)) / (2 * a)
          {:ok, [x1, x2]}
      end
    end

    defp default(name \\ "Pepito") do
      IO.puts("Good morning #{name}")
    end
end
```

### BNF Elixir modules and functions

```xml
<modulo>::= defmodule <nombreModulo> do <Contenido> end
<Contenido>::= <funcion> | <funcion> <contenido>
<funcion> ::= <def> <nombreFuncion>, do: <expresion>
            | <def> <nombreFuncion> do <expresiones> end
            | <def> <nombreFuncion> when <booleana>, do: <expresion>
            | <def> <nombreFuncion> when <booleana> do <expresiones> end


<nombreFuncion>::= <nF> | <nF> (<argumentos>)
<def>::= def | defp
<expresiones>::= <expresion> | <expresion> <expresiones>
<argumentos>::= <argumento> | <argumento> , <argumentos> 
<argumento>::= <variable> | <variable> // <argumento>
```

### EBNF Elixir modules and functions

```bash
MODULE ::= 'defmodule' NOMBRE_MODULO 'do' {Funcion} 'end'
FUNCION ::= ( 'def' | 'defp' ) NOMBRE_FUNCION ['('{ARGUMENTO}')'] ['when' (BOOLEANA)] ', do:' EXPRESION 
          | ( 'def' | 'defp' ) NOMBRE_FUNCION ['('{ARGUMENTO}')'] ['when' (BOOLEANA)] 'do' {EXPRESION} 'end'

ARGUMENTO ::= VARIABLE | VARIABLE '\\' ARGUMENTO
```


