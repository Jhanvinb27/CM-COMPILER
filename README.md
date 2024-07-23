Project Overview
The C++ code for the translator consists of five key components:

main: Handles the execution flow.
lexer: Performs lexical analysis.
symbol: Manages the symbol table.
parser: Conducts syntactic analysis.
inter: Contains classes for language constructs in the abstract syntax tree (AST).
The source program is initially parsed into a stream of tokens and then transformed into a syntax tree with constructs or nodes implemented as objects. These objects are responsible for constructing syntax-tree nodes, checking types, and generating three-address intermediate code.

Explore the code and understand how each component interacts to create a robust and efficient compiler front end in C++.
