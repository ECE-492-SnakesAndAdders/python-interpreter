# Testing

### Background

As with any software project, thorough testing is important. However, testing is even more important than usual for this project because we are working at such a low level. Users expect that all interpreters they use are bug-free; as Robert Nystrom says in [Crafting Interpreters](https://craftinginterpreters.com), people expect that the source of bugs is their code and not what implements it.

### Strategy

The C3 has very little features and support, so there is no existing testing framework that can be applied. The testing process cannot be easily automated either because the board has no file system (we cannot even pipe in an input file and compare output files). Thus, all tests written must be executed manually by a human into the REPL shell interface. Because of this limitation, rigorous formal testing is highly impractical and, thus, not a focus of or priority for this project.
