# C++ token types

by Pablo Sedano A01785330
5/17/2026

---
## Index
- [How to run](#how-to-run)
- [Token types/regex](#token-typesregex)
- [Time Complexity](#time-complexity)

## How to run
1. Make sure you have Elixir installed on your machine.
2. Clone the repository and navigate to the syntax_highlighter directory.
3. Run `iex elixir/syntax_highlighter.ex` to start the Elixir interactive shell.
4. In the shell, execute `SyntaxHighlighter.do_syntax("test_files/test1.cpp")` to process the first test file, or `SyntaxHighlighter.do_syntax("test_files/test2.cpp")` for the second test file. Alternatively, you can add more test files in the `test_files` directory and process them similarly with `SyntaxHighlighter.do_syntax("test_files/<your_test_file.cpp>")`.
5. The output will be saved as `output/output.html`. Open this file in a web browser to see the syntax-highlighted code.

Example usage in the Elixir shell:
```elixir
iex> SyntaxHighlighter.do_syntax("test_files/test1.cpp")
```

([Example output](example_output/output_example.html)) <-- Click

---
## Token types/regex
| Token type | Regex |
|------------|-------------|
| multiline_coment| `^\/\*[\s\S]*?\*\/` |
| singleline_comment | `^\/\/.*` |
| preprocessor_directive | `^\s*#.*` |
| float | `^\d+\.\d+([eE][+-]?\d+)?` |
| integer | `^\d+` |
| string | `^"(\\.\|[^"\\])*"` | (regex literal: "(\\.\|[^"\\])*")
| char | `^'(\\.\|[^'\\])'` | (regex literal: '(\\.|[^'\\])')
| boolean | `^\b(true\|false)\b` | (regex literal: \b(true|false)\b) in this case it is taken from a list containing only true and false
| keyword | from C++ keyword list taken from here [link](https://en.cppreference.com/cpp/keyword) |
| identifier | `^[a-zA-Z_][a-zA-Z0-9_]*` |
| operator | `^\+\|\-\|\*\|\/\|%\|=\|==\|!=\|<\|>\|<=\|>=\|\+\+\|\-\-\|\+=\|\-=\|\*=\|\/=\|%=` | (regex literal: \+|\-|\*|\/|%|=|==|!=|<|>|<=|>=|\+\+|\-\-|\+=|\-=|\*=|\/=|%=)


---
## Time Complexity

The time complexity of the syntax highlighter is $O(n^2)$ even though most of the functions are linear, my core function `turn_to_tokens` is quadratic because of the way it processes the line character by character and applies multiple regexes to find tokens. A line has an "n" amount of characters, and for each character, it has to apply multiple regexes to find the token types, which results in a quadratic time complexity. 
Realistically the time complexity of is $O(n^2)$ is not a problem for this case because cpp code lines are usually not very long and the quadratic time complexity comes from the line length, not the file length.

---
## Reflection
This project was a great opportunity to practice working with regular expressions and string manipulation in Elixir. I learned about pipe operators mainly because until this point it was basically copy and paste from example codes, but this time I had to understand what they do and how they work. I don't think this project has ethical implications because syntax highlighting is a common feature of code editors, so there is nothing new created. However I think it is a good example on how to use regular expressions to process and analyze text.