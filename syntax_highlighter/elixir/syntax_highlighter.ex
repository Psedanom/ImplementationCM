defmodule SyntaxHighlighter do
  @moduledoc """
  Tokenizes C++ code and generates an HTML file with syntax highlighting.\n
  Pablo Sedano Morlett A01785330\n
  May 17 2026
  """

  # List of C++ keywords taken from https://en.cppreference.com/w/cpp/keyword
  # true and false recognized as boolean so they are removed from the keyword list
  @keywords ~w(alignas alignof and and_eq asm auto bitand bitor bool break
    case catch char char8_t char16_t char32_t class compl concept const
    consteval constexpr constinit const_cast continue co_await co_return
    co_yield decltype default delete do double dynamic_cast else enum
    explicit export extern  float for friend goto if inline int long
    mutable namespace new noexcept not not_eq nullptr operator or or_eq
    private protected public register reinterpret_cast requires return
    short signed sizeof static static_assert static_cast struct switch
    template this thread_local throw try typedef typeid typename
    union unsigned using virtual void volatile wchar_t while xor xor_eq)

  # Main function to call the syntax highlighter
  def do_syntax(filename) do
    # Read the file, tokenize each line, render it to HTML, and write it to the output file
    lines =
      filename
      |> File.stream!()
      |> Enum.map(&turn_to_tokens/1)
      |> Enum.map(&write_line/1)

    File.write("output/output.html", write_html(filename, lines))
    "Syntax highlighting completed. Output written to output.html"
  end

  # Starts a tail recursion for tokenization of a line
  defp turn_to_tokens(line) do
    do_turn_to_tokens(line, [])
  end

  # If the string is empty reverse token (list of tuples) order to original order
  defp do_turn_to_tokens("", tokens), do: Enum.reverse(tokens)

  defp do_turn_to_tokens(text, tokens) do
    {token_type, token, rest} = get_token_type(text)
    # save tuple in the head of the list
    do_turn_to_tokens(rest, [{token_type, token} | tokens])
  end

  # runs regex to find the first token type and makes a tuple which
  defp get_token_type(text) do
    cond do
      result = Regex.run(~r/^\/\*[\s\S]*?\*\//, text) ->
        [head | _tail] = result
        {:multiline_comment, head, String.replace_prefix(text, head, "")}

      result = Regex.run(~r/^\/\/.*/, text) ->
        [head | _tail] = result
        {:singleline_comment, head, String.replace_prefix(text, head, "")}

      result = Regex.run(~r/^\s*#.*/, text) ->
        [head | _tail] = result
        {:preprocessor_directive, head, String.replace_prefix(text, head, "")}

      result = Regex.run(~r/^"(\\.|[^"\\])*"/, text) ->
        [head | _tail] = result
        {:string, head, String.replace_prefix(text, head, "")}

      result = Regex.run(~r/^'(\\.|[^'\\])'/, text) ->
        [head | _tail] = result
        {:char, head, String.replace_prefix(text, head, "")}

      result = Regex.run(~r/^\d+\.\d+([eE][+-]?\d+)?/, text) ->
        [head | _tail] = result
        {:float, head, String.replace_prefix(text, head, "")}

      result = Regex.run(~r/^\d+/, text) ->
        [head | _tail] = result
        {:integer, head, String.replace_prefix(text, head, "")}

      result = Regex.run(~r/^[a-zA-Z_][a-zA-Z0-9_]*/, text) ->
        [head | _tail] = result

        cond do
          head in ["true", "false"] ->
            {:boolean, head, String.replace_prefix(text, head, "")}

          head in @keywords ->
            {:keyword, head, String.replace_prefix(text, head, "")}

          true ->
            {:identifier, head, String.replace_prefix(text, head, "")}
        end

      result = Regex.run(~r/^(\+\+|--|<=|>=|==|!=|\+=|-=|\*=|\/=|%=|\+|-|\*|\/|%|=|<|>)/, text) ->
        [head | _tail] = result
        {:operator, head, String.replace_prefix(text, head, "")}

      # If no token type is found, take the first character as plain text and continue
      true ->
        head = String.first(text)
        {:plain, head, String.replace_prefix(text, head, "")}
    end
  end

  # Writes a line of tokens wrapping them with their class type
  defp write_line(tokens) do
    tokens
    |> Enum.map(&html_token/1)
    |> Enum.join("")
  end

  # Plain text is not wrapped in a span
  defp html_token({:plain, text}), do: html_escape(text)

  # Wraps token types that need colors
  defp html_token({token_type, text}) do
    "<span class=\"#{token_type}\">#{html_escape(text)}</span>"
  end

  # Escape HTML special characters to prevent breaking the HTML structure
  defp html_escape(text) do
    text
    |> String.replace("&", "&amp;")
    |> String.replace("<", "&lt;")
    |> String.replace(">", "&gt;")
    |> String.replace("\"", "&quot;")
  end

  defp write_html(filename, lines) do
    content = Enum.join(lines, "")

    """
    <!DOCTYPE html>
    <html>
    <link rel="stylesheet" href="token_colors.css">
      <head>
        <title>#{filename}</title>
      </head>
      <body>
        <pre>
    #{content}
        </pre>
      </body>
    </html>
    """
  end
end
