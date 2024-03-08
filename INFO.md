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

### Parser

The parser converts the list of tokens returned by the lexer into an abstract syntax tree that can be easily executed. Again, this implementation is targeted to support Python 1 but is based off of the [Python 3 documentation](https://docs.python.org/3/reference/grammar.html), with exceptions as noted:

* Certain constructs are not supported by the parser. This includes the following:
   * The use of the ternary operator (`first_statement if condition else second_statement`); this syntax is rarely used and was introduced in Python 2.5.
   * The use of list, set, and dict comprehensions (such as `[abs(x) for x in y]`); this syntax was added in Python 2.0 and later.
   * The use of implicitly combined logical operators (`not in` and `is not`); this may be fixed in future versions.
   * The use of chained comparison operators (`x < y < z` becomes `(x < y) < z` and not `(x < y) and (y < z)`); this may be fixed in future versions.
