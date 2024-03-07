# Testing

### Background

As with any software project, thorough testing is important. However, it is even more important than usual for this project because we are working at such a low-level. Users expect that interpreters they use are bug-free; as Robert Nystrom says in [Crafting Interpreters](https://craftinginterpreters.com), people expect that the source of bugs is their code and not what implements it.

### Strategy

The C3 has very little features and support, so there is no testing framework that can be used here. Testing cannot be automated easily either because the board has no file system (so we cannot pipe in an input file and compare output files). Thus, all tests written must be executed manually by a human into the REPL shell interface
