# Supplemental Information

### Background

This file serves to outline what features are supported by the interpreter at present. The target functionality is what was available in Python 1, with additional features where possible.

### Lexer

The lexer converts a text string into a discrete list of tokens to be anaylzed further. Because there is no widely-available documentation for the original Python 1 syntax and grammar, this implementation is based primarily off of the [Python 3 standard](https://docs.python.org/3/reference/lexical_analysis.html), with exceptions as noted:

* Certain modern capabilities may be supported by the lexer (for simplicity and future-proofing), but not implemented by the following stages. This includes the following:
   * The `@` and `@=` operators, used for matrix multiplication.
   * The `->` operator, used to declare the return type of a function.
   * The `:=` operator, used for assignment expressions.
* Other functionality is not supported by the lexer at all. This includes the following:
   * The use of floating-point numbers and related constructs (such as imaginary and complex numbers); this is due to underlying hardware limitations.
   * The use of f-strings and related constructs (such as r-strings and implicit string concatenation); these are a relatively recent addition and add unnecessary complexity.
   * The use of the `match` statement; this is a very recent addition and adds unnecessary complexity.
